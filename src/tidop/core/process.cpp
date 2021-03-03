﻿/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

/// https://github.com/eidheim/tiny-process-library

#include "tidop/core/process.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

#if defined WIN32
#include <windows.h>
//#include <atlstr.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#endif

#include <locale>
#include <codecvt>

/* ---------------------------------------------------------------------------------- */
/*          PROCESOS Y BATCH                                                          */
/* ---------------------------------------------------------------------------------- */


namespace tl
{


unsigned long Process::sProcessCount = 0;

Process::Process(Process *parent)
  : mStatus(Status::start),
    mParent(parent),
    mListeners(0),
    mProcessId(0),
    mProcessName("")
{
  if (mParent == nullptr) {
    mProcessId = ++sProcessCount;
  }
}

Process::~Process()
{
  if (mStatus == Status::running || mStatus == Status::pause || mStatus == Status::pausing) {
    stop();
  }
  mStatus = Status::finalized;
}

Process::Status Process::run(Progress *progressBar)
{
  if (mStatus != Status::finalized) { // Para saltar procesos ya realizados.
    runTriggered();
    mStatus = Status::running;
    //this->execute(progressBar);
    mThread = std::move(std::thread(&Process::execute, this, progressBar));
    mThread.join();
  }

  return mStatus;
}

void Process::runAsync(Progress *progressBar)
{
  if (mStatus != Status::finalized) { // Para saltar procesos ya realizados.
    runTriggered();
    mStatus = Status::running;
    //this->execute(progressBar);
    mThread = std::thread(&Process::execute, this, progressBar);
    mThread.detach();
  } 
}

void Process::pause()
{
  mStatus = Status::pausing;
  while (mStatus != Status::pause);
  pauseTriggered();
}

void Process::reset()
{
  TL_TODO("Si esta arrancado el proceso se tiene que detener antes")
  mStatus = Status::start;
}

void Process::resume()
{
  if (mStatus == Status::pause || mStatus == Status::pausing) {
    mStatus = Status::running;
    resumeTriggered();
  }
}

void Process::stop()
{
  if (mStatus == Status::running || mStatus == Status::pause || mStatus == Status::pausing) {
    TL_TODO("Estado Stopping")
    mStatus = Status::stopped;
    stopTriggered();
  }
}

Process::Status Process::status()
{
  return mStatus;
}

uint64_t Process::id() const
{
  return mProcessId;
}

std::string Process::name() const
{
  return mProcessName;
}

void Process::addListener(Listener *listener)
{
  if (mParent == nullptr) {
    mListeners.push_back(listener);
  }
}

void Process::removeListener(Listener *listener)
{
  if (!mListeners.empty()) {
    mListeners.remove(listener);
  }
}

void Process::processCountReset()
{
  sProcessCount = 0;
}

void Process::endTriggered()
{
  mStatus = Status::finalized;						  
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onEnd(id());
    }
  }
}

void Process::pauseTriggered()
{
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onPause(id());
    }
  }
}

void Process::resumeTriggered()
{
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onResume(id());
    }
  }
}

void Process::runTriggered()
{
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onRun(id());
    }
  }
}

void Process::stopTriggered()
{
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onStop(id());
    }
  }
}

void Process::errorTriggered()
{
  mStatus = Status::error;						  
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onError(id());
    }
  }
}


/* ---------------------------------------------------------------------------------- */


#ifdef WIN32
DWORD CmdProcess::sPriority = NORMAL_PRIORITY_CLASS;
#endif

CmdProcess::CmdProcess(const std::string &cmd, Process *parentProcess)
  : Process(parentProcess),
    mCmd(cmd)
{
#ifdef WIN32
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
#endif
}

CmdProcess::~CmdProcess()
{
#ifdef WIN32
  // Se cierran procesos e hilos 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
#endif
}

TL_DISABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)
//Process::Status CmdProcess::run(Progress *progressBar)
//{
//  Process::run();
//  return this->execute(progressBar);
//}

