#ifndef __OOL_XMENUITEM_H__
#define __OOL_XMENUITEM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMenuItem                             */
/*                       derived from: XMenu                                 */
/*                        last update: 10/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "xmenu.h"

class XString;

#ifdef OOL_FOR_OS2_X86
   //styles
   #define MENS_TEXT               0x0001
   #define MENS_BITMAP            0x0002
   #define MENS_SEPARATOR         0x0004
   #define MENS_OWNERDRAW         0x0008
   #define MENS_SUBMENU            0x0010
   #define MENS_MULTMENU         0x0020
   #define MENS_SYSCOMMAND         0x0040
   #define MENS_HELP               0x0080
   #define MENS_STATIC            0x0100
   #define MENS_BUTTONSEPARATOR  0x0200
   #define MENS_BREAK            0x0400
   #define MENS_BREAKSEPARATOR   0x0800
   #define MENS_GROUP            0x1000
   #define MENS_SINGLE            0x2000
   //attributes
   #define MENA_NODISMISS         0x0020
   #define MENA_FRAMED            0x1000
   #define MENA_CHECKED            0x2000
   #define MENA_DISABLED         0x4000
   #define MENA_HILITED            0x8000
#endif

class _export_ XMenuItem: public XMenu
{
      friend class XMenu;
   private:
      char * t;
      MENUITEM item;
   public:
      XMenuItem();
      XMenuItem( const char* text, const SHORT id, const LONG style = MIS_TEXT, const LONG position = MIT_END, const BOOL isSeparator = FALSE);
      virtual ~XMenuItem();
      void GetText( XString * buffer)  const;
      ULONG GetAttribute(void) const { return item.afAttribute; }
      ULONG GetID( void ) const { return item.id; }
      ULONG GetPosition(void) const { return item.iPosition; }
      ULONG GetStyle(void) const { return item.afStyle; }
      void SetID( const SHORT id) { item.id = id; }
      void SetAttribute( const LONG attr) { item.afAttribute = attr; }
      void SetPosition( const SHORT pos ) { item.iPosition = pos; }
      void SetStyle( const ULONG style) { item.afStyle = style; }
      void SetText( const char * p);
};


#endif
