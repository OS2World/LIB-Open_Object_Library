#include "xgraphdv.h"
#include "xlayer.h"
#include <stdlib.h>

/*@
@class XLayer
@parent XObject
@type overview
@symbol _
@remarks XLayer is a class which can store a large amount of graphic-objects.
Call AddObject() to add objects to the layer, with Draw you can draw the content
on a graphic device like XGraphicDevice or XPrinterDevice
*/


/*@ XLayer::AddObject(const XGraphicObject * obj)
@group Add/Remove objects
@remarks Adds an object
@parameters   XGraphicObject * the object to add
*/
void XLayer::AddObject(const XGraphicObject * obj)
{
   listCount ++;
   list = (XGraphicObject**) realloc( list, listCount * sizeof(void*));
   list[listCount -1] = (XGraphicObject*) obj;
}


/*@ XLayer::RemoveObject(XGraphicObject * obj, const BOOL destroyObject)
@group Add/Remove objects
@remarks Removes an object
@parameters
XGraphicObject * the object to add
<BR>
BOOL destroyObject   TRUE=call delete, FALSE=dont destroy, default is TRUE
*/
BOOL XLayer::RemoveObject(XGraphicObject * obj, const BOOL destroyObject)
{
   BOOL swap = FALSE;

   for(int i = 0; i < listCount; i++)
   {
      if(list[i] == obj)
      {
         swap = TRUE;
         if(destroyObject)
            delete list[i];
      } /* end if */
      if(swap && i < listCount - 1)
         list[i] = list[i + 1];
   } /* end for */
   if(swap)
      listCount -= 1;
   return swap;
}


XGraphicObject * XLayer::FindObject(const LONG x, const LONG y, const XGraphicObject * searchBehind)
{
/*
   LONG x1 = x;
   LONG y1 = y;

   XGraphicObject *b;

   if (searchBehind)
      b = searchBehind->next;
   else
      b = first;

   while (b)
   {
      if (b->HitTest(x1, y1) == TRUE)
         return b;
      b = b->next;
   }
*/
   return NULL;
}


/*@ XLayer::Empty(BOOL destroy)
@group Add/Remove objects
@remarks Removes all objects
@parameters
BOOL destroyAll   TRUE=call delete for all objects, FALSE=dont destroy, default is TRUE
*/
void XLayer::Empty(const BOOL destroy)
{
   if(destroy)
   {
      for(int i = 0; i < listCount; i++)
      {
         delete list[i];
      }
   }
   free(list);
   list = NULL;
   listCount = 0;
}
