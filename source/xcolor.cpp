#include "xcolor.h"

/*@
@class XColor
@type overview
@symbol _
@remarks XColor represents colors in RGB format.
*/

/*@ XColor::GetRed()
@group get components
@remarks get red component
@returns CHAR the component
*/

/*@ XColor::GetBlue()
@group get components
@remarks get blue component
@returns CHAR the component
*/

/*@ XColor::GetGreen()
@group get components
@remarks get green component
@returns CHAR the component
*/

/*@ XColor::SetRed(char)
@group ser components
@remarks set red component
@parameters CHAR the component
*/

/*@ XColor::SetBlue(char)
@group ser components
@remarks set blue component
@parameters CHAR the component
*/

/*@ XColor::SetGreen(char)
@group ser components
@remarks set green component
@parameters CHAR the component
*/

/*@ XColor::XColor(LONG)
@group constructors/destructors
@remarks The constructor of XColor.
@parameters LONG theColor The color to initialize. Valid values are:
                                 <t '�' c=1>
                                     �COL_BLACK
                                     �COL_BLUE
                                     �COL_RED
                                     �COL_PINK
                                     �COL_GREEN
                                     �COL_CYAN
                                     �COL_YELLOW
                                     �COL_BROWN
                                     �COL_DARKGRAY
                                     �COL_DARKBLUE
                                     �COL_DARKRED
                                     �COL_DARKPINK
                                     �COL_DARKGREEN
                                     �COL_DARKCYAN
                                     �COL_PALEGRAY
                                     �COL_WHITE
                                     �other: color in RGB-format, forth byte is ignored
            </t>
*/
XColor :: XColor(const LONG col)
{
   color[3] = 0;

   switch (col)
   {
   case COL_BLACK:
      color[0] = color[1] = color[2] = 0;
      break;
   case COL_BLUE:
      color[0] = 255;
      color[1] = 0;
      color[2] = 0;
      break;
   case COL_RED:
      color[0] = 0;
      color[1] = 0;
      color[2] = 255;
      break;
   case COL_PINK:
      color[0] = 255;
      color[1] = 0;
      color[2] = 255;
      break;
   case COL_GREEN:
      color[0] = 0;
      color[1] = 255;
      color[2] = 0;
      break;
   case COL_CYAN:
      color[0] = 255;
      color[1] = 255;
      color[2] = 0;
      break;
   case COL_YELLOW:
      color[0] = 0;
      color[1] = 255;
      color[2] = 255;
      break;
   case COL_BROWN:
      color[0] = 0;
      color[1] = 128;
      color[2] = 128;
      break;
   case COL_DARKGRAY:
      color[0] = color[1] = color[2] = 128;
      break;
   case COL_DARKBLUE:
      color[0] = 128;
      color[1] = 0;
      color[2] = 0;
      break;
   case COL_DARKRED:
      color[0] = 0;
      color[1] = 0;
      color[2] = 128;
      break;
   case COL_DARKPINK:
      color[0] = 128;
      color[1] = 0;
      color[2] = 128;
      break;
   case COL_DARKGREEN:
      color[0] = 0;
      color[1] = 128;
      color[2] = 0;
      break;
   case COL_DARKCYAN:
      color[0] = 128;
      color[1] = 128;
      color[2] = 0;
      break;
   case COL_PALEGRAY:
      color[0] = color[1] = color[2] = 206;
      break;
   case COL_WHITE:
      color[0] = color[1] = color[2] = 255;
      break;
   default:
      memcpy(&color[0], &col, 4);
   }
}

