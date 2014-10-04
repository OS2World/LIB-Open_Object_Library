#include "xentry.h"
#include "xdentry.h"
#include "xtentry.h"
#include "xdate.h"
#include "xkeybhdl.h"
#include "xkeyevnt.h"
#include "xdate.h"
#include "xtime.h"
#include "stdlib.h"
#include "xcountry.h"

/*@
@class XEntryField
@parent XControl
@type overview
@symbol _
@remarks XEntryField is a single-line text field.
*/


/*@ XEntryField::IsReadOnly(void)
@group misc
@remarks Query if the entryfild is in readonly-mode.
@returns BOOL result
*/
BOOL XEntryField::IsReadOnly(void) const
{
   return (BOOL) WinSendMsg(winhandle, EM_QUERYREADONLY, 0, 0);
}


/*@ XEntryField::SetReadOnly(const BOOL readOnly)
@group misc
@remarks Enable/disable readonly-mode.
@parameters BOOL readOnly  TRUE=enable, FALSE=disable readonly-mose
*/
void XEntryField::SetReadOnly(const BOOL readOnly) const
{
   WinSendMsg(winhandle, EM_SETREADONLY, MPFROMSHORT(readOnly), 0);
}


/*@ XEntryField::GetFirstChar(void)
@group misc
@remarks Query the index of the first visible charakter.
@returns SHORT index
*/
SHORT XEntryField::GetFirstChar(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, EM_QUERYFIRSTCHAR, 0, 0));
}


/*@ XEntryField::GetSelection(SHORT & start, SHORT & end)
@group clipboard functions
@remarks Query the boundarys of the current selection.
@parameters
<t 'ø' c=2>
øSHORT& østarting point of selection
øSHORT& øend point
</t>
*/
void XEntryField::GetSelection(SHORT & start, SHORT & end)
{
   MRESULT res = WinSendMsg(winhandle, EM_QUERYSEL, 0, 0);

   start = SHORT1FROMMR(res);
   end = SHORT2FROMMR(res);
}


/*@ XEntryField::SetLimit(const unsigned short len)
@group misc
@remarks Set maximum number of charakters.
@parameters USHORT length  maximum size
*/
void XEntryField::SetLimit(const unsigned short len) const
{
   WinSendMsg(winhandle, EM_SETTEXTLIMIT, MPFROMSHORT(len), 0);
}


/*@ XEntryField::IsChanged(void)
@group misc
@remarks Query if the content has changed.
@returns BOOL result
*/
BOOL XEntryField::IsChanged(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, EM_QUERYCHANGED, 0, 0));
}


/*@ XEntryField::EnableOverWrite(const BOOL overWrite)
@group misc
@remarks Enables/disables overwrite-mode.
@parameters BOOL overwrite    TRUE=overWrite, FALSE=disable overWrite
*/
void XEntryField::EnableOverWrite(const BOOL overWrite) const
{
   WinSendMsg(winhandle, EM_SETINSERTMODE, MPFROMSHORT(overWrite), 0);
}


/*@ XEntryField :: XEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font)
@group constructors/destructors
@remarks Constructs an entryfield.
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the entryfield
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
                                 <t 'ø' c=2>
                                    øEN_LEFT   ø
                                    øEN_CENTER   ø
                                    øEN_RIGHT   ø
                                    øEN_AUTOSCROLL   ø
                                    øEN_MARGIN   ø
                                    øEN_AUTOTAB   ø
                                    øEN_READONLY   ø
                                    øEN_UNREADABLE   ø
                                    øEN_AUTOSIZE   ø
                                    øEN_BORDER   ø
                                 </t>
                                 (can be or-ed)
            øconst char * string  øtext to display (default is NULL)
            øconst char * font    øfont to use (default is NULL)
            </t>
*/
XEntryField :: XEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font):XControl(&rec, style | FS_NOBYTEALIGN, owner, string, WC_ENTRYFIELD, id, font)
{
}


/*@ XEntryField::SelectText(const unsigned short start, const unsigned short end)
@group clipboard functions
@remarks Select text from startpoint to an endpoint.
@parameters
<t 'ø' c=2>
øUSHORT øthe starting point of selection
øUSHORT øend-point of selection
</t>
*/
void XEntryField::SelectText(const unsigned short start, const unsigned short end) const
{
   WinSendMsg(winhandle, EM_SETSEL, MPFROM2SHORT(start, end), 0);
}


