#include "XCntrevn.h"
#include "XDragEvn.h"
#include "XWindow.h"
#include "XPoint.h"
#include "XBitmap.h"
#include "XDragItm.h"
#include "XMousevn.h"
#include "XCntEvnt.h"
#include "XKeyEvnt.h"
#include "XContObj.h"
#include "XcnDrEvn.h"
#include "XCnEdEvn.h"
#include "xString.h"
#include "XContCol.h"
#include "XItmDEvn.h"
#include "XBackEvn.h "
#include "xnbevent.h"


#include <stdlib.h>

#ifdef OOL_FOR_OS2_X86
#define GBN_BUTTONUP      0x0524
#define GBN_BUTTONDOWN    0x0525
#endif


/*@
@class XEvent
@parent XObject
@type overview
@symbol _
*/


/*@ XContainerEvent::GetObject(void)
@remarks Returns the object which posted the event
@returns       XContainerObject * the Object (can be NULL)
*/
XContainerObject * XContainerEvent::GetObject(void)
{
   if (core)
   {
      RECORDCORE *pr = (RECORDCORE *) ((PBYTE) core + sizeof(RECORDCORE));
      XContainerObject *obj;

      memcpy(&obj, pr, 4);
      return obj;
   }
   else
      return NULL;
}


/*@
@class XNoteBookEvent
@parent XEvent
@type overview
@symbol _
@remarks XNoteBookEvent is posted for event in a XNoteBook. Possible event-idïs are:
<UL>
<LI>XNOTEBOOK_PAGESELECTED
<LI>XNOTEBOOK_PAGESELECTEDPENDING
<LI>XNOTEBOOK_HELP
<LI>XNOTEBOOK_PAGEDELETED
<LI>XNOTEBOOK_NEWPAGESIZE
</UL>
Except of the case XNOTEBOOK_NEWPAGESIZE you can retrieve the depending notebook-page
by calling XNoteBookPage::GetPage().
@new
*/

/*@ XNoteBookEvent::GetPage()
@remarks Retrive the related page
@returns XNoteBookPage* pointer to the page (can be NULL)
*/

XNoteBookEvent :: XNoteBookEvent( const XWindow * w, const void * mp1, const void * mp2):XControlEvent( (XWindow*) w, mp1, mp2)
{
   eventID = SHORT2FROMMP(mp1);
   HWND page = 0;
   switch( eventID )
   {
      case XNOTEBOOK_PAGESELECTED:
      case XNOTEBOOK_PAGESELECTEDPENDING:
         {
            PAGESELECTNOTIFY * notify = (PAGESELECTNOTIFY *) mp2;
            ULONG pageID = (ULONG) notify->ulPageIdNew;
            page = (HWND) WinSendMsg( window->GetHandle(), BKM_QUERYPAGEWINDOWHWND, (MPARAM) pageID, 0);
         }
         break;
      case XNOTEBOOK_HELP:
         page = (HWND) WinSendMsg( window->GetHandle(), BKM_QUERYPAGEWINDOWHWND, (MPARAM) mp2, 0);
         break;
      case XNOTEBOOK_PAGEDELETED:
         {
            DELETENOTIFY * notify = (DELETENOTIFY*) mp2;
            page = notify->hwndPage;
         }
         break;
      case XNOTEBOOK_NEWPAGESIZE:
         break;
   }
   bookPage = (XNoteBookPage*) WinQueryWindowULong( page, QWL_USER);
}


XContainerEvent :: XContainerEvent(const XContainerControl * w, const MPARAM mp1, const MPARAM mp2):XControlEvent(SHORT2FROMMP(mp1))
{
   core = NULL;

   if (mp1)
   {
      windowID = SHORT1FROMMP(mp1);
      window = (XWindow *) w;
   }

   if (!window)
      return;

   eventID = SHORT2FROMMP(mp1);

   switch (eventID)
   {
   case CN_EMPHASIS:
      core = ((PNOTIFYRECORDEMPHASIS) mp2)->pRecord;
      break;
   case CN_ENTER:
      core = ((NOTIFYRECORDENTER *) mp2)->pRecord;
      break;
   case CN_INITDRAG:
      core = ((CNRDRAGINIT *) mp2)->pRecord;
      break;
   default:
      core = (PRECORDCORE) mp2;
   }
}


