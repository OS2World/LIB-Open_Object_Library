#define DEBUG
#include "sample10.h"
#include "resource.h"
#include <stdlib.h>



class MyDragHandler: public XDragHandler
{
   public:
      MyDragHandler( XWindow * w): XDragHandler( w ) { ;}
      void HandleEvent(XDragEvent*);
};


/* here the method HandleEvent is overridden, so we can handle drag/drop events*/
void MyDragHandler :: HandleEvent ( XDragEvent * event)
{
   switch( event->GetEventID())
   {
      /* the mousepointer is over the MLE holding one or more objects */
      case DRG_DRAGOVER:
         {
            SHORT i;
            XDragItem item;
            /* ask for every object the type and format */
            for(i=0; i < event->GetDragItemCount(); i ++)
            {
               event->GetDragItem( &item, i);
               if(item.VerifyItemType( DRG_M_FILE ) == FALSE)
               {
                  /* no textfile, we will not accept these objects */
                  event->SetAcceptMode( DRG_NEVERDROP);
                  return;
               }
            }
            event->SetAcceptMode( DRG_DROP);       //ok, can be dropped
            event->SetOperation( DO_COPY);         //we will work with a copy
         }
         return;
      /* one or more object are dropped, because weïve tested the format (see above) */
      /* we donït have to test it here again, the object will be files */
      case DRG_DROP:
         {
            XDragItem item;
            XString name, container;
            SHORT i;

            /* query for every object the information we need */
            for(i=0; i < event->GetDragItemCount(); i++)
            {
               event->GetDragItem( &item, i);
               item.Accept();                        //let the sender know that we accept the object

               item.GetName( &name);               //query the filename
               item.GetContainerName( &container); //query the directory
               container+= name;                     //make a complete path
                                                   //get the parent window
               MyAppWindow * appWin = (MyAppWindow*) GetWindow();
               appWin->Load( container );        //load the file
            }
         }
         return;
      }
}


MyAppWindow :: MyAppWindow(char * filename): XScrollWindow( IDM_MAIN, "Sample10 - Picture viewer", XFrameWindow::defaultStyle | FRM_TASKLIST | FRM_MENU)
{
   init = TRUE;
   loaded = stretch = FALSE;

   //create a graphic device associated to the window
   graphWindow = new XGraphicDevice( this, FALSE, TRUE, PU_PELS, GPIA_ASSOC);

   //create a graphic device which resides in memory
   graphMemory = new XGraphicDevice( this, TRUE, TRUE, PU_PELS, GPIA_ASSOC | GPIT_MICRO);
   XColor color(COL_PALEGRAY);
   graphMemory->SetBackgroundColor(&color);
   graphWindow->SetBackgroundColor(&color);
   AddVertScroller( );
   AddHorzScroller( );

   //create a bitmap
   XPoint rp(0, 0);
   bmp = new XBitmap( &rp);

   //draw the objects ONLY ONE TIME
   graphMemory->FillBackground();

   XRect rect( 150, 150, 500, 500);
   SetSize(&rect);

   MyDragHandler * myDragHandler = new MyDragHandler(this);

   Activate();
   if(filename)
      Load(filename);
   init = FALSE;
}


void MyAppWindow :: CalcBitmap()
{
   XRect rect;
   GetSize(&rect);
   SetYScrollPos( 0 );
   SetXScrollPos( 0 );
   bmp->GetDimensions(&bmpSize);
   int offset = XWindow::GetSysValue(SV_CYHSCROLL) + XWindow::GetSysValue(SV_CYSIZEBORDER) * 2 + XWindow::GetSysValue(SV_CYMENU) + XWindow::GetSysValue(SV_CYTITLEBAR);
   rect.SetY( rect.GetY() + rect.GetHeight() - (bmpSize.GetHeight() + offset));
   rect.SetHeight(bmpSize.GetHeight() + offset);
   rect.SetWidth( bmpSize.GetWidth() + XWindow::GetSysValue(SV_CXSIZEBORDER) * 2 + XWindow::GetSysValue(SV_CXVSCROLL));
   SetSize(&rect);
   GetClientSize(&rect);

   if(rect.GetHeight() < bmpSize.GetHeight())
   {
      LONG diff = bmpSize.GetHeight() - rect.GetHeight();
      GetSize(&rect);
      rect.SetHeight( rect.GetHeight() + diff);
      rect.SetY( rect.GetY() - diff);
      SetSize(&rect);
   } /* end if */
   SetVirtualX( bmpSize.GetWidth());
   SetVirtualY( bmpSize.GetHeight());
   graphMemory->FillBackground();
   bmp->Draw(graphMemory);
   loaded = TRUE;
   Invalidate();
}


void MyAppWindow :: Load( char * filename)
{
   OOL_POINTERHANDLE handle = XWindow::GetPointerHandle();
   XWindow::SetPointer( XWindow::GetSystemPointerHandle(PTR_WAIT));
   try
   {
      XBitmapFilter* p = XBitmap :: GetImportFilterStruct (filename);
      XBitmap :: SetupImportFilter(this, filename, p);
      if (DID_OK == p->command)
          bmp->Load( filename, p);
      else
         return;
      CalcBitmap();
   } /* end try */
   catch( XException e )
   {
      XMessageBox((char*) e.GetErrorMessage(), (char*)"Error", MB_ERROR|MB_MOVEABLE|MB_OK, this);
   } /* end catch */
   XWindow::SetPointer(handle);
}


void MyAppWindow :: SetBitmapPos()
{
   GetClientSize(&clientRect);
   SetXPageSize( clientRect.GetWidth());
   SetYPageSize( clientRect.GetHeight());
   Invalidate();
}


