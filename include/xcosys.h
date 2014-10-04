#ifndef __OOL_XCOSYS_H__
#define __OOL_XCOSYS_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCoordinateSystem                     */
/*                       derived from: XObject                               */
/*                        last update: 3/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xfrmwnd.h"


class XCoordinateSystem: public XObject
{
   public:
      static void Convert( XRect * rect, const XWindow * window = NULL)
         {
            XRect r;
            if( window )
            {
               window->GetSize(&r);
               if(WinWindowFromID(window->GetHandle(), FID_TITLEBAR) != 0)
                  r.SetHeight( r.GetHeight() - XWindow::GetSysValue( SV_CYTITLEBAR ));
            } /* end if */
            else
               r.SetHeight( XWindow::GetSysValue( SV_CYSCREEN ) );
            rect->SetY(r.GetHeight() - rect->GetY() - rect->GetHeight());
         }
};

#endif