XContainerEditEvent :: XContainerEditEvent(const XWindow * w, const MPARAM mp1, const MPARAM mp2):XContainerEvent((XContainerControl *) w, mp1, mp2)
{
   cnEdit = (CNREDITDATA *) mp2;
   core = cnEdit->pRecord;
}



/*@ XContainerEditEvent::GetObject(void)
@remarks Returns a pointer to the object which is edited;
@returns XContainerObject * theObject
*/
XContainerObject * XContainerEditEvent::GetObject(void)
{
   RECORDCORE *pr = (RECORDCORE *) ((PBYTE) cnEdit->pRecord + sizeof(RECORDCORE));
   XContainerObject *obj;

   memcpy(&obj, pr, 4);
   return obj;
}


XContainerDragEvent :: XContainerDragEvent(const XWindow * w, const MPARAM mp1, const MPARAM mp2):XContainerEvent((XContainerControl *) w, mp1, mp2)
{
   printer = NULL;
   if (mp2)
   {
      core = ((CNRDRAGINFO *) mp2)->pRecord;
      dragInfo = ((CNRDRAGINFO *) mp2)->pDragInfo;
   }
}


BOOL XContainerDragEvent::GetDropPos(XPoint * p)
{
   if ((!(dragInfo)) || eventID != DRG_DROP)
      return FALSE;
   p->Set(dragInfo->xDrop, dragInfo->yDrop);
   return TRUE;
}


XContainerDragEvent :: ~XContainerDragEvent()
{
   if (dragInfo)
      DrgFreeDraginfo(dragInfo);
   dragInfo = NULL;
}


BOOL XContainerDragEvent::GetDragItem(XDragItem * itm, const SHORT index)
{
   if (!(dragInfo))
      return FALSE;
   if (index > dragInfo->cditem)
      return FALSE;

   DRAGITEM *pItem = DrgQueryDragitemPtr(dragInfo, index);

   itm->item = pItem;
   return TRUE;
}


XControlEvent :: XControlEvent(XWindow * w, const void *mp1, const void *mp2)
{
   windowID = SHORT1FROMMP(mp1);
   window = w->GetWindow(windowID);
   if (!window)
      return;

   char str[5];

   WinQueryClassName(window->GetHandle(), 5, (PCH) str);
   str[0] = ' ';
   SHORT type = atol(str);

   switch (type)
   {
   case 2:               // Combo
      switch (SHORT2FROMMP(mp1))
      {
      case CBN_LBSELECT:
         eventID = WIN_SELECTED;
         break;
      case CBN_ENTER:
         eventID = WIN_ENTER;
         break;
      case CBN_LBSCROLL:
         eventID = WIN_VSCROLL;
         break;
      case CBN_EFSCROLL:
         eventID = WIN_HSCROLL;
         break;
      case CBN_SHOWLIST:
         eventID = WIN_SHOWLIST;
         break;
      case CBN_EFCHANGE:
         eventID = WIN_CHANGED;
         break;
      }
      break;
   case 3:               // Button
      if (SHORT2FROMMP(mp1) == BN_CLICKED)
         eventID = WIN_CHANGED;
      break;
   case 6:               // Entry
      switch (SHORT2FROMMP(mp1))
      {
      case EN_CHANGE:
         eventID = WIN_CHANGED;
         break;
      case EN_SCROLL:
         eventID = WIN_HSCROLL;
         break;
      case EN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case EN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      }
      break;
   case 7:               // Listbox
      switch (SHORT2FROMMP(mp1))
      {
      case LN_SELECT:
         eventID = WIN_SELECTED;
         break;
      case LN_ENTER:
         eventID = WIN_ENTER;
         break;
      case LN_SCROLL:
         eventID = WIN_HSCROLL;
         break;
      case LN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case LN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      }
      break;
   case 10:               // MLE
      switch (SHORT2FROMMP(mp1))
      {
      case MLN_CHANGE:
         eventID = WIN_CHANGED;
         break;
      case MLN_VSCROLL:
         eventID = WIN_VSCROLL;
         break;
      case MLN_HSCROLL:
         eventID = WIN_HSCROLL;
         break;
      case MLN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case MLN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      }
      break;
   case 32:               // SpinButton
      switch (SHORT2FROMMP(mp1))
      {
      case SPBN_CHANGE:
         eventID = WIN_CHANGED;
         break;
      case SPBN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case SPBN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      case SPBN_UPARROW:
         eventID = WIN_UPARROW;
         break;
      case SPBN_DOWNARROW:
         eventID = WIN_DOWNARROW;
         break;
      case SPBN_ENDSPIN:
         eventID = WIN_ENDTRACK;
         break;
      }
      break;
   case 37:
      switch (SHORT2FROMMP(mp1))
      {
      case CN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      case CN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      }
      break;
   case 38:               // Slider
      switch (SHORT2FROMMP(mp1))
      {
      case SLN_CHANGE:
         eventID = WIN_CHANGED;
         break;
      case SLN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case SLN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      case SLN_SLIDERTRACK:
         eventID = WIN_TRACK;
         break;
      }
      break;
   case 39:               // Valueset
      switch (SHORT2FROMMP(mp1))
      {
      case VN_SELECT:
         eventID = WIN_SELECTED;
         break;
      case VN_ENTER:
         eventID = WIN_ENTER;
         break;
      case VN_SETFOCUS:
         eventID = WIN_SETFOCUS;
         break;
      case VN_KILLFOCUS:
         eventID = WIN_KILLFOCUS;
         break;
      }
      break;
   case 64:               // GraphicButton
      switch (SHORT2FROMMP(mp1))
      {
      case GBN_BUTTONDOWN:
      case GBN_BUTTONUP:
         eventID = WIN_CHANGED;
         break;
      }
      break;
   case 65:               // CircularSlider
      switch (SHORT2FROMMP(mp1))
      {
      case CSN_CHANGED:
         eventID = WIN_CHANGED;
         break;
      case CSN_SETFOCUS:
         {
            BOOL e = (BOOL) mp2;

            if (e)
               eventID = WIN_SETFOCUS;
            else
               eventID = WIN_KILLFOCUS;
         }
         break;
      case CSN_TRACKING:
         eventID = WIN_TRACK;
         break;
      }
   }
   window->DoControl(this);
}



