#ifndef __OOL_XCONTROL_H__
#define __OOL_XCONTROL_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XControl                              */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xwindow.h"

class XRect;
class XControlEvent;

#ifdef OOL_FOR_OS2_X86
   typedef struct _FORECOLORPARAM
   {
      ULONG id;
      ULONG cb;
      ULONG ulColor;
   }   FORECOLORPARAM;

   typedef struct _FONTPARAM
   {
      ULONG id;
      ULONG cb;
      CHAR szFontNameSize[20];
   }   FONTPARAM;


   typedef struct _PRES               // pres. params
   {
      ULONG cb;               // length
      FORECOLORPARAM fcparam;      // foreground color
      FONTPARAM fntparam;         // font name & size
   }   PRES;
#endif


class _export_ XControl: public XWindow
{
      friend MRESULT EXPENTRY ControlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
      friend MRESULT EXPENTRY ColorControlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
   protected:
      PFNWP oldfunc;
   protected:
      XControl ( const HWND hwnd );
   public:
      XControl( const XRect * rec, const ULONG style, const XWindow * ow, const char * title = "", const PSZ cl = 0, const USHORT id = 0, const char * font = NULL);
      virtual ~XControl() {;}
};


#endif
