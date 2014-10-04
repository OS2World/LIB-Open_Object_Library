#include "sample4.h"

#define DEPARTMENT 1
#define WORKER 2

#include <stdlib.h>

XIcon depIcon, empIcon;
BOOL icon = FALSE;
XContainerControl * contLeft, *contRight;
SHORT sortType = 1;

//our class for container object, it is derived from XContainerObject
class MyObjectClass: public XContainerObject
{
   public:
      SHORT type;
      MyObjectClass( XContainerControl * cont, SHORT r, SHORT columns, SHORT attr = 0):XContainerObject(cont, columns, attr) {type=r; }
};

class Employe; //forward

//class Department, it holds the employes
class Department: public MyObjectClass
{
   public:
      Department( char * n, XContainerControl * cont);
      SHORT count;                       //how much emps
      Employe ** employe;                //list of emps
      void AddEmploye( Employe *);       //methods
      void RemoveEmploye( Employe*);
      void Update( void );
};


Department :: Department( char * n, XContainerControl * cont): MyObjectClass( cont, DEPARTMENT, 0, 0) //CON_RECORDREADONLY
{
   SetTitle( n );
   employe = NULL;
   count = 0;
   SetIcon( &depIcon );
}


//class Employe
class Employe: public MyObjectClass
{
   public:
      XString name;                      //the name
      XString position;                  //what function
      ULONG income;                      //how much income
      Employe( char * n, char * p, ULONG in, XContainerControl * cont);
      SHORT Sort( const XContainerObject * p) const; //sorting method, overridden from XContainerControl
      BOOL AllocMemory( char * oldText, SHORT newTextLength, XContainerColumn* col);
      void TitleEdited( const char * text, XContainerColumn *);
};


BOOL Employe :: AllocMemory( char * oldText, SHORT newTextLength, XContainerColumn* col)
{
   if( col->GetColumnNumber() == 0)
   {
      name.GetBuffer(newTextLength);
      name.ReleaseBuffer(newTextLength);
   } /* end if */
   else if( col->GetColumnNumber() == 1)
   {
      position.GetBuffer(newTextLength);
      position.ReleaseBuffer(newTextLength);
   }
   return TRUE;
}

void Employe :: TitleEdited( const char * text, XContainerColumn * col)
{
   if( col->GetColumnNumber() == 0)
      name= text;
   else if( col->GetColumnNumber() == 1)
      position = text;
}

//if a new department is choosen, the employe-container must be updated
void Department :: Update( void )
{
   contRight->RemoveAll( FALSE );           //remove all, but dont delete the objects

   for( int i=0; i < count; i ++)
      contRight->AddObject( employe[i], NULL, NULL, FALSE );   //add the employes of this department

   //to improve performance we check if we are in icon-mode or not
   if( icon )
      contRight->Arrange();                    //rearrange the objects in icon-mode
   else
      contRight->InvalidateObject();           //draw the object in non-icon mode
}


//adds an employe to this department
void Department :: AddEmploye( Employe * e)
{
   count += 1;
   employe = (Employe**) realloc( employe, count * sizeof(void*));
   employe[count-1] = e;
}


//removes an employe from this department
void Department :: RemoveEmploye( Employe * e)
{
   BOOL swap = FALSE;

   for( int i=0; i < count-1; i++)
   {
      if( e == employe[i] )
         swap = TRUE;
      if( swap )
         employe[i] = employe[i+1];
   }
   count -= 1;
}


//the overridden sorting-method
SHORT Employe :: Sort( const XContainerObject * p) const
{
   Employe * e = (Employe*) p;           //typecast
   if(sortType == 1)                     //sorting by income
   {
      if( e->income == income)
         return 0;                        //income is equal
      return income > e->income ? 1 : -1; //income is different
   }
   else
      return name.StrCmp(e->name );    //sort by name
}


//constructor employe
Employe::Employe(char*n, char*p, ULONG in, XContainerControl*cont): MyObjectClass( cont, WORKER, 3)
{
   name = n;
   position = p;
   income = in;
   SetColumnData( 0, (char*) name);         //set column-data for detail-view, column 1
   SetColumnData( 1, (char*) position);     //column 2
   SetColumnData( 2, income);               //column 3
   SetIcon( &empIcon );
   SetTitle( (char*) name);                 //set the title for icon/name-view
}


//because we want to catch events from the containers we must generate a handler
class MyHandler: public XContainerHandler
{
   public:
      ULONG HandleEvent(XContainerEvent*);  //override this method
      MyHandler( XContainerControl * c): XContainerHandler(c) { ;}
};


