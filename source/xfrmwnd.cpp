#include "XFrmwnd.h"
#include "xexcept.h"
#include "XResLib.h"
#include "XMsgBox.h"
#include "XRes.h"
#include "XRect.h"
#include "XString.h"
#include "XFrame.h"
#include "XColor.h"
#include "XCombo.H"
#include "XMLE.h"
#include "XSlider.h"
#include "xapp.h"
#include "XEntry.h"
#include "XCheckBx.h"
#include "XPushBtn.h"
#include "XScrlBar.h"
#include "XSpinBtn.h"
#include "XListBox.h"
#include "XCircSld.h"
#include "XGroupBx.h"
#include "XSBitmap.h"
#include "XSIcon.h"
#include "XSText.h"
#include "XSFrame.h"
#include "XValuSet.h"
#include "XIcon.h"
#include "XRadio.h"
#include "XMenuBar.h"
#include "XContain.h"
#include "XProcess.h"
#include "Xcntrevn.h"
#include "XMedia.h"
#include "XTimer.h"
#include "XDragHdl.h"
#include "XDragEvn.h"
#include "XMousHdl.h"
#include "XMousevn.h"
#include "XWindow.h"
#include "XContHdl.h"
#include "XItmDHdl.h"
#include "XBackHdl.h"
#include "XStylHdl.h"
#include "XdefHdl.h"
#include "XTimer.h"
#include "XScrlWnd.h"
#include "XCntEvnt.h"
#include "XCnEdEvn.h"
#include "XCndrEvn.h"
#include "xkeyevnt.h"
#include "xkeybhdl.h"
#include "XItmDEvn.h"
#include "XBackEvn.h"
#include "XNoteBk.h"
#include "xnbevent.h"
#include "xnbhdlr.h"
#include "xcontobj.h"
#include "xcontcol.h"
#include "xlayout.h"
#include "xdragtra.h"
#include "xcolcont.h"

#include <stdlib.h>
#include <string.h>

LONG MAXX = 0;
LONG MAXY;
SHORT cxDlgFrame;
SHORT cyDlgFrame;
SHORT cxBorder;
SHORT cyBorder;
SHORT cxSizeBorder;
SHORT cySizeBorder;


MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled)
{
   handled = TRUE;

   switch (msg)
   {
      case 0x0601:
      {
         HWND hwnd;
         char className[50];
         HENUM enumWindow = WinBeginEnumWindows(w->GetHandle());
         while ((hwnd = WinGetNextWindow(enumWindow)) != 0)
         {
            WinQueryClassName(hwnd, 50, (PCH) className);
            if(strcmp(className, "ColorSelectClass") == 0)
            {
               XColorControl * x = (XColorControl *) WinQueryWindowULong(hwnd, QWL_USER+4);
               x->color = (ULONG) mp1;
               x->ColorSelected((ULONG) mp1);
               return 0;
            } /* end if */
         };
      }
      break;
//      case DM_DRAGERROR:
      case DM_RENDER:
      case DM_RENDERFILE:
      case DM_RENDERPREPARE:
      case DM_RENDERCOMPLETE:
         {
            for (int i = 0; i < w->handlers; i++)
            {
               if (w->regHandlers[i]->GetID() == OOL_DRAGHANDLER)
               {
                  XDragTransfer dev((PDRAGTRANSFER) mp1, msg, (ULONG) SHORT1FROMMP(mp2));
                  handled = ((XDragHandler *) w->regHandlers[i])->HandleRenderEvent(&dev);
                  return (MRESULT) handled;
               }
            }
         }
         return 0;
   case WM_TIMER:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_TIMER)
            {
               if (((XTimer *) w->regHandlers[i])->timerID == SHORT1FROMMP(mp1))
               {
                  ((XTimer *) w->regHandlers[i])->TimerEvent();
                  handled = TRUE;
                  return (MRESULT) handled;
               }
            }
         }
      }
      break;
   case WM_HELP:
      if (SHORT1FROMMR(WinSendMsg(WinQueryHelpInstance(w->winhandle), HM_DISPLAY_HELP, MPFROM2SHORT(WinQueryWindowUShort(WinQueryFocus(HWND_DESKTOP), QWS_ID), NULL), MPFROMSHORT(HM_RESOURCEID))) != 0)
      {
         if ((LONG) mp1 > 0)
         {
            if (SHORT1FROMMR(WinSendMsg(WinQueryHelpInstance(w->winhandle), HM_DISPLAY_HELP, mp1, MPFROMSHORT(HM_RESOURCEID))) != 0)
               WinSendMsg(WinQueryHelpInstance(w->winhandle), HM_DISPLAY_HELP, MPFROM2SHORT(WinQueryWindowUShort(WinQueryWindow(WinQueryFocus(HWND_DESKTOP), QW_PARENT), QWS_ID), NULL), MPFROMSHORT(HM_RESOURCEID));
         }
         else
            WinSendMsg(WinQueryHelpInstance(w->winhandle), HM_DISPLAY_HELP, MPFROM2SHORT(WinQueryWindowUShort(WinQueryWindow(WinQueryFocus(HWND_DESKTOP), QW_PARENT), QWS_ID), NULL), MPFROMSHORT(HM_RESOURCEID));
      }
      return 0;
   case WM_CONTROL:
      switch (SHORT2FROMMP(mp1))
      {
         case CN_REALLOCPSZ:
         {
            CNREDITDATA * cnEdit = (CNREDITDATA *) mp2;
            if( cnEdit->pRecord && cnEdit->pFieldInfo == NULL)
            {
               RECORDCORE *pr = (RECORDCORE *) ((PBYTE) cnEdit->pRecord + sizeof(RECORDCORE));
               XContainerObject *obj;
               memcpy(&obj, pr, 4);
               XString dummy;
               dummy.GetBuffer( cnEdit->cbText +1 );
               dummy.ReleaseBuffer(cnEdit->cbText);
               for(int i = 0; i < cnEdit->cbText; i++)
                  dummy[i] = ' ';
               dummy[cnEdit->cbText] = 0;
               obj->SetTitle( dummy );
               return (MRESULT) TRUE;
            }
            else if( cnEdit->pRecord && cnEdit->pFieldInfo )
            {
               RECORDCORE *pr = (RECORDCORE *) ((PBYTE) cnEdit->pRecord + sizeof(RECORDCORE));
               XContainerObject *obj;
               memcpy(&obj, pr, 4);
               XContainerColumn * col = (XContainerColumn *) cnEdit->pFieldInfo->pUserData;
               return (MRESULT) obj->AllocMemory( (char*) *cnEdit->ppszText, cnEdit->cbText, col );
            }
            else if( cnEdit->pFieldInfo == NULL && cnEdit->pRecord == NULL && cnEdit->id == CID_CNRTITLEWND)
            {
               XContainerControl * cont = (XContainerControl *) WinQueryWindowULong( cnEdit->hwndCnr, 0);
               cont->title.GetBuffer( cnEdit->cbText +1 );
               cont->title.ReleaseBuffer(cnEdit->cbText);
               for(int i = 0; i < cnEdit->cbText; i++)
                  cont->title[i] = ' ';
               cont->title[cnEdit->cbText] = 0;
               return (MRESULT) TRUE;
            }
            else if( cnEdit->pFieldInfo && cnEdit->pRecord == NULL && (cnEdit->id == CID_LEFTCOLTITLEWND ||  cnEdit->id == CID_RIGHTCOLTITLEWND))
            {
               XContainerColumn * col = (XContainerColumn *) cnEdit->pFieldInfo->pUserData;
               col->title.GetBuffer( cnEdit->cbText +1 );
               col->title.ReleaseBuffer(cnEdit->cbText);
               for(int i = 0; i < cnEdit->cbText; i++)
                  col->title[i] = ' ';
               col->title[cnEdit->cbText] = 0;
               return (MRESULT) TRUE;
            }
         }
         break;
         case CN_ENDEDIT:
         {
            CNREDITDATA * cnEdit = (CNREDITDATA *) mp2;
            if( cnEdit->pFieldInfo == NULL && cnEdit->pRecord )
            {
               RECORDCORE *pr = (RECORDCORE *) ((PBYTE) cnEdit->pRecord + sizeof(RECORDCORE));
               XContainerObject *obj;
               memcpy(&obj, pr, 4);
               obj->TitleEdited( (char*) *cnEdit->ppszText );
               return (MRESULT) TRUE;
            }
            else if( cnEdit->pFieldInfo && cnEdit->pRecord )
            {
               RECORDCORE *pr = (RECORDCORE *) ((PBYTE) cnEdit->pRecord + sizeof(RECORDCORE));
               XContainerObject *obj;
               memcpy(&obj, pr, 4);
               XContainerColumn * col = (XContainerColumn *) cnEdit->pFieldInfo->pUserData;
               obj->TitleEdited( (char*) *cnEdit->ppszText, col );
               return (MRESULT) TRUE;
            }
            else if( cnEdit->pFieldInfo == NULL && cnEdit->pRecord == NULL && cnEdit->id == CID_CNRTITLEWND)
            {
               XContainerControl * cont = (XContainerControl *) WinQueryWindowULong( cnEdit->hwndCnr, 0);
               cont->TitleEdited( cont->title );
               return (MRESULT) TRUE;
            }
            else if( cnEdit->pFieldInfo && cnEdit->pRecord == NULL && (cnEdit->id == CID_LEFTCOLTITLEWND ||  cnEdit->id == CID_RIGHTCOLTITLEWND))
            {
               XContainerColumn * col = (XContainerColumn *) cnEdit->pFieldInfo->pUserData;
               col->TitleEdited( col->title );
               return (MRESULT) TRUE;
            }
         }
         break;
         case CN_SCROLL:
         {
            PNOTIFYSCROLL scroll = (PNOTIFYSCROLL) mp2;
            XContainerControl *x = (XContainerControl*) WinQueryWindowPtr(WinWindowFromID(w->GetHandle(), SHORT1FROMMP(mp1)), 0);
            if( scroll->fScroll & CMA_VERTICAL )
               x->DoVScroll(scroll->lScrollInc);
            else
               x->DoHScroll(scroll->lScrollInc);
            return 0;
         }
         case CN_BEGINEDIT:
         case CN_ENTER:
         case CN_CONTEXTMENU:
         case CN_DRAGOVER:
         case CN_INITDRAG:
         case CN_DROP:
         case CN_EMPHASIS:
         {
            SHORT i, ms = SHORT2FROMMP(mp1);
            XWindow *x = (XWindow *) WinQueryWindowPtr(WinWindowFromID(w->GetHandle(), SHORT1FROMMP(mp1)), 0);

            if (!x)
               break;

            for (i = 0; i < x->handlers; i++)
            {
               if (x->regHandlers[i]->GetID() == OOL_CONTAINERHANDLER)
               {
                  if (ms == CN_DRAGOVER || ms == CN_DROP)
                  {
                     XContainerDragEvent dev(x, mp1, mp2);

                     ((XContainerHandler *) x->regHandlers[i])->HandleEvent(&dev);
                     return MRFROM2SHORT(dev.accept, dev.operation);
                  }
                  else if (ms == CN_BEGINEDIT)// || ms == CN_ENDEDIT || ms == CN_REALLOCPSZ)
                  {
                     XContainerEditEvent dev(x, mp1, mp2);
                     if (((XContainerHandler *) x->regHandlers[i])->HandleEvent(&dev) == TRUE)
                        handled = TRUE;
                     else
                        handled = FALSE;
                     return (MRESULT) 0;
/*
                     if( ms == CN_REALLOCPSZ )
                     {
                       return (MRESULT) handled;
                     else
                       return (MRESULT) 0;
*/
                  }
                  else
                  {
                     XContainerEvent dev((XContainerControl *) x, mp1, mp2);

                     if (((XContainerHandler *) x->regHandlers[i])->HandleEvent(&dev) == TRUE)
                        handled = TRUE;
                     return 0;
                  }
               }
            }
         }
         break;

      case XNOTEBOOK_PAGESELECTED:
      case XNOTEBOOK_PAGESELECTEDPENDING:
      case XNOTEBOOK_HELP:
      case XNOTEBOOK_PAGEDELETED:
      case XNOTEBOOK_NEWPAGESIZE:
      {
            handled = FALSE;
            for (int i = 0; i < w->handlers; i++)
            {
               if (w->regHandlers[i]->GetID() == OOL_NOTEBOOKHANDLER)
               {
                  XNoteBookEvent e(w, (void*) mp1, (void*) mp2);
                  if (((XNoteBookHandler *) w->regHandlers[i])->HandleEvent(&e) == TRUE)
                     handled = TRUE;
               }
            }
            return (MRESULT) handled;
      }
      break;
      default:
         {
            XControlEvent e(w, mp1, mp2);
            handled = FALSE;
         }
      }
      return (MRESULT) handled;
   case WM_DRAWITEM:
      {
         handled = FALSE;
         SHORT i;
         POWNERITEM o = (POWNERITEM) mp2;
         XWindow *x = (XWindow *) WinQueryWindowPtr(o->hwnd, 0);

         if (x == 0)
         {
            HWND hwnd = WinQueryWindow(o->hwnd, QW_OWNER);

            while (hwnd)
            {
               x = (XWindow *) WinQueryWindowPtr(hwnd, 0);
               if (x)
                  break;
               hwnd = WinQueryWindow(hwnd, QW_OWNER);
            }
            if (x == 0)
               break;
         }
         for (i = 0; i < x->handlers; i++)
         {
            if (x->regHandlers[i]->GetID() == OOL_ITMDRAWHANDLER)
            {
               XItemDrawEvent dev(msg, mp1, mp2, ((XItemDrawHandler *) x->regHandlers[i])->type);
               if (((XItemDrawHandler *) x->regHandlers[i])->HandleEvent(&dev) == TRUE)
               {
                  handled = TRUE;
                  return (MRESULT) TRUE;
               }
            }
         }
      }
      break;
   case WM_COMMAND:
      if (w->DoCommand(SHORT1FROMMP(mp1)) == FALSE)
         handled = FALSE;
      return 0;
   case WM_PRESPARAMCHANGED:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_STYLEHANDLER)
            {
               XEvent evn((LONG) mp1);

               if (((XStyleHandler *) w->regHandlers[i])->HandleEvent(&evn) == TRUE)
                  handled = TRUE;
               return (MRESULT) handled;
            }
         }
      }
      break;
   case CM_PAINTBACKGROUND:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_BACKDRAWHANDLER)
            {
               XBackgroundDrawEvent dev(msg, mp1);

               if (((XBackgroundDrawHandler *) w->regHandlers[i])->HandleEvent(&dev) == TRUE)
                  handled = TRUE;
               return (MRESULT) handled;
            }
         }
      }
      return (MRESULT) FALSE;
   case WM_BUTTON1DOWN:
   case WM_BUTTON1DBLCLK:
   case WM_BUTTON1CLICK:
   case WM_BUTTON1UP:
   case WM_BUTTON2DOWN:
   case WM_BUTTON2DBLCLK:
   case WM_BUTTON2CLICK:
   case WM_BUTTON2UP:
   case WM_BUTTON3DOWN:
   case WM_BUTTON3DBLCLK:
   case WM_BUTTON3CLICK:
   case WM_BUTTON3UP:
   case WM_BEGINDRAG:
   case WM_MOUSEMOVE:
   case MOU_ENTER:
   case MOU_EXIT:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_MOUSEHANDLER)
            {
               XMouseEvent dev(msg, mp1, mp2);

               if((handled = ((XMouseHandler *) w->regHandlers[i])->HandleEvent(&dev)) == TRUE)
                  return (MRESULT) handled;
            }
         }
      }
      break;

   case WM_CHAR:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_KEYBHANDLER)
            {
               XKeyboardEvent dev(SHORT1FROMMP(mp2), SHORT2FROMMP(mp2), CHAR4FROMMP(mp1), SHORT1FROMMP(mp1));
               if (((XKeyboardHandler *) w->regHandlers[i])->HandleEvent(&dev) == TRUE)
               {
                  handled = TRUE;
                  return (MRESULT) TRUE;
               }
            }
         }
      }
      break;
   case DM_DISCARDOBJECT:
   case DM_PRINTOBJECT:
   case DM_ENDCONVERSATION:
      {
         char className[5];

         WinQueryClassName(w->GetHandle(), 5, (PCH) className);
         className[0] = ' ';
         SHORT classtype = atol(className);
         XContainerControl *cd = NULL;

         if (classtype == 37)
         {
            if (!(w))
               return (MRESULT) FALSE;
         }
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_DRAGHANDLER)
            {
               XDragEvent dev(msg, mp1);

               if (msg == DM_PRINTOBJECT)
                  dev.printer = (PPRINTDEST) mp2;
               ((XDragHandler *) w->regHandlers[i])->HandleEvent(&dev);
               return MRFROM2SHORT(dev.accept, dev.operation);
            }
            if (w->regHandlers[i]->GetID() == OOL_CONTAINERHANDLER)
            {
               cd = (XContainerControl *) ((XContainerHandler *) w->regHandlers[i])->GetWindow();
               if (msg == DM_DISCARDOBJECT || msg == DM_PRINTOBJECT)
               {
                  XContainerDragEvent dev(cd, NULL, NULL);

                  dev.dragInfo = (PDRAGINFO) mp1;
                  dev.window = cd;
                  dev.windowID = WinQueryWindowUShort(cd->GetHandle(), QWS_ID);
                  dev.eventID = msg;
                  if (msg == DM_PRINTOBJECT)
                     dev.printer = (PPRINTDEST) mp2;
                  ((XContainerHandler *) w->regHandlers[i])->HandleEvent(&dev);
               }
               else
               {
                  XContainerEvent dev(cd, mp1, mp2);

                  dev.eventID = msg;
                  handled = TRUE;
                  return (MRESULT) ((XContainerHandler *) w->regHandlers[i])->HandleEvent(&dev);
               }
            }
         }
      }
      return (MRESULT) FALSE;
   case DM_DROP:
   case DM_DRAGOVER:
      {
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_DRAGHANDLER)
            {
               XDragEvent dev(msg, mp1);

               ((XDragHandler *) w->regHandlers[i])->HandleEvent(&dev);
               return MRFROM2SHORT(dev.accept, dev.operation);
            }
         }
         handled = FALSE;
      }
      return (MRESULT) FALSE;
   case WM_MOVE:
      handled = FALSE;
      w->DoMove();
      break;
   case WM_SIZE:
      {
         XSize s(SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_LAYOUTHANDLER)
            {
               ((XLayout *) w->regHandlers[i])->Arrange(&s);
               return 0;
            } /* end if */
         }
         w->DoSize( &s);
         handled = FALSE;
      }
      return 0;
   case WM_DESTROY:
      return 0;
   case WM_PAINT:
      w->Draw();
      handled = FALSE;
      break;
   case WM_MEASUREITEM:
      {
         handled = FALSE;
         XWindow *x;
         HWND hwnd = WinWindowFromID(w->GetHandle(), (ULONG) SHORT1FROMMP(mp1));

         if (hwnd)
            return 0;

         POWNERITEM o = (POWNERITEM) mp2;

         x = (XWindow *) WinQueryWindowPtr(o->hwnd, 0);
         if (x == 0)
         {
            hwnd = WinQueryWindow(o->hwnd, QW_OWNER);
            while (hwnd)
            {
               x = (XWindow *) WinQueryWindowPtr(hwnd, 0);
               if (x)
                  break;
               hwnd = WinQueryWindow(hwnd, QW_OWNER);
            }
            if (x == 0)
               break;
         }
         for (int i = 0; i < x->handlers; i++)
         {
            if (x->regHandlers[i]->GetID() == OOL_ITMDRAWHANDLER)
            {
               XItemDrawHandler *d = (XItemDrawHandler *) x->regHandlers[i];

               handled = TRUE;
               o->rclItem.yTop = d->height;
               o->rclItem.xRight = d->width;
               return MRFROM2SHORT(d->width, d->height);
            }
         }
      }
      break;
   case MEDIA_NOTIFY:
      {
         XControlEvent evn(SHORT2FROMMP(mp1));
         w->DoControl(&evn);
      }
      return 0;
   default:
      {
         handled = FALSE;
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_DEFAULTHANDLER)
            {
               LONG retVal = 0;
               if(((XDefaultHandler *) w->regHandlers[i])->HandleEvent(msg, mp1, mp2, &retVal) == TRUE)
               {
                  handled = TRUE;
                  return (MRESULT) retVal;
               } /* end if */
            }
         }
      }
      handled = FALSE;
      break;
   }
   return (MRESULT) FALSE;
}


