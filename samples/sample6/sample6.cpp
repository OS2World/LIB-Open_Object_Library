#include "sample6.h"
#include <stdlib.h>



class MyFrame: public XStaticFrame
{
   public:
   MyFrame( XWindow*w, XRect& r): XStaticFrame(w, r){ ;}
   void Draw( void );
};


class MenuDrawHandler: public XItemDrawHandler
{
      XBitmap * red, *green, *blue;
   public:
      MenuDrawHandler( XWindow * w);
      BOOL HandleEvent( XItemDrawEvent *);
};


BOOL MenuDrawHandler :: HandleEvent( XItemDrawEvent * event)
{
   switch( event->GetItemID() )
      {
         case IDM_RED:
            event->DrawItem( red, "Red");
            break;
         case IDM_GREEN:
            event->DrawItem( green, "Green");
            break;
         case IDM_BLUE:
            event->DrawItem( blue, "Blue");
            break;
         default:
            return FALSE;
      }
   return TRUE;
}


MenuDrawHandler :: MenuDrawHandler( XWindow * w):XItemDrawHandler(w, 80, 25)
{
   red = new XBitmap(w);
   red->LoadBMP( "red.bmp" );
   green = new XBitmap(w);
   green->LoadBMP( "green.bmp" );
   blue = new XBitmap(w);
   blue->LoadBMP( "blue.bmp" );
}


class ListDrawHandler: public XItemDrawHandler
{
      XBitmap * bmp[3];
   public:
      ListDrawHandler( XWindow * w);
      BOOL HandleEvent( XItemDrawEvent *);
};


ListDrawHandler :: ListDrawHandler( XWindow * w):XItemDrawHandler(w, 100, 20)
{
   bmp[0] = new XBitmap(w);
   bmp[1] = new XBitmap(w);
   bmp[2] = new XBitmap(w);
   bmp[0]->LoadBMP( "red.bmp" );
   bmp[1]->LoadBMP( "green.bmp" );
   bmp[2]->LoadBMP( "blue.bmp" );
}


BOOL ListDrawHandler :: HandleEvent( XItemDrawEvent * event)
{
   switch( event->GetItemID())
      {
         case 0:
            {
               XColor col( COL_RED);
               event->SetTextColor( &col);
               event->DrawItem( bmp[0], "Red");
            }
            break;
         case 1:
            {
               XColor col( COL_GREEN);
               event->SetTextColor( &col);
               event->DrawItem( bmp[1], "Green");
            }
            break;
         case 2:
            {
               XColor col( COL_BLUE);
               event->SetTextColor( &col );
               event->DrawItem( bmp[2], "Blue");
            }
            break;
         default:
            return FALSE;
      }

   return TRUE;
}


class MyPage: public XNoteBookPage
{
   public:
      BOOL DoCommand(LONG);
      void DoControl(XControlEvent*);
      MyPage( XNoteBook * b):XNoteBookPage(b, 1) { ;}
};


class MyTimer: public XTimer
{
      SHORT counter;
   public:
      virtual void TimerEvent(void);
      MyTimer(XWindow * w):XTimer(w) { counter = 0; }
};


void MyTimer :: TimerEvent()
{
   XTrafficLight * light= (XTrafficLight*) GetWindow()->GetWindow( ID_TRAFFIC );
   XLED * led = (XLED*) GetWindow()->GetWindow( ID_LED );
   if( counter % 2)
   {
      light->SetState( TRAFFIC_YELLOW );
      led->Enable();
   }
   else
   {
      light->SetState( TRAFFIC_OFF );
      led->Enable(FALSE);
   }
   counter ++;
}


MyTimer * timer;

class MyCalendar: public XCalendarControl
{
   public:
      MyCalendar( const XWindow * w, const XPoint& p= XPoint(), const USHORT id = 0, const XDate& date=XDate()):XCalendarControl(w,p,id,date) {;}
      virtual void DaySelected( const UCHAR selectedDay );
};


void MyCalendar :: DaySelected( const UCHAR selectedDay )
{
   XText * t = GetDayObject(selectedDay);

   if(t)
   {
       XColor col(COL_BLUE);
       t->SetColor( &col );
      Draw();
   }
}

class MyColorControl: public XColorControl
{
   public:
      MyColorControl(XWindow * w):XColorControl( w, XRect(10,10,250,190))
      {
         XColor co(COL_BLUE);
         SetColor(co);
      }
      virtual void ColorSelected( const XColor& newColor)
      {
         XString text = "Blue:";
         text += (LONG) newColor.GetBlue();
         text += ", Red: ";
         text += (LONG) newColor.GetRed();
         text += ", Green: ";
         text += (LONG) newColor.GetGreen();
         QueryWindow(QW_OWNER)->GetWindow(999)->SetText(text);
      }
};