void CmdProcess::execute(Progress *progressBar)
{

#ifdef WIN32
  size_t len = strlen(mCmd.c_str());
  std::wstring wCmdLine(len, L'#');
  mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
  //size_t outSize;
  //mbstowcs_s(&outSize, &wCmdLine[0], len, mCmd.c_str(), len);

  LPWSTR cmdLine = (LPWSTR)wCmdLine.c_str();
  if (!CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe",
    cmdLine,                      // Command line
    NULL,                         // Process handle not inheritable
    NULL,                         // Thread handle not inheritable
    FALSE,                        // Set handle inheritance to FALSE
    CREATE_NO_WINDOW | sPriority, // Flags de creación
    NULL,                         // Use parent's environment block
    NULL,                         // Use parent's starting directory 
    &si,                          // Pointer to STARTUPINFO structure
    &pi)) {                       // Pointer to PROCESS_INFORMATION structure

    msgError("CreateProcess failed (%d) %s", GetLastError(), formatErrorMsg(GetLastError()).c_str());
    errorTriggered();
    return;   
  }

  DWORD ret = WaitForSingleObject(pi.hProcess, INFINITE);
  if (ret == WAIT_FAILED) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    errorTriggered();
    return;  
  } else if (ret == WAIT_OBJECT_0) {
    msgInfo("Command executed: %s", mCmd.c_str());
  } else if (ret == WAIT_ABANDONED) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    errorTriggered();
    return;   
  } else if (ret == WAIT_TIMEOUT) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    errorTriggered();
    return;	   
  } /*else {
    msgInfo("Comando ejecutado: %s", mCmd.c_str());
    return Process::Status::FINALIZED;
  }*/
  DWORD exitCode;
  if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    errorTriggered();
    return;	   
  }

  endTriggered();

  return;
#else
  pid_t pid;
  char *cmd = nullptr;
  strcpy(cmd, mCmd.c_str());
  char *argv[] = {"sh", "-c", cmd, nullptr};
  int status;
  //printf("Run command: %s\n", cmd);
  status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
  if (status == 0) {
      //printf("Child pid: %i\n", pid);
      if (waitpid(pid, &status, 0) != -1) {
          //printf("Child exited with status %i\n", status);
        return;
      } else {
        return; //Process::Status::error;
      }
  } else {
      printf("posix_spawn: %s\n", strerror(status));
      msgError("Error (%i: %s) when executing the command: %s", status, strerror(status), mCmd.c_str());
      return; //Process::Status::error;
  }
//  int posix_spawn(pid_t *pid, const char *path,
//                  const posix_spawn_file_actions_t *file_actions,
//                  const posix_spawnattr_t *attrp,
//                  char *const argv[], char *const envp[]);
  /// Para escribir en un log la salida
  /// https://unix.stackexchange.com/questions/252901/get-output-of-posix-spawn

#endif
}
//Process::Status CmdProcess::run(Progress *progressBar)
//{
//  Process::run();
//
//#ifdef WIN32
//  size_t len = strlen(mCmd.c_str());
//  std::wstring wCmdLine(len, L'#');
//  //mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
//  size_t outSize;
//  mbstowcs_s(&outSize, &wCmdLine[0], len, mCmd.c_str(), len);
//
//  LPWSTR cmdLine = (LPWSTR)wCmdLine.c_str();
//  if (!CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe",
//    cmdLine,                      // Command line
//    NULL,                         // Process handle not inheritable
//    NULL,                         // Thread handle not inheritable
//    FALSE,                        // Set handle inheritance to FALSE
//    CREATE_NO_WINDOW | sPriority, // Flags de creación
//    NULL,                         // Use parent's environment block
//    NULL,                         // Use parent's starting directory 
//    &si,                          // Pointer to STARTUPINFO structure
//    &pi)) {                       // Pointer to PROCESS_INFORMATION structure
//
//    msgError("CreateProcess failed (%d) %s", GetLastError(), formatErrorMsg(GetLastError()).c_str());
//    return Process::Status::error;
//  }
//
//  DWORD ret = WaitForSingleObject(pi.hProcess, INFINITE);
//  if (ret == WAIT_FAILED) {
//    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
//    return Process::Status::error;
//  } else if (ret == WAIT_OBJECT_0) {
//    msgInfo("Command executed: %s", mCmd.c_str());
//    //return Process::Status::FINALIZED;
//  } else if (ret == WAIT_ABANDONED) {
//    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
//    return Process::Status::error;
//  } else if (ret == WAIT_TIMEOUT) {
//    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
//    return Process::Status::error;
//  } /*else {
//    msgInfo("Comando ejecutado: %s", mCmd.c_str());
//    return Process::Status::FINALIZED;
//  }*/
//  DWORD exitCode;
//  if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0) {
//    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
//    return Process::Status::error;
//  }
//  return Process::Status::finalized;
//#else
//  pid_t pid;
//  char *cmd = nullptr;
//  strcpy(cmd, mCmd.c_str());
//  char *argv[] = {"sh", "-c", cmd, nullptr};
//  int status;
//  //printf("Run command: %s\n", cmd);
//  status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
//  if (status == 0) {
//      //printf("Child pid: %i\n", pid);
//      if (waitpid(pid, &status, 0) != -1) {
//          //printf("Child exited with status %i\n", status);
//        return Process::Status::finalized;
//      } else {
//        return Process::Status::error;
//      }
//  } else {
//      printf("posix_spawn: %s\n", strerror(status));
//      msgError("Error (%i: %s) when executing the command: %s", status, strerror(status), mCmd.c_str());
//      return Process::Status::error;
//  }
////  int posix_spawn(pid_t *pid, const char *path,
////                  const posix_spawn_file_actions_t *file_actions,
////                  const posix_spawnattr_t *attrp,
////                  char *const argv[], char *const envp[]);
//  /// Para escribir en un log la salida
//  /// https://unix.stackexchange.com/questions/252901/get-output-of-posix-spawn
//
//#endif
//}
TL_ENABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

