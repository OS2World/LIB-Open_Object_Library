#include "XContain.h"
#include "XContInf.h"
#include "XContObj.h"
#include "XControl.h"
#include "XString.h"
#include "XMsgBox.h"
#include "XContCol.h"

#include <string.h>
#include <stdlib.h>


/*@
@class XContainerControl
@parent XControl
@type overview
@symbol _
@remarks XContainerControl represents a container with multiple views like tree-, list- or icon view for a large amount of objects.
*/


/*@ XContainerControl :: SetObjectEmphasis( const XContainerObject * obj, const SHORT emph, const BOOL enableEmph)
@group object settings
@remarks Sets the emphasis of an object so displaying the objects icon is changed
@parameters <t 'ø' c=2>
            øXContainerObject * øobject to change
            øSHORT øthe needed emphasis, valid values are:
<P>CON_CURSORED the object is cursored
<P>CON_DISABLED the object is disabled
<P>CON_INUSE the objects icon is displayed open
<P>CON_PICKED the objects icon is displayed dragged
<P>CON_SELECTED the object is selected
<P>CON_SOURCE the object get source-emphasis
<P>(can be or-ed)
            øBOOL øenable/disable emphasis
            </t>
*/
void XContainerControl::SetObjectEmphasis(const XContainerObject * obj, const SHORT emph, const BOOL enableEmph) const
{
   WinSendMsg(winhandle, CM_SETRECORDEMPHASIS, MPFROMP(obj->core), MPFROM2SHORT(enableEmph, emph));
}


/*@ XContainerControl::BeginEdit(const XContainerObject * obj, const XContainerColumn * col, const ULONG winID)
@group direct editing
@remarks Opens the window for direct editing
@parameters   <t 'ø' c=2>
øXContainerObject * øThe object to edit. If the title of the container or the title of a column
                            should be edited, this parameters must be NULL.
øXContainerColumn * øIf not in detail-view, this parameter must be NULL. In detail-view this
                             parameter is a pointer to the column in which data should be edited or
                             a pointer to the column which title should be edited.
øULONG øIf <B>not</B> in detail view, this parameters should be zero if the title of an
                            object should be edited. If the container title is to edit use CID_CNRTITLEWND.<P>
                            In detail-view:
<P>CID_CNRTITLEWND to edit the tilte of the container
<P>CID_LEFTCOLTITLEWND to edit the tilte of a column
<P>CID_LEFTDVWND to edit an oject
<P>If the container is split, you may use CID_RIGHTDVWND and CID_RIGHTCOLTITLEWND instead.
</t>
@returns      TRUE   success
*/
BOOL XContainerControl::BeginEdit(const XContainerObject * obj, const XContainerColumn * col, const ULONG winID) const
{
   CNREDITDATA cnredit;

   memset( &cnredit, 0, sizeof(cnredit));

   cnredit.cb = sizeof(cnredit);
   cnredit.hwndCnr = winhandle;

   if(obj)
      cnredit.pRecord = obj->core;

   if(col)
      cnredit.pFieldInfo = col->info;

   if( winID == 0)
      cnredit.id = WinQueryWindowUShort( winhandle, QWS_ID);
   else
      cnredit.id = winID;

   return (BOOL) WinSendMsg( winhandle, CM_OPENEDIT, MPFROMP(&cnredit), 0);
}


void XContainerControl::EndEdit() const
{
   WinSendMsg( winhandle, CM_CLOSEEDIT, 0, 0);
}


BOOL XContainerControl::GetObjectRect(XRect * r, const XContainerObject * o, const BOOL rightWin, const SHORT extension) const
{
   RECTL rect;
   QUERYRECORDRECT query;
   query.cb = sizeof(query);
   query.pRecord = o->core;
   query.fRightSplitWindow = rightWin;
   query.fsExtent = extension;
   if( (BOOL) WinSendMsg( winhandle, CM_QUERYRECORDRECT, MPFROMP(&rect), MPFROMP(&query)) == TRUE)
   {
      r->SetX( rect.xLeft);
      r->SetY( rect.yBottom);
      r->SetWidth( rect.xRight - rect.xLeft);
      r->SetHeight( rect.yTop - rect.yBottom );
      return TRUE;
   }
   else
      return FALSE;
}


