#include "XContInf.h"
#include "XContObj.h"
#include "XString.h"
#include "XContain.h"
#include "XContCol.h"
#include "XDate.h"
#include "XTime.h"
#include "XBitmap.h"
#include "XIcon.h"
#include "xsize.h"

#include <string.h>

/*@
@class XContainerColumn
@parent XObject
@type overview
@symbol _
*/

/*@
@class XContainerObject
@parent XObject
@type overview
@symbol _
*/
XContainerColumn :: ~XContainerColumn()
{
   if (owner && info)
      WinSendMsg(owner->GetHandle(), CM_FREEDETAILFIELDINFO, MPARAM(info), MPFROMSHORT(1));
}


/*@ XContainerColumn::XContainerColumn( XContainerControl * ow, void * titleData, SHORT dataColumn, LONG dataSettings, LONG titleSettings, LONG width)
@group constructors/destructors
@remarks If a container should be displayed in detail-mode you must add columns to it. Therfore
you construct one or more XContainerColums, add these with XContainerControll::AddColumn
and finaly call XContainerControll::UpdateColumns.
@parameters    <t 'ø' c=2>
               øXContainerControl *   øthe owner of the column
               øvoid *                 øthe title of the column, the parameter can be a
                                               pointer to a string or contain a handle for a
                                               bitmap or icon
               øSHORT                øa zero-based index which points to the data-column
                                               which will be used in XContainerObject::SetColumnData()
               øLONG               øInformation about the datas in the column
<P>COL_LEFT field is left-justified
<P>COL_RIGHT field is right-justified
<P>COL_CENTER          field is horizontal centered
<P>COL_TOP             field is top-justified
<P>COL_VCENTER         field is vertictal centered
<P>COL_BOTTOM          field is bottom-justified
<P>COL_INVISIBLE       the column is invisible
<P>COL_SEPARATOR       vertical separator
<P>COL_HORZSEPARATOR   horizontal separator
<P>COL_BITMAPORICON    the data contain a handle to a bitmap/icon
<P>COL_STRING          the data contain a string
<P>COL_DATE            the data contain a date
<P>COL_TIME            the data contain a time-value
<P>COL_ULONG           the data contain a ULONG value
<P>COL_FIREADONLY      datas are readonly
<P>Can be or-ed;
               øLONG titleSettings              øInformation about the title of the column
<P>COL_LEFT            title is left-justified
<P>COL_RIGHT           title is right-justified
<P>COL_CENTER          title is horizontal centered
<P>COL_TOP             title is top-justified
<P>COL_VCENTER         title is vertictal centered
<P>COL_BOTTOM          title is bottom-justified
<P>COL_SEPARATOR       vertical separator
<P>COL_HORZSEPARATOR   horizontal separator
<P>COL_BITMAPORICON    the title contain is a bitmap/icon
<P>COL_FTITLEREADONLY  title is readonly
<P>Can be or-ed;
               øLONG widthOfColumn              øthe width of the column in window-pixel. If zero
                                               the width of the column is set dynamicaly.<BR>
                                            Default is zero.
               </t>
*/

XContainerColumn :: XContainerColumn(XContainerControl * ow, void *titleData, SHORT dataColumn, LONG dataSettings, LONG titleSettings, LONG width)
{
   owner = ow;
   info = (FIELDINFO *) WinSendMsg(owner->GetHandle(), CM_ALLOCDETAILFIELDINFO, MPFROMSHORT(1), 0);

   info->flData = dataSettings;
   info->flTitle = titleSettings;
   info->offStruct = sizeof(RECORDCORE) + dataColumn * 4 + sizeof(void *);
   info->pUserData = this;
   col = dataColumn;
   if (width > 0)
      info->cxWidth = width;
   info->pTitleData = titleData;
   if( info->flData & COL_STRING )
   {
      title = (char*) info->pTitleData;
      info->pTitleData = (char*) title;
   }
}


/*@ XContainerColumn::GetColumnNumber()
@group misc
@remarks Returns the number of the column in the container
@returns SHORT number
*/


/*@ XContainerColumn::SetTitle()
@group title
@remarks Set the title
@parameters char * title
*/