MRESULT HandleFrameDefault(XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled)
{
   handled = TRUE;

   switch (msg)
   {
      case WM_COMMAND:
         handled = TRUE;
         w->DoCommand(SHORT1FROMMP(mp1));
         return 0;
      case WM_INITMENU:
      {
//         XMenu *menu = (XMenu *) WinQueryWindowPtr((HWND) mp2, 0);

         w->InitMenu(w->GetMenu());//
         handled = FALSE;
      }
      return 0;
   case WM_CLOSE:
      {
         handled= TRUE;
         if (w->QueryForClose() == TRUE)
            delete w;
      }
      return 0;
   case WM_FOCUSCHANGE:
      {
         MRESULT res;
         res = WinDefWindowProc( w->winhandle, msg, mp1, mp2);
         w->FocusChanged( SHORT1FROMMP(mp2) );
         handled = FALSE;
         return res;
      }
      break;
   case WM_SIZE:
      {
         handled = TRUE;
         if( w->dlgHandle == 0)
            WinDefWindowProc( w->GetHandle(), msg, mp1, mp2);
         else
         {
            WinDefDlgProc( w->GetHandle(), msg, mp1, mp2);
            WinSetWindowPos(w->dlgHandle, 0, 0, 0, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2), SWP_SIZE);
         }
         XSize s(SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
         for (int i = 0; i < w->handlers; i++)
         {
            if (w->regHandlers[i]->GetID() == OOL_LAYOUTHANDLER)
               ((XLayout *) w->regHandlers[i])->Arrange(&s);
         }
         w->DoSize( &s);
      }
      return 0;
   }
   return HandleDefault(w, msg, mp1, mp2, handled);
}


