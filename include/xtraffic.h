#ifndef __OOL_XTRAFIC_H__
#define __OOL_XTRAFIC_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTraficLight                          */
/*                       derived from: XUserWindow                           */
/*                        last update: 7/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "XfrmWnd.h"
#include "xuserwnd.h"
#include "xtext.h"

class XBitmap;
class XGraphicDevice;

#define TRAFFIC_OFF         0
#define TRAFFIC_GREEN         1
#define TRAFFIC_YELLOW      2
#define TRAFFIC_RED         3
#define TRAFFIC_YELLOWRED   4

class _export_ XTrafficLight: public XUserWindow
{
   private:
      SHORT state;
      XGraphicDevice * dev;
      XBitmap * map;
   public:
      virtual void Draw( void );
      XTrafficLight( const XWindow *, const XPoint& = XPoint(), const USHORT id = 0);
      ~XTrafficLight();
      void SetState( const SHORT );
      SHORT GetState() { return state; }
};

#endif
