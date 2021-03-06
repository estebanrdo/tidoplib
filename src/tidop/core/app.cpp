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

#include "tidop/core/app.h"

#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#endif

#include <array>

namespace tl
{

std::unique_ptr<App> App::sObjApp;
std::mutex App::mtx;
std::once_flag App::sInitFlag;

App::App()
{
  init();
}

App &App::instance()
{
  std::call_once(sInitFlag, []() {
    sObjApp.reset(new App());
  });

  return *sObjApp;
}

tl::Path App::path() const
{
  static std::array<char, TL_MAX_PATH> runfile;

#ifdef WIN32
  ::GetModuleFileNameA(NULL, runfile.data(), TL_MAX_PATH);
  return tl::Path(std::string(runfile.data()));
#else
  std::array<char, 32> _path{};
  sprintf(_path.data(), "/proc/%d/exe", getpid());
  long len = readlink(_path.data(), runfile.data(), runfile.size());
  if (len >= 0)
    runfile.at(static_cast<size_t>(len)) = '\0';

  return tl::Path(std::string(runfile.data()));
#endif
}

std::string App::version() const
{
  return std::string();
}

void App::init()
{
}

} // namespace tl
