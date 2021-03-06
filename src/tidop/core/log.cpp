/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/core/log.h"
#include "tidop/core/app.h"
#include "tidop/core/messages.h"
#include "tidop/core/chrono.h"
#include "tidop/core/path.h"

#include <fstream>

namespace tl
{

std::unique_ptr<Log> Log::sObjLog;
std::string Log::sLogFile;
EnumFlags<MessageLevel> Log::sLevel = MessageLevel::msg_error;
std::string Log::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";
std::mutex Log::mtx;
std::once_flag Log::sInitFlag;

#ifdef TL_MESSAGE_HANDLER
bool Log::sPauseListener = false;
#endif

Log::Log()
#ifdef TL_MESSAGE_HANDLER
  : MessageManager::Listener(false)
#endif
{
}

Log::~Log()
{
}

Log &Log::instance()
{
  std::call_once(sInitFlag, []() {
    sObjLog.reset(new Log());
  });

  return *sObjLog;
}

EnumFlags<MessageLevel> Log::logLevel()
{
  return sLevel;
}

void Log::setMessageLevel(MessageLevel level)
{
  sLevel = level;
}
  
#ifdef TL_ENABLE_DEPRECATED_METHODS
void Log::setLogLevel(MessageLevel level)
{
  sLevel = level;
}
#endif

void Log::setLogFile(const std::string &file)
{
  sLogFile = file;
}

void Log::write(const std::string &message)
{

  std::string date = formatTimeToString("%d/%b/%Y %H:%M:%S");
  this->_write(message, date);
}

#ifdef TL_MESSAGE_HANDLER

void Log::pauseListener()
{
  sPauseListener = true;
}

void Log::resumeListener()
{
  sPauseListener = false;
}

void Log::onMsgDebug(const std::string &message, 
                     const std::string &date)
{
  if (sLevel.isActive(MessageLevel::msg_debug)) {
    _write(message, date);
  }
}

void Log::onMsgInfo(const std::string &message, 
                    const std::string &date)
{
  if (sLevel.isActive(MessageLevel::msg_info)) {
    _write(message, date);
  }
}

void Log::onMsgWarning(const std::string &message, 
                       const std::string &date)
{
  if (sLevel.isActive(MessageLevel::msg_warning)) {
    _write(message, date);
  }
}

void Log::onMsgError(const std::string &message, 
                     const std::string &date)
{
  if (sLevel.isActive(MessageLevel::msg_error)) {
    _write(message, date);
  }
}

void Log::_write(const std::string &message, 
                 const std::string &date)
{
  if (sLogFile.empty()) {
    // Log por defecto
    Path log_path(App::instance().path());
    log_path.replaceExtension(".log");
    sLogFile = log_path.toString();
  }
  
  Path log_parent_path = Path(sLogFile).parentPath();
  if (!log_parent_path.exists()) {
    int err = log_parent_path.createDirectories();
    if (err == -1) {
      MessageManager::instance().removeListener(this);
      return;
    }
  }

  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << message << "\n";
    hLog.close();
  } else {
    MessageManager::instance().removeListener(this);
  }
}


#endif // TL_MESSAGE_HANDLER


} // End mamespace tl