/*@ XDragEvent::SetAcceptMode()
@remarks Set the operation supported by the application which receives
the drag-event
@parameters    SHORT accept              the opperation, possible values are
                                          <t 'ø' c=2>
                                             øDRG_DROP       øaccept the items
                                             øDRG_NODROP     ødont accept the items in this case
                                             øDRG_NEVERDROP  ønever accept the items
                                          </t>
*/


/*@ XDragEvent::SetOperation()
@remarks Set the operation supported by the application which generates
the drag-event
@parameters    SHORT operation           the opperation, possible values are
                                          <t 'ø' c=2>
                                             øDRG_COPY   øcopy the items
                                             øDRG_MOVE   ømove the items
                                             øDRG_LINK   ølink the items
                                          </t>
*/


XDragEvent :: ~XDragEvent()
{
   if (dragInfo)
      DrgFreeDraginfo(dragInfo);
   dragInfo = NULL;
}


XDragEvent :: XDragEvent(const LONG event, const void *v)
{
   printer = NULL;
   switch (event)
   {
   case DM_DROP:
      eventID = DRG_DROPPED;
      break;
   case DM_DRAGOVER:
      eventID = DRG_DRAGOVER;
      break;
   case DM_DISCARDOBJECT:
      eventID = DRG_DISCARDOBJECT;
      break;
   case DM_ENDCONVERSATION:
      eventID = DRG_ENDCONVERSATION;
      break;
   }

   dragInfo = NULL;
   DRAGINFO *info = (DRAGINFO *) v;

   if (DrgAccessDraginfo(info))
      dragInfo = info;
   accept = DRG_NODROP;
   operation = 0;
}


/*@ XDragEvent::GetDropPos()
@remarks Query the position where the objects were dropped
@parameters    XPoint * position         buffer which will get the position
*/
BOOL XDragEvent::GetDropPos(XPoint * p) const
{
   if ((!(dragInfo)) || eventID != DRG_DROP)
      return FALSE;
   p->Set(dragInfo->xDrop, dragInfo->yDrop);
   return TRUE;
}


