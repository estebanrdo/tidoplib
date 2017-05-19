#include "messages.h"

#include "core/defs.h"
#include "core/utils.h"
#include "core/console.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/utility.hpp"
#endif

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include <cstdarg>
#if defined WIN32
# include <windows.h>
#endif
#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "core/defs.h"

#ifdef I3D_MESSAGE_HANDLER

namespace I3D
{


I3D_DISABLE_WARNING(4100)

#ifdef HAVE_OPENCV
// manejador de error para OpenCV. Para evitar los mensajes por consola de OpenCV
int handleError( int status, const char* func_name, const char* err_msg, const char* file_name, int line, void* userdata )
{
  MessageManager::release(MessageManager::Message(err_msg).getMessage(), MessageLevel::MSG_ERROR, file_name, line, func_name);
  return 0;
}

#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
// Manejador de eventos para GDAL 
void handleErrorGDAL(CPLErr err, CPLErrorNum eNum, const char *err_msg) 
{
  MessageLevel ml;
  if (err == CE_Debug) {
    ml = MessageLevel::MSG_DEBUG;
  } else if (err == CE_Warning) {
    ml = MessageLevel::MSG_WARNING;
  } else if (err == CE_Failure) {
    ml = MessageLevel::MSG_ERROR;
  } else if (err == CE_Fatal) {
    ml = MessageLevel::MSG_ERROR;
  } else {
    ml = MessageLevel::MSG_INFO;
  }
  MessageManager::release(MessageManager::Message(err_msg).getMessage(), MessageLevel::MSG_ERROR);
  return;
}
#endif // HAVE_GDAL

I3D_ENABLE_WARNING(4100)



struct _msgProperties {
  const char *normal;
  const char *extend;
};

struct _msgProperties _msgTemplate[] = {   
  { "Debug: %s",   "Debug: %s (%s:%u, %s)"},
  { "Info: %s",    "Info: %s (%s:%u, %s)"},
  { "Warning: %s", "Warning: %s (%s:%u, %s)"},
  { "Error: %s",   "Error: %s (%s:%u, %s)"}
};

_msgProperties getMessageProperties(MessageLevel msgLevel) 
{
  int iLevel = 0;
  switch (msgLevel) {
  case I3D::MessageLevel::MSG_DEBUG:
    iLevel = 0;
    break;
  case I3D::MessageLevel::MSG_INFO:
    iLevel = 1;
    break;
  case I3D::MessageLevel::MSG_WARNING:
    iLevel = 2;
    break;
  case I3D::MessageLevel::MSG_ERROR:
    iLevel = 3;
    break;
  default:
    iLevel = 3;
    break;
  }
  return _msgTemplate[iLevel];
}

std::unique_ptr<MessageManager> MessageManager::sObjMessage;
bool MessageManager::sStopHandler = false;

std::string MessageManager::Message::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";

MessageManager::MessageManager() : mListeners(0)
{
}

MessageManager::~MessageManager()
{
}

void MessageManager::addListener(Listener *listener)
{ 
  mListeners.push_back(listener);
}

MessageManager &MessageManager::getInstance()
{
  if (sObjMessage.get() == 0) {
    sObjMessage.reset(new MessageManager());
  }
  return *sObjMessage;
}

void MessageManager::initExternalHandlers()
{
#ifdef HAVE_OPENCV
  cv::redirectError(handleError);
#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
  CPLPushErrorHandler((CPLErrorHandler)handleErrorGDAL);
#endif // HAVE_GDAL
}

void MessageManager::pause()
{
  sStopHandler = true;
}

void MessageManager::resume()
{
  sStopHandler = false;
}

void MessageManager::onDebug(const char *msg, const char *date)
{
#ifdef _DEBUG
  if (!sStopHandler && !mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onMsgDebug(msg, date);
    }
  }
#endif
}

void MessageManager::onInfo(const char *msg, const char *date)
{
  if (!sStopHandler && !mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onMsgInfo(msg, date);
    }
  }
}