/*@ XContainerColumn::SetTitle()
@group title
@remarks Set the title
@parameters XBitmap * bitmap for the title (tilt-attributes must have COL_BITMAPORICON)
*/


/*@ XContainerColumn::SetTitle()
@group title
@remarks Set the title
@parameters XIcon * bitmap for the title (tilt-attributes must have COL_BITMAPORICON)
*/


/*@ XContainerColumn::SetTitleAttributes()
@group title
@remarks Set attributes for the title
@parameters LONG attruibute (see constructor for details)
*/


/*@ XContainerColumn::SetDataAttributes()
@group data
@remarks Set attributes for the datas
@parameters LONG attruibute (see constructor for details)
*/


/*@ XContainerColumn::GetDataAttributes()
@group data
@remarks Query attributes for the datas
@returns LONG attruibute (see constructor for details)
*/


/*@ XContainerColumn::GetTitleAttributes()
@group title
@remarks Query attributes for the title
@returns LONG attruibute (see constructor for details)
*/


/*@ XContainerColumn::GetTitle()
@group title
@remarks Query the title of a column
@parameters   XString * buffer   buffer to hold the data
*/
void XContainerColumn::GetTitle(XString * buffer)
{
   strcpy(buffer->GetBuffer(100), (char *) info->pTitleData);
   buffer->ReleaseBuffer();
}


/*@ XContainerColumn::GetNextColumn()
@group misc
@remarks Returns a pointer to the next column of the container
@returns XContainerColumn * column
*/


/*@ XContainerObject::GetIcon( XIcon * icon )
@group icon/bitmap
@remarks Querry the icon of the object
@parameters XIcon * buffer
*/
void XContainerObject::GetIcon(XIcon * icon)
{
   if (icon->handle && icon->loaded)
   {
      WinFreeFileIcon(icon->handle);
      icon->loaded = FALSE;
   }
   icon->handle = core->hptrIcon;
}


/*@ XContainerObject :: GetBitmap( XBitmap * bitmap )
@group icon/bitmap
@remarks Get the bitmap of the object
@parameters XBitmap * buffer
*/
void XContainerObject::GetBitmap(XBitmap * bitmap)
{
   if (bitmap->hbm)
      GpiDeleteBitmap(bitmap->hbm);
   bitmap->hbm = core->hbmBitmap;
}


/*@ XContainerObject::XContainerObject( XContainerControl * owner, SHORT columns, LONG emphasis)
@group constructors/destructors
@remarks Construct a container-item
@parameters
<t 'ø' c=2>
øXContainerControl * øowner
øSHORT øcount of columns in detail-view (default is NULL)
</t>
*/
XContainerObject :: XContainerObject( const XContainerControl * owner, const SHORT columns, const LONG emphasis)
{
   core = (RECORDCORE *) WinSendMsg(owner->GetHandle(), CM_ALLOCRECORD, MPFROMLONG(sizeof(void *) + columns * sizeof(void *)), MPFROMSHORT(1));
   core->flRecordAttr = emphasis;
   RECORDCORE *pr = (RECORDCORE *) ((PBYTE) core + sizeof(RECORDCORE));
   void *pp = this;

   memcpy(pr, &pp, sizeof(void *));
}


/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øchar * ødata
</t>
*/

/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øLONG ødata
</t>
*/

/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øXBitmap * ødata
</t>
*/

/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øXIcon * ødata
</t>
*/

/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øXDate * ødata
</t>
*/


/*@ XContainerObject::SetColumnData()
@group columns
@remarks Set data for an column for detail-view
@parameters
<t 'ø' c=2>
øSHORT øcolumn-number (zero-based index)
øXTime * ødata
</t>
*/


/*@ XContainerObject::SetIcon()
@group icon/bitmap
@remarks Set object-icon
@parameters XIcon * icon
*/


/*@ XContainerObject::SetMiniIcon()
@group icon/bitmap
@remarks Set object mini-icon
@parameters XIcon * icon
*/

/*@ XContainerObject::SetBitmap()
@group icon/bitmap
@remarks Set object-bitmap.
@parameters XBitmap * bitmap
*/

/*@ XContainerObject::SetMiniBitmap()
@group icon/bitmap
@remarks Set the mini-bitmap of the object
@parameters XBitmap * bitmap
*/

