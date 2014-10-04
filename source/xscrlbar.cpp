#include "xscrlbar.h"


/*@ 
@class XScrollBar
@parent XControl
@type overview
@symbol _
*/


XScrollBar :: XScrollBar(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style):XControl(&rec, style, owner, "", WC_SCROLLBAR, id)
{
}


BOOL XScrollBar::SetPos(const LONG pos) const
{
	return (BOOL) WinSendMsg(winhandle, SBM_SETPOS, MPFROMSHORT(pos), 0);
}


BOOL XScrollBar::SetRange(const SHORT first, const SHORT last, const SHORT sliderPos) const
{
	return (BOOL) WinSendMsg(winhandle, SBM_SETSCROLLBAR, MPFROMSHORT(sliderPos), MPFROM2SHORT(first, last));
}


BOOL XScrollBar::SetSliderSize(const SHORT visible, const SHORT total) const
{
	return (BOOL) WinSendMsg(winhandle, SBM_SETTHUMBSIZE, MPFROM2SHORT(visible, total), 0);
}


SHORT XScrollBar::GetPos(void) const
{
	return SHORT1FROMMR(WinSendMsg(winhandle, SBM_QUERYPOS, 0, 0));
}


void XScrollBar::GetRange(SHORT & first, SHORT & last)
{
	MRESULT mr = WinSendMsg(winhandle, SBM_QUERYRANGE, 0, 0);

	first = SHORT1FROMMR(mr);
	last = SHORT2FROMMR(mr);
}