/*@ XDragEvent::GetDragItem()
@remarks Query a dragitem.
@parameters
<t 'ø' c=2>
øXDragItem * øbuffer which will get the item-information<br>
øSHORT øzero-based index of item to query
</t>
*/
BOOL XDragEvent::GetDragItem(XDragItem * itm, const SHORT index)
{
   if (!(dragInfo))
      return FALSE;
   if (index > dragInfo->cditem)
      return FALSE;

   itm->item = DrgQueryDragitemPtr(dragInfo, index);
   return TRUE;
}


XMouseEvent :: XMouseEvent(const LONG e, const void *v1, const void *v2)
{
   switch (e)
   {
      case WM_BUTTON1DOWN:
         eventID = MOU_BTN1DOWN;
         break;
      case WM_BUTTON1DBLCLK:
         eventID = MOU_BTN1DBLCLICK;
         break;
      case WM_BUTTON1CLICK:
         eventID = MOU_BTN1CLICK;
         break;
      case WM_BUTTON1UP:
         eventID = MOU_BTN1UP;
         break;
      case WM_BUTTON2DOWN:
         eventID = MOU_BTN2DOWN;
         break;
      case WM_BUTTON2DBLCLK:
         eventID = MOU_BTN2DBLCLICK;
         break;
      case WM_BUTTON2CLICK:
         eventID = MOU_BTN2CLICK;
         break;
      case WM_BUTTON2UP:
         eventID = MOU_BTN2UP;
         break;
      case WM_BUTTON3DOWN:
         eventID = MOU_BTN3DOWN;
         break;
      case WM_BUTTON3DBLCLK:
         eventID = MOU_BTN3DBLCLICK;
         break;
      case WM_BUTTON3CLICK:
         eventID = MOU_BTN3CLICK;
         break;
      case WM_BUTTON3UP:
         eventID = MOU_BTN3UP;
         break;
      default:
         eventID = e;
         break;
   }

//   p.Set( (LONG) SHORT1FROMMP(v1), (LONG) SHORT2FROMMP(v1));
SHORT x = SHORT1FROMMP(v1);
SHORT y = SHORT2FROMMP(v1);
p.Set( x, y);
   keyboardState = SHORT2FROMMP((MPARAM) v2);
}


XItemDrawEvent :: XItemDrawEvent(const LONG msg, const void *mp1, const void *mp2, const SHORT t)
{
   type = t;
   eventID = msg;
   windowID = SHORT1FROMMP(mp1);
   item = (POWNERITEM) mp2;
   bColor = color = 0;
   textStyle = DT_VCENTER | DT_LEFT;
}


/*@ XItemDrawEvent::GetObject(void)
@group comtainer-related functions
@remarks Returns the object from which an item must be drawn. Only use
this function in container-controls!
@returns    XContainerObject *   the object
*/
XContainerObject * XItemDrawEvent::GetObject(void)
{
   CNRDRAWITEMINFO *info = (CNRDRAWITEMINFO *) item->hItem;

   if (!(info->pRecord))
      return NULL;
   RECORDCORE *pr = (RECORDCORE *) ((PBYTE) info->pRecord + sizeof(RECORDCORE));
   XContainerObject *obj;

   memcpy(&obj, pr, 4);
   return obj;
}


/*@ XItemDrawEvent::GetColumn(void)
@group comtainer-related functions
@remarks Returns the column from which an item must be drawn. Only use
this function in container-controls!
@returns    XContainerColumn *   the column
*/
XContainerColumn * XItemDrawEvent::GetColumn(void) const
{
   CNRDRAWITEMINFO *info = (CNRDRAWITEMINFO *) item->hItem;

   if (info->pFieldInfo)
      return (XContainerColumn *) info->pFieldInfo->pUserData;
   else
      return NULL;
}


