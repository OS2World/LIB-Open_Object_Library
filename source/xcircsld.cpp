#include "xcircsld.h"

/*@
@class XCircularSlider
@parent XControl
@type overview
@symbol _
*/

/*@ XCircularSlider::XCircularSlider( const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char*font)
@group constructors/destructors
@remarks Constructs a XCircularSlider.
@parameters <t '�' c=2>
            �XWindow * owner      �the owner of the XCircularSlider
            �XRect& rect         �the rectangle
            �USHORT id            �id of the window
            �ULONG style          �style, valid values are:
<UL>
<li>CS_NOBUTTON
<li>CS_NOTEXT
<li>CS_NONUMBER
<li>CS_POINTSELECT
<li>CS_360
<li>CS_MIDPOINT
<li>CS_PROPORTIONALTICKS
<li>CS_NOTICKS
<li>CS_CIRCULARVALUE
<P>(can be or-ed)
            �char * font            �font to use (e.g. "8.Helv")
            </t>
*/
XCircularSlider :: XCircularSlider(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *font):XControl(&rec, style, owner, "", WC_CIRCULARSLIDER, id, font)
{
}


/*@ XCircularSlider :: GetValue( void )
@group set/get value
@remarks Return the current value
@returns SHORT theValue
*/
SHORT XCircularSlider::GetValue(void)
{
   SHORT val;

   WinSendMsg(winhandle, CSM_SETRANGE, MPFROMP(&val), 0);
   return val;
}


/*@ XCircularSlider :: SetIncrements( const SHORT ticks, const SHORT steps)
@group misc
@remarks Set ticks and steps
@parameters
<t '�' c=2>
�SHORT �ticks
�SHORT �steps
</t>
@returns    BOOL result
*/
BOOL XCircularSlider::SetIncrements(const SHORT ticks, const SHORT steps) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, CSM_SETINCREMENT, MPFROMSHORT(steps), MPFROMSHORT(ticks)));
}


/*@ XCircularSlider :: SetLimits( const LONG lowerLimit, const LONG upperLimit)
@group limits
@remarks Set limits
@parameters
<t '�' c=2>
�LONG �lowerLimit
�LONG �upperLimit
</t>
@returns BOOL result
*/
BOOL XCircularSlider::SetLimits(const LONG lowerLimit, const LONG upperLimit) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, CSM_SETRANGE, MPFROMSHORT(lowerLimit), MPFROMSHORT(upperLimit)));
}


/*@ XCircularSlider :: SetValue( const LONG val)
@group set/get value
@remarks Set a new value
@parameters LONG value
@returns BOOL result
*/
BOOL XCircularSlider::SetValue(const LONG val) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, CSM_SETVALUE, MPFROMSHORT(val), 0));
}


/*@ XCircularSlider :: GetLimits( SHORT& lowerLimit, SHORT& upperLimit )
@group limits
@remarks Query the limits set to the circular slider
@parameters
<t '�' c=2>
�SHORT& �lowerLimit
�SHORT& �upperLimit
</t>
*/
void XCircularSlider::GetLimits(SHORT & lowerLimit, SHORT & upperLimit)
{
   WinSendMsg(winhandle, CSM_QUERYRANGE, MPFROMP(&lowerLimit), MPFROMP(&upperLimit));
}


/*@ XCircularSlider :: GetIncremets( SHORT& scrollIncrement, SHORT& tickIncrement )
@group misc
@remarks Query the increments set to the slider
@parameters
<t '�' c=2>
�SHORT& �scrollIncrement
�SHORT& �tickIncrement
</t>
*/
void XCircularSlider::GetIncremets(SHORT & scrollIncrement, SHORT & tickIncrement)
{
   WinSendMsg(winhandle, CSM_QUERYRANGE, MPFROMP(&scrollIncrement), MPFROMP(&tickIncrement));
}