void MessageManager::onWarning(const char *msg, const char *date)
{
  if (!sStopHandler && !mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onMsgWarning(msg, date);
    }
  }
}

void MessageManager::onError(const char *msg, const char *date)
{
  if (!sStopHandler && !mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onMsgError(msg, date);
    }
  }
}

void MessageManager::release(const char *msg, const MessageLevel &level, const char *file, int line, const char *function)
{
  getInstance();

  if (sStopHandler) return;

  // Bloqueo aqui para evitar problemas entre hilos
  char date[64];
  std::time_t now = std::time(NULL);
  std::tm *_tm = std::localtime(&now);
  
  if (_tm) {
    std::strftime(date, sizeof(date), "%d/%b/%Y %H:%M:%S", _tm); 
  } else {
    strcpy(date, "NULL");
  }

  char buf[1000];
  #if defined _MSC_VER
    if (line != -1)
      sprintf_s(buf, 1000, getMessageProperties(level).normal, msg, file, line, function);
    else
      sprintf_s(buf, 1000, getMessageProperties(level).extend, msg, file, line, function);
  #else
    if (line != -1)
      snprintf(buf, 1000, getMessageProperties(level).normal, msg, file, line, function);
    else
      snprintf(buf, 1000, getMessageProperties(level).extend, msg, file, line, function);
  #endif

  switch (level) {
  case I3D::MessageLevel::MSG_DEBUG:
    sObjMessage->onDebug(buf, date);
    break;
  case I3D::MessageLevel::MSG_INFO:
    sObjMessage->onInfo(buf, date);
    break;
  case I3D::MessageLevel::MSG_WARNING:
    sObjMessage->onWarning(buf, date);
    break;
  case I3D::MessageLevel::MSG_ERROR:
    sObjMessage->onError(buf, date);
    break;
  default:
    break;
  }
}

void MessageManager::release(const Message &msg)
{
  getInstance();
  
  if (sStopHandler) return;

  std::string msg_out;
  if (msg.getLine() == -1 && msg.getFile() == "" && msg.getFunction() == "") {
    msg_out = msg.getMessage();
  } else {
    char buf[1000];
#if defined _MSC_VER
    sprintf_s(buf, 1000, "%s (%s:%u, %s)", msg.getMessage(), msg.getLine(), msg.getLine(), msg.getFunction());
#else
    snprintf(buf, 1000, "%s (%s:%u, %s)", msg.getMessage(), msg.getLine(), msg.getLine(), msg.getFunction());
#endif
    msg_out =  std::string(buf);
  }

  switch (msg.getLevel()) {
  case I3D::MessageLevel::MSG_DEBUG:
    sObjMessage->onDebug(msg_out.c_str(), msg.getDate());
    break;
  case I3D::MessageLevel::MSG_INFO:
    sObjMessage->onInfo(msg_out.c_str(), msg.getDate());
    break;
  case I3D::MessageLevel::MSG_WARNING:
    sObjMessage->onWarning(msg_out.c_str(), msg.getDate());
    break;
  case I3D::MessageLevel::MSG_ERROR:
    sObjMessage->onError(msg_out.c_str(), msg.getDate());
    break;
  default:
    break;
  }
}

/* ---------------------------------------------------------------------------------- */

MessageManager::Message::Message(const char *msg, ...)
  : mLevel(MessageLevel::MSG_ERROR),
    mFile(""), mLine(-1),
    mFunction("")
{
  try {
    char date[64];
    std::time_t now = std::time(NULL);
    std::tm *_tm = std::localtime(&now);

    if (_tm) {
      std::strftime(date, sizeof(date), sTimeLogFormat.c_str()/*"%d/%b/%Y %H:%M:%S"*/, _tm);
    } else {
      strcpy(date, "NULL");
    }
    mDate = date;

    char buf[500];
    memset(buf, 0, sizeof(buf));
    std::string aux(msg);
    I3D::replaceString(&aux, "% ", "%% ");
    va_list args;
    va_start(args, msg);
#ifdef _MSC_VER
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
#else
  vsnprintf(buf, sizeof(buf), aux.c_str(), args);
#endif
    va_end(args);
    mMessage = buf;
  } catch (...) {

    // Por evitar un error en la constructora... 
  }
}