//our handler handles here
ULONG MyHandler :: HandleEvent( XContainerEvent * e)
{
   static Department * dropDepartment;

   switch(e->GetEventID())               //what type of event?
   {
      case CON_EMPHASIS:
         if( e->GetWindowID() == CONT_LEFT)
         {
            Department * d = (Department*) e->GetObject();
            if( d->GetEmphasis() & CON_SELECTED)
               d->Update( );
         }
         break;
      case CON_CONTEXTMENU:
         if( e->GetWindowID() == CONT_RIGHT)
         {
                  XPoint p;

                  XPopupMenu * menu = new XPopupMenu( IDM_EMPLOYE, GetWindow());

                  XRect r;
                  GetWindow()->GetPointerPos( &p);

                  GetWindow()->QueryWindow(QW_PARENT)->GetSize( &r);
                  p.SetX( p.GetX() - r.GetX());
                  p.SetY( p.GetY() - r.GetY());

                  GetWindow()->GetSize( &r);
                  p.SetX( p.GetX() - r.GetX());
                  p.SetY( p.GetY() - r.GetY());
                  menu->Display( &p, IDM_VIEW);
               }
               break;
         case CON_DRAGOVER:                             //a flying object about one of our windows
            if( e->GetWindowID() == CONT_LEFT)          //the left container?
               {
                  XContainerDragEvent * dr = (XContainerDragEvent*) e;
                  XContainerObject * o;
                  if( ( o = dr->GetObject()) != NULL)   //is there an object under the mouse?
                     {
                         if( o->GetEmphasis() & CON_SELECTED)  //yes, the current department
                            dr->SetAcceptMode( DRG_NODROP );     //in this case dont accept the objects
                         else
                            {                                    //here is not the actual department under the mouse
                               dr->SetAcceptMode( DRG_DROP );    //accept the object
                               dr->SetOperation( DRG_MOVE );     //moving is our method
                               dropDepartment = (Department*) o; //this department receive the object if they are dropped
                            }
                     }
                  else
                     dr->SetAcceptMode( DRG_NODROP ); //no object under the mouse, disable drop
               }
               return 0;
         case CON_INITDRAG:                                         //initialize dragging
            if( e->GetWindowID() == CONT_RIGHT)                     //the right container?
              {
                  SHORT i = 0;
                  Employe* obj = (Employe*)e->GetObject();          //which object is under the mouse
                  XContainerControl * c = (XContainerControl*) e->GetWindow();  //get a pointer to the window
                  if(obj->GetEmphasis() & CON_SELECTED)           //is the object selected?
                     {
                         XContainerObject * o = c->GetObject();   //count the selected objects
                         do
                            {
                                o = c->GetObject( o, CON_SELECTED);
                                i++;
                            } while( o );
                     }
                  else
                     i=1;                                           //nothing selected, only one object to drag

                  XWindowDrag wDrag( c, i );                        //our drag-class, give a pointer to the window
                                                                    //and the count of objects to be dragged
                  if( i == 1)                                       //only one (non-selected) object
                     {
                        XDragItem item( c );                        //construct a drag-item
                        wDrag.SetDragItem( &item, 0 );              //add the drag-item at the first position
                        XIcon ico;
                        obj->GetIcon(&ico);
                        wDrag.AddImage( &ico);    //add a image
                     }
                  else
                     {
                        SHORT j=0;                                  //more than one objects to drag
                        XContainerObject * o = c->GetObject();    //ask for the first selected object
                        do                                          //add all selected objects
                           {                                        //to the drag-class
                              XDragItem item( c );                  //create a new drag-item
                              wDrag.SetDragItem( &item, j );        //add it at the right position
                              if( j < 5)                            //for the first 5 items an image is added (stretched)
                                 {
                                    XIcon ico;
                                    o->GetIcon(&ico);
                                    wDrag.AddImage( &ico, IMG_ICON | DRG_STRETCH, j * 10, j * 10, 40 - j * 8);
                                 }
                              o = c->GetObject( o, CON_SELECTED); //ask for the next selected object
                              j++;
                           } while( o );
                     }
                  if( wDrag.PerformDrag() )                         //do the drag operation
                     {
                        Department * d = (Department*) contLeft->GetObject(); //dragging was performed corectly
                                                                                //ask for the current department
                        if( d)
                           {
                              if(obj->GetEmphasis() & CON_SELECTED)           // if the drag-object was selected...
                                {
                                   Employe * e = (Employe*) contRight->GetObject();
                                   do                                           //..do this for every selected object
                                      {
                                         d->RemoveEmploye( e );                 //remove the emp from the current department
                                         dropDepartment->AddEmploye( e);        //add the emp to the new department
                                         e = (Employe*) contRight->GetObject( e, CON_SELECTED); //get the next selected object
                                      } while( e );
                                }
                              else                                              //no object selected, method like above
                                {
                                    d->RemoveEmploye( obj );
                                    dropDepartment->AddEmploye( obj );
                                }
                              d->Update( );                                     //update
                           }
                     }
              }
            break;
      }
   return FALSE;
}


