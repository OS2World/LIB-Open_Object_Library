#include "xslider.h"
#include "xstring.h"
#include "xindicat.h"


/*@
@class XSlider
@parent XControl
@type overview
@symbol _
*/


/*@ XSlider::GetDetentPos(const LONG detentID)
@group detents
@remarks Get position of a detent
@parameters LONG id of the detent
@returns    USHORT positionin pixels
*/
USHORT XSlider::GetDetentPos(const LONG detentID) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, SLM_QUERYDETENTPOS, (MPARAM) detentID, 0));
}


/*@ XSlider::GetScaleText(const SHORT tickNumber, XString * buffer)
@group text functions
@remarks Get the text of a tick
@parameters
<t 'ø' c=2>
øSHORT øtickNumber
øXString* øbuffer to hold the text
</t>
*/
void XSlider::GetScaleText(const SHORT tickNumber, XString * buffer) const
{
   WinSendMsg(winhandle, SLM_QUERYSCALETEXT, MPFROM2SHORT(tickNumber, 512), MPFROMP(buffer->GetBuffer(512)));
   buffer->ReleaseBuffer();
}


/*@ XSlider :: XSlider(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font)
@group constructors/destructors
@remarks Constructs a XSlider
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the XSlider
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
<BR>
SL_HORIZONTAL
<BR>
SL_VERTICAL
<BR>
SL_CENTER
<BR>
SL_BOTTOM
<BR>
SL_TOP
<BR>
SL_LEFT
<BR>
SL_RIGHT
<BR>
SL_SNAPTOINCREMENT
<BR>
SL_BUTTONSBOTTOM
<BR>
SL_BUTTONSTOP
<BR>
SL_BUTTONSLEFT
<BR>
SL_BUTTONSRIGHT
<BR>
SL_READONLY
<BR>
SL_RIBBONSTRIP
<BR>
SL_HOMEBOTTOM
<BR>
SL_HOMETOP
<BR>
SL_HOMELEFT
<BR>
SL_HOMERIGHT
<BR>
SL_PRIMARYSCALE1
<BR>
SL_PRIMARYSCALE2
<BR>
                                (can be or-ed)
            øchar * font            øfont to use, e.g. "8.Helv"
            </t>
*/
XSlider :: XSlider(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(&rec, style, owner, "", WC_SLIDER, id, font)
{
}


/*@ XSlider::SetScales(const USHORT scale1Incr, const USHORT scale1Space, const USHORT scale2Incr, const USHORT scale2Space)
@group misc
@remarks Set scales 1 or 2 (depending if SL_PRIMARYSCALE1 or SL_PRIMARYSCALE2 is set)
@parameters <t 'ø' c=2>
            øUSHORT scale1Incr    øincrements for scale 1
            øUSHORT scale1Space   øspaceing between increments for scale 1
            øUSHORT scale2Incr    øincrements for scale 2 (default is 0)
            øUSHORT scale2Space   øspaceing between increments for scale 2 (default is 0)
            </t>
*/
void XSlider::SetScales(const USHORT scale1Incr, const USHORT scale1Space, const USHORT scale2Incr, const USHORT scale2Space)
{
   SLDCDATA data;

   data.cbSize = sizeof(data);
   data.usScale1Increments = scale1Incr;
   data.usScale2Increments = scale2Incr;
   data.usScale1Spacing = scale1Space;
   data.usScale2Spacing = scale2Space;

   WNDPARAMS params;

   params.fsStatus = WPM_CTLDATA;
   params.cbCtlData = sizeof(data);
   params.pCtlData = &data;

   WinSendMsg(winhandle, WM_SETWINDOWPARAMS, MPFROMP(&params), 0);
}


/*@ XSlider::SetTickSize(const SHORT tickNumber, const SHORT tickLength)
@group size
@remarks Set the size of a tick
@parameters
<t 'ø' c=2>
øSHORT øthe tick to change (zero-based index)
øSHORT ølength of the tick in pixels
</t>
*/
void XSlider::SetTickSize(const SHORT tickNumber, const SHORT tickLength) const
{
   WinSendMsg(winhandle, SLM_SETTICKSIZE, MPFROM2SHORT(tickNumber, tickLength), 0);
}


/*@ XSlider::SetScaleText(const SHORT tickNumber, const char *text)
@group text functions
@remarks Add a text to a tick
@parameters
<t 'ø' c=2>
øSHORT øthe tick to get the text (zero-based index)
øchar * øtext to add
</t>
*/
void XSlider::SetScaleText(const SHORT tickNumber, const char *text) const
{
   WinSendMsg(winhandle, SLM_SETSCALETEXT, MPFROMSHORT(tickNumber), MPFROMP(text));
}


/*@ XSlider::AddDetent(const SHORT detentPos)
@group detents
@remarks Add a detend to the primary scale
@parameters SHORT pos   position of the detent
@returns    LONG the id of the detent
*/
LONG XSlider::AddDetent(const SHORT detentPos) const
{
   return (LONG) WinSendMsg(winhandle, SLM_ADDDETENT, MPFROMSHORT(detentPos), 0);
}


/*@ XSlider::RemoveDetent(const LONG detentID)
@group detents
@remarks Removes a detend from the primary scale
@parameters LONG the id of the detent to remove
*/
void XSlider::RemoveDetent(const LONG detentID) const
{
   WinSendMsg(winhandle, SLM_REMOVEDETENT, (MPARAM) detentID, 0);
}


/*@ XSlider::SetSliderPos(const LONG pos)
@group set/get value
@remarks Set the sliders position
@parameters LONG the new position
*/
void XSlider::SetSliderPos(const LONG pos) const
{
   WinSendMsg(winhandle, SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE), (MPARAM) pos);
}


/*@ XSlider::GetSliderPos(void)
@group set/get value
@remarks Returns the sliders position
@returns LONG  position
*/
LONG XSlider::GetSliderPos(void) const
{
   return (LONG) SHORT2FROMMR(WinSendMsg(winhandle, SLM_QUERYSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE), 0));
}


/*@ XSlider::SetSliderSize(const SHORT length, const SHORT breadth)
@group size
@remarks Set the size of the slider
@parameters
<t 'ø' c=2>
øSHORT ølength of the slider
øSHORT øbreadth
</t>
*/
void XSlider::SetSliderSize(const SHORT length, const SHORT breadth) const
{
   WinSendMsg(winhandle, SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS, 0), MPFROM2SHORT(length, breadth));
}


/*@ XSlider::SetShaftSize(const SHORT size)
@group size
@remarks Set the size of the shaft
@parameters SHORT size  new size of the shaft
*/
void XSlider::SetShaftSize(const SHORT size) const
{
   WinSendMsg(winhandle, SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SHAFTDIMENSIONS, 0), MPFROMSHORT(size));
}


XIndicator :: XIndicator(const XWindow * owner, const XRect& rec, const USHORT id, const BOOL showScale, const BOOL showText, const char *font):XSlider(owner, rec, id, SL_RIBBONSTRIP | SL_READONLY | WIN_VISIBLE | SL_HOMELEFT | SL_LEFT | SL_HORIZONTAL)
{
   SetScales(101, 0, 101, 0);
   if (showScale)
   {
      SetTickSize(SMA_SETALLTICKS, 2);
      SetTickSize(0, 5);
      SetTickSize(25, 5);
      SetTickSize(50, 5);
      SetTickSize(75, 5);
      SetTickSize(100, 5);
   }
   if (showText)
   {
      SetScaleText(0, "0");
      SetScaleText(25, "25");
      SetScaleText(50, "50");
      SetScaleText(75, "75");
      SetScaleText(100, "100");
   }
}
