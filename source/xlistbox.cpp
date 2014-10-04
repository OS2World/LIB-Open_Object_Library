#include "xlistbox.h"
#include "xstring.h"
#include "xcombo.h"


/*@
@class XListBox
@parent XControl
@type overview
@symbol _
*/


/*@
@class XComboBox
@parent XListBox
@type overview
@symbol _
*/


/*@ XComboBox::IsReadOnly(void)
@group misc
@remarks Query if the entryfild is in readonly-mode.
@returns BOOL result
*/
BOOL XComboBox::IsReadOnly(void) const
{
   return (BOOL) WinSendMsg(winhandle, EM_QUERYREADONLY, 0, 0);
}


/*@ XComboBox::SetReadOnly(const BOOL readOnly)
@group misc
@remarks Enable/disable readonly-mode.
@parameters BOOL readOnly  TRUE=enable, FALSE=disable readonly-mose
*/
void XComboBox::SetReadOnly(const BOOL readOnly) const
{
   WinSendMsg(winhandle, EM_SETREADONLY, MPFROMSHORT(readOnly), 0);
}


/*@ XComboBox::GetFirstChar(void)
@group misc
@remarks Get the index of the first fisible charakter.
@returns SHORT index
*/
SHORT XComboBox::GetFirstChar(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, EM_QUERYFIRSTCHAR, 0, 0));
}


/*@ XComboBox::GetSel(SHORT & start, SHORT & end)
@group clipboard functions
@remarks Get the boundarys of the current selection.
@parameters
<t 'ø' c=2>
øSHORT& østarting point of selection<BR>
øSHORT& øend point
</t>
*/
void XComboBox::GetSel(SHORT & start, SHORT & end)
{
   MRESULT res = WinSendMsg(winhandle, EM_QUERYSEL, 0, 0);

   start = SHORT1FROMMR(res);
   end = SHORT2FROMMR(res);
}


/*@ XComboBox::SetLimit(const unsigned short len)
@group misc
@remarks Set maximum number of charakters.
@parameters USHORT length  maximum size
*/
void XComboBox::SetLimit(const unsigned short len) const
{
   WinSendMsg(winhandle, EM_SETTEXTLIMIT, MPFROMSHORT(len), 0);
}


/*@ XComboBox::HasChanged(void)
@group misc
@remarks Get if the content has changed.
@returns BOOL result
*/
BOOL XComboBox::HasChanged(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, EM_QUERYCHANGED, 0, 0));
}


/*@ XComboBox::EnableOverWrite(const BOOL overWrite)
@group misc
@remarks Enables/disables overwrite-mode.
@parameters BOOL overwrite    TRUE=overWrite, FALSE=disable overWrite
*/
void XComboBox::EnableOverWrite(const BOOL overWrite) const
{
   WinSendMsg(winhandle, EM_SETINSERTMODE, MPFROMSHORT(overWrite), 0);
}


/*@ XComboBox :: XComboBox(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font)
@group constructors/destructors
@remarks Construct a combobox
@parameters    <t 'ø' c=2>
               øXWindow * theOwner        øThe owning window.
               øXRect& rectangle         øPosition and size of the combobox. You must specify
                                         the size the combobox should have when the listbox is
                                         shown!
               øUSHORT ID                 øThe ID of the combobox.
                                         Default is zero.
               øULONG style               øThe style of the window. Valid values are:<BR>
CB_SIMPLE listbox is always shown, the user can enter text<BR>
CB_DROPDOWN listbox is activated with special button, the user can enter text<BR>
CB_DROPDOWNLIST  listbox is activated with special button, the user cannot enter text<BR>
                                          Default is CB_DROPDOWN.
               øchar * font                 øfont to use, e.g. "8.Helvetica"
               </t>
*/
XComboBox :: XComboBox(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XListBox(&rec, style, owner, id, WC_COMBOBOX, font)
{
}


/*@ XComboBox::IsListShowing(void)
@group misc
@remarks Get if the list is displayed.
@returns BOOL result
*/
BOOL XComboBox::IsListShowing(void) const
{
   return (BOOL) WinSendMsg(winhandle, CBM_ISLISTSHOWING, 0, 0);
}


/*@ XComboBox::ShowList(const BOOL show)
@group misc
@remarks Show/hide the list of the combo.
@parameters BOOL show    TRUE=show, FALSE=hide the list
*/
void XComboBox::ShowList(const BOOL show) const
{
   WinSendMsg(winhandle, CBM_SHOWLIST, MPFROMSHORT(show), 0);
}


/*@ XComboBox::Hilite(const BOOL hilite)
@group misc
@remarks Enable/disable hiliting.
@parameters BOOL hilite    TRUE=enable, FALSE=disable hiliting
*/
void XComboBox::Hilite(const BOOL hilite) const
{
   WinSendMsg(winhandle, CBM_HILITE, MPFROMSHORT(hilite), 0);
}


/*@ XListBox::GetTopIndex(void)
@group misc
@remarks Returns the index of the first visible entry.
@returns SHORT index
*/
SHORT XListBox::GetTopIndex(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_QUERYTOPINDEX, 0, 0));
}