class MyCont: public XContainerControl
{
   public:
      MyCont( XWindow*w, XRect& r):XContainerControl( w, r, CONT_RIGHT, WIN_BORDER|WIN_VISIBLE|CON_MULTIPLESEL, "8.Helv") { ; }
      BOOL DoCommand( LONG com);
};


BOOL MyCont :: DoCommand( LONG com)
{
   switch( com )
      {
         case IDM_NAME:
            sortType = 0;
            SortObjects();
            break;
         case IDM_INCOME:
            sortType = 1;
            SortObjects();
            break;
         case IDM_BITMAP:
            {
               XContainerInfo info;
               GetInfo( &info);
               BOOL bit = info.IsBackgroundPaintingEnabled();
               if( bit)
                  bit = FALSE;
               else
                  bit = TRUE;
               info.EnableBackgroundPainting( bit );
               SetInfo( &info);
               Invalidate(TRUE);
               Arrange();           //re-arrange the container
            }
            break;
         case IDM_ICON:                        //the user selected a new view from
         case IDM_DETAIL:                      //the popup-menu
         case IDM_TEXT:
            {
               icon = FALSE;
               XContainerInfo info;
               GetInfo( &info);

               BOOL bit = info.IsBackgroundPaintingEnabled();

               if(com == IDM_ICON)             //icon-view requested
                  {
                     info.SetAttributes( CO_ICON | CO_TITLE );
                     icon = TRUE;
                  }
               if(com == IDM_DETAIL)           //detail-view requested
                  info.SetAttributes( CO_DETAIL | CO_TITLE | CO_DETAILTITLES );
               if(com == IDM_TEXT)             //text-view requested
                  info.SetAttributes( CO_TEXT | CO_TITLE );

               if( bit )
                  info.EnableBackgroundPainting( bit );

               SetInfo( &info);
               InvalidateObject();           //repaint
               Arrange();           //re-arrange the container
            }
            break;
      }
   return TRUE;
}


class MyDrawHandler: public XBackgroundDrawHandler
{
      XBitmap * bmp;
   public:
      MyDrawHandler( XWindow * );
      BOOL HandleEvent( XBackgroundDrawEvent *);
};


MyDrawHandler :: MyDrawHandler( XWindow * w): XBackgroundDrawHandler(w)
{
   bmp = new XBitmap(w);
   bmp->LoadBMP( "back.bmp" );
}


BOOL MyDrawHandler :: HandleEvent( XBackgroundDrawEvent * e)
{
   e->Draw( bmp );
   return TRUE;
}


class MyItemDrawHandler: public XItemDrawHandler
{
      XBitmap * greenBmp, * redBmp;
   public:
      MyItemDrawHandler( XWindow * );
      BOOL HandleEvent( XItemDrawEvent *);
};


MyItemDrawHandler :: MyItemDrawHandler( XWindow * w): XItemDrawHandler(w, 20,20)
{
   //create and load two bitmaps
   greenBmp = new XBitmap( w );
   greenBmp->LoadBMP( "green.bmp" );

   redBmp = new XBitmap( w );
   redBmp->LoadBMP( "red.bmp" );
}


BOOL MyItemDrawHandler :: HandleEvent( XItemDrawEvent * e)
{
   Employe * emp = ( Employe *) e->GetObject(); //ask for the object to draw

   if( ! emp )                                  //no object, the title must be redrawn
      return FALSE;                             //let the container do it

   XString buffer = (LONG) emp->income;         //convert the income to a string
   if( emp->income < 800 )                      //what? less than 800 ?
      e->DrawItem( redBmp, (char*) buffer );            //draw a red bitmap!
   else                                         //800 or more is ok (per week)
      e->DrawItem( greenBmp, (char*) buffer );          //draw a green bitmap

   return TRUE;
}