/*@ XItemDrawEvent::DrawItem(const XBitmap * bitmap, const char *title, const BOOL drawOver)
@group drawing
@remarks Draws an item using a bitmap and/or text in
XListBox, XContainerControl and XMenuBar
@parameters <t 'ø' c=2>
            øXBitmap * bitmap     øA bitmap to display (can be NULL)
            øchar * text          øText to display (can be NULL)
            øBOOL drawOver        øIf FALSE (default) first the bitmap is drawn and the text is drawn right from the bitmap.
                                 If TRUE, the text is drawn over the bitmap.
            </t>
*/
void XItemDrawEvent::DrawItem(const XBitmap * bitmap, const char *title, const BOOL drawOver)
{
   POINTL p;

   GpiCreateLogColorTable(item->hps, LCOL_RESET, LCOLF_RGB, 0L, 0L, NULL);

   if(bColor == 0)
   {
      if (type == 4)
         WinFillRect(item->hps, &item->rclItem, SYSCLR_MENU);
      else if (type == 37)
      {
         if (item->fsAttribute & CRA_SELECTED)
            WinFillRect(item->hps, &item->rclItem, SYSCLR_HILITEBACKGROUND);
      }
      else
         WinFillRect(item->hps, &item->rclItem, (item->fsState == 1 ? SYSCLR_HILITEBACKGROUND : SYSCLR_ENTRYFIELD));

      if (item->fsAttribute & MIA_HILITED && type == 4)
      {
         GpiSetColor(item->hps, SYSCLR_BUTTONDARK);
         p.x = item->rclItem.xLeft + 2;
         p.y = item->rclItem.yBottom;
         GpiMove(item->hps, &p);
         p.y = item->rclItem.yTop - 1;
         GpiLine(item->hps, &p);
         p.x = item->rclItem.xRight - item->rclItem.xLeft - 1;
         GpiLine(item->hps, &p);
         GpiSetColor(item->hps, SYSCLR_BUTTONLIGHT);
         p.y = item->rclItem.yBottom;
         GpiLine(item->hps, &p);
         p.x = item->rclItem.xLeft + 2;
         GpiLine(item->hps, &p);

         item->rclItem.xLeft += 3;
      }
   }
   else
      WinFillRect(item->hps, &item->rclItem, bColor);

   if ((item->fsAttributeOld & MIA_CHECKED || item->fsAttribute & MIA_CHECKED) && type == 4)
   {
      p.y = (item->rclItem.yTop - item->rclItem.yBottom - bitmap->cy) / 2 + item->rclItem.yBottom - (item->fsAttribute & MIA_HILITED ? 2 : 0) + 1;
      p.x = item->rclItem.xLeft;

      HBITMAP hb = WinGetSysBitmap(HWND_DESKTOP, SBMP_MENUCHECK);

      WinDrawBitmap(item->hps, hb, NULL, (PPOINTL) & p, (item->fsAttribute & MIA_DISABLED ? SYSCLR_MENUDISABLEDTEXT : SYSCLR_OUTPUTTEXT), SYSCLR_MENU, DBM_NORMAL);   // /XColor(COL_PALEGRAY).
                                                                                                                              // GetColor()
      item->fsAttributeOld = (item->fsAttribute &= ~MIA_CHECKED);
      GpiDeleteBitmap(hb);
   }

   if (type == 4)
      item->rclItem.xLeft += 14;

   if (type == 37)
   {
      if (GetObject() && drawOver == FALSE)
         item->rclItem.xLeft += 12;
   }
   else if (drawOver == FALSE)
      item->rclItem.xLeft += 3;

   if (bitmap)
   {
      RECTL rect;

      rect.yBottom = (item->rclItem.yTop - item->rclItem.yBottom - bitmap->cy) / 2 + item->rclItem.yBottom - (item->fsAttribute & MIA_HILITED ? 2 : 0) + 1;
      rect.xLeft = item->rclItem.xLeft;
      if (type == 4)
         rect.yBottom += 1;
      rect.xRight = rect.xLeft + bitmap->width;
      rect.yTop = rect.yBottom + bitmap->height;
      WinDrawBitmap(item->hps, bitmap->hbm, NULL, (PPOINTL) &rect, 0, 0, DBM_NORMAL | DBM_STRETCH );
   }

   if (title)
   {
      if (bitmap && drawOver == FALSE)
         item->rclItem.xLeft += bitmap->cx + 3;
      if (item->fsAttribute & MIA_HILITED && type == 4)
      {
         item->rclItem.yBottom -= 2;
         item->rclItem.yTop -= 2;
      }
      LONG backCol, frontCol;

      switch (type)
      {
      case 4:
         frontCol = (item->fsAttribute & MIA_DISABLED ? SYSCLR_MENUDISABLEDTEXT : color);
         backCol = SYSCLR_MENU;
         break;
      case 37:
         frontCol = (item->fsAttribute & CRA_SELECTED ? SYSCLR_HILITEFOREGROUND : color);
         backCol = (item->fsAttribute & CRA_SELECTED ? SYSCLR_HILITEBACKGROUND : SYSCLR_WINDOW);
         break;
      default:
         frontCol = (item->fsState == 1 ? SYSCLR_HILITEFOREGROUND : color);
         backCol = (item->fsState == 1 ? SYSCLR_HILITEBACKGROUND : SYSCLR_ENTRYFIELD);
         break;
      }

      if( bColor )
         backCol = bColor;
        if( color )
            frontCol = color;

      if (textStyle & DT_WORDBREAK)
      {
         int drawn = 0, totalDrawn, length = strlen(title);
         int fontSize = 12;

         for (totalDrawn = 0; totalDrawn < length; item->rclItem.yTop -= (LONG) (fontSize * 1.3))
         {
            drawn = WinDrawText(item->hps, length - totalDrawn, (PSZ) title + totalDrawn, &item->rclItem, frontCol, backCol, textStyle);
            if (drawn)
               totalDrawn += drawn;
            else
               break;
         }
      }
      else
         WinDrawText(item->hps, strlen(title), (PCH) title, &item->rclItem, frontCol, backCol, textStyle);
   }

   if (type != 4)
      item->fsState = item->fsStateOld = 0;
   else
   {
      if (item->fsAttribute & MIA_HILITED)
         item->fsAttribute ^= MIA_HILITED;
      if (item->fsAttributeOld & MIA_HILITED)
         item->fsAttributeOld ^= MIA_HILITED;
   }
}


