#ifndef __OOL_XCLIPBRD_H__
#define __OOL_XCLIPBRD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XClipBoard                            */
/*                       derived from: XObject                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xatom.h"

class XWindow;
class XBitmap;
class XString;

class _export_  XClipBoard: public XObject
{
      BOOL isOpen;
   public:
      XClipBoard( );
      virtual ~XClipBoard();
      void Empty(void) const;
      BOOL IsFormatAvaible( ULONG format);
      BOOL IsOpen(void) const {return isOpen; }
      BOOL GetBitmap( XBitmap * );
      void * GetData( const XAtom&);
      BOOL GetText( XString * );
      BOOL SetBitmap( const XBitmap *);
      BOOL SetText( const char * p, const ULONG len = 0);
      BOOL SetData( const void * p, const XAtom&);
};

#endif
