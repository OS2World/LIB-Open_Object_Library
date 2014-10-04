#ifndef __OOL_XBUBLE_H__
#define __OOL_XBUBLE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XBubbleHelp                           */
/*                       derived from: XUserWindow                           */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "XfrmWnd.h"
#include "xuserwnd.h"
#include "xtext.h"

class XBitmap;
class XGraphicDevice;
class XResource;

class _export_ XBubbleHelp: public XUserWindow
{
      friend class bubbletimer;
      friend class bubblehandler;
      friend class bhandler;
   private:
      XGraphicDevice * dev;
      XBitmap * map1, * map2, *map3;
      BOOL isActive;
      XText * text;
      bubbletimer * timer;
      void Draw( void );
      void Show( const BOOL s = TRUE);
      XWindow * aktWin;
   public:
      XBubbleHelp( XWindow *);
      ~XBubbleHelp();
      void Enable( const BOOL enable = TRUE) { isActive = enable; }
      XText * GetTextObject(void) { return text; }
      XGraphicDevice * GetGraphicDevice() { return dev; }
      virtual BOOL SetMsgText( ULONG ) { return FALSE; }
      void SetText( const char * t) { text->SetText( (char*)t); }
};

#endif
