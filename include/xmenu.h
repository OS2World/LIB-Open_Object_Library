#ifndef __OOL_XMENU_H__
#define __OOL_XMENU_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMenu                                 */
/*                       derived from: XWindow                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xwindow.h"

class XMenuItem;

class _export_ XMenu: public XWindow
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   protected:
      virtual BOOL IsPopup(void) const { return FALSE; }
   public:
      XMenu( const XMenu * m = NULL);
      virtual ~XMenu() {;}
      void CascadeItem( const SHORT itemID);
      void CheckItem( const SHORT id, const BOOL check = TRUE );
      void DeleteItem( const SHORT id) const ;
      void EnableItem( const SHORT id, const BOOL enable = TRUE );
      void InsertItem( const XMenuItem * ins, XMenuItem * subMenu = NULL);
      USHORT GetDefaultItemID( void ) const ;
      BOOL GetItem( XMenuItem *itm, const SHORT id);
      void GetItemText( const USHORT itemID, XString * buffer);
      SHORT GetItemCount( void ) const ;
      SHORT GetItemID( const SHORT pos) const ;
      SHORT GetItemPos( const USHORT itemID) const ;
      void RemoveItem( const SHORT id ) const ;
      void SelectItem( const USHORT itemID) const ;
      void SetDefaultItemID( const USHORT itemID ) const ;
      void SetItemHandle( const USHORT itemID, const ULONG handle);
      void SetItemText( const SHORT id, const char * text) const ;
      BOOL UpdateItem( const XMenuItem *);
};

#endif