/*@ XContainerControl :: ExpandTreeObject( const XContainerObject * obj, const BOOL expand)
@group expanding
@remarks Expands/collaps an object in tree-view.
@parameters   <t 'ø' c=2>
            øXContainerObject * obj      øthe object to expand/collaps
            øBOOL expand                  øTRUE=expand<BR>FALSE=collapse
            </t>
@returns      TRUE   success
*/
BOOL XContainerControl::ExpandTreeObject(const XContainerObject * obj, const BOOL expand) const
{
   if (expand)
      return (BOOL) WinSendMsg(winhandle, CM_EXPANDTREE, MPFROMP(obj->core), 0);
   else
      return (BOOL) WinSendMsg(winhandle, CM_COLLAPSETREE, MPFROMP(obj->core), 0);
}


SHORT EXPENTRY SortRecord(const PRECORDCORE p1, const PRECORDCORE p2, const void *p3);


/*@ XContainerControl :: SortObjects( void )
@group misc
@remarks Sort the objects in the container. On default the objects are sorted
by the title of the object. To sort them on a user-defined way you must override
the method Sort() of the XContainerObject.
*/
void XContainerControl::SortObjects(void) const
{
   WinSendMsg(winhandle, CM_SORTRECORD, MPFROMP(SortRecord), 0);
}


/*@ XContainerControl :: HScroll( const LONG pix )
@group display
@remarks Scrolls the container content horizontal
@parameters LONG how much pixels to scroll
*/
void XContainerControl::HScroll(const LONG pix) const
{
   WinSendMsg(winhandle, CM_SCROLLWINDOW, MPFROMSHORT(CMA_HORIZONTAL), (MRESULT) pix);
}


/*@ XContainerControl :: VScroll( const LONG pix )
@group display
@remarks Scrolls the container content vertical
@parameters LONG how much pixels to scroll
*/
void XContainerControl::VScroll(const LONG pix) const
{
   WinSendMsg(winhandle, CM_SCROLLWINDOW, MPFROMSHORT(CMA_VERTICAL), (MRESULT) pix);
}


/*@ XContainerControl :: UpdateColumns( void )
@group columns
@remarks Update columns in detail-view. If you have inserted, deleted or modified
one ore more columns you must call UpdateColumns to make your changes visible.
*/
void XContainerControl::UpdateColumns(void) const
{
   WinSendMsg(winhandle, CM_INVALIDATEDETAILFIELDINFO, 0, 0);
}


/*@ XContainerControl :: FindObject( const XContainerObject * startObject, const SHORT select)
@group searching
@remarks Get an object from the container with relative positions
@parameters <t 'ø' c=2>
            øXContainerObject * startObject  øsearch relative to this object (default is NULL)
            øSHORT select                    øserach settings, valid values are:
<P>CON_FIRST get the first object of the container
<P>CON_FIRSTCHILD get the first child of startObject
<P>CON_LAST get the last object of the container
<P>CON_LASTCHILD get the last child of startObject
<P>CON_NEXT get the next object relative to startObject
<P>CON_PARENT get the parent of startObject
<P>CON_PREV get the previous object relative to startObject
<P>default is CON_FIRST
            </t>
@returns    XContainerObject *              pointer to the found object (NULL if nothing is found)
*/
XContainerObject *XContainerControl::FindObject(const XContainerObject * startObject, const SHORT select)
{
   RECORDCORE *res, *rel = NULL;

   if (startObject != NULL)
      rel = startObject->core;

   res = (RECORDCORE *) WinSendMsg(winhandle, CM_QUERYRECORD, MPFROMP(rel), MPFROM2SHORT(select, CMA_ITEMORDER));

   if (res > 0)
   {
      RECORDCORE *pr = (RECORDCORE *) ((PBYTE) res + sizeof(RECORDCORE));
      XContainerObject *obj;

      memcpy(&obj, pr, 4);
      return obj;
   }
   else
      return NULL;
}


