#include "sample7.h"


#include <stdlib.h>

#define COMMAND_PRINT 200


/* Sample 7 shows how to draw objects and to print       */
/* For drawing there are two methods avaible                 */
/* 1: Using one graphic device                           */
/*      The most simple way is to use one graphic device   */
/*      in this case you only need to create the objects   */
/*      to draw in this device, every time it has to be    */
/*      redrawn (when the method Draw() of the owning       */
/*      window is called) you call the method Draw() of      */
/*      the device.                                          */
/*   2:   Using two graphic devices                           */
/*      If you have a litle bit more object you should use   */
/*      this method because it works faster. Create one    */
/*      device in memory (the second parameter of the       */
/*      constructor must be TRUE) and create the objects   */
/*      to draw on this device. Only call Draw() if you    */
/*      have inserted/removed/edited one or more objcts.    */
/*      Create a second graphic device associated to the   */
/*      owning window. If the method Draw() of the owning  */
/*      window is called, copy the content of the memory-   */
/*      device to the window-associated device with Copy().*/
/*   This sample show you how to use the second method.      */
MyAppWindow :: MyAppWindow(): XFrameWindow( 100, "Sample7 - Graphic Functions", XFrameWindow::defaultStyle | FRM_TASKLIST )
{
   //create a toolbar
   XToolBar * t = new XToolBar(this, TB_LEFT | TB_RIPABLE, 0, 0, 10);

   //create a user defined button
   XRect rectB( 0, 0, 46, 46);
   XUserButton * back = new XUserButton( t, rectB, COMMAND_PRINT, WIN_VISIBLE);

   //create a bitmap
   XBitmap bitmap( back);
   bitmap.Load( "printer.bmp");
   XRect re(0, 0, 42, 42);

   //set the size of the bitmap to show
   bitmap.SetOutputSize( &re );

   // add the bitmap to the button
   back->SetBitmap( &bitmap, 0);

   //add the button to the toolbar
   t->AddWindow( back, FALSE, TRUE);

   //create a graphic device associated to the window
   graphWindow = new XGraphicDevice( this, FALSE, TRUE, PU_PELS, GPIA_ASSOC);//, PU_PELS);

   //create a graphic device which resides in memory
   graphMemory = new XGraphicDevice( this, TRUE, TRUE, PU_PELS, GPIA_ASSOC | GPIT_MICRO); //

   //now create some objects to show
   XColor colWhite( COL_WHITE);
   graphMemory->SetBackgroundColor( &colWhite );

   XPoint p1(100,100);
   XPoint p2(500, 500);

   XLine * line = new XLine( &p1, &p2);
   line->SetLineType( LINETYPE_DASHDOT );
   XColor colRed( COL_RED);
   line->SetColor( &colRed);
   line->SetLineWidth( 2 );
   layer.AddObject(line);

   p1.Set(400, 300);
   XCircle * ci = new XCircle( &p1, 100, TRUE, 200, -40, 50);
   XColor colBlue( COL_BLUE);
   ci->SetColor( &colBlue);
   layer.AddObject(ci);

   p1.Set(105, 105);
   XArc * a = new XArc( &p1, 100, 0, 70, TRUE, TRUE);
   a->SetColor( &colBlue);
   layer.AddObject(a);

   XColor colYellow(COL_YELLOW);
   XArc * b = new XArc( &p1, 100, 70, 100, TRUE, TRUE);
   b->SetColor( &colYellow);
   layer.AddObject(b);

   XArc * c = new XArc( &p1, 100, 170, 170, TRUE, TRUE);
   XColor colGreen(COL_GREEN);
   layer.AddObject(c);

   c->SetColor( &colGreen);
   XArc * d = new XArc( &p1, 100, 340, 20, TRUE, TRUE);
   d->SetColor( &colRed);
   layer.AddObject(d);

   p1.Set(200,200);
   p2.Set(300,100);
   XBox * box = new XBox( &p1, &p2, FALSE);
   XColor colPaleGray(COL_PALEGRAY);
   box->SetColor( &colPaleGray);
   box->SetLineWidth( 40 );
   box->SetLineEnd( LINEEND_FLAT );
   box->SetLineType( LINETYPE_DASHDOT );
   layer.AddObject(box);

   p1.Set(200,200);
   XMarker * m1 = new XMarker( &p1);
   layer.AddObject(m1);
   XSize size;
   m1->GetSize( & size );
   p1.Set(200- size.GetWidth()/2, 200+ size.GetHeight()/2);
   m1->Move( &p1);
   p1.Set( 300 + size.GetWidth()/2, 200 + size.GetHeight()/2);
   XMarker * m2 = new XMarker( &p1);
   p1.Set( 200 - size.GetWidth()/2, 100 - size.GetHeight()/2);
   layer.AddObject(m2);
   XMarker * m3 = new XMarker( &p1);
   layer.AddObject(m3);
   p1.Set( 300 + size.GetWidth()/2, 100 - size.GetHeight()/2);
   XMarker * m4 = new XMarker( &p1);
   layer.AddObject(m4);

   p1.Set(300,100);
   p2.Set(400,0);

   XBox * box2 = new XBox( &p1, &p2, TRUE);
   layer.AddObject(box2);

   p1.Set(320, 80);
   p2.Set(380, 20);
   XBox * box3 = new XBox( &p1, &p2, TRUE);
   layer.AddObject(box3);
   box3->SetColor( &colWhite);

   XLine * line2 = new XLine( &p1, &p2);
   XColor colBlack(COL_BLACK);
   line2->SetColor( &colBlack);
   line2->SetLineWidth( 2 );
   layer.AddObject(line2);

   p1.SetY(20);
   p2.SetY(80);
   XLine * line3 = new XLine( &p1, &p2);
   line3->SetColor( &colBlack);
   line3->SetLineWidth( 2 );
   layer.AddObject(line3);

   XFont * font = new XFont( graphMemory, "Tms Rmn Bold Italic", 14 );
   if(font->GetPixelSize() < 4)
   {
      //the font seems not to be installed, choose a default font
      delete font;
      font = new XFont( graphMemory, "Helvetica", 10 );
   }
   XRect rect2( 200, 250, 400, 450);
   XText * text = new XText( font, &rect2, "Text with the font \"Tms Rmn Bold Italic\". If you have not installed the font, youlïll see this with a default font.", DT_WORDBREAK);
   text->SetColor( &colRed);
   layer.AddObject(text);

   XRect rect3( 100,100,350, 250);
   XFont * font2 = new XFont( graphMemory, "Helv Bold", 12 );
   if(font2->GetPixelSize() < 4)
   {
      //the font seems not to be installed, choose a default font
      delete font2;
      font2 = new XFont( graphMemory, "Helvetica", 12 );
   }
   XText * text2 = new XText( font2, &rect3, "Text with the font \"Helv Bold\". If you have not installed the font, youlïll see this with a default font.", DT_WORDBREAK);
//   text2->SetShear( 2, 3);
   layer.AddObject(text2);

   //create a bitmap
   XPoint rp(50, 300);
   XBitmap * bmp = new XBitmap( &rp);

   //load the bitmap from a file
   bmp->Load( "sample.bmp" );
   layer.AddObject(bmp);

   //draw the objects ONLY ONE TIME
   graphMemory->FillBackground();
   layer.Draw(graphMemory);

   XRect rect( 150, 150, 500, 500);
   SetSize(&rect);

   Activate();
}


