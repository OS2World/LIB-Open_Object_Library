#ifndef __OOL_XCOLORCONTROL_H__
#define __OOL_XCOLORCONTROL_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XColorControl                         */
/*                       derived from: XControl                              */
/*                        last update: 4/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcontrol.h"
#include "xreslib.h"

class _export_ XColorControl: public XControl
{
   friend MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
      ULONG color;
      XResourceLibrary lib;
   public:
      XColorControl( const XWindow * owner, const XRect& rect);
      void GetColor( XColor * col) const { col->SetColor(color); }
      void SetColor( const XColor& col) const;
      virtual void ColorSelected( const XColor& newColor) {;}
};

#endif