/*@ XContainerControl :: GetObject( const XContainerObject * startObject, const SHORT filter)
@group searching
@remarks Get an object from the container by querying the objects emphasis
@parameters <t 'ø' c=2>
            øXContainerObject * østart-object, search relative to this object (default is NULL)
            øSHORT øthe emphasis to search
<P>CON_CURSORED the object has the cursored
<P>CON_DISABLED the object is disabled
<P>CON_INUSE the objects icon is displayed open
<P>CON_PICKED the objects icon is displayed dragged
<P>CON_SELECTED the object is selected
<P>CON_SOURCE the object get source-emphasis
<P>(default is CON_SELECTED, can be or-ed)
            </t>
@returns    XContainerObject *              pointer to the found object (NULL if nothing is found)
*/
XContainerObject *XContainerControl::GetObject(const XContainerObject * startObject, const SHORT filter)
{
   RECORDCORE *res, *rel;
   XContainerObject *obj;

   if (startObject)
      rel = startObject->core;
   else
      rel = (PRECORDCORE) CON_FIRST;

   res = (RECORDCORE *) WinSendMsg(winhandle, CM_QUERYRECORDEMPHASIS, MPFROMP(rel), MPFROMSHORT(filter));

   if (res > 0)
   {
      PRECORDCORE pr = (PRECORDCORE) ((PBYTE) res + sizeof(RECORDCORE));

      memcpy(&obj, pr, 4);
      return obj;
   }
   else
      return NULL;
}


/*@ XContainerControl :: InvalidateObject( const XContainerObject * obj, const SHORT option) const
@group display
@remarks Invalidates one ore all objects
@parameters <t 'ø' c=2>
            øXContainerObject * øthe object to invalidate (default is NULL)
           øSHORT øoptions, valid values are:
<P>CON_ERASE erase the background
<P>CON_REPOSITION reposition
<P>CON_NOREPOSITION no reposition
<P>CON_TEXTCHANGED redraw the text
<P>(default is CON_REPOSITION, can be or-ed)
         </t>
*/
void XContainerControl::InvalidateObject(const XContainerObject * obj, const SHORT option) const
{
   if (obj)
      WinSendMsg(winhandle, CM_INVALIDATERECORD, MPFROMP(&obj->core), MPFROM2SHORT(1, option));
   else
      WinSendMsg(winhandle, CM_INVALIDATERECORD, 0, 0);
}


/*@ XContainerControl :: RemoveObject( XContainerObject * obj, const BOOL destroyObject, const BOOL redraw)
@group inserting/removing objects
@remarks Removes an object from the container
@parameters <t 'ø' c=2>
            øXContainerObject * øthe object to remove
            øBOOL øif TRUE the destructor of the object is called
                                            if FALSE it exist (usefull if an object is member
                                            of multiple container-controls)
                                            (default is TRUE)
            øBOOL øredraw the container (default is TRUE)
            </t>
@returns    BOOL                            success
*/
BOOL XContainerControl::RemoveObject(XContainerObject * obj, const BOOL destroyObject, const BOOL redraw) const
{
   SHORT settings = 0;
   if(obj == NULL)
      return FALSE;
   if(obj->core == NULL)
      return FALSE;
   if (destroyObject)
      settings = CMA_FREE;
   if (redraw)
      settings |= CMA_INVALIDATE;

   if ((LONG) WinSendMsg(winhandle, CM_REMOVERECORD, MPFROMP(&obj->core), MPFROM2SHORT(1, settings)) == -1)
      return FALSE;

   if (destroyObject)
      delete obj;            // new
   return TRUE;
}


BOOL XContainerControl :: FreeObject(XContainerObject*obj)
{
   BOOL res = (BOOL) WinSendMsg(winhandle, CM_FREERECORD, MPFROMP(&obj->core), MPFROMSHORT(1));
   obj->core = NULL;
   return res;
}