void MyAppWindow :: Draw( void )
{
   //the content of the window has to be drawn
   if(graphWindow && graphMemory)
   {
      //query the size of the rect to copy
      XRect rect;
      GetUpdateRect( &rect);

      //copy the content of the memory-device to the window-device
      //without any modifications
      XRect rect2 = rect;

      //make sure the update-rect will not be stretched
      rect2.SetWidth(0);
      graphWindow->Copy( graphMemory, &rect, &rect2);
   }
}


BOOL MyAppWindow :: DoCommand(LONG command)
{
   //the toolbar button has been pressed, get ready to print
   if(command == COMMAND_PRINT)
   {
      XPrinterDevice printer(this);
      XString queue, fileName;

      //setup the printer
      if( printer.SetupPrinter("Print", this, &queue, &fileName) == FALSE)
         return TRUE;

      if(fileName.GetLength())
      {
         XMessageBox("cannot print into file, aborting");
         return TRUE;
      } /* end if */
      //open a printer-job
      if( printer.OpenPrinterJob( "Test Job") == FALSE)
      {
         XMessageBox( "error, cannot create printer-job" );
         return TRUE;
      } /* end if */

      XSize size;
      LONG cx, cy;

      //query the size of the used sheet
      printer.GetPageSize( &size );

      //create a bitmap to draw
      XPoint rp( 300, 1500);
      XBitmap * bmp = new XBitmap( &rp);
      bmp->Load( "sample.bmp" );

      //draw the bitmap bigger
      XSize s;
      bmp->GetDimensions( &s );
      cx = s.GetWidth() * 2;
      cy = s.GetHeight() * 2;
      XRect newRect(0,0,cx,cy);
      bmp->SetOutputSize(&newRect);

      //set the bitmap centered
      rp.SetY( size.GetHeight() - cy - 200);
      rp.SetX( size.GetWidth() / 2 - cx /2 );
      bmp->Move( &rp );
      bmp->Draw(&printer);

      //create a table
      XPoint p1, p2;
      p1.Set(100, rp.GetY() - 300);
      p2.Set( size.GetWidth() - 100, p1.GetY());
      XLine * baseLine = new XLine( &p1, &p2);
      baseLine->SetLineWidth( LINEWIDTH_THICK );
      baseLine->Draw(&printer);

      p1.SetX( 400 );
      p1.SetY( p1.GetY() + 50 );
      p2.SetX( 400 );
      p2.SetY( rp.GetY() - 1300 );
      XLine * line1 = new XLine( &p1, &p2);
      line1->Draw(&printer);

      p1.SetX( 800 );
      p2.SetX( 800 );
      XLine * line2 = new XLine( &p1, &p2);
      line2->Draw(&printer);

      p1.Set(100, rp.GetY() - 1300);
      p2.Set( size.GetWidth() - 100, rp.GetY() - 1300);
      XLine * lineBase = new XLine( &p1, &p2);
      lineBase->SetLineType( LINETYPE_LONGDASH );
      lineBase->Draw(&printer);

      p1.Set( 500, rp.GetY() - 100);
      XFont * fontBakerBook = new XFont( &printer, "BakerBook", 14 );
      if(fontBakerBook->GetPixelSize() < 4)
      {
         delete fontBakerBook;
         fontBakerBook = new XFont( &printer, "Helvetica", 14 );
      }
      XText * titleText = new XText( fontBakerBook, &p1, "Printer sample of the Open Object Library");
      titleText->Draw(&printer);

      //Set labels for the table
      XFont * fontLabel = new XFont( &printer, "Helv", 12 );
      p1.Set(100, rp.GetY() - 290);

      XText * text1 = new XText( fontLabel, &p1, "Column 1");
      text1->SetShear( 2, 3);
      text1->Draw(&printer);

      p1.SetX(420);
      XText * text2 = new XText( fontLabel, &p1, "Column 2");
      text2->SetShear( 2, 3);
      text2->Draw(&printer);

      p1.SetX(820);
      XText * text3 = new XText( fontLabel, &p1, "Column 3");
      text3->SetShear( 2, 3);
      text3->Draw(&printer);

      //print some rows
      XFont * fontText = new XFont( &printer, "Times New Roman", 10 );

      LONG yPos = rp.GetY() - 300 - fontText->GetPixelSize() - 5;
      LONG lineCounter = 1;
      XString text;
      while( yPos > rp.GetY() - 1300)
      {
         p1.Set( 100, yPos );
         text = "Row ";
         text += lineCounter;
         text += " column 1";
         XText line1( fontText, &p1, text);
         line1.Draw(&printer);

         p1.SetX( 420);
         text = "Row ";
         text += lineCounter;
         text += " column 2";
         XText line2( fontText, &p1, text);
         line2.Draw(&printer);

         p1.SetX( 820 );
         text = "Row ";
         text += lineCounter;
         text += ", this line is a little bit longer";
         XText line3( fontText, &p1, text);
         line3.Draw(&printer);
         yPos-= (fontText->GetPixelSize() - 3);
         lineCounter ++;
      }

      //print a business-chart
      p1.Set( size.GetWidth() / 2, 400);
      XArc a(&p1, 300, 0, 70, TRUE, TRUE);
      a.SetPattern(8);
      a.Draw(&printer);

      XArc b(&p1, 300, 70, 100, TRUE, TRUE);
      b.SetPattern(9);

      XArc c(&p1, 300, 170, 170, TRUE, TRUE);
      c.SetPattern(12);
      c.Draw(&printer);

      XArc d(&p1, 300, 340, 20, TRUE, TRUE);
      d.SetPattern(7);
      d.Draw(&printer);

      //close the job
      printer.ClosePrinterJob( );
   }
   return TRUE;
}


BOOL MyAppWindow :: QueryForClose()
{
   XApplication::GetApplication()->Terminate();      //make shure all windows will be closed
                                                   //nessacary if the user had ripped the toolbar
   return TRUE;
}


void main ( void)
{
   try
   {
      MyAppWindow * window = new MyAppWindow();   //create new framewindow (see above)
      XApplication::GetApplication()->Start();  //let the application work
   }
   catch( XException e)
   {
      e.ShowError();   //shit
      exit(-1);
   }
}