/*@ XContainerObject::SetTitle()
@group title
@remarks Set the title of the object. Memory for the title is allocated by the object.
@parameters char * title
*/

/*@ XContainerObject::GetEmphasis()
@group emphasis
@remarks Query emphasis of the object.
@returns LONG emphasis (see XContainer::SetObjectEmphasis() for details)
*/


/*@ XContainerObject :: GetTitle( XString * s)
@group title
@remarks Query the title of the object;
@parameters XString * buffer
*/
void XContainerObject::GetTitle(XString * s)
{
   *s = title;
}


SHORT EXPENTRY SortRecord(const PRECORDCORE p1, const PRECORDCORE p2, const void *)
{
   XContainerObject *obj1, *obj2;

   PRECORDCORE pr = (PRECORDCORE) ((PBYTE) p1 + sizeof(RECORDCORE));

   memcpy(&obj1, pr, 4);

   pr = (PRECORDCORE) ((PBYTE) p2 + sizeof(RECORDCORE));
   memcpy(&obj2, pr, 4);

   return obj1->Sort(obj2);
}


/*@
@class XContainerInfo
@type overview
@symbol _
@remarks XContainerInfo is a class to set/query general attributes for a XContainerControl.
To make changes to the settings:
<OL COMPACT>
<LI>query the current setting via XContainerControl::GetInfo()
<LI>make the changes in the used instance of XContainerInfo
<LI>call XContainerControl::SetInfo() with the used instance of XContainerInfo
</OL>
*/


/*@ XContainerInfo::XContainerInfo( char * title, LONG style)
@group constructors/destructors
@remarks Constructor of XContainerInfo
@parameters    <t 'ø' c=2>
               øchar * theTitle           øThe title of the container. Default is empty.
               øLONG theStyle             østyle of the container. Valid styles are:
<P>CO_TEXT                  the container is in text-mode
<P>CO_NAME                  the container is in name-mode
<P>CO_ICON                  the container is in icon-mode
<P>CO_DETAIL                the container is in detail-mode
<P>CO_FLOW                  arrange objects dynamicaly (text and name mode only)
<P>CO_MINI                  the container use small icons
<P>CO_TREE              the container is in tree-mode
<P>CO_OWNERPAINTBACKGROUND  the owner will draw the background see XBackgroundDrawEvent and XBackgroundDrawHandler)
<P>CO_TREELINE              draw the tree-line in tree-mode
<P>CO_DRAWBITMAP            draw icons
<P>CO_DRAWICON              draw bitmaps
<P>CO_TITLE                 show the container-title
<P>CO_TITLELEFT             title is left-justified
<P>CO_TITLERIGHT            title is right-justified
<P>CO_TITLECENTER           title is centered
<P>CO_TITLESEPARATOR        the title is drawn with a separator
<P>CO_TITLEREADONLY         the title cannot been edited by the user
<P>CO_DETAILTITLES          in detail-mode the titles of XContainerColumn are shown
<P>The attributes can be or-ed, default is CO_ICON.
                  </t>
*/
XContainerInfo :: XContainerInfo(char *title, LONG style)
{
   memset(&cnrinfo, 0, sizeof(cnrinfo));
   changes = 0;

   if (title)
      SetTitle(title);
   if (style)
      SetAttributes(style);
}


/*@ XContainerInfo :: EnableSorting( const BOOL enable)
@group misc
@remarks Enables sorting records when they are inserted.
@parameters    BOOL sort, set TRUE if the container items should be sorted
                                         when they are inserted. Therefor you must override the
                                         method XContainerObject::Sort().
                                         Default is TRUE.
*/
void XContainerInfo::EnableSorting(const BOOL enable)
{
   if (enable)
      cnrinfo.pSortRecord = (void *) SortRecord;
   else
      cnrinfo.pSortRecord = NULL;
   changes |= CMA_PSORTRECORD;
}


/*@ XContainerInfo :: SetAttributes( const LONG attribute )
@group set/query attributes
@remarks Specify here how the container should be displayed. After you have set
up the XContainerInfo use XContainerColumn::SetInfo()
@parameters   LONG attributes how the container should be displayed ( see XContainerInfo() )
*/
void XContainerInfo::SetAttributes(const LONG attribute)
{
   cnrinfo.flWindowAttr = attribute;
   changes |= CMA_FLWINDOWATTR;
}