/*@ XContainerControl :: RemoveObjectList( XContainerObject ** obj, const SHORT count, const BOOL destroyObject, const BOOL redraw)
@group inserting/removing objects
@remarks Removes an array of objects from the container
@parameters <t 'ø' c=2>
            øXContainerObject ** øan array of pointer to objects to remove
            øSHORT øthe count of object in the array
            øBOOL øif TRUE the destructor of the objects are called
                                            if FALSE it exist (usefull if an object is member
                                            of multiple container-controls)
                                            (default is TRUE)
            øBOOL øredraw the container (default is TRUE)
            </t>
@returns    BOOL                            success
*/
BOOL XContainerControl::RemoveObjectList(XContainerObject ** obj, const SHORT count, const BOOL destroyObject, const BOOL redraw) const
{
   SHORT settings = 0;

   if (destroyObject)
      settings = CMA_FREE;
   if (redraw)
      settings |= CMA_INVALIDATE;
   RECORDCORE **core = (RECORDCORE **) malloc(count * sizeof(void *));
   int i;

   if(obj == NULL)
      return FALSE;

   for (i = 0; i < count; i++)
   {
      if(obj[i]->core == NULL)
         return FALSE;
      core[i] = obj[i]->core;
   } /* end for */

   if ((LONG) WinSendMsg(winhandle, CM_REMOVERECORD, MPFROMP(core), MPFROM2SHORT(count, settings)) == -1)
      return FALSE;

   if (destroyObject)
   {
      for (i = 0; i < count; i++)
         delete obj[i];
   }
   free(core);

   return TRUE;
}


/*@ XContainerControl::AddObject(XContainerObject * object, XContainerObject * parentObject, XContainerObject * sibObject, BOOL draw)
@group inserting/removing objects
@remarks Add an object to the container. If you insert a large amount of objects you should set
parameter <redraw> to FALSE and call InvalidateObject(NULL) if you have finished inserting.
@parameters <t 'ø' c=2>
            øXContainerObject * øobject to add
            øXContainerObject * øparent of the object (for tree-view), default is NULL
            øXContainerObject * øsibling object to insert behind, or:
<P>CON_FIRST insert at the top
<P>CON_END insert at the end
<P>In this cases you have to make a typecast: (XContainerObject *), default is CON_END
            øBOOL øredraw the object
            </t>
@returns    BOOL                            success
*/
BOOL XContainerControl::AddObject(XContainerObject * object, XContainerObject * parentObject, XContainerObject * sibObject, BOOL draw)
{
   RECORDINSERT ri;

   ri.cb = sizeof(RECORDINSERT);
   ri.zOrder = CMA_BOTTOM;
   ri.cRecordsInsert = 1;
   ri.fInvalidateRecord = draw;
   ri.pRecordParent = parentObject ? parentObject->core : NULL;
   if (sibObject)
   {
      if (sibObject != (XContainerObject *) CMA_END && sibObject != (XContainerObject *) CMA_FIRST)
         ri.pRecordOrder = sibObject->core;
      else
         ri.pRecordOrder = (PRECORDCORE) sibObject;
   }
   else
      ri.pRecordOrder = (PRECORDCORE) CMA_END;
   return (BOOL) WinSendMsg(winhandle, CM_INSERTRECORD, (MPARAM) object->core, (MPARAM) & ri);
}


/*@ XContainerControl::AddObjectList(XContainerObject ** objectList, const USHORT count, XContainerObject * parentObject, XContainerObject * sibObject, BOOL draw)
@group inserting/removing objects
@remarks Add an object to the container. If you insert a large amount of objects you should set
parameter <redraw> to FALSE and call InvalidateObject(NULL) if you have finished inserting.
@parameters <t 'ø' c=2>
            øXContainerObject ** theObject    øan array of pointer to objects to add
            øSHORT count                        øcount of object in the array
            øXContainerObject * parentObject   øparent of the objects (for tree-view)
                                            (default is NULL)
            øXContainerObject * sibObject   øsibling object to insert behind, or:
<P>CON_FIRST insert at the top
<P>CON_END insert at the end
<P>In this cases you have to make a typecast: (XContainerObject *), default is CON_END
            øBOOL redraw                    ødraw the objects
            </t>
@returns    BOOL                            success
*/
BOOL XContainerControl::AddObjectList(XContainerObject ** objectList, const USHORT count, XContainerObject * parentObject, XContainerObject * sibObject, BOOL draw)
{
   if (count == 0)
      return TRUE;
   RECORDINSERT ri;

   ri.cb = sizeof(RECORDINSERT);
   ri.zOrder = CMA_BOTTOM;
   ri.cRecordsInsert = count;
   ri.fInvalidateRecord = draw;
   ri.pRecordParent = parentObject ? parentObject->core : NULL;
   if (sibObject)
   {
      if (sibObject != (XContainerObject *) CMA_END && sibObject != (XContainerObject *) CMA_FIRST)
         ri.pRecordOrder = sibObject->core;
      else
         ri.pRecordOrder = (PRECORDCORE) sibObject;
   }
   else
      ri.pRecordOrder = (PRECORDCORE) CMA_END;
   RECORDCORE **core = (RECORDCORE **) malloc(count * sizeof(void *));

   for (int i = 0; i < count; i++)
      core[i] = objectList[i]->core;
   return (BOOL) WinSendMsg(winhandle, CM_INSERTRECORDARRAY, (MPARAM) core, (MPARAM) & ri);
   free(core);
}