MRESULT EXPENTRY WindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XFrameWindow *w;

   if (msg == WM_CREATE)
   {
//      WinSetWindowPtr(hwnd, 0, (void *) mp1);
//      mp1 = NULL;
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
   }
   else
      w = (XFrameWindow *) WinQueryWindowPtr(hwnd, 0);

   if (w )
   {
      if (w->enabled == FALSE && msg != WM_PAINT && msg != WM_CONTROLPOINTER && msg != WM_DRAWITEM && msg != WM_ERASEBACKGROUND && msg != WM_QUERYBORDERSIZE)
         return 0;

      BOOL handeld = FALSE;
      MRESULT mr = HandleFrameDefault(w, msg, mp1, mp2, handeld);
      if (handeld)
         return mr;
   }
   return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


MRESULT EXPENTRY DProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   XFrameWindow * w = (XFrameWindow *) WinQueryWindowPtr(hwnd, 0);

   if (w)
   {
      if (w->enabled == FALSE && msg != WM_PAINT && msg != WM_CONTROLPOINTER && msg != WM_DRAWITEM && msg != WM_ERASEBACKGROUND && msg != WM_QUERYBORDERSIZE && msg != WM_HELP)   // && msg != WM_CONTROL)
         return 0;
      BOOL handeld = FALSE;
      if(msg == WM_FOCUSCHANGE)
      {
         MRESULT res = WinDefDlgProc( hwnd, msg, mp1, mp2);
         w->FocusChanged( SHORT1FROMMP(mp2) );
         return res;
      }

      MRESULT mr = HandleFrameDefault(w, msg, mp1, mp2, handeld);

      if (handeld)
         return mr;
   }
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/*@
@class XFrameWindow
@parent XWindow
@type overview
@symbol _
@remarks XFrameWindow is a window class which has a child and a frame. It supports menus and toolbars.
*/


/*@ XFrameWindow::Activate(void)
@group display a window
@remarks Activate the window
*/
void XFrameWindow::Activate(void) const
{
   WinSetActiveWindow(HWND_DESKTOP, frame);
}

/*@ XFrameWindow::Minimize(void)
@group display a window
@remarks Minimize the window
*/
void XFrameWindow::Minimize(void) const
{
   WinSetWindowPos( frame, HWND_TOP, 0,0,0,0,SWP_MINIMIZE);
}

/*@ XFrameWindow::Maximize(void)
@group display a window
@remarks Maximize the window
*/
void XFrameWindow::Maximize(void) const
{
   WinSetWindowPos( frame, HWND_TOP, 0,0,0,0,SWP_MAXIMIZE);
}

/*@ XFrameWindow::Restore(void)
@group display a window
@remarks Restores the window if it is maximized or minimized
*/
void XFrameWindow::Restore(void) const
{
   WinSetWindowPos( frame, HWND_TOP, 0,0,0,0,SWP_RESTORE);
}