/*@ XListBox::SetTopIndex(const SHORT index)
@group misc
@remarks Set the first visible entry.
@parameters SHORT index    index of the first visible entry
*/
void XListBox::SetTopIndex(const SHORT index) const
{
   WinSendMsg(winhandle, LM_SETTOPINDEX, MPFROMSHORT(index), 0);
}


/*@ XListBox::SetItemHeight(const SHORT height)
@group misc
@remarks Set items height
@parameters SHORT height
*/
void XListBox::SetItemHeight(const SHORT height) const
{
   WinSendMsg(winhandle, LM_SETITEMHEIGHT, MPFROMSHORT(height), 0);
}


/*@ XListBox::SetItemWidth(const SHORT width)
@group misc
@remarks Set items width
@parameters SHORT width
*/
void XListBox::SetItemWidth(const SHORT width) const
{
   WinSendMsg(winhandle, LM_SETITEMWIDTH, MPFROMSHORT(width), 0);
}


XListBox :: XListBox(const XRect * rec, const ULONG style, const XWindow * owner, const USHORT id, const PSZ className, const char *font):XControl(rec, style, owner, "", className, id, font)
{
}


/*@ XListBox :: XListBox(const XWindow * owner, const XRect * rec, const USHORT id, const ULONG style, const char *font)
@group constructors/destructors
@remarks Construct a listbox
@parameters    <t 'ø' c=2>
               øXWindow * theOwner        øThe owning window
               øXRect& rectangle         øPosition and size of the listbox
               øUSHORT ID                 øThe ID of the window.
                                         Default is zero.
               øULONG style               øThe style of the window. Valid values are:<BR>
LS_SCROLL    the listbox gets a horizontal scrollbar<BR>
LS_MULTI     one ore more items can be selected<BR>
LS_EXTENDED  extended selection is enabled (should be used if LS_MULTI is specified).<BR>
                                         The values can be or-ed.
               øchar * font                 øfont to use, e.g. "8.Helvetica"
               </t>
*/
XListBox :: XListBox(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(&rec, style, owner, "", WC_LISTBOX, id, font)
{
}


/*@ XListBox::InsertItem(const char *item, const SHORT pos)
@group inserting/removing
@remarks Adds a an item to the listbox
@parameters    <t 'ø' c=2>
               øchar * theTitle           øtitle of the item
               øSHORT position            øwhere to insert. You can specify the zero-based index<BR>
LS_LAST (the item is inserted at the end of the list)<BR>
LS_ASCENDING (the items are sorted ascending)<BR>
LS_DESCENDING (items are sorted descending)<BR>
                                      Default is LS_LAST.
               </t>
@returns       SHORT                     the zero-based index of the item
*/
SHORT XListBox::InsertItem(const char *item, const SHORT pos) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_INSERTITEM, MPFROMSHORT(pos), (PSZ) item));
}


/*@ XListBox::GetCount(void)
@group misc
@remarks Returns the count of items.
@returns       SHORT                      the count of items in the listbox
*/
SHORT XListBox::GetCount(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_QUERYITEMCOUNT, 0, 0));
}


/*@ XListBox::GetItemHandle(const SHORT pos)
@group using handles
@remarks Gets a handle of an item.
@parameters     SHORT theItem             Zero-based index of the item
@returns       LONG                      the handle of the item
*/
LONG XListBox::GetItemHandle(const SHORT pos) const
{
   return (LONG) WinSendMsg(winhandle, LM_QUERYITEMHANDLE, MPFROMSHORT(pos), 0);
}