const char *MessageManager::Message::getMessage() const
{
  return mMessage.c_str();
}

const char *MessageManager::Message::getDate() const
{
  return mDate.c_str();
}

MessageLevel MessageManager::Message::getLevel() const
{
  return mLevel;
}

std::string MessageManager::Message::getFile() const
{
  return mFile;
}

int MessageManager::Message::getLine() const
{
  return mLine;
}
    
std::string MessageManager::Message::getFunction() const
{
  return mFunction;
}
    
void MessageManager::Message::setTimeLogFormat( const char *timeTemplate)
{
  sTimeLogFormat = timeTemplate;
}

void MessageManager::Message::setMessageLevel(const MessageLevel &level)
{
  mLevel = level;
}
    
void MessageManager::Message::setMessageProperties(const MessageLevel &level, const char *file, int line, const char *function)
{
  mLevel = level;
  mLine = line;
  mFile = file;
  mFunction = function;
}

/* ---------------------------------------------------------------------------------- */

std::unique_ptr<Log> Log::sObjLog;
std::string Log::sLogFile = "";
EnumFlags<MessageLevel> Log::sLevel = MessageLevel::MSG_ERROR;
std::string Log::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";
std::mutex Log::mtx;

Log &Log::getInstance()
{
  if (sObjLog.get() == 0) {
    sObjLog.reset(new Log());
  }
  return *sObjLog;
}

EnumFlags<MessageLevel> Log::getLogLevel() const
{
  return sLevel;
}

void Log::setLogFile(const char* file)
{
  sLogFile = file;
}

void Log::setLogLevel(MessageLevel level)
{
  sLevel = level;
}

void Log::write(const char *msg)
{

  char date[64];
  std::time_t now = std::time(NULL);
  std::tm *_tm = std::localtime(&now);
  if (_tm) {
    //std::strftime(date, sizeof(date), "%d/%b/%Y %H:%M:%S", _tm);
    std::strftime(date, sizeof(date), sTimeLogFormat.c_str(), _tm);
  } else {
    strcpy(date, "NULL");
  }

  if (sLogFile.empty()) {
    // Log por defecto
    char _logfile[I3D_MAX_PATH];
    changeFileExtension(getRunfile(), "log", _logfile, I3D_MAX_PATH);
    sLogFile = _logfile;
  }
  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << msg << "\n";
    hLog.close();
  } else {
    //Error al abrir/crear archivo. Se saca el error por consola
    //Message::message("The file %s was not opened\n", sLogFile.c_str()).print(MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE);
  }
}

void Log::onMsgDebug(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_DEBUG)) {
    _write(msg, date);
  }
}

void Log::onMsgInfo(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_INFO)) {
    _write(msg, date);
  }
}

void Log::onMsgWarning(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_WARNING)) {
    _write(msg, date);
  }
}

void Log::onMsgError(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_ERROR)) {
    _write(msg, date);
  }
}

void Log::_write(const char *msg, const char *date)
{

  if (sLogFile.empty()) {
    // Log por defecto
    char _logfile[I3D_MAX_PATH];
    changeFileExtension(getRunfile(), "log", _logfile, I3D_MAX_PATH);
    sLogFile = _logfile;
  }
  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << msg << "\n";
    hLog.close();
  } else {
    //Error al abrir/crear archivo. Se saca el error por consola
    //Message::message("The file %s was not opened\n", sLogFile.c_str()).print(MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE);
  }
}





} // End mamespace I3D


#endif  // I3D_MESSAGE_HANDLER