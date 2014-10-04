#ifndef __OOL_XCHART_H__
#define __OOL_XCHART_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XChart                                */
/*                       derived from: XUserWindow                           */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xuserwnd.h"
#include "xgraphdv.h"
#include "XLayer.h"
#include "xstring.h"
#include "xbox.h"
#include "XText.h"
#include "XFont.h"


typedef struct chartItem
{
   double value;
   //LONG color;
} ChartItem;


class _export_ XChart:public XUserWindow
{
   protected:
      XGraphicDevice * graph;
      XLayer layer;
      chartItem ** item;
      USHORT itemCount;
      LONG style;
      XBox * box;
      XChart( const XWindow*, const XRect& rec=XRect(), const LONG id=0, const LONG style=0, const char * t = "");
      XFont * titleFont;
      XText * title;
   public:
      virtual ~XChart();
      virtual void Draw() { graph->FillBackground(); title->Draw(graph); layer.Draw(graph); }
      void GetBackgroundColor( XColor * c) const;
      virtual void GetItemColor( const USHORT index, XColor * ) { ;}
      double GetItemValue( const USHORT index );
      XGraphicDevice * GetGraphicDevice( void) const { return graph; }
      XText * GetTitleObject( void ) const { return title; }
      virtual BOOL SetBackgroundColor( const XColor * rgbColor);
      virtual void SetItemColor( const USHORT index, const XColor *) { ;}
      virtual void SetItemCount( const USHORT count );
      void SetItemValue( const USHORT index, const double value);
      void SetSize( const XRect*) const;
      void SetSize( const ULONG xPos, const ULONG yPos, const ULONG width, const ULONG height) const ;
      virtual void SetText(const char * text);
};

#endif