void MyAppWindow :: VScroll( LONG y)
{
   if(init == FALSE)
   {
      int pos = GetVertPos() + y;
      if(pos < 0 )
         pos = 0;
      if(pos > bmpSize.GetHeight() - clientRect.GetHeight())
         pos = bmpSize.GetHeight() - clientRect.GetHeight();
      SetYScrollPos( pos );
      Invalidate();
   } /* end if */
}


void MyAppWindow :: HScroll( LONG x)
{

   if(init == FALSE)
   {
      int pos = GetHorzPos() + x;
      if(pos < 0 )
         pos = 0;
      if(pos > bmpSize.GetWidth() - clientRect.GetWidth())
         pos = bmpSize.GetWidth() - clientRect.GetWidth();
      SetXScrollPos( pos );
      Invalidate();
   } /* end if */
}


void MyAppWindow :: Draw( void )
{
   //the content of the window has to be drawn
   if(graphWindow && graphMemory)
   {
      if(loaded == FALSE)
      {
         graphWindow->FillBackground();
         return;
      } /* end if */
      else if(stretch)
      {
         XRect rect = clientRect;
         rect.SetX(0);
         rect.SetY(0);
         XRect rect2;
         rect2.SetWidth(bmpSize.GetWidth());
         rect2.SetHeight(bmpSize.GetHeight());
         graphWindow->Copy( graphMemory, &rect, &rect2);
      }
      else
      {
         //query the size of the rectangle to draw
         XRect rect;
         GetUpdateRect( &rect);

         //copy only the update-rectangle
         XRect rect2;
         rect2.SetWidth(0);
         rect2.SetHeight(0);

         if(bmpSize.GetHeight() > clientRect.GetHeight())
         {
            if( rect.GetHeight() + rect.GetY() < clientRect.GetHeight() )
               rect2.SetY( bmpSize.GetHeight() - clientRect.GetHeight() - GetVertPos() + rect.GetY());
            else
               rect2.SetY( bmpSize.GetHeight() - rect.GetHeight() - GetVertPos() );
         } /* end if */
         else
            rect2.SetY(rect.GetY());

         if(bmpSize.GetWidth() > clientRect.GetWidth())
            rect2.SetX(GetHorzPos() + rect.GetX());
         else
            rect2.SetX(rect.GetX());
         graphWindow->Copy( graphMemory, &rect, &rect2);
      }
   }
}


BOOL MyAppWindow :: DoCommand(LONG command)
{
   switch(command)
   {
      case IDM_FILE_EXIT:
         Close();
         break;
      case IDM_FILE_SAVE:
         {
            XGLibFileDialog save(this, "*", "Save bitmap");
            if (USER_OK == save.GetCommand ())
            {
               XString filename;
               save.GetFileName (&filename);
               try
               {
                  XBitmapFilter* p = XBitmap :: GetExportFilterStruct (filename);
                  XBitmap :: SetupExportFilter (this, filename, p);
                  if (DID_OK == p->command)
                     bmp->Save (filename, p);
               }
               catch (XException& e)
               {
                  e.ShowError ();
               }
            }
         }
         break;
      case IDM_FILE_OPEN:
         {
            XGLibFileDialog fileDlg(this, "*", "Open bitmap" );

            /* the user selected a file */
            if( fileDlg.GetCommand() == USER_OK)
            {
               XString fileName;
               fileDlg.GetFileName( &fileName, 0 );
               Load(fileName);
            }
         }
         break;
      case IDM_EDIT_COPY:
         {
            if(loaded)
            {
               XClipBoard clipBoard;
               clipBoard.SetBitmap(bmp);
            } /* end if */
         }
         break;
      case IDM_EDIT_PASTE:
         {
            XClipBoard clipBoard;
            if(clipBoard.GetBitmap(bmp))
               CalcBitmap();
         }
         break;
      case IDM_OPTION_STRETCH:
         stretch = stretch ? FALSE : TRUE;
         if(stretch)
         {
            SetVirtualX( 0 );
            SetVirtualY( 0 );
         }
         else
         {
            SetVirtualX( bmpSize.GetWidth());
            SetVirtualY( bmpSize.GetHeight());
         } /* end if */
         Invalidate();
      default :
         break;
   } /* end switch */
   return TRUE;
}


void MyAppWindow :: InitMenu( XMenu * menu)
{
   if(loaded)
   {
      menu->EnableItem(IDM_EDIT_COPY, TRUE);
      menu->EnableItem(IDM_FILE_SAVE, TRUE);
   } /* end if */
   else
   {
      menu->EnableItem(IDM_EDIT_COPY, FALSE);
      menu->EnableItem(IDM_FILE_SAVE, FALSE);
   }
   XClipBoard clipBoard;
   if(clipBoard.IsFormatAvaible(CF_BITMAP))
      menu->EnableItem(IDM_EDIT_PASTE, TRUE);
   else
      menu->EnableItem(IDM_EDIT_PASTE, FALSE);
   if(stretch)
      menu->CheckItem(IDM_OPTION_STRETCH, TRUE);
   else
      menu->CheckItem(IDM_OPTION_STRETCH, FALSE);
}


void MyAppWindow :: DoSize( XSize*)
{
   GetClientSize(&clientRect);
   HScroll(0);
   VScroll(0);
   SetBitmapPos();
}


void main ( int argc, void ** argv)
{
   try
   {
      XMemoryCheck memObj;
      XConsole::CheckConsole(argc, argv);
      MyAppWindow * window = new MyAppWindow(argc > 1 ? (char*) argv[1] : NULL);   //create new framewindow (see above)
      XApplication::GetApplication()->Start();  //let the application work
   }
   catch( XException e)
   {
      e.ShowError();   //shit
   }
}