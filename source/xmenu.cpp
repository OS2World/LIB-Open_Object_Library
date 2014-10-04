#include "XPopMenu.h"
#include "XMenuBar.h"
#include "XMenuItm.h"
#include "XPoint.h"
#include "XRes.h"
#include "XResLib.h"
#include "XFrmWnd.h"
#include "XString.h"
#include "xexcept.h"
#include "xdialog.h"

#include <stdlib.h>
#include <string.h>


/**** XMenuItem ***/

/*@
@class XMenuItem
@parent XMenu
@type overview
@symbol _
*/


/*@
@class XMenu
@parent XWindow
@type overview
@symbol _
*/

/*@ XMenuItem::GetText(XString * s)
@group text
@remarks Get the text of the item
@parameters XString * buffer to hold the data
*/
void XMenuItem::GetText(XString * s) const
{
   if (t)
   {
      SHORT l = strlen(t);

      memcpy(s->GetBuffer(l + 1), t, l + 1);
      s->ReleaseBuffer(l);
   }
}


XMenuItem :: XMenuItem()
{
   winhandle = 0;
   memset(&item, 0, sizeof(item));
   t = NULL;
}


XMenuItem :: XMenuItem(const char *text, SHORT const id, const LONG style, LONG const position, const BOOL isSeparator)
{
   memset(&item, 0, sizeof(item));
   winhandle = 0;
   if (isSeparator)
   {
      item.afStyle = MIS_SEPARATOR;
      item.iPosition = position;
      t = NULL;
   }
   else
   {
      t = (char *) malloc(strlen(text) + 1);
      strcpy(t, text);
      item.iPosition = position;
      item.afStyle = style;
      item.id = id;
   }
}


XMenuItem :: ~XMenuItem()
{
   if (t)
      free(t);
   winhandle = 0;
}


/*@ XMenuItem::SetText(const char *p)
@group text
@remarks Set the text of the item
@parameters XString * text to display
*/
void XMenuItem::SetText(const char *p)
{
   t = (char *) realloc(t, strlen(p) + 1);
   strcpy(t, p);
   winhandle = 0;
}


/**** XMenu ****/

BOOL XMenu::UpdateItem(const XMenuItem * item)
{
   return (BOOL) WinSendMsg(winhandle, MM_SETITEM, MPFROM2SHORT(0, TRUE), (MPARAM) & item->item);
}


void XMenu::SetItemHandle(const USHORT itemID, const ULONG handle)
{
   WinSendMsg(winhandle, MM_SETITEMHANDLE, MPFROMSHORT(itemID), (MPARAM) handle);
}


SHORT XMenu::GetItemPos(const USHORT itemID) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MM_ITEMPOSITIONFROMID, MPFROM2SHORT(itemID, TRUE), 0));
}


USHORT XMenu::GetDefaultItemID(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MM_QUERYDEFAULTITEMID, 0, 0));
}


void XMenu::SetDefaultItemID(const USHORT itemID) const
{
   WinSendMsg(winhandle, MM_SETDEFAULTITEMID, MPFROMSHORT(itemID), 0);
}


void XMenu::SelectItem(const USHORT itemID) const
{
   WinSendMsg(winhandle, MM_SELECTITEM, MPFROM2SHORT(itemID, TRUE), 0);
}


/*@ XMenu::GetItemText(const USHORT itemID, XString * buffer)
@group text functions
@remarks Get the text of an item
@parameters
<t 'ø' c=2>
øUSHORT øthe id of the requested item
øXString * øbuffer to hold the data
</t>
*/
void XMenu::GetItemText(const USHORT itemID, XString * buffer)
{
   SHORT r = SHORT1FROMMR(WinSendMsg(winhandle, MM_QUERYITEMTEXT, MPFROM2SHORT(itemID, 512), MPFROMP(buffer->GetBuffer(512))));

   buffer->ReleaseBuffer(r);
}


XMenu :: XMenu(const XMenu * m)
{
   if (m)
      winhandle = m->winhandle;
}


/*@ XMenu::CascadeItem(const SHORT itemID)
@group misc
@remarks If you call CascadeItem() a menuitem with a submenu becomes cascaded, that means
a button is inserted which enables the submenu. If you have made a menuitem cascaded
you must select a default-item from the submenu of that menuitem by calling CheckItem().
@parameters SHORT the menuItem to become cascade
*/
void XMenu::CascadeItem(const SHORT itemID)
{
   MENUITEM menuitem;
   ULONG style;

   WinSendMsg(winhandle, MM_QUERYITEM, MPFROM2SHORT(itemID, TRUE), MPFROMP(&menuitem));

   style = WinQueryWindowULong(menuitem.hwndSubMenu, QWL_STYLE);
   WinSetWindowULong(menuitem.hwndSubMenu, QWL_STYLE, style | MS_CONDITIONALCASCADE);
}


