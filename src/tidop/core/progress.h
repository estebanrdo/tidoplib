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

#ifndef TL_CORE_PROGRESS_H
#define TL_CORE_PROGRESS_H

#include "config_tl.h"

#include <mutex>

#include "tidop/core/defs.h"
#include "tidop/core/event.h"
#include "tidop/core/console.h"

namespace tl
{

/*!
 * \addtogroup core
 *
 * \{
 */

constexpr auto ProgressBarSize = 50;

class TL_EXPORT Progress
{

public:

	Progress() = default;

  /*!
   * \brief Operador de llamada.
   *
   * Se llama cada vez que se quiera avanzar en la función de progreso
   */
  virtual bool operator() (size_t increment = 1.) = 0;
  
  virtual void setRange(size_t min, size_t max) = 0;
  virtual size_t minimum() const = 0;
  virtual void setMinimum(size_t min) = 0;
  virtual size_t maximum() const = 0;
  virtual void setMaximum(size_t max) = 0;
  virtual void setText(const std::string &text) = 0;

  virtual void reset() = 0;

  static bool isRunning();

protected:

  static bool sIsRunning;
};



/* ---------------------------------------------------------------------------------- */




class TL_EXPORT ProgressBase
  : public Progress
{

public:

	ProgressBase();
  ProgressBase(size_t min, size_t max);
  virtual ~ProgressBase() = default;

// Progress

  bool operator()(size_t increment = 1) override;
  void setRange(size_t min, size_t max) override;
  size_t minimum() const override;
  void setMinimum(size_t min) override;
  size_t maximum() const override;
  void setMaximum(size_t max) override;
  void setText(const std::string &text) override;
  void reset() override;

protected:

  virtual void initialize();
  void updateScale();
  int percent();
  virtual void updateProgress() = 0;
  virtual void terminate();

private:

  /*!
   * \brief Valor mínimo
   */
  size_t mMinimum{0};

  /*!
   * \brief Valor máximo
   */
  size_t mMaximum{0};

  /*!
   * \brief Mensaje que se puede añadir con información del proceso.
   */
  std::string mMessage;

  /*!
   * \brief Valor actual
   */
  double mProgress{0};

  /*!
   * \brief Valor actual en tanto por ciento
   */
  int mPercent{-1};

  double mScale{1.};
  static std::mutex sMutex;
};



/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBar
  : public ProgressBase
{

public:

  /*!
   * \brief Constructora
   */
  ProgressBar();

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   */
  ProgressBar(size_t min, size_t max);

  /*!
   * \brief Destructora
   */
  ~ProgressBar() override = default;

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

private:

  int mProgressBarSize{ ProgressBarSize };

};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBarColor
  : public ProgressBase
{

public:

  /*!
   * \brief Constructora
   */
  ProgressBarColor();

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   */
  ProgressBarColor(size_t min, size_t max);

  /*!
   * \brief Destructora
   */
  ~ProgressBarColor() override = default;

  void setCompleteColor(Console::Color color);
  void setRemainigColor(Console::Color color);

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

private:

  int mProgressBarSize{ ProgressBarSize };
  Console::Color mCompleteColor{Console::Color::green};
  Console::Color mRemainigColor{Console::Color::yellow};
};



/* ---------------------------------------------------------------------------------- */



/*!
 * \brief Progreso en porcentaje
 */
class TL_EXPORT ProgressPercent
  : public ProgressBase
{

public:

  /*!
   * \brief Constructora ProgressPercent
   */
  ProgressPercent();

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   */
  ProgressPercent(size_t min, size_t max);

  /*!
   * \brief Destructora
   */
  ~ProgressPercent() override = default;

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

};


/*! \} */ // end of core

} // End namespace tl

#endif // TL_CORE_PROGRESS_H