/*@
@class XBackgroundDrawEvent
@parent XEvent
@type overview
@symbol _
*/


/*@ XBackgroundDrawEvent::Draw(const XBitmap * bitmap)
@group drawing
@remarks Draws a bitmap
@parameters XBitmap * bitmap     bitmap to draw
*/
void XBackgroundDrawEvent::Draw(const XBitmap * bitmap)
{
   WinDrawBitmap(back->hps, bitmap->GetHandle(), (PRECTL) & back->rclBackground, (PPOINTL) & back->rclBackground, 0, 0, DBM_NORMAL);
}


////////////////////////docs only
/*@ XEvent::GetEventID()
@remarks GetEventID return the ID of the event which ocures. Valid
IDïs are specified by the classes derived from XEvent
@returns      ULONG theEventID
*/



/*@ XControlEvent::GetEventID()
@remarks Returns a pointer to the window which has send the event
@returns     LONG id                     the id of the event, see XControlEvent.
*/



/*@ XControlEvent::GetWindow()
@remarks Returns a pointer to the window which has send the event
@returns     XWindow * thePointer    the pointer of the sending window, if you
                                     know the window type, you can typecast to
                                     the needed class
*/


/*@ XControlEvent::GetWindowID()
@remarks Returns the ID of the window which has send the event
@returns     LONG theWindowID        the ID of the sending window
*/


/*@ XMouseEvent::GetEventID()
@remarks Returns the ID of the mouse-event. To get a mouse-event you must
register a XMouseHandler!
@returns     LONG theID              the ID of the mouse-event, see XMouseEvent.
*/


/*@ XMouseEvent::GetKeyInfo()
@remarks Returns the state of the keyboard
@returns     SHORT keyInfo           information of the keyboard
*/


/*@
@class XContainerEditEvent
@parent XContainerEvent
@type overview
@symbol _
@remarks An XContainerEditEvent is catched with a XContainerHandler. This event occures
if the user edit the text of a container-item.
*/


/*@ XContainerEditEvent :: GetText( XString * s)
@remarks Returns the text of an edited item. On CON_REALLOC
the old text is returned, on CON_ENDEDIT the new text is avaible.
@parameters XString * buffer     buffer to hold the data
*/




/*@ XContainerEditEvent::GetColumn()
@remarks Returns a pointer to that column in which
an item is edited;
@returns XContainerColumn * theColumn
*/