//here a window is created out of the resource-file, the last parameter tells the library
//to build the window with the ID . Works with resource-DLLs too.
MyAppWindow :: MyAppWindow(): XFrameWindow( IDM_MENU, "Sample6 - extended controls", defaultDialogStyle | FRM_TITLEBAR | FRM_TASKLIST | FRM_CENTER, NULL, NULL, FALSE, TRUE)
{
   XColor col( COL_PALEGRAY);
   XRect rec(100, 100, 450, 400);
   SetSize(&rec);
   XMenuBar * bar;

   try
   {
      bar = new XMenuBar( this, IDM_MENU);
   }
   catch( XException& e)
   {
      XMessageBox( e.GetErrorMessage());
      exit(-1);
   }

   MenuDrawHandler * hdl = new MenuDrawHandler( bar );

   SetBackgroundColor( &col);

   bar->CheckItem( IDM_RED);

   EnableWindowUpdate(FALSE);

   XRect rect( 20, 20, 400, 300);
   //enable new notebook-style too
   XNoteBook * notes = new XNoteBook( this, rect, 0, NB_TABBEDDIALOG|WIN_VISIBLE|NB_SOLIDBIND|NB_BACKPAGESBR|NB_SQUARETABS|NB_TABTEXTCENTER|NB_STATUSTEXTLEFT, "8.Helv");
   notes->SetMajorTabSize( 135, 25);
   notes->SetMajorBackgroundColor( &col);
   notes->SetBackgroundColor( &col);

   MyPage * page1 = new MyPage( notes);
   page1->SetText( "XValueSet" );

   //construct some controls

   //XValueSet
   XValueSet * vSet = new XValueSet( page1, XRect( 20, 15, 170, 170), ID_VALUESET, VA_TEXT|WIN_VISIBLE|VA_BORDER|VA_ITEMBORDER);

   vSet->SetDimensions( 3, 3 );
   XSize size(10, 10);
   vSet->SetItemSpacing( &size );
   size.SetWidth(40);
   size.SetHeight(40);
   vSet->SetItemSize( &size );

   XStaticText * text = new XStaticText( page1, "XValueSet", XRect( 20, 190, 140, 20), WIN_VISIBLE, ID_TEXT);

   //fill the first column with text
   vSet->SetItem( 0, 0, "0:0" );
   vSet->SetItem( 1, 0, "1:0" );
   vSet->SetItem( 2, 0, "2:0" );

   //fill the second column with icons
   vSet->SetItemAttribute( 0, 1, VA_ICON );
   vSet->SetItemAttribute( 1, 1, VA_ICON );
   vSet->SetItemAttribute( 2, 1, VA_ICON );

   XIcon * icon1, * icon2, * icon3;
   icon1 = new XIcon();
   icon1->Load( "paint.ico");

   icon2 = new XIcon();
   icon2->Load( "lib.ico");

   icon3 = new XIcon();
   icon3->Load( "mond.ico");

   vSet->SetItem( 0, 1, icon1);
   vSet->SetItem( 1, 1, icon2);
   vSet->SetItem( 2, 1, icon3);


   //fill the last column with colors
   vSet->SetItemAttribute( 0, 2, VA_COLOR );
   vSet->SetItemAttribute( 1, 2, VA_COLOR );
   vSet->SetItemAttribute( 2, 2, VA_COLOR );

   XColor col1( COL_RED );
   vSet->SetItem(0, 2, &col1);
   XColor col2( COL_GREEN);
   vSet->SetItem(1, 2, &col2);
   XColor col3( COL_BLUE );
   vSet->SetItem(2, 2, &col3);

   MyPage * page2 = new MyPage( notes);
   page2->SetTop();
   page2->SetText("XSlider");

   //XSlider
   XSlider * slider = new XSlider( page2, XRect( 10, 90, 200, 70), ID_SLIDER, WIN_VISIBLE | SL_HOMELEFT | SL_LEFT | SL_HORIZONTAL | SL_RIBBONSTRIP | SL_TOP | SL_BUTTONSRIGHT);
   slider->SetScales(11, 10, 20, 5);
   slider->SetTickSize( SMA_SETALLTICKS, 5);
   slider->SetTickSize( 0,10 );
   slider->SetTickSize( 5,10 );
   slider->SetTickSize( 10, 10 );

   slider->SetScaleText(0, "0");
   slider->SetScaleText(5, "5");
   slider->SetScaleText(10, "10");

   slider->AddDetent( 35 );
   slider->AddDetent( 75 );

   slider->SetSliderSize( 10, 30 );
   slider->SetShaftSize( 15 );
   slider->SetSliderPos( 50 );

   MyPage * page3 = new MyPage( notes);
   page3->SetTop();
   page3->SetText("XSpinButton");

   //XSpinButton
   XSpinButton * spin = new XSpinButton( page3, XRect( 80, 85, 50, 20), ID_SPIN, WIN_VISIBLE|SP_NUMERIC|SP_MASTER);

   spin->SetLimits(0, 100);
   spin->SetValue( 50 );

   MyPage * page4 = new MyPage( notes);
   page4->SetTop();
   page4->SetText("XCircularSlider");

   //XCircularSlider
   XCircularSlider * circ = new XCircularSlider( page4, XRect( 50, 40, 110, 110), ID_CIRC, WIN_VISIBLE|CS_CIRCULARVALUE|CS_NOBUTTON);
   circ->SetIncrements( 10, 5);
   circ->SetLimits( 0, 100);
   circ->SetValue( 50 );

   MyPage * page5 = new MyPage( notes);
   page5->SetTop();
   page5->SetText("XUserButton");

   //XUserButton
   btn = new XUserButton( page5, XRect( 50, 90, 100, 50), ID_BTN, BU_NOPOINTERFOCUS | WIN_VISIBLE, "0");

   XBitmap map1( btn );
   map1.LoadBMP( "EXIT.BMP" );
   btn->SetBitmap( &map1 );

   XBitmap map2( btn );
   map2.LoadBMP( "EXIT2.BMP" );
   btn->SetBitmap( &map2, 1);

   XPoint p(5, 5);
   btn->GetBitmap( 0 )->Move( &p );
   btn->GetBitmap( 1 )->Move( &p );

   p.Set(50,15);
   btn->GetTextPointer()->Move(&p);

   MyPage * page6 = new MyPage( notes);
   page6->SetTop();
   page6->SetText("XListBox");

   XListBox * list = new XListBox( page6, XRect( 60, 50, 100, 60), 700, WIN_VISIBLE|LS_OWNERDRAW);

   ListDrawHandler * handler = new ListDrawHandler( list );
   list->InsertItem( "Red", 0);
   list->InsertItem( "Green", 1);
   list->InsertItem( "Blue", 2);
   list->InsertItem( "no color", 3);

   XStaticText * text6 = new XStaticText( page6, "XListBox\nwith bitmaps", XRect( 60, 120, 120, 40), WIN_VISIBLE|TX_WORDBREAK);

   doorOpen = TRUE;

   MyPage * page7 = new MyPage( notes);
   page7->SetTop();

   //this page get a bitmap on the tab instead of a text
   XBitmap * pieBmp = new XBitmap( page7 );
   pieBmp->LoadBMP( "bar.bmp" );
   page7->SetBitmap( pieBmp );

   //Create a simple chart

   pie = new XPie( page7, XRect(60, 90, 100, 100), 300, 0, "a XPie" );

   pie->SetItemCount( 6 );

   pie->SetItemValue( 0, 20);
   pie->SetItemValue( 1, 80);
   pie->SetItemValue( 2, 20);
   pie->SetItemValue( 3, 80);
   pie->SetItemValue( 4, 10);
   pie->SetItemValue( 5, 20);

   XPushButton * b = new XPushButton( page7, XRect( 60, 20, 100, 30), ID_RAND, WIN_VISIBLE, "Randomize!" );

   pie->Calculate();
   XColor col4( COL_PALEGRAY);
   pie->SetBackgroundColor( &col4);

   //change the color of the pieïs title and itïs title

   XText * t = pie->GetTitleObject();
   XColor col5( COL_RED );
   t->SetColor( &col5);
   t->SetText("Sample chart");

   pie->Show();

   MyPage * page8 = new MyPage( notes);
   page8->SetTop();
   page8->SetText("XTrafficLight/XLED");
   XPoint point( 155, 95 );
   XTrafficLight * light = new XTrafficLight(page8, point, ID_TRAFFIC);
   light->SetState( 0 );
   XPushButton * b2 = new XPushButton( page8, XRect( 120, 50, 100, 30), ID_TRAFFIC_TEST, WIN_VISIBLE, "Change!" );

   XPushButton * b3 = new XPushButton( page8, XRect( 120, 140, 100, 30), ID_TRAFFIC_BLINK, WIN_VISIBLE, "Blink!" );
   timer = new MyTimer( page8 );

   XPoint p2( 220, 110);
   XLED * led = new XLED(page8, p2, ID_LED);

   MyPage * page9 = new MyPage( notes);
   page9->SetTop();
   page9->SetText("XPeakMeter");

   XPoint pointP( 50, 50);
   XPeakMeter * peak1 = new XPeakMeter( page9, pointP);
   peak1->SetLevel( 1 );

   pointP.SetY( 65 );
   XPeakMeter * peak2 = new XPeakMeter( page9, pointP);
   peak2->SetLevel( 14 );

   pointP.SetX( 300 );
   pointP.SetY( 20 );
   XPeakMeter * peak3 = new XPeakMeter( page9, pointP, 0, FALSE);
   peak3->SetLevel( 10 );

   pointP.SetX( 320 );
   XPeakMeter * peak4 = new XPeakMeter( page9, pointP, 0, FALSE);
   peak4->SetLevel( 11 );

   pointP.SetX( 10);
   pointP.SetY( 100 );
   XDate date;
   date.GetCurrentDate();
   date.SetDays(1);
   date.SetMonths(9);

//   XCalendarControl * cal = new XCalendarControl( page9, pointP, 0, date);
   MyCalendar * cal = new MyCalendar( page9, pointP, 0, date);

   MyPage * page10 = new MyPage( notes);
   page10->SetTop();
   page10->SetText("XColorControl");

   MyColorControl * cc = new MyColorControl(page10);
   XStaticText * text7 = new XStaticText( page10, "", XRect( 20, 5, 250, 20), WIN_VISIBLE, 999);

   page1->SetTop();
   notes->Show();
   EnableWindowUpdate(TRUE);

   //Activate the window
   Activate();
}


