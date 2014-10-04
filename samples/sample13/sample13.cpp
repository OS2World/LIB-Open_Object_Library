/*
 * Fire - a sample program for the XGame library
 * Copyright (C) 1997 by Thomas Bonk (thomas@ghecko.saar.de).
 * All rights reserved.
 * Fire.cpp - main module (class definitions)
 */

#include "sample13.h"

void FlameThread::PrepPal( void )
{
        ULONG    data[256];
        int i;


        pPal = new XPalette();

        // initialize palette for the fire
        memset( data, 0, sizeof( data ) );      // everything black
        for( i = 0; i <= 7; i++ )
        {
                data[11 + i] = i * 9;           // col 10-17: ascending blue
                data[19 + i] = 63 - i * 9;      // col 18-25: descending blue
        } // for
        for( i = 8; i <= 31; i++ )
                data[11 + i] |= (((i - 8) * 255 / 23) << 16);   // col 8-31: ascending red
        for( i = 32; i <= 55; i++ )             // col 32-55: ascending green, red constant
        {
                data[11 + i] |= 255 << 16;
                data[11 + i] |= (((i - 32) * 255 / 23) << 8);
        } // for
        for( i = 56; i <= 79; i++ )             // col 56-79: ascending blue, red and green constant
        {
                data[11 + i] |= ((255 << 16) | (255 << 8));
                data[11 + i] |= ((i - 56) * 255 / 23);
        } // for
        for( i = 80; i <= 245; i++ )
                data[11 + i] = (255 << 16) | (255 << 8) | 255;

        pPal->SetColors( data, 10, 236 );
        pBlitter->SetPalette( pPal );
} // FlameThread::PrepPal

void FlameThread::ScrollUp( void )
{
        int   x, y;
        PBYTE pbuf = &pBuffer[31360];

        memmove( &pBuffer[31360], &pBuffer[31680], 32000 );
        for( y = 98; y < 199; y++ )
                for( x = 0; x < 320; x++, pbuf++ )
                {
                        if( 0 == x )
                                *pbuf = (*pbuf         + *(pbuf - 320) + *(pbuf - 319) +
                                         *(pbuf + 1)   + *(pbuf + 321) + *(pbuf + 320)  ) / 6;
                        else if( 319 == x )
                                *pbuf = (*pbuf         + *(pbuf - 320) + *(pbuf + 320) +
                                         *(pbuf - 1)   + *(pbuf - 321) + *(pbuf - 319)  ) / 6;
                        else
                                *pbuf = (*pbuf         + *(pbuf - 320) + *(pbuf - 319) +
                                         *(pbuf + 1)   + *(pbuf + 321) + *(pbuf + 320) +
                                         *(pbuf - 1)   + *(pbuf - 321) + *(pbuf - 319)  ) / 9;
                        if( *pbuf > 10 )
                                (*pbuf)--;
                        else
                                *pbuf = 10;
                } // for
} // FlameThread::ScrollUp

void FlameThread::NewLine( void )
{
        int   x, y;
        PBYTE pbuf = &pBuffer[63040];

        for( y = 197; y < 200; y++ )
                for( x = 0; x < 320; x++, pbuf++ )
                {
                        BYTE c;

                        do
                        {
                                c = rand();
                        } while( (c < 75) || (c > 90) );
                        *pbuf = c;
                } // for

        for( y = 0; y < rand() % 45; y++ )
        {
                x = rand() % 320;
                pbuf = &pBuffer[x + 63040];

                *pbuf           = 255;
                *(pbuf + 320)   = 255;
                *(pbuf + 640)   = 255;
                *(pbuf + 1)     = 255;
                *(pbuf + 321)   = 255;
                *(pbuf + 641)   = 255;
                *(pbuf - 1)     = 255;
                *(pbuf - 321)   = 255;
                *(pbuf - 641)   = 255;
        } // for
} // FlameThread::NewLine

void FlameThread::Init( void )
{
        PrepPal();
        pBuffer = pBlitter->GetVirtualScreen()->GetBuffer();
        srand( 42 );

        while( TRUE )
        {
                ScrollUp();
                NewLine();
                pBlitter->Blit();
                DosSleep( 0 );
        } // while
} // FlameThread::Init

/***
FlameApp::FlameApp( void ) : XApplication()
{
        // initialize virt. screen and blitter
        pScreen  = new XVirtualScreen( 320, 200 );
        pBlitter = new XBlitter();
        pBlitter->AssociateVirtualScreen( pScreen );

        // initialize window
        XResource        xres( 100, GetResourceLibrary() );
        XRect            xrect( 100, 100, 320, 230 );
        pWindow = new FlameWindow( &xres,
                                   "Flame demo",
                                   pBlitter,
                                   BLT_NODIRTYRECT,
                                   XFrameWindow::defaultStyle,
                                   &xrect                      );

        pThread = new FlameThread( pBlitter );
        pThread->Run();
} // FlameApp::FlameApp


FlameApp::~FlameApp() //virtual raus, svb
{
        pThread->Terminate();
        delete pThread;
        pBlitter->DisassociateVirtualScreen();
        delete pBlitter;
        delete pScreen;
        delete pWindow;
} // FlameApp::~FlameApp
***********/

void main( void )
{
   XVirtualScreen * pScreen  = new XVirtualScreen( 320, 200 );
   XBlitter * pBlitter = new XBlitter();
   pBlitter->AssociateVirtualScreen( pScreen );

   XRect xrect( 100, 100, 320, 230 );
   // initialize window
   FlameWindow * pWindow = new FlameWindow( 100,
                                   "Flame demo",
                                   pBlitter,     0,
                                   XFrameWindow::defaultStyle,
                                   xrect                      );

   FlameThread * pThread = new FlameThread( pBlitter );
   pThread->Run();

   XApplication::GetApplication()->Start();
/****
   pThread->Terminate();

   delete pThread;
   pBlitter->DisassociateVirtualScreen();
   delete pBlitter;
   delete pScreen;
****/
//   delete pWindow; wird automatisch gel”scht

}

