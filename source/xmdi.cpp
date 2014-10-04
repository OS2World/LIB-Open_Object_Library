#include "xmdiserv.h"
#include "xmdicli.h"
#include "xmenubar.h"
#include "xmenuitm.h"


#include <stdlib.h>

/*@
@class XMDIClient
@parent XFrameWindow
@type overview
@symbol _
@remarks XMDIClient is a client window class to use with XMDIServer, see there for further information.
*/


/*@
@class XMDIServer
@parent XFrameWindow
@type overview
@symbol _
@remarks XMDIServer is a window to use the multiple document interface (MDI).
The basic concept of MDI:
<UL>
<LI>An application opens only one main window.
<LI>In the client-region of the mainwindow for every opened document a
client window is opened. This clients cannot be moved out of the client area of
the main window.
<LI>Only the main window has a toolbar and/or menubar.
<LI>If a client-window is activated/recieves the focus it manipulates the menubar/toolbar
of the main window.
</UL>
XMDIServer is a window which does the needed work. Use XMDIClient for the client windows.
<P>XMDIServer has also some service-functions. It can arrange windows and can manage
a menu-area for you. From this reason your menu which you are using with XMDIServer
should contain the following code in the resources (you must make shure that you use the same
ID-values):
<PRE>

   #define IDM_WINDOW   17800
   #define IDM_TILE      17850
   #define IDM_CASCADE   17851
   #define IDM_NEXT_WIN   17852
   #define IDM_PREV_WIN   17853

   //your menu code is placed here

   SUBMENU "Windows", IDM_WINDOW
   BEGIN
      MENUITEM    "~Tile",       IDM_TILE      // show the windows tiled
      MENUITEM    "~Cascade",    IDM_CASCADE   // show the windows casecaded
      MENUITEM    "~Next",       IDM_NEXT_WIN  // show the next window
      MENUITEM    "~Prev",       IDM_PREV_WIN  // show the previous window
   END
</PRE>
To this submenu the XMDIServer attachs entrys for every opened client window.
<P>Sample 15 shows you the easy usage of this class.
*/
void XMDIClient :: Setup( const XMDIServer * p, const XRect * rect)
{
   ids = 0;
   owner = (XMDIServer*) p;
   owner->AddWindow(this);
   if(rect == NULL)
   {
      XRect rec;
      owner->GetClientSize(&rec);
      SetSize(&rec);
   }
   SetFocus();
}


BOOL XMDIClient :: DoSysCommand( USHORT syscom)
{
   if( syscom == SC_MAXIMIZE)
      owner->max = TRUE;
   else if( (syscom == SC_RESTORE || syscom == SC_MINIMIZE) && owner->max)
      owner->max = FALSE;
   return TRUE;
}


void XMDIClient :: SetText( const char * p)
{
   owner->UpdateMenuText( ids, p);
   XFrameWindow::SetText(p);
}


XMDIClient :: ~XMDIClient()
{
   owner->RemoveWindow(this);
}


void XMDIClient :: FocusChanged( BOOL set)
{
   if(set)
      owner->SetClientFocus(this);
}


void XMDIServer :: FocusChanged( BOOL set)
{
   if(set)
   {
      XWindow * win = GetFocusWindow();
      if(win)
         win->SetFocus();
   }
}


void XMDIServer :: ShowClient( XMDIClient * c, BOOL canSize)
{
   if(max)
   {
      EnableWindowUpdate( FALSE );
      WinSetWindowPos( c->frame, HWND_TOP, 0,0,0,0, SWP_RESTORE);
      WinSetWindowPos( c->frame, HWND_TOP, 0,0,0,0, SWP_MAXIMIZE|SWP_SHOW|SWP_ACTIVATE);
      EnableWindowUpdate( TRUE );
   }
   else if( canSize)
      c->Show();
}


void XMDIServer :: DoSize( XSize * s)
{
   if( focus )
      ShowClient(focus, FALSE);
}


void XMDIServer :: SetClientFocus( XMDIClient * c)
{
     XMenuBar * menu = GetMenu();

   if( c != focus )
   {
      if(focus && menu)
         menu->CheckItem( focus->GetID(), FALSE);

      focus = (XMDIClient*) c;

      if(focus && menu)
         menu->CheckItem( focus->GetID(), TRUE);

      if( c && menu)
         c->UpdateMenu( menu );
   }
}


