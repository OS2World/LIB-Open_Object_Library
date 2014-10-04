#include "xmle.h"
#include "xmlesear.h"
#include <string.h>
#include <stdlib.h>

#include "xstring.h"

/*@
@class XMultiLineEdit
@parent XControl
@type overview
@symbol _
*/

/*@
@class XMLESearch
@type overview
@symbol _
@remarks XMLESearch is a helping class for searching/replacing text in a XMultiLineEdit.
*/


/*@ XMLESearch::GetSearchItem(XString * s)
@remarks Query the item which is to search
@parameters XString * buffer
*/
void XMLESearch::GetSearchItem(XString * s)
{
   strcpy(s->GetBuffer(sStruct.cchFind), sStruct.pchFind);
   s->ReleaseBuffer(sStruct.cchFind);
}


/*@ XMLESearch::GetReplaceItem(XString * s)
@remarks Query the item which is to insert
@parameters XString * buffer
*/
void XMLESearch::GetReplaceItem(XString * s)
{
   strcpy(s->GetBuffer(sStruct.cchReplace + 1), sStruct.pchReplace);
   s->ReleaseBuffer(sStruct.cchReplace);
}


XMLESearch :: XMLESearch()
{
   memset(&sStruct, 0, sizeof(sStruct));
   mode = MLFSEARCH_SELECTMATCH;
   sStruct.iptStop = -1;
   sStruct.cb = sizeof(sStruct);
}


/*@ XMLESearch::GetStartPoint(void)
@remarks Query the point where the search starts.
@returns LONG startPoint
*/
LONG XMLESearch::GetStartPoint(void)
{
   return sStruct.iptStart;
}


/*@ XMLESearch::SetCaseSensitive(BOOL set)
@remarks Set if the search is case-sensitive
@parameters BOOL case   TRUE=case sensitive, FALSE=not case sensitive
*/
void XMLESearch::SetCaseSensitive(BOOL set)
{
   if (set)
      mode |= MLFSEARCH_CASESENSITIVE;
   else if (mode & MLFSEARCH_CASESENSITIVE)
      mode ^= MLFSEARCH_CASESENSITIVE;
}


/*@ XMLESearch::SetReplaceAll(BOOL set)
@remarks Set if replacing should be done for all found strings
@parameters BOOL case   TRUE=replace all, FALSE=not for all
*/
void XMLESearch::SetReplaceAll(BOOL set)
{
   if (set)
      mode |= MLFSEARCH_CHANGEALL;
   else if (mode & MLFSEARCH_CHANGEALL)
      mode ^= MLFSEARCH_CHANGEALL;
}


/*@ XMLESearch::SetSearchItem(char *item)
@remarks Set the string to search for
@parameters char * item      the string to search
*/
void XMLESearch::SetSearchItem(char *item)
{
   SHORT i = strlen(item) + 1;

   sStruct.pchFind = (char *) realloc(sStruct.pchFind, i);
   memcpy(sStruct.pchFind, item, i);
   sStruct.cchFind = i - 1;
}


/*@ XMLESearch::SetReplaceItem(char *item)
@remarks Set the string to replace with
@parameters char * item      the string to replace with
*/
void XMLESearch::SetReplaceItem(char *item)
{
   SHORT i = strlen(item) + 1;

   sStruct.pchReplace = (char *) realloc(sStruct.pchReplace, i);
   memcpy(sStruct.pchReplace, item, i);
   sStruct.cchReplace = i - 1;
}


/*@ XMLESearch::SetStartPoint(LONG x)
@remarks Set the point where to start
@parameters LONG point
*/
void XMLESearch::SetStartPoint(LONG x)
{
   sStruct.iptStart = x;
}


/*@ XMLESearch::SetSearchEndPoint(LONG x)
@remarks Set the point where to end the search
@parameters LONG point   where to stop search (-1=search overall)
*/
void XMLESearch::SetSearchEndPoint(LONG x)
{
   sStruct.iptStop = x;
}