void MyAppWindow :: Draw( void )
{
   FillBackground();
}


MyAppWindow :: ~MyAppWindow()
{
}


/* here the commands of the menu and the pushbuttons are posted */
BOOL MyPage :: DoCommand( LONG com)
{
   static LONG r = 0;

   switch( com )
      {
         case ID_TRAFFIC_BLINK:
            {
               XTrafficLight * light= (XTrafficLight*) GetWindow( ID_TRAFFIC );
                 light->SetState( TRAFFIC_YELLOW );
               timer->Start( 700 );
            }
            break;
         case ID_TRAFFIC_TEST:
            {
               XTrafficLight * light= (XTrafficLight*) GetWindow( ID_TRAFFIC );
               SHORT state = light->GetState();
               state ++;
               timer->Stop();

               if(state > TRAFFIC_YELLOWRED)
                  state = TRAFFIC_GREEN;
               light->SetState( state );
               XLED * led = (XLED*) GetWindow( ID_LED );
               if(led->GetState() == TRUE)
                  led->Enable(FALSE);
               else
                  led->Enable(TRUE);
            }
            break;
         case ID_RAND:
            {
               XPie * pie = (XPie*) GetWindow( 300 );
               SHORT count = (rand() % 10); //randomize the count of items (max 10 in this case)
               if(count < 2 )
                  count = 2;
               pie->SetItemCount( count );  //set the count of items
               for(int i=0; i < count; i++)
                  pie->SetItemValue( i, (rand()+1) % 100); //randomize values
               pie->Calculate();            //recalculate
               pie->Invalidate();
               pie->Draw();           //draw!
            }
            break;
         case ID_BTN:
            {
               XUserButton * btn = (XUserButton *) GetWindow( ID_BTN);
               r++;
               XString text = (LONG) r;
               btn->SetText( text );
               btn->Invalidate();
            }
            break;
      }
   return TRUE;
}