//BOOL XMDIServer :: DoSysCommand(USHORT com)
BOOL XMDIServer :: QueryForClose()
{
//   if( com == SC_CLOSE)
   {
      while(counter > 0)
      {
         windowList[0]->Show();
         if( windowList[0]->QueryForClose() == FALSE)
            return FALSE;
         else
            delete windowList[0];
      }
   }
   return TRUE;
}


void XMDIServer :: UpdateMenuText( const int id, const char * p)
{
   XMenuBar * m = GetMenu();
   if( m )
       m->SetItemText( id, p);
}


void XMDIServer :: InitMenu( XMenu * menu)
{
   if( focus && menu)
   {
      focus->UpdateMenu( (XMenuBar*) menu );
      XMenuItem item;
      if(menu->GetItem(&item, 17800))
      {
         if( item.GetAttribute() & MIA_HILITED)
            menu->SelectItem(focus->GetID());
      }
   } /* end if */
}


void XMDIServer :: AddWindow( XMDIClient * c)
{
   counter += 1;
   ids ++;

   windowList = (XMDIClient**) realloc( windowList, counter * sizeof(void*));
   windowList[counter-1] = (XMDIClient*) c;

     XMenuBar * m = GetMenu();

   if( m )
   {
       if(counter > 1)
      {
         m->EnableItem( 17850, TRUE);
         m->EnableItem( 17851, TRUE);
         m->EnableItem( 17852, TRUE);
         m->EnableItem( 17853, TRUE);
      }
       XMenuItem item;
      if( m->GetItem(&item, 17800))
      {
         SHORT pos = item.GetItemCount();
           if(counter == 1)
         {
            XMenuItem sep;
            sep.SetPosition(pos);
            sep.SetID( 17900 );
            pos += 1;
            sep.SetStyle( MENS_SEPARATOR );
            item.InsertItem( &sep );
         }
         XMenuItem m;
         m.SetPosition( pos );
         m.SetID( 17900 + ids);
         c->SetID( 17900 + ids);
         XString buffer;
         c->GetText( &buffer );
         m.SetText( buffer );
         item.InsertItem( &m );
      }
   }
}


/*@ XMDIServer :: DoCommand(LONG com)
@group misc
@remarks Receive a command from the menu. If you overwrite
this method you must make shure that for the commands, which are not handled
by your window, are send to the XMDIServer, usualy done by calling XMDIServer::DoCommand(com)
*/
BOOL XMDIServer :: DoCommand(LONG com)
{
   switch( com )
   {
      case 17850:
         TileWindows();
         break;
      case 17851:
         CascadeWindows();
         break;
      case 17852:
         NextWindow();
         break;
      case 17853:
         PrevWindow();
         break;
      default:
         if( com > 17900 && com < 18000)
         {
            for(int i = 0; i < counter; i++)
            {
               if(windowList[i]->GetID() == com)
               {
                  ShowClient(windowList[i], TRUE);
                  return TRUE;
               }
            }
         }
         else if( focus )
            return focus->DoCommand(com);
         return FALSE;
      }
   return TRUE;
}


void XMDIServer :: PrevWindow(void)
{
   SHORT i;

   if( counter == 0)
      return;

   for (i = 0; i < counter; i++)
   {
      if (windowList[i] == focus)
      {
         if(i > 0 )
         {
            ShowClient(windowList[i-1]);
            windowList[i-1]->SetFocus();
         }
         else
         {
            ShowClient(windowList[counter-1]);
            windowList[counter-1]->SetFocus();
         }
         return;
      }
   }
}


void XMDIServer :: NextWindow(void)
{
   SHORT i;

   if( counter == 0)
      return;

   for (i = 0; i < counter; i++)
   {
      if (windowList[i] == focus)
      {
           if(i < counter - 1 )
         {
            ShowClient(windowList[i+1]);
            windowList[i+1]->SetFocus();
         }
         else
         {
            ShowClient(windowList[0]);
            windowList[0]->SetFocus();
         }
         return;
      }
   }
}