/*@ XContainerControl::GetInfo(XContainerInfo * info)
@group info
@remarks Querys information about the container.
@parameters    XContainerInfo * buffer to hold the information (See the description of XContainerInfo )
*/
void XContainerControl::GetInfo(XContainerInfo * info)
{
   WinSendMsg(winhandle, CM_QUERYCNRINFO, MPFROMP(&info->cnrinfo), (MRESULT) sizeof(CNRINFO));
   info->changes = 0;
}


/*@ XContainerControl::SetInfo(XContainerInfo * info)
@group info
@remarks Set information how to display the container.
@parameters    XContainerInfo * See the description of XContainerInfo
*/
void XContainerControl::SetInfo(XContainerInfo * info)
{
   if( info->cnrinfo.pszCnrTitle )
   {
      title = (char*) info->cnrinfo.pszCnrTitle;
      info->cnrinfo.pszCnrTitle = (PSZ) (char*) title;
   }
   WinSendMsg(winhandle, CM_SETCNRINFO, MPFROMP(&info->cnrinfo), MPFROMLONG(info->changes));
   info->changes = 0;
}


/*@ XContainerControl::Arrange(void)
@group display
@remarks Rearrange the object is the container
*/
void XContainerControl::Arrange(void) const
{
   WinSendMsg(winhandle, CM_ARRANGE, 0, 0);
}


void XContainerControl::Clean(PRECORDCORE first)
{
   XContainerObject *o;
   PRECORDCORE pr;
   PRECORDCORE core = first, buffer;

   while (core)
   {
      pr = (RECORDCORE *) ((PBYTE) core + sizeof(RECORDCORE));
      PRECORDCORE co2 = (PRECORDCORE) WinSendMsg(winhandle, CM_QUERYRECORD, core, MPFROM2SHORT(CMA_FIRSTCHILD, CMA_ITEMORDER));

      if (co2)
         Clean(co2);
      memcpy(&o, pr, 4);
      if (o)
         delete o;

      buffer = (PRECORDCORE) WinSendMsg(winhandle, CM_QUERYRECORD, core, MPFROM2SHORT(CMA_NEXT, CMA_ITEMORDER));
      WinSendMsg(winhandle, CM_REMOVERECORD, &core, MPFROM2SHORT(1, CMA_FREE));
      core = buffer;
   }
}


/*@ XContainerControl::RemoveAll(BOOL destroyRecords)
@group inserting/removing objects
@remarks Remove all objects from a container
@parameters    BOOL destroy objects, the destructors of the objects are called and
                                         memory ascociated with the container is freed.
*/
void XContainerControl::RemoveAll(BOOL destroyRecords)
{
   if (destroyRecords)
   {
      PRECORDCORE core = (PRECORDCORE) WinSendMsg(winhandle, CM_QUERYRECORD, NULL, MPFROM2SHORT(CMA_FIRST, CMA_ITEMORDER));

      if (core == (PRECORDCORE) - 1)
         return;
      Clean(core);
   }
   else
      WinSendMsg(winhandle, CM_REMOVERECORD, 0, 0);
}