/*@ XMenu::GetItemCount(void)
@group misc
@remarks Returns the count of items
@returns SHORT itemCount
*/
SHORT XMenu::GetItemCount(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MM_QUERYITEMCOUNT, 0, 0));
}


/*@ XMenu::GetItemID(const SHORT pos)
@group item attributes
@remarks Get the ID of an item from its position
@parameters SHORT position of the item
@returns SHORT theID
*/
SHORT XMenu::GetItemID(const SHORT pos) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MM_ITEMIDFROMPOSITION, MPFROMSHORT(pos), 0));
}


/*@ XMenu::DeleteItem(const SHORT id)
@group inserting/removing items
@remarks Removes an item
@parameters SHORT the id of the item to delete
*/
void XMenu::DeleteItem(const SHORT id) const
{
   WinSendMsg(winhandle, MM_DELETEITEM, MPFROM2SHORT(id, TRUE), 0);
}


/*@ XMenu::GetItem(XMenuItem * it, const SHORT id)
@group misc
@remarks Get an item of a XMenuBar or XPopupMenu
@parameters
<t 'ø' c=2>
øXMenuItem * øbuffer to hold the information
øSHORT øthe ID of the requested menu-item
</t>
@returns    BOOL  success
*/
BOOL XMenu::GetItem(XMenuItem * it, const SHORT id)
{
   BOOL res = SHORT1FROMMR(WinSendMsg(winhandle, MM_QUERYITEM, MPFROM2SHORT(id, TRUE), MPFROMP(&it->item)));

   if (res)
   {
      SHORT l = SHORT1FROMMR(WinSendMsg(winhandle, MM_QUERYITEMTEXTLENGTH, MPFROM2SHORT(it->item.id, TRUE), 0));

      it->t = (char *) realloc(it->t, l + 1);
      WinSendMsg(winhandle, MM_QUERYITEMTEXT, MPFROM2SHORT(it->item.id, l + 1), (MPARAM) it->t);
      it->winhandle = it->item.hwndSubMenu;
   }
   return res;
}


/*@ XMenu::InsertItem(const XMenuItem * it, XMenuItem * subMenu)
@group inserting/removing items
@remarks inserting/removing items
@parameters
<t 'ø' c=2>
øXMenuItem * øitem to insert
øXMenuItem * øa submenu where to insert the item. If nessacary a submenu is created (default is NULL).
</t>
*/
void XMenu::InsertItem(const XMenuItem * it, XMenuItem * subMenu)
{
   if (subMenu)
   {
      if (subMenu->item.hwndSubMenu == 0)
      {
         subMenu->winhandle = subMenu->item.hwndSubMenu = WinCreateWindow(HWND_OBJECT, WC_MENU, (PSZ) "", WS_VISIBLE, 0, 0, 0, 0, HWND_OBJECT, HWND_TOP, 991, NULL, NULL);
         subMenu->item.afStyle |= MIS_SUBMENU;
         WinSendMsg(winhandle, MM_DELETEITEM, MPFROM2SHORT(subMenu->item.id, TRUE), 0);
         WinSendMsg(subMenu->winhandle, MM_INSERTITEM, MPFROMP(&it->item), MPFROMP(it->t));
         WinSendMsg(winhandle, MM_INSERTITEM, MPFROMP(&subMenu->item), MPFROMP(subMenu->t));
         return;
      }
      WinSendMsg(subMenu->item.hwndSubMenu, MM_INSERTITEM, MPFROMP(&it->item), MPFROMP(it->t));
   }
   else
      WinSendMsg(winhandle, MM_INSERTITEM, MPFROMP(&it->item), MPFROMP(it->t));
}


/*@ XMenu::EnableItem(const SHORT id, const BOOL enable)
@group item attributes
@remarks Enables/disables
@parameters
<t 'ø' c=2>
øSHORT øthe id of the requested item
øBOOL øenable  TRUE=enable, FALSE=disable
</t>
*/
void XMenu::EnableItem(const SHORT id, const BOOL enable)
{
   SHORT r = 0;

   if (enable == FALSE)
      r = MIA_DISABLED;

   WinSendMsg(winhandle, MM_SETITEMATTR, MPFROM2SHORT(id, TRUE), MPFROM2SHORT(MIA_DISABLED, r));
}