/*@
@class XItemDrawEvent
@parent XEvent
@type overview
@symbol _
@remarks An XItemDrawEvent is catched with a XItemDrawHandler. This event occures
if a listbox, menu or container is set with the style OWNERDRAW. In this case the
items must be drwan by the application.
*/


/*@ XItemDrawEvent::GetItemHandle()
@group item-related
@remarks Returns a handle to the item which must redrawn. Only use
this function in listbox-controls! (There you can set the handle
with XListBox::SetItemhandle() )
@returns    ULONG theHandle
*/


/*@ XItemDrawEvent::GetWindowID()
@group misc
@remarks Returns the ID of the window.
@returns    LONG theID
*/


/*@ XItemDrawEvent::GetItemID()
@group item-related
@remarks Returns the ID of the item to draw. Donït use
this function in container-controls! (use XItemDrawEvent::GetObject() and XItemDrawEvent::GetColumn()
to find out what you have to draw )
@returns    LONG theID
*/


/*@ XItemDrawEvent::GetWindowHandle()
@remarks Returns the sytem-define window handle.
@returns    OOL_WINDOWHANDLE handle
*/


/*@ XItemDrawEvent::SetTextColor()
@group colors
@remarks Set the text color for non-selected items
@parameters XColor * color    new color
*/


/*@ XDragEvent::GetDragItemCount()
@remarks Return the count of drag-items of this dragevent
@returns       SHORT                     count of items
*/


/*@ XDragEvent::GetSourceWindow()
@remarks Query the system-define handle of the window where the objects
were dropped.
@returns:       OOL_WINDOWHANDLE          The system-defined window handle.
*/


/*@
@class XContainerDragEvent
@parent XContainerEvent
@type overview
@symbol _
@remarks Drag/drop events in a container generate a XContainerDragEvent which is derived
from XContainerEvent and has the same functionality like XDragEvent (see there for
further information).
*/


/*@ XContainerEvent::GetObject()
@remarks Query the object which belongs to the event.
@returns XContainerObject * theObject
*/


/*@
@class XContainerEvent
@parent XControlEvent
@type overview
@symbol _
@remarks Events in a container generate a XContainerEvent, if you want to catch these
events you must generate a XContainerHandler. Possible event-IDs are:
<t 'ø' 2>
   øCON_BEGINEDIT         øthe user start to edit a field
   øCON_COLLAPSTREE       øin tree-view the tree or a part of it is collapsed
   øCON_CONTEXTMENU       øa context-menu is requested
   øCON_DRAGOVER          øone or more objects fly over the container
   øCON_DROP              øone or more objects are dropped
   øCON_EMPHASIS          øthe emphasis of an item has changed
   øCON_ENTER             øENTER was pressed or a double-click with the mouse occured
   øCON_EXPANDTREE        øin tree-view the tree or a part of it is expanded
   øCON_INITDRAG          øa drag-operation is requested
   øCON_PAINTBACKGOUND    øthe background of the container must be redrawn
</t>
which you can get with XEvent::GetEventID(). In the cases of CON_BEGINEDIT, CON_ENDEDIT and CON_REALLOC
a event of the Type XContainerEditEvent is posted, in the case od CON_DROP and CON_DRAGOVER a XContainerDragEvent is posted, you can simple typecast to them.
*/


/*@
@class XControlEvent
@parent XEvent
@type overview
@symbol _
@remarks The XControlEvent is send to a XFrameWindow when the user has performed
some interaction with a client window of the frame window. If you have caught the XControlEvent
by overriding XFrameWindow::DoControl you can get information about the sending window
and the type (ID) of the event. Valid event-IDïs are:
<t 'ø' 2>
   øWIN_CHANGED          øthe content of the client has changed
   øWIN_DBLCLICK         øthe user double-clicked on the window
   øWIN_PAINT            øthe window will be redrawn
   øWIN_ENTER            øthe user pressed ENTER
   øWIN_SELECTED         øan item of the window was selected
   øWIN_VSCROLL          øthe window scrolls it contents
   øWIN_HSCROLL          øthe window scrolls it contents
   øWIN_SETFOCUS         øthe window recieves the focus
   øWIN_KILLFOCUS        øthe window lost the focus
   øWIN_SHOWLIST         øthe list of a XComboBox will be displayed
   øWIN_TRACK            øthe user tracks the window (in XSlider)
   øWIN_ENDTRACK         øthe user stopped tracking (in XSlider)
   øWIN_UPARROW          øthe user pressed the arrow "up" (in XSpinButton)
   øWIN_DOWNARROW        øthe user pressed the arrow "down" (in XSpinButton)
   øMEDIA_PLAYED         øa media-window has completed playing a file
   øMEDIA_PAUSED         øa media-window paused playing a file
   øMEDIA_REWINDED       øa media-window completed rewinding a file
   øMEDIA_STOPED         øa media-window stoped playing a file
</t>
*/