/*@ XContainerInfo :: SetTitle( const char * t )
@group set/query container title
@remarks Set the title of the container.
@parameters    char * theTitle
*/
void XContainerInfo::SetTitle(const char *t)
{
   cnrinfo.pszCnrTitle = (PSZ) t;
   changes |= CMA_CNRTITLE;
}


/*@ XContainerInfo :: EnableBackgroundPainting( const BOOL enable)
@group set/query attributes
@remarks Enable/disable background drawing by the application. To draw the background you must install
a XBackgroundDrawHandler
@parameters    BOOL enable, TRUE=enable, FALSE=disable
*/
void XContainerInfo::EnableBackgroundPainting(const BOOL enable)
{
   if (enable)
      cnrinfo.flWindowAttr |= CA_OWNERPAINTBACKGROUND;
   else
   {
      if (cnrinfo.flWindowAttr & CA_OWNERPAINTBACKGROUND)
         cnrinfo.flWindowAttr ^= CA_OWNERPAINTBACKGROUND;
   }
   changes |= CMA_FLWINDOWATTR;
}


/*@ XContainerInfo :: IsBackgroundPaintingEnabled()
@group set/query attributes
@remarks Query if owner-draw for the background is enabled or not.
@returns BOOL  result
*/
BOOL XContainerInfo::IsBackgroundPaintingEnabled() const
{
   return (cnrinfo.flWindowAttr & CA_OWNERPAINTBACKGROUND ? TRUE : FALSE);
}


/*@ XContainerInfo :: SetSplitbarColumn( const XContainerColumn * col)
@group splitbar
@remarks Set a splitbar in a container.
@parameters    XContainerColumn * the XContainerColumn after that the splitbar will be displayed
*/
void XContainerInfo::SetSplitbarColumn(const XContainerColumn * col)
{
   cnrinfo.pFieldInfoLast = col->info;
   changes |= CMA_PFIELDINFOLAST;
}


/*@ XContainerInfo :: GetAttributes( void )
@group set/query container title
@remarks Query the attributes of the container.
@returns       LONG  the attributes which are set for the container (can be or-ed). See XContainerInfo()
*/
LONG XContainerInfo::GetAttributes(void) const
{
   return cnrinfo.flWindowAttr;
}


/*@ XContainerInfo :: GetTitle( XString * s )
@group set/query container title
@remarks Query the containers title
@parameters    XString * buffer
*/
void XContainerInfo::GetTitle(XString * s)
{
   char *b = s->GetBuffer((int) strlen((char *) cnrinfo.pszCnrTitle) + 1);

   strcpy(b, (char *) cnrinfo.pszCnrTitle);
}


/*@ XContainerInfo :: SetSplitbarPos( const LONG pos )
@group splitbar
@remarks Set the position of the splitbar of a container.
@parameters    LONG the position in window-pixels
*/
void XContainerInfo::SetSplitbarPos(const LONG pos)
{
   changes |= CMA_XVERTSPLITBAR;
   cnrinfo.xVertSplitbar = pos;
}


/*@ XContainerInfo::GetSplitbarPos()
@group splitbar
@remarks Query the position of the splitbar of a container.
@returns       LONG the position in window-pixels
*/


/*@ XContainerInfo::SetTreeBitmapSize(const XSize * size)
@group misc
@remarks Set the size of the expanded and collapsed bitmaps/icons in tree-view
@parameters XSize * size
*/
void XContainerInfo::SetTreeBitmapSize(const XSize * size)
{
   changes |= CMA_SLTREEBITMAPORICON;
   cnrinfo.slTreeBitmapOrIcon.cx = size->GetWidth();
   cnrinfo.slTreeBitmapOrIcon.cy = size->GetHeight();
}


/*@ XContainerInfo::SetBitmapSize(const XSize * size)
@group misc
@remarks Set the size of bitmaps/icons
@parameters XSize * size
*/
void XContainerInfo::SetBitmapSize(const XSize * size)
{
   changes |= CMA_SLBITMAPORICON;
   cnrinfo.slBitmapOrIcon.cx = size->GetWidth();
   cnrinfo.slBitmapOrIcon.cy = size->GetHeight();
}