/*@ XMenu::RemoveItem(const SHORT itemId)
@group inserting/removing items
@remarks Removes an item
@parameters SHORT the id of the item to remove
*/
void XMenu::RemoveItem(const SHORT itemId) const
{
   WinSendMsg(winhandle, MM_REMOVEITEM, MPFROM2SHORT(itemId, TRUE), 0);
}


/*@ XMenu::SetItemText(const SHORT itemId, const char *text)
@group text functions
@remarks Set the text of an item
@parameters
<t 'ø' c=2>
øSHORT øitem-ID
øchar* øtext to display
</t>
*/
void XMenu::SetItemText(const SHORT itemId, const char *text) const
{
   WinSendMsg(winhandle, MM_SETITEMTEXT, MPFROM2SHORT(itemId, TRUE), (MPARAM) text);
}


/*@ XMenu::CheckItem(const SHORT itemId, const BOOL check)
@group item attributes
@remarks Set an item checked/not checked
@parameters
<t 'ø' c=2>
øSHORT øthe id of the requested item
øBOOL øcheck  TRUE=check, FALSE=not checked
</t>
*/
void XMenu::CheckItem(const SHORT itemId, const BOOL check)
{
   SHORT r = MIA_CHECKED;

   if (check == FALSE)
      r = 0;

   WinSendMsg(winhandle, MM_SETITEMATTR, MPFROM2SHORT(itemId, TRUE), MPFROM2SHORT(MIA_CHECKED, r));
}


/*****  XPopupMenu *****/
/*@
@class XPopupMenu
@parent XMenu
@type overview
@symbol _
*/


void XPopupMenu :: CreateMenu( const XResourceLibrary * lib, const ULONG id, const XWindow * ow)
{
   owner = (XWindow*) ow;
   if( lib )
      winhandle = WinLoadMenu(ow->GetHandle(), lib->GetModuleHandle(), id);
   else
      winhandle = WinLoadMenu( ow->GetHandle(), (HMODULE) XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id);

   if (winhandle == 0)
      OOLThrow("error loading menu - resources may missing", -10);

   WinSetWindowPtr(winhandle, 0, this);
}


/*@ XPopupMenu :: XPopupMenu(XResource * res, XWindow * ow)
@remarks Creates a popup-menu, to show it call XPopupMenu::Display()
@parameters
<t c=2 'ø'>
øXResource * øthe resource which defines the menu
øXWindow * øthe owner-window
</t>
*/
XPopupMenu :: XPopupMenu( const XResource * res, const XWindow * ow):XMenu()
{
   CreateMenu( res->GetResourceLibrary(), res->GetID(), ow);
}


/*@ XPopupMenu :: XPopupMenu(XResource * res, XWindow * ow)
@remarks Creates a popup-menu, to show it call XPopupMenu::Display()
@parameters
<t c=2 'ø'>
øULONG             øID of the resource
øXWindow *        øthe owner-window
</t>
*/
XPopupMenu :: XPopupMenu( const ULONG id, const XWindow * ow):XMenu()
{
   CreateMenu( NULL, id, ow);
}

/*@ XPopupMenu::Display(XPoint * p, USHORT def, ULONG style)
@remarks shows a created popup-menu
@parameters  <t 'ø' c=2>
            øXPoint * point   øThe point where to show the menu in window-coordinates of the owner
             øUSHORT defID   øThe default-menuitem to display under the mouse. (default is 0)
             øULONG style      øStyle to display the menu
            </t>
*/
void XPopupMenu::Display( const XPoint * p, const USHORT def, const ULONG style) const
{
   LONG s = style;
   if(def)
      s |= PU_POSITIONONITEM;
   WinPopupMenu(owner->GetHandle(), owner->GetHandle(), winhandle, p->GetX(), p->GetY(), def, s);
}


/**** XMenuBar ****/
/*@
@class XMenuBar
@parent XMenu
@type overview
@symbol _
*/


void XMenuBar :: CreateMenu( const OOL_WINDOWHANDLE fr, const ULONG id, const XResourceLibrary * lib)
{
   if(lib )
      winhandle = WinLoadMenu(fr, (HMODULE) lib->GetModuleHandle(), id);
   else
      winhandle = WinLoadMenu(fr, (HMODULE) XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id);

   if (winhandle == 0)
      OOLThrow("error loading menu - resources may missing", -10);

   WinSetWindowPtr(winhandle, 0, this);
   WinSetOwner(winhandle, fr);
   WinSendMsg(fr, WM_UPDATEFRAME, (MPARAM) FCF_MENU, (MPARAM) 0);
}