XMLESearch :: ~XMLESearch()
{
   free(sStruct.pchReplace);
   free(sStruct.pchFind);
}

#define IMPORTBUFFER 10000

void XMultiLineEdit::SetText(const char *text)
{
   XWindow::SetText("");

   ULONG size = strlen(text);

   ULONG i = 0, n = 0;
   char *s = (char *) text;

   LONG insert = 0;

   do
   {
      i = size - n;
      if (i > 0)
      {
         if (i > IMPORTBUFFER)
            i = IMPORTBUFFER;
         WinSendMsg(winhandle, MLM_SETIMPORTEXPORT, MPFROMP(s), MPFROMLONG(i));
         WinSendMsg(winhandle, MLM_IMPORT, MPFROMP(&insert), (MPARAM) i);
         n += i;
         s += i;
      }
   } while (i > 0);
}



void XMultiLineEdit::GetText(XString * buffer) const
{
   LONG insert = 0;

   char string[IMPORTBUFFER + 1];
   ULONG i, n = 0;

   i = GetTextLength();

   WinSendMsg(winhandle, MLM_SETIMPORTEXPORT, MPFROMP(string), (MPARAM) IMPORTBUFFER);
   *buffer = "";

   do
   {
      if (i > 0)
      {
         n = (LONG) WinSendMsg(winhandle, MLM_EXPORT, MPFROMP(&insert), MPFROMP(&i));
         string[n] = 0;
         *buffer += string;
      }
   } while (i > 0);
}


/*@ XMultiLineEdit::EnableRefresh(const BOOL refresh)
@group misc
@remarks Enable/disable window-refresh
@parameters BOOL enable    TRUE=enable, FALSE=disable refresh
*/
void XMultiLineEdit::EnableRefresh(const BOOL refresh) const
{
   if (refresh)
      WinSendMsg(winhandle, MLM_ENABLEREFRESH, 0, 0);
   else
      WinSendMsg(winhandle, MLM_DISABLEREFRESH, 0, 0);
}


/*@ XMultiLineEdit::GetLineFromChar(const LONG point)
@group line related functions
@remarks Query the line number of the point given in parameter 1
@parameters LONG index
@returns LONG line-number
*/
LONG XMultiLineEdit::GetLineFromChar(const LONG point) const
{
   return (LONG) WinSendMsg(winhandle, MLM_LINEFROMCHAR, (MPARAM) point, 0);
}


/*@ XMultiLineEdit::GetLineLength(const LONG point)
@group line related functions
@remarks Query the length of the line which contains the point given in parameter 1
@parameters LONG index
@returns LONG length of the line
*/
LONG XMultiLineEdit::GetLineLength(const LONG point) const
{
   return (LONG) WinSendMsg(winhandle, MLM_QUERYLINELENGTH, (MPARAM) point, 0);
}


/*@ XMultiLineEdit::GetLineStart(const LONG line)
@group line related functions
@remarks Query the starting point of the line given in parameter 1
@parameters LONG line-number
@returns LONG point of begining
*/
LONG XMultiLineEdit::GetLineStart(const LONG line) const
{
   return (LONG) WinSendMsg(winhandle, MLM_CHARFROMLINE, (MPARAM) line, 0);
}


/*@ XMultiLineEdit::GetLineCount(void)
@group line related functions
@remarks Query the number of lines
@returns LONG lines
*/
LONG XMultiLineEdit::GetLineCount(void) const
{
   return (LONG) WinSendMsg(winhandle, MLM_QUERYLINECOUNT, 0, 0);
}


/*@ XMultiLineEdit::ResetUndo(void)
@group undo
@remarks Reset the undo-buffer
*/
void XMultiLineEdit::ResetUndo(void) const
{
   WinSendMsg(winhandle, MLM_RESETUNDO, 0, 0);
}