void CmdProcess::setPriority(int priority)
{
#ifdef WIN32
  if (priority == 0) {
    sPriority = REALTIME_PRIORITY_CLASS;
  } else if (priority == 1) {
    sPriority = HIGH_PRIORITY_CLASS;
  } else if (priority == 2) {
    sPriority = ABOVE_NORMAL_PRIORITY_CLASS;
  } else if (priority == 3) {
    sPriority = NORMAL_PRIORITY_CLASS;
  } else if (priority == 4) {
    sPriority = BELOW_NORMAL_PRIORITY_CLASS;
  } else if (priority == 5) {
    sPriority = IDLE_PRIORITY_CLASS;
  }
#endif
}

#ifdef WIN32
std::string CmdProcess::formatErrorMsg(DWORD errorCode)
{
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_IGNORE_INSERTS
    | FORMAT_MESSAGE_MAX_WIDTH_MASK;
  
  TCHAR errorMessage[1024] = TEXT("");

  FormatMessage(flags,
                NULL,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                sizeof(errorMessage)/sizeof(TCHAR),
                NULL);

  //std::string strError = CW2A(errorMessage);
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::string strError = converter.to_bytes(errorMessage);
  return strError;
}
#endif


/* ---------------------------------------------------------------------------------- */

BatchProcessing::BatchProcessing()
  : mStatus(Status::start),
    mProcessList(0),
    mListeners(0),
    mThread(),
    mCurrentProcess(nullptr)
{
}

BatchProcessing::BatchProcessing(const BatchProcessing &batchProcess)
  : mStatus(Status::start),
    mProcessList(batchProcess.mProcessList),
    mListeners(batchProcess.mListeners),
    mThread(),
    mCurrentProcess(nullptr)
{
  for (auto process : mProcessList) {
    process->addListener(this);
  }
}

BatchProcessing::BatchProcessing(std::initializer_list<std::shared_ptr<Process>> procList)
  : mStatus(Status::start),
    mProcessList(procList),
    mThread(),
    mCurrentProcess(nullptr)
{
  for (auto process : mProcessList) {
    process->addListener(this);
  }
}

BatchProcessing::~BatchProcessing()
{
  if (mStatus == Status::running || mStatus == Status::pause || mStatus == Status::pausing) {
    stop();
  }
  mStatus = Status::finalized;
}

BatchProcessing::Status BatchProcessing::run(Progress * progressBarTotal, Progress * progressBarPartial)
{
  mStatus = Status::running;
  if (progressBarTotal) progressBarTotal->init(0., static_cast<double>(mProcessList.size()));
  for (const auto &process : mProcessList) {
    if (mStatus == Status::pausing) {
      mStatus = Status::pause;
      while (mStatus == Status::pause);
    } else if (mStatus == Status::stopped) {
      // Se fuerza la terminación
      return Status::stopped;
    } else {
      //if (process->run(progressBarPartial) == Process::Status::FINALIZED_ERROR) {
      //  return Status::FINALIZED_ERROR;
      //} else {
      //  if (progressBarTotal) (*progressBarTotal)();
      //}
      process->run(progressBarPartial);
      if (progressBarTotal) (*progressBarTotal)();
    }
  }
  return (mStatus = Status::finalized);
}