void XMDIServer :: RemoveWindow( XMDIClient * c)
{
   SHORT i;
   BOOL swap = FALSE;

   for (i = 0; i < counter; i++)
   {
      if (windowList[i] == c)
         swap = TRUE;
      if (swap == TRUE)
         windowList[i] = windowList[i + 1];
   }
   counter -= 1;

   focus = NULL;

     XMenuBar * m = GetMenu();

   if( m)
   {
       if(counter < 2)
      {
         m->EnableItem( 17850, FALSE);
         m->EnableItem( 17851, FALSE);
         m->EnableItem( 17852, FALSE);
         m->EnableItem( 17853, FALSE);
      }
       XMenuItem item;
      if( m->GetItem(&item, 17800))
      {
         item.RemoveItem( c->GetID());
          if(counter == 0)
            item.RemoveItem(17900);
      }
   }

   if(counter == 0 )
   {
      if( m )
         SetEmptyMenu( m );
   }
   else if( counter > 0)
      windowList[counter-1]->SetFocus();
}


void XMDIServer :: Setup(void)
{
   max = FALSE;
   windowList = NULL;
   counter = 0;
   focus = NULL;

   XColor color( COL_PALEGRAY);
   SetBackgroundColor( &color );
   Invalidate();

   XMenuBar * b = GetMenu();
   if(b)
   {
      b->EnableItem( 17850, FALSE);
      b->EnableItem( 17851, FALSE);
      b->EnableItem( 17852, FALSE);
      b->EnableItem( 17853, FALSE);
   }

   Activate();
}


/*@ XMDIServer :: XMDIServer( const XResource * r, const char * title, const LONG style, const XRect& rect)
@group constructors
@remarks see XFrameWindow for details
*/
XMDIServer :: XMDIServer( const XResource * r, const char * title, const LONG style, const XRect& rect): XFrameWindow( r, title, style, rect)
{
   Setup();
}


/*@ XMDIServer :: XMDIServer( const ULONG id, const char * title, const LONG style, const XRect& rect)
@group constructors
@remarks see XFrameWindow for details
*/
XMDIServer :: XMDIServer( const ULONG id, const char * title, const LONG style, const XRect& rect): XFrameWindow( id, title, style, rect)
{
   Setup();
}


void XMDIServer :: Init()
{
   XMenuBar *b = GetMenu();

   SetEmptyMenu( b );
}


XMDIServer :: ~XMDIServer()
{
//   free(windowList);
}


void XMDIServer :: CascadeWindows()
{
   if(counter == 0)
      return;

   EnableWindowUpdate(FALSE);

   XRect rect;
   GetClientSize( &rect );
   SHORT wins = counter - 1;

   SHORT height = rect.GetHeight() - wins * 25;
   SHORT width = rect.GetWidth() - wins * 25;
   XRect r( 0, rect.GetHeight()-height, width, height);
   for(int i = 0; i < counter; i ++)
   {
       if(i > 0)
      {
         r.SetX( r.GetX() + 25);
         r.SetY( r.GetY() - 25 );
      }
      windowList[i]->Show();
      windowList[i]->SetSize( &r );
      windowList[i]->SetTop();
   }
   EnableWindowUpdate();
   if( focus )
      focus->SetFocus();
   else
      windowList[counter-1]->SetFocus();
}


void XMDIServer :: TileWindows()
{
   if(counter == 0)
      return;

   EnableWindowUpdate(FALSE);

   XRect rect;
   GetClientSize( &rect );
   SHORT wins = counter % 2 + counter / 2;
   SHORT height = rect.GetHeight() / wins;
   SHORT width = rect.GetWidth() / 2;
   XRect r( 0, rect.GetHeight()-height, width, height);
   for(int i = 0; i < counter; i ++)
   {
        if( i % 2 && i > 0)
         r.SetX( width);
      else
         r.SetX( 0 );
      if( i % 2 == 0 && i > 0)
         r.SetY( r.GetY() - height );
      if( i == counter-1 && (i+1) % 2)
      {
         r.SetX(0);
         r.SetWidth( rect.GetWidth());
      }
      windowList[i]->Show();
      windowList[i]->SetSize( &r );
   }
   EnableWindowUpdate();
   if( focus )
      focus->SetFocus();
   else
      windowList[counter-1]->SetFocus();
}


void XMDIServer :: Draw( void )
{
   FillBackground( );
}