/*@ XListBox::GetItemText(const SHORT pos, XString * buffer)
@group text input/output
@remarks Get the text of an item
@parameters
<t 'ø' c=2>
øSHORT øZero-based index of the item
øXString * øbuffer which will hold the text
</t>
@returns       SHORT                     length of the text
*/
SHORT XListBox::GetItemText(const SHORT pos, XString * buffer)
{
   SHORT r = SHORT1FROMMR(WinSendMsg(winhandle, LM_QUERYITEMTEXTLENGTH, MPFROMSHORT(pos), 0));

   WinSendMsg(winhandle, LM_QUERYITEMTEXT, MPFROM2SHORT(pos, r + 1), buffer->GetBuffer(r + 1));
   buffer->ReleaseBuffer(r);
   return r;
}


/*@ XListBox::GetSelection(const SHORT start)
@group set/query selection
@remarks Returns the zero-based index of the first selected item
which is found after the specified item.
@parameters
SHORT Zero-based index of the item behind which the search starts (default is LS_FIRST)
@returns
SHORT Zero-based index of the item found. If no item is found return is LS_NONE
*/
SHORT XListBox::GetSelection(const SHORT start) const
{
   return SHORT1FROMMR( WinSendMsg(winhandle, LM_QUERYSELECTION, MPFROMSHORT(start), 0) );
}


/*@ XListBox::RemoveAll(void)
@group inserting/removing
@remarks Removes all items from a listbox.
@returns       BOOL result
*/
BOOL XListBox::RemoveAll(void) const
{
   SHORT i = InsertItem("");

   SelectItem(i);
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_DELETEALL, 0, 0));
}


/*@ XListBox::RemoveItem(const SHORT pos)
@group inserting/removing
@remarks Removes an item
@parameters     SHORT Zero-based index of the item to remove
*/
void XListBox::RemoveItem(const SHORT pos) const
{
   WinSendMsg(winhandle, LM_DELETEITEM, MPFROMSHORT(pos), 0);
}


/*@ XListBox::SearchString(const char *p, const SHORT start, const SHORT match)
@group misc
@remarks Search a string in the listbox
@parameters     <t 'ø' c=2>
               øchar * theText            øtext to be searched
               øSHORT startPosition       øZero-based index of item where to start the search
               øSHORT matchCode           øHow to search. Valid values are:<BR>
LS_CASESENSITIVE  search casesensitive<BR>
LS_PREFIX         search-string must be at the beginning of the items title<BR>
LS_SUBSTRING      search-string can be in the middle of the items title<BR>
LS_CASESSENSITIVE can be or-ed with one of the other defines.
               </t>
@returns       SHORT                     Zero-based index of the item which is found. If no item
                                         is found, LS_NONE is returned.
*/
SHORT XListBox::SearchString(const char *p, const SHORT start, const SHORT match) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_SEARCHSTRING, MPFROM2SHORT(match, start), MPFROMP(p)));
}


/*@ XListBox::SelectItem(const SHORT pos, const BOOL select)
@group set/query selection
@remarks Select an item
@parameters     SHORT Zero-based index of the item to select
*/
BOOL XListBox::SelectItem(const SHORT pos, const BOOL select) const
{
   return (BOOL) WinSendMsg(winhandle, LM_SELECTITEM, MPFROMSHORT(pos), MPFROMSHORT(select));
}


/*@ XListBox::SetItemHandle(const SHORT pos, const LONG handle)
@group using handles
@remarks Adds a handle to an item
@parameters
<t 'ø' c=2>
øSHORT øZero-based index of the item which will get the handle<BR>
øLONG øthe handle which is add to the item
</t>
@returns       BOOL                      success
*/
BOOL XListBox::SetItemHandle(const SHORT pos, const LONG handle) const
{
   return (BOOL) WinSendMsg(winhandle, LM_SETITEMHANDLE, MPFROMSHORT(pos), (MPARAM) handle);
}


/*@ XListBox::SetItemText(const SHORT pos, const char *p)
@group text input/output
@remarks Set the text of an item.
@parameters
<t 'ø' c=2>
øSHORT øZero-based index of the item
øchar * øthe new text
</t>
@returns BOOL success
*/
BOOL XListBox::SetItemText(const SHORT pos, const char *p) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, LM_SETITEMTEXT, MPFROMSHORT(pos), MPFROMP(p)));
}