BatchProcessing::Status BatchProcessing::run_async(Progress * progressBarTotal, Progress * progressBarPartial)
{
  mStatus = Status::running;

  auto f_aux = [&](Progress *progress_bar_total, Progress *progress_bar_partial) {
    if (progress_bar_total) progress_bar_total->init(0., static_cast<double>(mProcessList.size()));
    for (const auto &process : mProcessList) {
      if (progress_bar_total) {
        // Se han añadido nuevos procesos asi que se actualiza
        progress_bar_total->setMaximun(static_cast<double>(mProcessList.size()));
        progress_bar_total->updateScale();
      }
      mCurrentProcess = process.get();
      if (mStatus == Status::pausing) {
        mStatus = Status::pause;
        while (mStatus == Status::pause);
      } else if (mStatus == Status::stopped) {
        // Se fuerza la terminación
        return Status::stopped;
      } else {
        //process->run(progress_bar_partial);
        if (process->run(progress_bar_partial) == Process::Status::error) {
        //  return Status::FINALIZED_ERROR;
          if (progress_bar_partial) progress_bar_partial->restart();
          //TODO: evento de error con el id de proceso
        }
          
        if (progress_bar_total) (*progress_bar_total)();
        
      }
    }
    endTriggered();
    return (mStatus = Status::finalized);
  };

  mThread = std::thread(f_aux, progressBarTotal, progressBarPartial);
  mThread.detach();

  return mStatus;
}

void BatchProcessing::push_back(const std::shared_ptr<Process> &process)
{
  mProcessList.push_back(process);
  process->addListener(this);
}

void BatchProcessing::addListener(Listener * listener)
{
  mListeners.push_back(listener);
}

void BatchProcessing::removeListener(Listener * listener)
{
  if (!mListeners.empty()) {
    mListeners.remove(listener);
  }
}

void BatchProcessing::remove(uint64_t id)
{
  for (std::list<std::shared_ptr<Process>>::iterator it = mProcessList.begin(); it != mProcessList.end(); it++) {
    if ((*it)->id() == id) {
      remove(*it);
      break;
    }
  }
}

void BatchProcessing::remove(const std::shared_ptr<Process> &process)
{
  process->removeListener(this);
  mProcessList.remove(process);
}

bool BatchProcessing::isRunning() const
{
  return (mStatus == Status::running || mStatus == Status::pausing || mStatus == Status::pause);
}

void BatchProcessing::pause()
{
  mStatus = Status::pausing;
  if (mCurrentProcess) {
    mCurrentProcess->pause();
  }
}

void BatchProcessing::reset()
{
  //TODO: Si esta corriendo no se puede hacer un reset
  if (mStatus == Status::running) {
    msgWarning("No se puede hacer un reset mientras el batch esta corriendo. Utilice el método stop() para cancelar los procesos");
  } else {
    mStatus = Status::start;
    mProcessList.clear();
    Process::processCountReset();
  }
}

void BatchProcessing::resume()
{
  if (mStatus == Status::pause || mStatus == Status::pausing) {
    mStatus = Status::running;
    if (mCurrentProcess) 
      mCurrentProcess->resume();
  }
}

void BatchProcessing::stop()
{
  mStatus = Status::stopped;
  if (mCurrentProcess) {
    mCurrentProcess->stop();
  }
}

void BatchProcessing::initCounter()
{
  Process::processCountReset();
}

void BatchProcessing::onPause(uint64_t id)
{
  msgInfo("Process %i paused", id);
}

void BatchProcessing::onResume(uint64_t id)
{
  msgInfo("Process %i resumed", id);
}

void BatchProcessing::onRun(uint64_t id)
{
  msgInfo("Process %i running", id);
}

void BatchProcessing::onStop(uint64_t id)
{
  msgInfo("Process %i stopped", id);
}

void BatchProcessing::onEnd(uint64_t id)
{
  msgInfo("Process %i completed", id);
}

void BatchProcessing::onError(uint64_t id)
{
  msgInfo("Process %i. Process error", id);
  errorTriggered();
}

void BatchProcessing::endTriggered()
{
  mStatus = Status::finalized;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onEnd();
    }
  }
}

void BatchProcessing::errorTriggered()
{
  mStatus = Status::error;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onError();
    }
  }
}



/* ---------------------------------------------------------------------------------- */


//ProcessBlock::ProcessBlock(int nBlocks, TL::Progress *progressBar)
//  : mCount(nBlocks), 
//    mProgressBar(progressBar)
//{
//  if (mProgressBar) {
//    mProgressBar->init(0, nBlocks);
//  }
//}
//
//ProcessBlock::~ProcessBlock()
//{
//}
//
//void ProcessBlock::next()
//{
//  if (mProgressBar) (*mProgressBar)();
//  if (mStatus == Status::PAUSING) {
//    pauseTriggered();
//    while (mStatus == Status::PAUSE);
//  } else if (mStatus == Status::STOPPED) {
//    // Se fuerza la terminación
//    stopTriggered();
//    return Status::STOPPED;
//  }
//}




} // End namespace tl