/*@ XMultiLineEdit::EnableWrap(const BOOL wrap)
@group wrap
@remarks Enable/disable word-wrap
@parameters BOOL wrap      TRUE=enable, FALSE=disable word-wrap
*/
void XMultiLineEdit::EnableWrap(const BOOL wrap) const
{
   WinSendMsg(winhandle, MLM_SETWRAP, MPFROMSHORT(wrap), 0);
}


/*@ XMultiLineEdit::IsWrap(void)
@group wrap
@remarks Query if word-wrap is active
@returns BOOL result
*/
BOOL XMultiLineEdit::IsWrap(void) const
{
   return (BOOL) WinSendMsg(winhandle, MLM_QUERYWRAP, 0, 0);
}


/*@ XMultiLineEdit::IsReadOnly(void)
@group misc
@remarks Query if readonly-mode is active
@returns BOOL result
*/
BOOL XMultiLineEdit::IsReadOnly(void) const
{
   return (BOOL) WinSendMsg(winhandle, MLM_QUERYREADONLY, 0, 0);
}


/*@ XMultiLineEdit::SetReadOnly(const BOOL readOnly)
@group misc
@remarks Enable/disable readonly-mode
@parameters BOOL readOnly     TRUE=enable, FALSE=disable readonly-mode
*/
void XMultiLineEdit::SetReadOnly(const BOOL readOnly) const
{
   WinSendMsg(winhandle, MLM_SETREADONLY, MPFROMSHORT(readOnly), 0);
}


/*@ XMultiLineEdit::GetFirstChar(void)
@group misc
@remarks Query the number of the first visible char
@returns LONG number
*/
LONG XMultiLineEdit::GetFirstChar(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MLM_QUERYFIRSTCHAR, 0, 0));
}


/*@ XMultiLineEdit :: XMultiLineEdit(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font)
@group constructors/destructors
@remarks Constructs an XMultiLineEdit
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the XMultiLineEdit
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
                                 <t 'ø' c=1>
                                    øMLE_BORDER
                                    øMLE_READONLY
                                    øMLE_WORDWRAP
                                    øMLE_HORZSCROLL
                                    øMLE_VERTSCROLL
                                    øMLE_IGNORETAB
                                    øMLE_DISABLEUNDO
                                 </t>
                                 (can be or-ed)
            øconst char * string  øtext to display
            </t>
*/
XMultiLineEdit :: XMultiLineEdit(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font):XControl(&rec, style, owner, string, WC_MLE, id, font)
{
}


/*@ XMultiLineEdit::GetSelection(LONG & start, LONG & end)
@group clipboard functions and selection
@remarks Query the boundarys of the current selection
@parameters
<t 'ø' c=2>
øLONG& østarting point of selection
øLONG& øend point
</t>
*/
void XMultiLineEdit::GetSelection(LONG & start, LONG & end)
{
   MRESULT mr = WinSendMsg(winhandle, MLM_QUERYSEL, (MPARAM) MLFQS_MINMAXSEL, 0);

   start = SHORT1FROMMR(mr);
   end = SHORT2FROMMR(mr);
}


/*@ XMultiLineEdit::SetLimit(const LONG limit)
@group misc
@remarks Set maximum number of charakters
@parameters LONG maximum size
*/
void XMultiLineEdit::SetLimit(const LONG limit) const
{
   WinSendMsg(winhandle, MLM_SETTEXTLIMIT, (MPARAM) limit, 0);
}


/*@ XMultiLineEdit::EnableOverWrite(const BOOL overWrite)
@group misc
@remarks Enables/disables overwrite-mode
@parameters BOOL overwrite    TRUE=overWrite, FALSE=disable overWrite
*/
void XMultiLineEdit::EnableOverWrite(const BOOL overWrite) const
{
   BOOL o = FALSE;

   if (overWrite == FALSE)
      o = TRUE;
   WinSendMsg(winhandle, EM_SETINSERTMODE, MPFROMSHORT(o), 0);
}