/*@ XContainerInfo :: GetBitmapSize( XSize * )
@group misc
@remarks Query the size of bitmaps/icons
@parameters XSize * size   buffer to hold the size
*/
void XContainerInfo::GetBitmapSize( XSize * s)
{
   s->SetWidth( cnrinfo.slBitmapOrIcon.cx );
   s->SetHeight( cnrinfo.slBitmapOrIcon.cy );
}


/*@ XContainerInfo :: SetExpandedBitmap( const XBitmap * bmp)
@group misc
@remarks Replace the expanded-bitmap.
@parameters XBitmap * the new bitmap
*/
void XContainerInfo::SetExpandedBitmap(const XBitmap * bmp)
{
   changes |= CMA_TREEBITMAP;
   cnrinfo.hbmExpanded = bmp->GetHandle();
}


/*@ XContainerInfo :: SetExpandedBitmap( const XIcon * ico)
@group misc
@remarks Replace the expanded-icon.
@parameters XIcon * the new icon
*/
void XContainerInfo::SetExpandedBitmap(const XIcon * ico)
{
   changes |= CMA_TREEICON;
   cnrinfo.hptrExpanded = ico->GetHandle();
}


/*@ XContainerInfo :: SetCollapsedBitmap( const XBitmap * bmp)
@group misc
@remarks Replace the collapsed-bitmap.
@parameters XBitmap * the new bitmap
*/
void XContainerInfo::SetCollapsedBitmap(const XBitmap * bmp)
{
   changes |= CMA_TREEBITMAP;
   cnrinfo.hbmCollapsed = bmp->GetHandle();
}


/*@ XContainerInfo :: SetCollapsedBitmap( const XIcon * ico)
@group misc
@remarks Replace the collapsed-icon.
@parameters XIcon * the new icon
*/
void XContainerInfo::SetCollapsedBitmap(const XIcon * ico)
{
   changes |= CMA_TREEICON;
   cnrinfo.hptrCollapsed = ico->GetHandle();
}


/*@ XContainerInfo::GetObjectCount()
@group misc
@remarks Query the count of objects in the container.
@returns LONG objectCount
*/


/*@ XContainerInfo :: SetTreeLineWidth( const SHORT w)
@group misc
@remarks Set the width of the trees line in tree-view.
@parameters SHORT width (in pixels)
*/
void XContainerInfo::SetTreeLineWidth(const SHORT w)
{
   changes |= CMA_CXTREELINE;
   cnrinfo.cxTreeLine = w;
}


/*@ XContainerInfo :: SetTreeLineSpacing( const SHORT w)
@group misc
@remarks Set the horizontal spacing between two levels in tree-view.
@parameters SHORT width (in pixels)
*/
void XContainerInfo::SetTreeLineSpacing(const SHORT w)
{
   changes |= CMA_CXTREEINDENT;
   cnrinfo.cxTreeIndent = w;
}


/*@ XContainerInfo :: SetSpacing( const SHORT w)
@group misc
@remarks Set the vertical spacing between two objects.
@parameters SHORT spacing (in pixels)
*/
void XContainerInfo::SetSpacing(const SHORT w)
{
   changes |= CMA_LINESPACING;
   cnrinfo.cyLineSpacing = w;
}


/*@ XContainerColumn::TitleEdited(const char * )
@group misc
@remarks This method is called if the user has edited the title of the column.
Overwrite this method if you need the information
@parameters char * the new Title
*/


/*@ XContainerObject::TitleEdited(const char *, XContainerColumn* )
@group misc
@remarks This method is called if the user has edited the title of the object or the objects title of a column in detail view.
Overwrite this method if you need the information
@parameters
<t 'ø' c=2>
øchar * øthe new Title
øXContainerColumn* øin detail view this parameter contains a pointer to the related column.
</t>
*/

/*@ XContainerObject::AllocMemory( char * oldText, SHORT newTextLength, XContainerColumn* )
@group misc
@remarks This method is called if it is nessacary to realloc memory for objects in detail-view
@parameters
<t 'ø' c=2>
øchar * øthe new Title
øSHORT ølenght of the new text
øXContainerColumn* øin detail view this parameter contains a pointer to the related column.
</t>
*/