/*@ XContainerControl :: XContainerControl(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(rec, style, owner, "", WC_CONTAINER, id, font)
@group constructors/destructors
@remarks
@parameters <t 'ø' c=2>
               øXWindow * theOwner        øThe owning window.
               øXRect& rectangle         øPosition and size.
               øUSHORT id                 øThe id of the window.
                                         Default is zero.
               øULONG style               øThe style. Valid values are (additonaly to the XWindow styles):
<P>CON_AUTOPOSITION  objects are arranged automaticaly
<P>CON_EXTENDSEL extended selection is enabled
<P>CON_MULTIPLESEL mutiple selection is enabled
<P>CON_READONLY directe diting is disabled
<P>CON_SINGLESEL only one object can be selected
            </t>
*/
XContainerControl :: XContainerControl(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(&rec, style, owner, "", WC_CONTAINER, id, font)
{
}


/*@ XContainerControl::RemoveColumn(XContainerColumn * column, const SHORT option)
@group columns
@remarks Removes a column
@parameters    <t 'ø' c=2>
               øXContainerColumn * theColumn   øThe column to delete
               øSHORT option                   øHow to delete. Valid values are:
<P>CON_FREE Destruct the column and free container related memory
<P>CON_INVALIDATE Redraw the container
               </t>
@returns       BOOL                           success
*/
BOOL XContainerControl::RemoveColumn(XContainerColumn * column, const SHORT option)
{
   LONG res = (LONG) WinSendMsg(winhandle, CM_REMOVEDETAILFIELDINFO, MPFROMP(column->info), MPFROM2SHORT(1, option));

   if (option & CON_FREE)
      delete column;

   if (res < 0)
      return FALSE;
   else
      return TRUE;
}


/*@ XContainerControl::GetColumn(const XContainerColumn * col, const SHORT option)
@group columns
@remarks Query a column from a container.
@parameters    <t 'ø' c=2>
               øXContainerColumn * øRelative position to begin search. In cases CMA_FIRST and CMA_LAST ignored.
               øSHORT øValid values are:
<P>CON_FIRST Get the first column.
<P>CON_LAST Get the last column.
<P>CON_NEXT Get the next relative to theColumnToSearch.
<P>CON_PREV Get the previous relative to theColumnToSearch.
               </t>
@returns       XContainerControl *                   The found column.
*/
XContainerColumn * XContainerControl::GetColumn(const XContainerColumn * col, const SHORT option)
{
   FIELDINFO * i = (FIELDINFO *) WinSendMsg(winhandle, CM_QUERYDETAILFIELDINFO, MPFROMP( col ? col->info : NULL), MPFROMSHORT(option));
   if (i == NULL || i == (FIELDINFO *) - 1)
      return NULL;
   else
   {
      return (XContainerColumn *) i->pUserData;
   }
}


/*@ XContainerControl::InsertColumn(const XContainerColumn * column, const XContainerColumn * insertBehind, const BOOL redraw)
@group columns
@remarks Insert a column into a container. After you have added one or more columns,
you must call Invalidate() so the columns are drawn.
@parameters    <t 'ø' c=2>
øXContainerColumn * øThis column should be inserted
øXContainerColumn * øThe position to insert. Valid values are:<BR>
CON_FIRST Insert as the first column<BR>
CON_LAST Insert as the last column<BR>
XContainerColumn*  The column behind which the column should be inserted.
               </t>
@returns       BOOL                                  success
*/
BOOL XContainerControl::InsertColumn(const XContainerColumn * column, const XContainerColumn * insertBehind, const BOOL redraw)
{
   FIELDINFOINSERT insert;

   insert.cb = sizeof(insert);

   insert.fInvalidateFieldInfo = redraw;
   if (insertBehind)
   {
      if (insertBehind != (XContainerColumn *) CMA_FIRST && insertBehind != (XContainerColumn *) CMA_END)
         insert.pFieldInfoOrder = insertBehind->info;
      else
         insert.pFieldInfoOrder = (FIELDINFO *) insertBehind;
   }
   else
      insert.pFieldInfoOrder = (FIELDINFO *) CMA_FIRST;

   insert.cFieldInfoInsert = 1;

   if (SHORT1FROMMR(WinSendMsg(winhandle, CM_INSERTDETAILFIELDINFO, MPFROMP(column->info), MPFROMP(&insert))) == 0)
      return FALSE;
   else
      return TRUE;
}


/*@ XContainerControl::TitleEdited(const char * )
@group misc
@remarks This method is called if the user has edited the title of the container.
Overwrite this method if you need the information
@parameters char * the new Title
*/

