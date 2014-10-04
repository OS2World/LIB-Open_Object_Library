#ifndef __OOL_XFONT_H__
#define __OOL_XFONT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFont                                 */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

class XFrameWindow;
class XGraphicDevice;

#define FONT_BOLD        FATTR_SEL_BOLD
#define FONT_UNDERSCORE  FATTR_SEL_UNDERSCORE
#define FONT_ITALIC      FATTR_SEL_ITALIC
#define FONT_STRIKEOUT   FATTR_SEL_STRIKEOUT
#define FONT_OUTLINE     FATTR_SEL_OUTLINE


class _export_ XFont: public XObject
{
      friend class XGraphicDevice;
      friend class XPrinterDevice;
      friend class XText;
   private:
      char fontId;
      SHORT fontSize;
      SHORT realSize, pixelSize;
      BOOL fixed;
   public:
      XFont( XGraphicDevice *, const char * fontName, const SHORT size, const LONG options = 0);
      virtual ~XFont() {;}
      SHORT GetPixelSize() const { return pixelSize; }
};

#endif