/*@ XMenuBar :: XMenuBar(const XFrameWindow * fr, const XResource * res)
@group constructors/destructors
@remarks Create a menubar
@parameters
<t 'ø' c=2>
øXFrameWindow * øThe owner-window which will get the menubar.
øXResource * øA resource which defines the menubar (resource-id and library to load from).
</t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XMenuBar :: XMenuBar(const XFrameWindow * fr, const XResource * res)
{
   CreateMenu( fr->frame, res->GetID(), res->GetResourceLibrary());
}


/*@ XMenuBar :: XMenuBar(const XFrameWindow * fr, ULONG )
@group constructors/destructors
@remarks Create a menubar
@parameters
<t 'ø' c=2>
øXFrameWindow * øThe owner-window which will get the menubar.
øULONG øID of the resource in the EXE-file
</t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XMenuBar :: XMenuBar( const XFrameWindow * fr, const ULONG id)
{
   CreateMenu( fr->frame, id, NULL);
}

/*@ XMenuBar :: XMenuBar(const XDialog * fr, XResource* )
@group constructors/destructors
@remarks Create a menubar
@parameters
<t 'ø' c=2>
øXDialog* øThe owner-window which will get the menubar.
øXResource* øA resource which defines the menubar (resource-id and library to load from).
</t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XMenuBar :: XMenuBar(const XDialog * fr, const XResource * res)
{
   CreateMenu( fr->GetHandle(), res->GetID(), res->GetResourceLibrary());
}


/*@ XMenuBar :: XMenuBar(const XDialog * fr, ULONG )
@group constructors/destructors
@remarks Create a menubar
@parameters
<t 'ø' c=2>
øXDialog* øThe owner-window which will get the menubar.
øULONG øID of the resource in the EXE-file
</t>
@exceptions   If the method fails an exception of the type XException is thrown.
*/
XMenuBar :: XMenuBar( const XDialog * fr, const ULONG id)
{
   CreateMenu( fr->GetHandle(), id, NULL);
}


XMenuBar :: XMenuBar(const LONG hwnd)
{
   winhandle = hwnd;
   WinSetWindowPtr(winhandle, 0, this);
}


XMenuBar :: ~XMenuBar()
{
}


/*@ XMenuBar::Show(const BOOL show)
@group misc
@remarks Show/hide a menubar
@parameters BOOL show   TRUE=show, FALSE=hide
*/
void XMenuBar::Show(const BOOL show)
{
   if (show)
      WinSetParent(winhandle, WinQueryWindow(winhandle, QW_OWNER), TRUE);
   else
      WinSetParent(winhandle, HWND_OBJECT, FALSE);
   WinSendMsg(WinQueryWindow(winhandle, QW_OWNER), WM_UPDATEFRAME, (MPARAM) FCF_MENU, 0);
}


//docs
/*@ XMenuItem::GetID()
@group ID
@remarks returns the ID
@returns ULONG
*/

/*@ XMenuItem::GetAttribute()
@group attributes
@remarks returns the attribute
@returns ULONG, posssible (or'ed values):
<UL compact>
<LI>MENA_NODISMISS
<LI>MENA_FRAMED
<LI>MENA_CHECKED
<LI>MENA_DISABLED
<LI>MENA_HILITED
</UL>
*/


/*@ XMenuItem::GetStyle()
@group style
@remarks returns the style
@returns ULONG, posssible (or'ed values):
<UL compact>
<LI>MENS_TEXT
<LI>MENS_BITMAP
<LI>MENS_SEPARATOR
<LI>MENS_OWNERDRAW
<LI>MENS_SUBMENU
<LI>MENS_MULTMENU
<LI>MENS_SYSCOMMAND
<LI>MENS_HELP
<LI>MENS_STATIC
<LI>MENS_BUTTONSEPARATOR
<LI>MENS_BREAK
<LI>MENS_BREAKSEPARATOR
<LI>MENS_GROUP
<LI>MENS_SINGLE
</UL>
*/

/*@ XMenuItem::GetPosition()
@group position
@remarks returns the position
@returns ULONG
*/

/*@ XMenuItem::SetPosition(SHORT)
@group position
@remarks set the position
@parameters SHORT position
*/

/*@ XMenuItem::SetID(SHORT)
@group ID
@remarks set the ID
@parameters SHORT the ID
*/

/*@ XMenuItem::SetAttribute(LONG)
@group attributes
@remarks set the attributes
@parameters LONG (or'ed) values like decribed in GetAttribute()
*/

/*@ XMenuItem::SetStyle(LONG)
@group style
@remarks set the style
@parameters LONG (or'ed) values like decribed in GetStyle()
*/