MyAppWindow :: MyAppWindow( ): XFrameWindow( (ULONG) 0, "Sample4 - Container", XFrameWindow::defaultDialogStyle | FRM_TASKLIST )
{
   XColor c( COL_PALEGRAY);              //background-color
   SetBackgroundColor( &c);
   XRect re( 100, 100, 600, 400);
   SetSize( &re);                        //size

   XRect r1(10,30,280, 330);             //generate a container
   contLeft = new XContainerControl( this, r1, CONT_LEFT, WIN_BORDER|WIN_VISIBLE, "8.Helv");

   //setup for the container (tree view)
   XContainerInfo info( "Departments", CO_TREE | CO_TITLE | CO_TREELINE );

   //we use only a very small icon
   XSize size(16, 16);
   info.SetBitmapSize( &size );
   info.SetTreeBitmapSize( &size );

   //enable the changes
   contLeft->SetInfo( &info);

   //second container
   XRect r2( 300,30,280, 330);
   contRight = new MyCont( this, r2);

   //setup (detail view)
   XContainerInfo info2( "Employees", CO_DETAIL | CO_TITLE | CO_DETAILTITLES );//| CO_TITLEREADONLY
   info2.EnableSorting();                //enable sorting
   info2.EnableBackgroundPainting();     //we want to draw the background
   contRight->SetInfo( &info2);

   //for detail view we genrate 3 columns in the right container:
   XContainerColumn * col = new XContainerColumn( contRight, "Name", 0, COL_HORZSEPARATOR | COL_STRING | COL_SEPARATOR, COL_LEFT | COL_HORZSEPARATOR | COL_TOP );///COL_FITITLEREADONLY |
   contRight->InsertColumn( col);
   XContainerColumn * col2 = new XContainerColumn( contRight, "Function", 1, COL_SEPARATOR | COL_HORZSEPARATOR | COL_STRING, COL_LEFT | COL_FITITLEREADONLY | COL_HORZSEPARATOR | COL_TOP );
   contRight->InsertColumn( col2, col);
   //this column we draw ourselves:
   XContainerColumn * col3 = new XContainerColumn( contRight, "Income", 2, COL_HORZSEPARATOR | COL_ULONG | COL_OWNERDRAW, COL_LEFT | COL_FITITLEREADONLY | COL_HORZSEPARATOR | COL_TOP );
   contRight->InsertColumn( col3, col2);

   //update columns
   contRight->UpdateColumns();

   //generate 3 departments and add them to the left container
   Department * head = new Department( "Headquarter", contLeft);
   contLeft->AddObject( head);             //this object is the root

   Department * dev = new Department( "Development", contLeft);
   contLeft->AddObject( dev, head);        //headQuarter is parent of this object

   Department * sell = new Department( "Selling", contLeft);
   contLeft->AddObject( sell, head);         //headQuarter is parent of this object

   contLeft->InvalidateObject();

   //generate 4 employes
   Employe * e1 = new Employe( "Smith", "Manager", 900, contRight);
   Employe * e2 = new Employe( "Bond", "Manager", 800, contRight);
   Employe * e3 = new Employe( "Stuart", "Programmer", 500, contRight);
   Employe * e4 = new Employe( "Miller", "Clerk", 300, contRight);
   contRight->InvalidateObject();

   //add the employes to different departments
   head->AddEmploye( e1 );
   head->AddEmploye( e2 );
   dev->AddEmploye( e3 );
   sell->AddEmploye( e4 );


   //generate handlers for container-events
   MyHandler * h = new MyHandler( contLeft );
   MyHandler * h2 = new MyHandler( contRight );

   //generate a handler to draw the background
   MyDrawHandler * h3 = new MyDrawHandler( contRight );

   //generate a handler to draw the items
   MyItemDrawHandler * h4 = new MyItemDrawHandler( contRight );

   //select the first department to show the content
   head->Update( );

   //go!
   Activate();

   ///contLeft->BeginEdit( head );//, NULL, CONT_LEFT);// CID_CNRTITLEWND );
   contRight->BeginEdit( e2, col, CID_LEFTDVWND);//,
}


MyAppWindow :: ~MyAppWindow()
{
}


//we only fill the background
void MyAppWindow :: Draw( void )
{
   FillBackground( );
}


void main ( void)
{
   try
   {
      depIcon.Load( "folder.ico", TRUE);
      empIcon.Load( "person.ico", TRUE);
      MyAppWindow * window = new MyAppWindow( );   //create new framewindow (see above)
      XApplication::GetApplication()->Start();        //let the application work
   }
   catch( XException e)
   {
      e.ShowError();
   }
}