/*@
@class XDragEvent
@parent XEvent
@type overview
@symbol _
@remarks For drag-events a XDragEvent is generated, to catch them you need to install
a XDragHandler. Possible event-IDs are:
<t 'ø' 2>
   øDRG_DROPPED          øAn item was dropped.
   øDRG_DRAGOVER         øAn item fly over the window.
   øDRG_ENDCONVERSATION  øThe converation ends. No informations about dragitems avaible!
   øDRG_DISCARDOBJECT    øDelete the items(s).
   øDRG_PRINTOBJECT      øPrint the item(s). Use QueryPrinterInfo() to get information about the requested printer.
</t>
For drag-events in a container see:
<UL>
<LI>XContainerDragEvent
<LI>XContainerEvent
<LI>XContainerHandler
</UL>
*/


/*@
@class XMouseEvent
@parent XEvent
@type overview
@symbol _
@remarks To catch events from the mouse like moving using mouse-buttons etc you must
install a XMouseHandler. If you use this handler you recieve XMouseEvents
which contains information about the mouse-state. For drag/drop you donït
need a XMousehandler but a XDragHandler. Possible event-IDs are
<t 'ø' c=2>
   øMOU_BTN1CLICK      øbutton 1 clicked
   øMOU_BTN1DBLCLICK   øbutton 1 double-click
   øMOU_BTN1DOWN       øbutton 1 down
   øMOU_BTN1UP         øbutton 1 up
   øMOU_BTN2CLICK      øbutton 2 clicked
   øMOU_BTN2DBLCLICK   øbutton 2 double-click
   øMOU_BTN2DOWN       øbutton 2 down
   øMOU_BTN2UP         øbutton 2 up
   øMOU_BTN3CLICK      øbutton 3 clicked
   øMOU_BTN3DBLCLICK   øbutton 3 double-click
   øMOU_BTN3DOWN       øbutton 3 down
   øMOU_BTN3UP         øbutton 3 up
   øMOU_INITDRAG       øthe user requested a drag-operation
   øMOU_MOVE           ømouse moved
   øMOU_ENTER           ømouse entered the window (Warp 4)
   øMOU_EXIT           ømouse leaves the window (Warp 4)
</t>
*/


/*@
@class XKeyboardEvent
@parent XEvent
@type overview
@symbol _
@remarks A XKeyboardEvent represents a user input to the keyboard, to catch these
events install a XKeyboardHandler.<P>
XKeyboardEvent::GetEventID() returns the ASCII-code of the key which was pressed, with XKeyboardEvent::GetVirtualKey()
and XKeyboardEvent::GetScanCode() you receivemore information.
*/


/*@ XKeyboardEvent::GetVirtualKey()
@remarks Use this function to get the virtual key defined by the OS
@returns
SHORT key   virtual key, see OS/2-information
*/


/*@ XKeyboardEvent::GetScanCode()
@remarks Use this function to get the scancode
@returns SHORT code
*/

/*@ XKeyboardEvent::GetFlags()
@remarks Use this function to get the flags
@returns SHORT flags:
         <t 'ø' c=2>
            øXKC_KEYUP    øThe event is a key-up transition
            øXKC_PREVDOWN           øThe key has been previously down
             øXKC_LONEKEY           øIndicates if the key is pressed and released without any other keys
             øXKC_SHIFT             øThe SHIFT state is active
             øXKC_ALT           øThe ALT state is active
             øXKC_CTRL           øThe CTRL state was active
            </t>
            This values can be or-ed.
</t>
*/
