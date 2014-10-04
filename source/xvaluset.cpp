#include "xvaluset.h"
#include "xstring.h"
#include "xcolor.h"
#include "xicon.h"
#include "xbitmap.h"


/*@
@class XValueSet
@parent XControl
@type overview
@symbol _
*/

/*@ XValueSet :: XValueSet(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font)
@group constructors/destructors
@remarks Construct a value-set
@parameters <t 'ø' c=2>
            øXWindow * owner         øowner of the value-set
            øXRect& rect            øposition and size
            øUSHORT id               øcontrol-id
            øULONG style             østyle, possible values are:
<BR>
VA_ICON
<BR>
VA_BITMAP
<BR>
VA_COLOR
<BR>
VA_TEXT
<BR>
VA_BORDER
<BR>
VA_ITEMBORDER
<BR>
(can be or-ed)
            øchar * font               øfont to use, e.g. "8.Helv"
            </t>
*/
XValueSet :: XValueSet(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(&rec, style, owner, "", WC_VALUESET, id, font)
{
}


/*@ XValueSet::SetDimensions(const SHORT rows, const SHORT cols)
@group misc
@remarks Set the number of colums and rows.
@parameters
<t 'ø' c=2>
øSHORT ørows
øSHORT øcols
</t>
*/
void XValueSet::SetDimensions(const SHORT rows, const SHORT cols)
{
   VSCDATA data;

   data.cbSize = sizeof(data);
   data.usRowCount = rows;
   data.usColumnCount = cols;

   WNDPARAMS params;

   params.fsStatus = WPM_CTLDATA;
   params.cbCtlData = sizeof(data);
   params.pCtlData = &data;

   WinSendMsg(winhandle, WM_SETWINDOWPARAMS, MPFROMP(&params), 0);
}


/*@ XValueSet::SetItem(const SHORT row, const SHORT col, const XColor * color)
@group item contents
@remarks Set the items color (if VA_COLOR is set)
@parameters <t 'ø' c=2>
            øSHORT row    ørow of the item
            øSHORT col    øcolum of the item
            øXColor * col øthe new color
            </t>
*/
void XValueSet::SetItem(const SHORT row, const SHORT col, const XColor * color) const
{
   WinSendMsg(winhandle, VM_SETITEM, MPFROM2SHORT(row + 1, col + 1), (MPARAM) color->GetColor());
}


/*@ XValueSet::SetItem(const SHORT row, const SHORT col, const char *text)
@group item contents
@remarks Set the items text (if it is in text-mode)
@parameters <t 'ø' c=2>
            øSHORT row   ørow of the item
            øSHORT col   øcolum of the item
            øchar * text øthe new text
            </t>
*/
void XValueSet::SetItem(const SHORT row, const SHORT col, const char *text) const
{
   WinSendMsg(winhandle, VM_SETITEM, MPFROM2SHORT(row + 1, col + 1), MPFROMP(text));
}


/*@ XValueSet::SetItem(const SHORT row, const SHORT col, const XIcon * icon)
@group item contents
@remarks Set the items content
@parameters <t 'ø' c=2>
            øSHORT row    ørow of the item
            øSHORT col    øcolum of the item
            øXIcon *      øicon
            </t>
*/
void XValueSet::SetItem(const SHORT row, const SHORT col, const XIcon * icon) const
{
   WinSendMsg(winhandle, VM_SETITEM, MPFROM2SHORT(row + 1, col + 1), (MPARAM) icon->GetHandle());
}


/*@ XValueSet::SetItem(const SHORT row, const SHORT col, const XBitmap * bitmap)
@group item contents
@remarks Set the items content
@parameters <t 'ø' c=2>
            øSHORT row    ørow of the item
            øSHORT col    øcolum of the item
            øXBitmap *    øbitmap
            </t>
*/
void XValueSet::SetItem(const SHORT row, const SHORT col, const XBitmap * bitmap) const
{
   WinSendMsg(winhandle, VM_SETITEM, MPFROM2SHORT(row + 1, col + 1), (MPARAM) bitmap->GetHandle());
}


/*@ XValueSet::SetItemAttribute(const SHORT row, const SHORT col, const SHORT attr, const BOOL enable)
@group item attributes
@remarks Set the items attributes
@parameters <t 'ø' c=2>
            øSHORT row    ørow of the item
            øSHORT col    øcolum of the item
            øSHORT attr   øthe attribute:
<BR>
VA_ICON
<BR>
VA_BITMAP
<BR>
VA_COLOR
<BR>
VA_TEXT
<BR>
VA_ITEMBORDER
            øBOOL enable  enabe/disable attribute
            </t>
*/
void XValueSet::SetItemAttribute(const SHORT row, const SHORT col, const SHORT attr, const BOOL enable) const
{
   WinSendMsg(winhandle, VM_SETITEMATTR, MPFROM2SHORT(row + 1, col + 1), MPFROM2SHORT(attr, enable));
}


/*@ XValueSet::SelectItem(const SHORT row, const SHORT col)
@group selection
@remarks Select an item
@parameters
<t 'ø' c=2>
øSHORT ørow of the item
øSHORT øcolum of the item
</t>
*/
void XValueSet::SelectItem(const SHORT row, const SHORT col) const
{
   WinSendMsg(winhandle, VM_SELECTITEM, MPFROM2SHORT(row + 1, col + 1), 0);
}


/*@ XValueSet::SetItemSpacing(const XSize * s)
@group size
@remarks Set the spacing between the items.
@parameters XSize * size
*/
void XValueSet::SetItemSpacing(const XSize * s) const
{
   WinSendMsg(winhandle, VM_SETMETRICS, MPFROMSHORT(VMA_ITEMSPACING), MPFROM2SHORT( s->GetWidth(), s->GetHeight()));
}


/*@ XValueSet::SetItemSize(const XSize * s)
@group size
@remarks Set the size of the items.
@parameters XSize * size
*/
void XValueSet::SetItemSize(const XSize * s) const
{
   WinSendMsg(winhandle, VM_SETMETRICS, MPFROMSHORT(VMA_ITEMSIZE), MPFROM2SHORT(s->GetWidth(), s->GetHeight()));
}


/*@ XValueSet::GetSelection(SHORT & row, SHORT & col)
@group selection
@remarks Returns column/row of the selected item.
@parameters
<t 'ø' c=2>
øSHORT& ørow of the item
øSHORT& øcolum of the item
</t>
*/
void XValueSet::GetSelection(SHORT & row, SHORT & col) const
{
   MRESULT mr = WinSendMsg(winhandle, VM_QUERYSELECTEDITEM, 0, 0);

   row = SHORT1FROMMR(mr) - 1;
   col = SHORT2FROMMR(mr) - 1;
}


/*@ XValueSet::GetItem(const SHORT row, const SHORT col, XString * s)
@group item contents
@remarks Get the items text (if it is in text-mode)
@parameters <t 'ø' c=2>
            øSHORT row        ørow of the item
            øSHORT col        øcolum of the item
            øXString * øbuffer buffer to hold the text
            </t>
*/
void XValueSet::GetItem(const SHORT row, const SHORT col, XString * s) const
{
   VSTEXT text;

   text.pszItemText = (PSZ) s->GetBuffer(512);
   text.ulBufLen = 512;
   WinSendMsg(winhandle, VM_QUERYITEM, MPFROM2SHORT(row + 1, col + 1), MPFROMP(&text));
   s->ReleaseBuffer();
}


/*@ XValueSet::GetItem(const SHORT row, const SHORT col, XColor * color)
@group item contents
@remarks Get the items color
@parameters <t 'ø' c=2>
            øSHORT row        ørow of the item
            øSHORT col        øcolum of the item
            øXColor * col     øbuffer to hold the color
            </t>
*/
void XValueSet::GetItem(const SHORT row, const SHORT col, XColor * color) const
{
   color->SetColor((LONG) WinSendMsg(winhandle, VM_QUERYITEM, MPFROM2SHORT(row + 1, col + 1), 0));
}


/*@ XValueSet::GetItem(const SHORT row, const SHORT col, LONG & val)
@group item contents
@remarks Get the items icon/bitmap-handle
@parameters <t 'ø' c=2>
            øSHORT row        ørow of the item
            øSHORT col        øcolum of the item
            øLONG& value      øicon/bitmap-handle from the item
            </t>
*/
void XValueSet::GetItem(const SHORT row, const SHORT col, LONG & val) const
{
   val = (LONG) WinSendMsg(winhandle, VM_QUERYITEM, MPFROM2SHORT(row + 1, col + 1), 0);
}


/*@ XValueSet::GetItem(const SHORT row, const SHORT col, LONG & val)
@group item attributes
@remarks Get the items attributes
@parameters
<t 'ø' c=2>
øSHORT ørow of the item
øSHORT øcolum of the item
</t>
@returns  SHORT theAttribute
*/
LONG XValueSet::GetItemAttribute(const SHORT row, const SHORT col) const
{
   return (LONG) WinSendMsg(winhandle, VM_QUERYITEMATTR, MPFROM2SHORT(row + 1, col + 1), 0);
}


/*@ XValueSet::GetItemSpacing(XSize*)
@group size
@remarks Returns the spacing between the items
@parameters XSize * buffer to hold horizontal(vertical spacing
*/
void XValueSet::GetItemSpacing( XSize * s) const
{
   MRESULT mr = WinSendMsg(winhandle, VM_QUERYMETRICS, MPFROMSHORT(VMA_ITEMSPACING), 0);

   s->SetWidth( SHORT1FROMMR(mr));
   s->SetHeight( SHORT2FROMMR(mr));
}


/*@ XValueSet::GetItemSize(XSize *)
@group size
@remarks Returns the size of the items
@parameters XSize * bufer to hold the size
*/
void XValueSet::GetItemSize(XSize * size) const
{
   MRESULT mr = WinSendMsg(winhandle, VM_QUERYMETRICS, MPFROMSHORT(VMA_ITEMSIZE), 0);

   size->SetWidth( SHORT1FROMMR(mr));
   size->SetHeight( SHORT2FROMMR(mr));
}