/*@ XFrameWindow :: ~XFrameWindow()
@group constructors/destructors
@remarks Destructors of windows are called automaticaly when the window is closed.
The destructor of XFrameWindow calles the destructor of every client window, menus and
toolbars. Also the destructors of handlers (derived classes of XHandler) which are
attached with it are called. If the last XFrameWindow of a process is closed, the process
will terminate.<BR>
If you want to close a window yourself, destruct the window with delete.
*/
XFrameWindow :: ~XFrameWindow()
{
   HWND hwnd;
   HENUM enumWindow;

   if( sEntry )
      WinRemoveSwitchEntry(sEntry);

   if (lockingWin)
   {
      lockingWin->enabled = TRUE;
      lockingWin->Enable();
      lockingWin = NULL;
   }

   WinSetWindowPtr(frameWin->winhandle, 0, NULL);
   WinSetWindowPtr(winhandle, 0, NULL);

   hwnd = WinWindowFromID(frame, FID_MENU);
   XMenuBar *m = (XMenuBar *) WinQueryWindowPtr(hwnd, 0);

   if (m)
      delete m;

   if (dlgHandle)
   {
      enumWindow = WinBeginEnumWindows(dlgHandle);
      while ((hwnd = WinGetNextWindow(enumWindow)) != 0)
      {
         XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

         if (w)
            delete w;
      };
      WinEndEnumWindows(enumWindow);
      WinDismissDlg(dlgHandle, 1);
      WinDestroyWindow(dlgHandle);
   }

   enumWindow = WinBeginEnumWindows(winhandle);
   char str[5];

   while ((hwnd = WinGetNextWindow(enumWindow)) != NULLHANDLE)
   {
      XWindow *w = (XWindow *) WinQueryWindowPtr(hwnd, 0);

      if (w && w != this)
      {
         WinQueryClassName(hwnd, 5, (PCH) str);
         if (strcmp(str, "#1") == 0)
         {
            XFrame *f = (XFrame *) w;
            if(f)
            {
/*               if (f->frame)
                  delete f->frame;
               else */
                  delete f;
            } /* end if */
         }
         else
            delete w;
      }
   }
   WinEndEnumWindows(enumWindow);

//   XApplication *p = (XApplication *) WinQueryWindowULong(winhandle, 4);

   XApplication::GetApplication()->RemoveWindow(this);      // test

   delete frameWin;

   if (clientWindow)
      delete clientWindow;

   WinDestroyWindow(frame);
}


/*@ XFrameWindow::ShowModal(XFrameWindow * w)
@group display a window
@remarks ShowModal displays a window modal for another window which is specified in the first parameter,
that means that the user cannot perform any input with the specified window.
WARNING: you should only use this function for windows which are constructed with a resource template!
If you do so the operating system stops with executing the calling code until the window is closed,
otherwise the calling code is executed while the window is displayed.
@parameters XFrameWindow * theOwnerWindow   the window which is disabled until this window is closed
*/
void XFrameWindow::ShowModal(XFrameWindow * w)
{
   if (!w)
      return;

   lockingWin = w;
   w->enabled = FALSE;
   w->Enable(FALSE);

   if (dlgHandle)
      WinProcessDlg(dlgHandle);
}


/*@ XFrameWindow::InitMenu()
@group display a window
@remarks If a menu must be initialized this function is called. If you
want to setup the menu dynamicaly, override this function.
@parameters   XMenu * theMenu
*/


/*@ XFrameWindow::GetHandle()
@group misc
@remarks GetHandle returns the window-handle defined by the operating system. The handle is different
from the ID of the window and the pointers used by the OOL. In XFrameWindow the function returns:
<OL COMPACT>
<LI>if you have set a client window with function XFrameWindow::SetClient() the handle of the client
<LI>if the XFrameWindow displays a resource-defined window, the resource window handle is returned
<LI>on default, the handle of the client-rect is returned
</OL>
@returns       OOL_WINDOWHANDLE  theSystemHandle
*/
OOL_WINDOWHANDLE XFrameWindow::GetHandle() const
{
   if (clientWindow)
      return clientWindow->winhandle;
   return dlgHandle ? dlgHandle : winhandle;
}


/*@ XFrameWindow::Enable(const BOOL enable)
@group display a window
@remarks Enables/disables a window for user-input
@parameters  BOOL enable  enables/disables the window (default is TRUE)
*/
void XFrameWindow::Enable(const BOOL enable)
{
   WinEnableWindow(frame, enable);
}


/*@ XFrameWindow::EnableWindowUpdate(const BOOL enable)
@group drawing a window
@remarks This function can stop drawing the window contents until you allow drawing.
For complex windows there can be some performance-advantages.<BR>
<B>WARNING:</B> If you disable drawing of a window and attach a XToolBar to it, the position
and size of the toolbar can not set correctly.
@parameters    BOOL enable    enable/disable window drawing (default is TRUE)
*/
void XFrameWindow::EnableWindowUpdate(const BOOL enable) const
{
   WinEnableWindowUpdate(winhandle, enable);
}


/*@ XFrameWindow::FillBackground(void)
@group drawing a window
@remarks FillBackground fills the background in the color you have set with
SetBackgroundColor (default is black). Usualy you call this function when
you have overridden XFrameWindow::Draw(). You donït have to call this function if a resource defined
window is displayed or a window is set as client with XFrameWindow::SetClient().
*/
void XFrameWindow::FillBackground(void)
{
   RECTL rec;
   HPS hps = WinBeginPaint(winhandle, NULLHANDLE, &rec);

   GpiCreateLogColorTable(hps, LCOL_RESET, LCOLF_RGB, 0, 1, &backCol);
   WinFillRect(hps, &rec, (LONG) backCol);
   WinEndPaint(hps);
}


/*@ XFrameWindow::SetBackgroundColor(const XColor * col)
@group colors
@remarks With this function you set the color wich is used to fill the
background in XFrameWindow::FillBackground(). Default color is black. If you change
background color, you should do this before you start to insert
client-windows.
@parameters XColor * theNewColor
*/
BOOL XFrameWindow::SetBackgroundColor(const XColor * col)
{
   backCol = col->GetColor();
   if (dlgHandle)
      WinSetPresParam(dlgHandle, PP_BACKGROUNDCOLOR, 4, &backCol);
   return TRUE;
}


void XFrameWindow::GetBackgroundColor(XColor * rgb) const
{
   if (dlgHandle)
   {
      LONG c;

      WinQueryPresParam(dlgHandle, PP_BACKGROUNDCOLOR, 0, NULL, 4, &c, QPF_PURERGBCOLOR);
      rgb->SetColor(c);
   }
   else
      rgb->SetColor(backCol);
}

/*@ XFrameWindow::GetText(XString * buffer)
@group text functions
@remarks Query the title of the window
@parameters    XString * theBuffer       theBuffer will contain the title of the window
*/
void XFrameWindow::GetText(XString * buffer) const
{
   SHORT r = WinQueryWindowTextLength(frame);

   r = WinQueryWindowText(frame, r + 1, (PCH) buffer->GetBuffer(r + 1));
   buffer->ReleaseBuffer(r);
}


/*@ XFrameWindow::SetBottom(void)
@group size/position/order
@remarks Set the window to the bottom (in z-order)
*/
void XFrameWindow::SetBottom(void) const
{
   WinSetWindowPos(frame, HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER);
}


/*@ XFrameWindow::SetIcon(const XIcon * icon)
@group misc
@remarks Set the icon which is displayed in the button of the system menu.
@parameters    OOL_ICONHANDLE theNewIcon
*/
void XFrameWindow::SetIcon(const XIcon * icon)
{
   WinSendMsg(frame, WM_SETICON, MPFROMP(icon->GetHandle()), 0);
}


