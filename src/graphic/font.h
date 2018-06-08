#ifndef TL_GRAPHIC_FONT_H
#define TL_GRAPHIC_FONT_H

#include <string>

#include "config_tl.h"

#include "core/defs.h"
#include "core/exception.h"
#include "core/flags.h"

namespace TL
{

namespace graph
{

/*!
 * \brief 
 */
class TL_EXPORT Font
{

public:

  enum class Style : uint8_t
  {
    DEFAULT = 0 << 0,
    BOLD = 1 << 0,
    ITALIC = 1 << 1
  };

private:

  /*!
   * \brief Nombre de fuente
   */
  std::string mName;

  /*!
   * \brief Tama�o de fuente
   */
  int mSize;

  /*!
   * \brief Estilo
   * \see Style
   */
  EnumFlags<Style> mStyle;

  /*!
   * \brief Subrayado de fuente
   */
  bool mUnderline;

  /*!
   * \brief Tachado de fuente
   */
  bool mStrikethrough;

public:

  /*!
   * \brief Constructora por defecto
   */
  Font();
  
  /*!
   * \brief Constructora de copia
   * \param[in] font Fuente
   */
  Font(const Font &font);

  /*!
   * \brief Constructora
   * \param[in] name Nombre de la fuente
   * \param[in] style Estilo de la fuente
   * \param[in] underline Subrayado
   * \param[in] strikethrough Tachado
   */
  Font(const std::string name, int size = 10, Style style = Style::DEFAULT, bool underline = false, bool strikethrough = false);

  /*!
   * \brief Destructora
   */
  ~Font() {}

  /*!
   * \brief Devuelve el nombre de la fuente
   * \return Nombre de la fuente
   */
  std::string getName() const;

  /*!
   * \brief Devuelve el tama�o de la fuente
   * \return Tama�o de la fuente
   */
  int getSize() const;

  /*!
   * \brief Fuente en negrita
   * \return Verdadero si la fuente est� en negrita
   */
  bool isBold() const;

  /*!
   * \brief Fuente en cursiva
   * \return Verdadero si la fuente est� en cursiva
   */
  bool isItalic() const;

  /*!
   * \brief Fuente con subrayado
   * \return Verdadero si la fuente subrayada
   */
  bool isUnderline() const;

  /*!
   * \brief Fuente tachada
   * \return Verdadero si la fuente est� tachada
   */
  bool isStrikethrough() const;

  /*!
   * \brief Establece el nombre de la fuente
   * \param[in] name Nombre de la fuente
   */
  void setName(const std::string &name);

  /*!
   * \brief Establece el tama�o de la fuente
   * \param[in] size Tama�o de la fuente
   */
  void setSize(int size);

  void setStyle(Style style);
  void setBold(bool active);
  void setItalic(bool active);
  void setUnderline(bool active);
  void setStrikethrough(bool active);

  Font &operator = (const Font &font);
};

ALLOW_BITWISE_FLAG_OPERATIONS(Font::Style)

} // End namespace graph


} // End namespace TL

#endif // TL_GRAPHIC_FONT_H