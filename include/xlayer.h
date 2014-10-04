#ifndef __OOL_XLAYER_H__
#define __OOL_XLAYER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XLayer                                */
/*                       derived from: XObject                               */
/*                        last update: 1/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xgraphob.h"

class _export_ XLayer: public XObject
{
   private:
      XGraphicObject ** list;
      USHORT listCount;
   public:
      XLayer() { list = NULL; listCount = 0; }
      void Draw( XGraphicDevice*dev)
         {
            for(int i = 0; i < listCount; i++)
               list[i]->Draw(dev);
         }
      void Empty(const BOOL destroyAll=TRUE);
      void AddObject ( const XGraphicObject*);
      XGraphicObject * FindObject( const LONG x, const LONG y, const XGraphicObject * searchBehind = NULL);
      BOOL RemoveObject( XGraphicObject*, const BOOL destroyObject = TRUE);
};

#endif