/*@ XFrameWindow::SetText(const char *buffer)
@group text functions
@remarks Set the Text which is displayed in the titlebar
@parameters    char * theNewText
*/
void XFrameWindow::SetText(const char *buffer)
{
   if( sEntry )
   {
      SWCNTRL switchEntry;
      WinQuerySwitchEntry( sEntry, &switchEntry );
      XString bu = buffer;
      if( bu.GetLength() > MAXNAMEL )
         bu = bu.Left( MAXNAMEL );
      strncpy( switchEntry.szSwtitle, (char*) bu, bu.GetLength() + 1);
      WinChangeSwitchEntry( sEntry, &switchEntry );
   }
   WinSetWindowText(frame, (PSZ) buffer);
}


/*@ XFrameWindow::SetTop(void)
@group size/position/order
@remarks Set the window to the top (in z-order)
*/
void XFrameWindow::SetTop(void) const
{
   WinSetWindowPos(frame, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER);
}


/*@ XFrameWindow::Show(const BOOL show)
@group size/position/order
@remarks Displayes a window. The window wil be activated an be shown at the topmost position
(in z-order). If the window is minimized the old position and size will be restored.
@parameters BOOL show            TRUE=show, FALSE=hide
*/
void XFrameWindow::Show(const BOOL show)
{
   if (show)
      WinSetWindowPos(frame, HWND_TOP, 0, 0, 0, 0, SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE | SWP_RESTORE);
   else
      WinSetWindowPos(frame, 0, 0, 0, 0, 0, SWP_HIDE);
}


/*@ XFrameWindow::SetClient(const XWindow * c)
@group misc
@remarks SetClient is used to set a window, for example a control like a XMultiLineEdit,
as a clientwindow of the XFrameWindow so sizing, moving and painting is automaticaly done.
Moving/Sizing automaticaly may fail if the client is a frame-window.
@parameters XWindow * theNewClient
*/
BOOL XFrameWindow::SetClient(const XWindow * c)
{
   if(c)
   {
   if (c->GetHandle() == 0)
      return FALSE;
   if (frameWin)
      frameWin->adds += 1;
   WinSetOwner(c->winhandle, frame);
   WinSetParent(c->winhandle, frame, FALSE);
   clientWindow = (XWindow *) c;
   SWP swp;

   WinQueryWindowPos(winhandle, &swp);
   WinSetWindowPos(c->winhandle, 0, swp.x, swp.y, swp.cx, swp.cy, SWP_SIZE | SWP_MOVE | SWP_SHOW);
   }
   else
      clientWindow = NULL;
   return TRUE;
}


void BuildChilds(HWND dlgHandle)
{
   HWND hwnd;
   char className[50];
   SHORT classtype;
   HENUM enumWindow = WinBeginEnumWindows(dlgHandle);
   XWindow *du;

   while ((hwnd = WinGetNextWindow(enumWindow)) != 0)
   {
      WinQueryClassName(hwnd, 50, (PCH) className);
      className[0] = ' ';
      classtype = atol(className);
      switch (classtype)
      {
      case 1:            // Frame
         du = new XFrame(hwnd);
         break;
      case 2:            // Combobox
         du = new XComboBox(hwnd);
         break;
      case 3:            // Button
         {
            ULONG style = WinQueryWindowULong(hwnd, QWL_STYLE);

            if (style & BS_AUTOCHECKBOX)
               du = new XCheckBox(hwnd);   // CheckBox
            else
            {
               if (style & BS_AUTORADIOBUTTON)   // RadioButton
                  du = new XRadioButton(hwnd);
               else
                  du = new XPushButton(hwnd);   // PushButton
            }
         }
         break;
      case 4:            // Menu
         du = new XMenuBar(hwnd);
         break;
      case 5:            // Static
         {
            ULONG style = WinQueryWindowULong(hwnd, QWL_STYLE);

            if (style & SS_GROUPBOX)
               du = new XGroupBox(hwnd);
            if (style & SS_TEXT)
               du = new XStaticText(hwnd);
            if (style & SS_ICON)
               du = new XStaticIcon(hwnd);
            if (style & SS_BITMAP)
               du = new XStaticBitmap(hwnd);
         }
         break;
      case 6:            // EntryField
         du = new XEntryField(hwnd);
         break;
      case 7:            // Listbox
         du = new XListBox(hwnd);
         break;
      case 8:            // ScrollBar
         du = new XScrollBar(hwnd);
         break;
      case 10:            // MLE
         du = new XMultiLineEdit(hwnd);
         break;
      case 32:            // SpinButton
         du = new XSpinButton(hwnd);
         break;
      case 37:            // Container
         du = new XContainerControl(hwnd);
         break;
      case 38:            // Slider
         du = new XSlider(hwnd);
         break;
      case 39:            // ValueSet
         du = new XValueSet(hwnd);
         break;
      case 40:            // Notebook
         du = new XNoteBook(hwnd);
         break;
/*
                 case 64:                                                   //GraphicButton
                    du = new XGraphicButton( hwnd ); break;
*/
      case 65:            // CircularSlider
         du = new XCircularSlider(hwnd);
         break;
      }
   }
   WinEndEnumWindows(enumWindow);
}


/*@ XFrameWindow::GetMenu(void)
@group menu functions
@remarks Retrieve a pointer to the windows main-window
@returns XMenuBar* a pointer to the menuBar
@fixed
*/
XMenuBar * XFrameWindow::GetMenu(void) const
{
   HWND hwnd = WinWindowFromID( WinQueryWindow(GetHandle(), QW_PARENT), FID_MENU);
     return (XMenuBar *) WinQueryWindowPtr( hwnd, 0);
}


void GetScreenValues()
{
   MAXX = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
   MAXY = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
   cxDlgFrame = WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME);
   cyDlgFrame = WinQuerySysValue(HWND_DESKTOP, SV_CYDLGFRAME);
   cxBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXBORDER);
   cyBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYBORDER);
   cxSizeBorder = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   cySizeBorder = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
}


