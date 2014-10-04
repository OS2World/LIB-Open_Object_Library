#ifndef __OOL_XCOLOR_H__
#define __OOL_XCOLOR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XColor                                */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "string.h"

#define COL_BLACK                        1
#define COL_BLUE                         2
#define COL_RED                          3
#define COL_PINK                         4
#define COL_GREEN                        5
#define COL_CYAN                         6
#define COL_YELLOW                       7
#define COL_BROWN                        8
#define COL_DARKGRAY                     9
#define COL_DARKBLUE                     10
#define COL_DARKRED                      11
#define COL_DARKPINK                     12
#define COL_DARKGREEN                    13
#define COL_DARKCYAN                     14
#define COL_PALEGRAY                     15
#define COL_WHITE                        16


class _export_ XColor: public XObject
{
   private:
      char color[4];
   public:
      XColor( const LONG col = COL_WHITE);
      XColor( const char red, const char green, const char blue)
         {
            color[2] = red;
            color[1] = green;
            color[0] = blue;
            color[3] = 0;
         }
      virtual ~XColor() {;}
      void SetColor( const LONG col) { memcpy(&color[0], &col, 3); }
      void SetRed( const char red) { color[2] =  red; }
      void SetGreen( const char green) { color[1] =  green; }
      void SetBlue( const char blue) { color[0] = blue; }
      char GetRed(void) const { return color[2]; }
      char GetGreen(void) const { return color[1]; }
      char GetBlue(void) const { return color[0]; }
      LONG GetColor(void) const
         {
            LONG c=0;
            memcpy( &c, &color[0], 3);
            return c;
         }
      XColor operator =(const XColor& col) { memcpy(&color, &col.color, sizeof(4)); return *this; }
};


#endif