/*@ XEntryField::ShowFirstChar(const SHORT firstChar)
@group misc
@remarks Select the first char to display
@parameters SHORT firstChar   the first char which will be displayed
*/
void XEntryField::ShowFirstChar(const SHORT firstChar) const
{
   WinSendMsg(winhandle, EM_SETFIRSTCHAR, MPFROMSHORT(firstChar), 0);
}


/*@ XEntryField::Clear(void)
@group clipboard functions
@remarks Delete the marked text.
*/
void XEntryField::Clear(void) const
{
   WinSendMsg(winhandle, EM_CLEAR, 0, 0);
}


/*@ XEntryField::Copy(void)
@group clipboard functions
@remarks Copy the marked text to the clipboard.
*/
void XEntryField::Copy(void) const
{
   WinSendMsg(winhandle, EM_COPY, 0, 0);
}


/*@ XEntryField::Paste(void)
@group clipboard functions
@remarks Paste text from the clipboard.
*/
void XEntryField::Paste(void) const
{
   WinSendMsg(winhandle, EM_PASTE, 0, 0);
}


/*@ XEntryField::Cut(void)
@group clipboard functions
@remarks Cut marked text and put it to the clipboard.
*/
void XEntryField::Cut(void) const
{
   WinSendMsg(winhandle, EM_CUT, 0, 0);
}


/*@
@class XDateEntryField
@parent XEntryField
@type overview
@symbol _
@remarks XDateEntryField is a single-line text field for dates.
*/

class dateHandler: public XKeyboardHandler
{
   public:
      dateHandler(XWindow*w):XKeyboardHandler(w) {;}
      BOOL HandleEvent( XKeyboardEvent *);
};


BOOL dateHandler :: HandleEvent( XKeyboardEvent * e)
{
   if(( e->GetEventID()  < '0' || e->GetEventID() > '9') && e->GetEventID() != '.' && e->GetVirtualKey() == 0)
      return TRUE;
   return FALSE;
}


/*@ XDateEntryField :: XDateEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const XDate * date, const char *font, BOOL checkD)
@group constructors/destructors
@remarks Constructs an entryfield for dates.
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the entryfield
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window (default is zero)
            øULONG style          østyle, valid values are:
                                 <t 'ø' c=2>
                                    øEN_LEFT   ø
                                    øEN_CENTER   ø
                                    øEN_RIGHT   ø
                                    øEN_AUTOSCROLL   ø
                                    øEN_MARGIN   ø
                                    øEN_AUTOTAB   ø
                                    øEN_READONLY   ø
                                    øEN_UNREADABLE   ø
                                    øEN_AUTOSIZE   ø
                                    øEN_BORDER   ø
                                 </t>
                                 (can be or-ed)
            øconst char * string  øtext to display
            øconst char * font    øfont to use
            øBOOL checkDates      øTRUE=check entered date when the focus is lost. If the entered date is not
                                   a legal date, the background turns red.<BR>FALSE=dont check entered values.
            </t>
*/
XDateEntryField :: XDateEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const XDate * date, const char *font, BOOL checkD):XEntryField(owner, rec, id, style, "", font)
{
   checkDate = checkD;
   XCountryInfo info;
   if( info.GetDateFormat() == 1)//euro
      dateMode = TRUE;
   else
      dateMode = FALSE;
   SetLimit(10);
   if(date)
      SetText((XDate*) date);
   dateHandler * e = new dateHandler(this);
}


void XDateEntryField :: FocusChanged( BOOL set )
{
   if(set == FALSE && checkDate)
   {
      XDate date;
      if( GetText(&date) == FALSE)
      {
         XString t;
         GetText(&t);
         t.Strip();
         if( t.GetLength() == 0)
            return;
         XColor col( COL_RED);
         SetBackgroundColor( &col );
      }
      else
      {
         XColor col( COL_WHITE);
         SetBackgroundColor( &col );
         SetText(&date);
      }
   }
}


void XDateEntryField :: SetText( const XDate * date)
{
   XString buffer;
   if(dateMode) //euro
      date->Format( &buffer, "%d.%m.%Y");
   else
      date->Format( &buffer, "%m.%d.%Y");
   XEntryField::SetText( (char*) buffer );
}