/*@ XFrameWindow :: XFrameWindow(const XResource * id, const char *title, const ULONG style, const XRect * rec, const XFrameWindow * parent, const BOOL build, const BOOL animate)
@group constructors/destructors
@remarks Construct a frame-window
Note that destructors of windows are called automaticaly when a window is closed! (see ~XFrameWindow)
@parameters <t 'ø' c=2>
            øXResource * resource        øa XResource contains two informations, an ID and a pointer
                                        to a XResourceLibrary. If you want to create a window out of
                                        a resourcefile you must specify the ID (otherwise it can be zero)
                                        and the XResourceLibrary which contains the window-resource.
                                        The window which is created always belongs to the process who
                                        owns the resource library, so if you work with multiple processes
                                        every process must have its own resource library.
            øchar * theTitle             øThe title of the window which is displayed in the titlebar
            øULONG style                 øYou can specify the style of the window with the following defines,
                                        which can be or-ed:
<BR>
FRM_TITLEBAR the window gets a titlebar
<BR>
FRM_SYSMENU the window gets the system menu
<BR>
FRM_MINBUTTON the titlebar get a button to minimize the window
<BR>
                                           FRM_MAXBUTTON the titlebar get a button to maximize the window
<BR>
                                           FRM_CENTER the window is created in the midle of the workplace
<BR>
                                           FRM_SIZEBORDER the windowsize can be changed by the user
<BR>
                                           FRM_DIALOGBORDER the window gets a thick border
<BR>
                                           FRM_BORDER the window gets a thin border
<BR>
                                           FRM_TASKLIST the window is displayed in the tasklist
<BR>
                                           FRM_NOMOVEWITHOWNER the window dontït move when the parent is moved
<BR>
                                           FRM_ICON  the window get an icon wich is identified by theResourceID,                                                            if the icon is not found in the resource-library, an error ocurses
<BR>
                                           FRM_ACCELTABLE an acceltable will be loaded from the resources with the windows id.
<BR>
                                           FRM_SYSMODAL the window is displayed system-modal
<BR>
FRM_SCREENALIGN
<BR>
FRM_MOUSEALIGN
<BR>
FRM_HIDEBUTTON
<BR>
FRM_HIDEMAX
<BR>
FRM_AUTOICON
<BR>
there are three static member-variables for default styles
<BR>
defaultStyle default setting for a framewindow
<BR>
defaultClientStyle default setting for windows wich are displayed as a clientwindow of a framewindow
<BR>
defaultDialogStyle default setting for windows wich are displayed as a dialog
<BR>
                                        Default is defaultStyle.
            øXRect * theRectangle        øOn default a window is created with length and hight of zero. Windows
                                        which are created with an resource template get the size of the template.
                                        Default is NULL.
                                        If theRectangle is specified, the window gets the size of it.
            øXFrameWindow * parent       øIf parent is specified the window is a client of the parent. The
                                        behavior depends on the styles you have set.
                                        Default is NULL.
            øBOOL buildFromResource      øIf this variable is set OOL try to build the window with a resource
                                        template which is identified by theResourceID. If the template is
                                        not found, an error ocurses.
                                        Default is FALSE.
            øBOOL animate                øEnable/disable animation on window creation. Default is FALSE
            </t>
@exceptions   If the method fails an exception of the type XException is thrown.
@fixed
*/

XFrameWindow :: XFrameWindow(const XResource * id,
          const char *title,
          const ULONG style,
          const XRect& rec,
          const XFrameWindow * parent,
          const BOOL build,
          const BOOL animate)
{
   CreateWindow( id->GetID(), id->GetResourceLibrary(), title, style, &rec, parent, build, animate);
}


XFrameWindow :: XFrameWindow( const ULONG id,
                     const char * title,
                     const ULONG style,
                     const XRect& rec,
                     const XFrameWindow * parent,
                     const BOOL build,
                     const BOOL animate)
{
   CreateWindow( id, NULL, title, style, &rec, parent, build, animate);
}

void XFrameWindow :: CreateWindow( const ULONG id, const XResourceLibrary * lib,
                     const char * title,
                     const ULONG style,
                     const XRect * rec,
                     const XFrameWindow * parent,
                     const BOOL build,
                     const BOOL animate)
{
   HWND p = parent ? parent->winhandle : HWND_DESKTOP;
   SWP swp;
   LONG xRight = 0, yBottom = 0;
   ULONG st = style;

   memset(&swp, 0, sizeof(SWP));

   frame = 0;
   clientWindow = NULL;
   vert = horz = NULL;
   lockingWin = NULL;

   if (MAXX == 0)
      GetScreenValues();

   enabled = TRUE;
   dummy = 0;

   if (WinRegisterClass( XApplication::GetApplication()->hab, (PSZ) "OOL_FRAMEWND", (PFNWP) WindowProc, CS_SIZEREDRAW | CS_MOVENOTIFY | CS_SYNCPAINT, 4) == FALSE)
      OOLThrow("error creating framewindow - couldnït register class", -10);

   backCol = 0;
   dlgHandle = 0;

//   FRAMECDATA fcmd;

   BOOL sw = FALSE;
   if( st & FCF_TASKLIST)
   {
      sw = TRUE;
      st ^= FCF_TASKLIST;
   }
   FRAMECDATA fcmd;
   fcmd.cb = sizeof(fcmd);
   fcmd.flCreateFlags = st | FCF_NOBYTEALIGN;
   if(lib)
      fcmd.hmodResources = lib->GetModuleHandle();
   else
      fcmd.hmodResources = 0;

   fcmd.idResources = id;

   frame = WinCreateWindow(p, WC_FRAME, (PSZ) title, 0, 0, 0, 0, 0, p, HWND_TOP, id, &fcmd, NULL);
//   winhandle = WinCreateWindow(frame, (PSZ) "OOL_FRAMEWND", (PSZ) "", (animate ? WIN_ANIMATE : 0), 0, 0, 0, 0, 0, HWND_TOP, FID_CLIENT, this, NULL);
   winhandle = WinCreateWindow(frame, (PSZ) "OOL_FRAMEWND", (PSZ) "", (animate ? WIN_ANIMATE : 0), 0, 0, 0, 0, 0, HWND_TOP, FID_CLIENT, NULL, NULL);
   WinSetWindowULong(winhandle, QWL_USER, (ULONG) this);

   if( sw )
   {
       SWCNTRL switchEntry;
      switchEntry.hwnd = frame;
      switchEntry.hwndIcon = NULLHANDLE;
      switchEntry.hprog = NULLHANDLE;
      switchEntry.idProcess = 0;
      switchEntry.idSession = 0;
      switchEntry.uchVisibility = SWL_VISIBLE;
      switchEntry.fbJump = SWL_JUMPABLE;
      if(title)
      {
         XString bu = title;
         if( bu.GetLength() > MAXNAMEL )
            bu = bu.Left( MAXNAMEL );
         strncpy( switchEntry.szSwtitle, (char*) bu, bu.GetLength() + 1);
      }
      else
         switchEntry.szSwtitle[0] = 0;
      sEntry = WinAddSwitchEntry( &switchEntry );
      if (sEntry == NULLHANDLE)
         OOLThrow("error creating task-switch!", -10);
   }
   else
      sEntry = 0;

   if (frame == 0)
      OOLThrow("error creating framewindow - couldnït create frame, perhaps resources missing", -10);

   WinSetWindowULong(winhandle, 4, (LONG) XApplication::GetApplication()->GetApplication());

   frameWin = new XFrame(this);

   if (st & FRM_MENU)
   {
      HWND menu = WinWindowFromID( WinQueryWindow(GetHandle(), QW_PARENT), FID_MENU);
      XMenuBar *b = new XMenuBar(menu);
   }

   if (build)
   {
      XColor col(COL_PALEGRAY);

      SetBackgroundColor(&col);

      if(lib)
         dlgHandle = WinLoadDlg(winhandle, winhandle, (PFNWP) DProc, lib->GetModuleHandle(), id, NULL);
      else
         dlgHandle = WinLoadDlg(winhandle, winhandle, (PFNWP) DProc, XApplication::GetApplication()->GetResourceLibrary()->GetModuleHandle(), id, NULL);
      if (dlgHandle == 0)
      {
         OOLThrow("could not create window from resources!", 0);
         return;
      }
      WinSetWindowPtr(dlgHandle, 0, this);
/*******************************
      BuildChilds(dlgHandle);
************************************/
      SHORT x = 0, y = 0;

      if (st & FCF_TITLEBAR)
         y = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
      if (style & FCF_DLGBORDER)
      {
         x = cxDlgFrame;
         y += cyDlgFrame;
      }
      if (st & FCF_SIZEBORDER)
      {
         x = cxBorder;
         y += cyBorder;
      }
      if (st & FCF_BORDER)
      {
         x = cxSizeBorder;
         y += cySizeBorder;
      }

      x += x;

      WinQueryWindowPos(dlgHandle, &swp);

      WinSetWindowPos(dlgHandle, 0, 0, 0, 0, 0, SWP_MOVE | SWP_SHOW);
      swp.cx += x;
      swp.cy += y;
   }
   else
   {
      if (rec)
      {
         swp.cx = rec->GetWidth();
         swp.cy = rec->GetHeight();
      }
   }

   if (st & FRM_CENTER)
   {
      if (dlgHandle)
      {
         xRight = MAXX / 2 - swp.cx / 2;
         yBottom = MAXY / 2 - swp.cy / 2;
      }
      else
      {
         if (rec)
         {
            xRight = MAXX / 2 - rec->GetWidth() / 2;
            yBottom = MAXY / 2 - rec->GetHeight() / 2;
         }
      }
   }
   else
   {
      if (rec)
      {
         xRight = rec->GetX();
         yBottom = rec->GetY();
      }
   }

   ULONG m = SWP_MOVE | SWP_SIZE | SWP_ZORDER;

   if (st & WIN_VISIBLE)
      m |= SWP_SHOW;
   WinSetWindowPos(frame, HWND_TOP, xRight, yBottom, swp.cx, swp.cy, m);
   XApplication::GetApplication()->AddWindow(this);
}