/*@ XMultiLineEdit::CanUndo(void)
@group undo
@remarks Query if an undo-operation is possible
@returns BOOL result
*/
BOOL XMultiLineEdit::CanUndo(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MLM_QUERYUNDO, 0, 0));
}


/*@ XMultiLineEdit::InsertString(const char *p)
@group misc
@remarks Insert a string at the current cursor-position
@parameters const char * text to insert
*/
void XMultiLineEdit::InsertString(const char *p) const
{
   WinSendMsg(winhandle, MLM_INSERT, MPFROMP(p), 0);
}


/*@ XMultiLineEdit::IsChanged(void)
@group misc
@remarks Query if the content has changed
@returns BOOL result
*/
BOOL XMultiLineEdit::IsChanged(void) const
{
   return (BOOL) WinSendMsg(winhandle, MLM_QUERYCHANGED, 0, 0);
}


/*@ XMultiLineEdit::GetTextLength(void)
@group misc
@remarks Returns the length of the text in the MLE including linebreaks (\n)
@returns LONG length
*/
LONG XMultiLineEdit::GetTextLength(void) const
{
   return (LONG) WinSendMsg(winhandle, MLM_QUERYTEXTLENGTH, 0, 0) + (LONG) WinSendMsg(winhandle, MLM_QUERYLINECOUNT, 0, 0);
}


/*@ XMultiLineEdit::SelectText(const LONG start, const LONG end)
@group clipboard functions and selection
@remarks Marks an area of text
@parameters
<t 'ø' c=2>
øLONG øbegin of marking-area
øLONG øend of marking-area
</t>
*/
void XMultiLineEdit::SelectText(const LONG start, const LONG end) const
{
   WinSendMsg(winhandle, MLM_SETSEL, (MPARAM) start, (MPARAM) end);
}


/*@ XMultiLineEdit::ShowFirstChar(const LONG firstChar)
@group misc
@remarks Select the point of the text to start displaying
@parameters LONG the first char to display
*/
void XMultiLineEdit::ShowFirstChar(const LONG firstChar) const
{
   WinSendMsg(winhandle, MLM_SETFIRSTCHAR, MPFROMSHORT(firstChar), 0);
}


/*@ XMultiLineEdit::Undo(void)
@group undo
@remarks Undo the last action
@returns BOOL result
*/
BOOL XMultiLineEdit::Undo(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MLM_UNDO, 0, 0));
}


/*@ XMultiLineEdit::Search(const XMLESearch * s)
@group misc
@remarks Search and/or replace strings in the text (see XMLESearch for details)
@parameters XMLESearch *  searchClass  pointer to an initialized instance of XMLESearch
@returns    BOOL result
*/
BOOL XMultiLineEdit::Search(const XMLESearch * s) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, MLM_SEARCH, (MPARAM) s->mode, MPFROMP(&s->sStruct)));
}


/*@ XMultiLineEdit::Clear(void)
@group clipboard functions and selection
@remarks Clears marked text
*/
void XMultiLineEdit::Clear(void) const
{
   WinSendMsg(winhandle, MLM_CLEAR, 0, 0);
}


/*@ XMultiLineEdit::Copy(void)
@group clipboard functions and selection
@remarks Copy marked text
*/
void XMultiLineEdit::Copy(void) const
{
   WinSendMsg(winhandle, MLM_COPY, 0, 0);
}


/*@ XMultiLineEdit::Paste(void)
@group clipboard functions and selection
@remarks Paste text from the clipboard
*/
void XMultiLineEdit::Paste(void) const
{
   WinSendMsg(winhandle, MLM_PASTE, 0, 0);
}


/*@ XMultiLineEdit::Cut(void)
@group clipboard functions and selection
@remarks Cut marked text
*/
void XMultiLineEdit::Cut(void) const
{
   WinSendMsg(winhandle, MLM_CUT, 0, 0);
}