BOOL XDateEntryField :: GetText( XDate * date) const
{
   XString buffer;
   XEntryField::GetText( &buffer );
   buffer.Strip();
   if(buffer.GetLength() == 0)
      return FALSE;

   ULONG fo1, fo2;

   if(dateMode) //euro
   {
      if( buffer.Find( fo1, "."))
      {
         date->SetDays( atol(buffer.Left(fo1)));
         if(buffer.Find( fo2, ".", fo1+1))
         {
            date->SetMonths( atol(buffer.Mid(fo1+1, fo2-fo1-1)));
            date->SetYears( atol( buffer.Right(buffer.GetLength() - fo2 - 1)));
         }
      }
   }
   else
   {
      if( buffer.Find( fo1, "."))
      {
         date->SetMonths( atol(buffer.Left(fo1)));
         if(buffer.Find( fo2, ".", fo1+1))
         {
            date->SetDays( atol(buffer.Mid(fo1+1, fo2-fo1-1)));
            date->SetYears( atol( buffer.Right(buffer.GetLength() - fo2 - 1)));
         }
      }
   }

   if( date->GetYears() < 100)
   {
      if( date->GetYears() < 20)
         date->SetYears( date->GetYears() + 2000 );
      else
         date->SetYears( date->GetYears() + 1900 );
   }

   return date->IsLegalDate();
}


class timeHandler: public XKeyboardHandler
{
   public:
      timeHandler(XWindow*w):XKeyboardHandler(w) {;}
      BOOL HandleEvent( XKeyboardEvent *);
};


BOOL timeHandler :: HandleEvent( XKeyboardEvent * e)
{
   if(( e->GetEventID()  < '0' || e->GetEventID() > '9') && e->GetEventID() != ':' && e->GetVirtualKey() == 0)
      return TRUE;
   return FALSE;
}


/*@
@class XTimeEntryField
@parent XEntryField
@type overview
@symbol _
*/

/*@ XTimeEntryField :: XTimeEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const XTime * Time, const char *font, BOOL checkD)
@group constructors/destructors
@remarks Constructs an entryfield for Times.
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the entryfield
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window (default is zero)
            øULONG style          østyle, valid values are:
                                 <t 'ø' c=2>
                                    øEN_LEFT   ø
                                    øEN_CENTER   ø
                                    øEN_RIGHT   ø
                                    øEN_AUTOSCROLL   ø
                                    øEN_MARGIN   ø
                                    øEN_AUTOTAB   ø
                                    øEN_READONLY   ø
                                    øEN_UNREADABLE   ø
                                    øEN_AUTOSIZE   ø
                                    øEN_BORDER   ø
                                 </t>
                                 (can be or-ed)
            øconst char * string  øtext to display
            øconst char * font    øfont to use
            øBOOL checkTimes      øTRUE=check entered Time when the focus is lost. If the entered Time is not
                                   a legal Time, the background turns red.<BR>FALSE=dont check entered values.
            </t>
*/
XTimeEntryField :: XTimeEntryField(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const XTime * time, const char *font, BOOL checkD):XEntryField(owner, rec, id, style, "", font)
{
   checkTime = checkD;

   SetLimit(8);
   if(time)
      SetText((XTime*) time);
   timeHandler * e = new timeHandler(this);
}


void XTimeEntryField :: FocusChanged( BOOL set )
{
   if(set == FALSE && checkTime)
   {
      XTime time;
      if( GetText(&time) == FALSE)
      {
         XString t;
         GetText(&t);
         t.Strip();
         if( t.GetLength() == 0)
            return;
         XColor col( COL_RED);
         SetBackgroundColor( &col );
      }
      else
      {
         XColor col( COL_WHITE);
         SetBackgroundColor( &col );
         SetText(&time);
      }
   }
}


void XTimeEntryField :: SetText( const XTime * time)
{
   XString buffer;
   time->Format( &buffer, "%H:%M:%S");
   XEntryField::SetText( (char*) buffer );
}


BOOL XTimeEntryField :: GetText( XTime * time) const
{
   XString buffer;
   XEntryField::GetText( &buffer );
   buffer.Strip();
   if(buffer.GetLength() == 0)
      return FALSE;

   ULONG fo1, fo2;

   if( buffer.Find( fo1, ":"))
   {
       time->SetHours( atol(buffer.Left(fo1)));
      if(buffer.Find( fo2, ":", fo1+1))
      {
           time->SetMinutes( atol(buffer.Mid(fo1+1, fo2-fo1-1)));
           time->SetSeconds( atol( buffer.Right(buffer.GetLength() - fo2 - 1)));
      }
      else
         time->SetMinutes( atol( buffer.Right(buffer.GetLength() - fo1 - 1)));
   }

   return time->IsLegalTime();
}