/*@ XFrameWindow::SetSize(const XRect * rect)
@group size/position/order
@remarks Set the size and/or position of a window
@parameters    XRect * theNewSize
*/
void XFrameWindow::SetSize(const XRect * rect) const
{
   WinSetWindowPos(frame, 0, rect->x, rect->y, rect->cx, rect->cy, SWP_SHOW | SWP_MOVE | SWP_SIZE);
}


/*@ XFrameWindow::SetSize(SetSize( const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height)
@group size/position/order
@remarks Set the size and/or position of a window
@parameters
<t 'ø' c=2>
øULONG øxPosition
øULONG øyPosition
øULONG øwidth
øULONG øheight
</t>
*/
void XFrameWindow::SetSize(const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const
{
   WinSetWindowPos(frame, 0, xPos, yPos, width, height, SWP_SHOW | SWP_MOVE | SWP_SIZE);
}


/*@ XFrameWindow::GetSize(XRect * rect)
@group size/position/order
@remarks Get the size and position of a window
@parameters    XRect * buffer to hold data
*/
void XFrameWindow::GetSize(XRect * rect) const
{
   SWP swp;

   WinQueryWindowPos(frame, &swp);
   rect->x = swp.x;
   rect->y = swp.y;
   rect->cx = swp.cx;
   rect->cy = swp.cy;
}


/*@ XFrameWindow::GetClientSize(XRect * rect)
@group size/position/order
@remarks Query the size of the client area
@parameters XRect * rect   buffer to hold the data
*/
void XFrameWindow::GetClientSize(XRect * rect) const
{
   XWindow::GetSize(rect);
   rect->SetX(0);
   rect->SetY(0);
}


/*@ XFrameWindow::GetWindow(const ULONG id)
@group misc
@remarks With GetWindow you can get a pointer of a child-window of XFrameWindow.
Usualy it is used if the XFrameWindow is created with a resource template.<BR>
Also you can get a pointer to windows which are created dynamicaly if you have specified an
ID in the constructors of these windows. In this case you donït need to store the pointers
in your program.
@parameters    ULONG theWindowID      the ID of the client window
@returns       XWindow * thePointer   the pointer to the window you have asked for. If you
                                      know the type of the window, you can make a typcast to
                                      the needed class.
*/
XWindow * XFrameWindow::GetWindow(const ULONG id)
{
   XWindow *win = XWindow::GetWindow(id);

   if (win)
      return win;
   if (clientWindow)
   {
      if (WinQueryWindowUShort(clientWindow->winhandle, QWS_ID) == id)
         return clientWindow;
   }
   return NULL;
}


/*@ XFrameWindow::QueryForClose()
@group misc
@remarks If the user trys to close a XFrameWindow this function is called. If you want
to check if the window can be closed or not or to perform a security-check (for example
with XMessageBox), override this function. This function is not called if you destroy
the window with delete.
@returns       BOOL canBeClosed    return TRUE if the window can be closed, otherwise return FALSE
*/


/*@ XFrameWindow::DoCommand()
@group user input
@remarks If the user selected a menuitem (from XMenuBar or XPopupMenu) if the
user pressed a button of a toolbar which is attached to it the framewindow, this function
is called. To get the ID of the action the user requested, override this function.
Return TRUE if you have handled the command, otherwise return FALSE.
@parameters   LONG theCommandID    the ID of the menuitem/toolbar-button
@returns      BOOL handled
*/



/*@ XFrameWindow::Draw()
@group drawing a window
@remarks This function is called if a window or a part of it must be redrawn.
If you use a window which is not created from a resource template or you dontït
have set a client with SetClient(), you must override this function and call
FillBackground().
*/