BOOL MyAppWindow :: QueryForClose( void )
{
   XMessageButton * mb[2];

   mb[0] = new XMessageButton( "Terminate", 1);
   mb[1] = new XMessageButton("donït Terminate", 4, BS_DEFAULT);
   XIcon icon;
   icon.Load( "exit.ico");
   XExtendedMessageBox msb( "Do you realy want to terminate?", "Sample6", MB_MOVEABLE, 2, (const XMessageButton **) mb, (const XIcon *) &icon, (XWindow*) this);
   if( msb.GetCommand() == 1)
      return TRUE;
   else
      return FALSE;
}


void MyAppWindow :: DoControl( XControlEvent * event)
{

}


//here the control-events of the window-clients are posted
void MyPage :: DoControl( XControlEvent * event)
{
   switch( event->GetEventID()) //what type of event?
      {
         case WIN_SELECTED:      //window-content selected
            {
               switch( event->GetWindowID()) //which window posted the event?
                  {
                     case ID_VALUESET: //the valueset
                        {
                           XValueSet * set = (XValueSet*) event->GetWindow();  //get a pointer to that window
                           SHORT row, col;
                           set->GetSelection( row, col);
                           XString string = "selected: ";
                           string += "(";
                           string += (int) row;
                           string += ":";
                           string += (int) col;
                           string += ")";
                           XStaticText * text = (XStaticText *) GetWindow( ID_TEXT);
                           text->SetText( string );
                        }
                     default:
                        break;
                  }
            }
      }
}


void main ( void)
{
   try
   {
      MyAppWindow * window = new MyAppWindow();
      XApplication::GetApplication()->Start();               //let the application work
   }
   catch( XException& e)
   {
      e.ShowError();
   }
}
