#include "xspinbtn.h"

/*@
@class XSpinButton
@parent XControl
@type overview
@symbol _
*/

/*@ XSpinButton :: XSpinButton(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font)
@group constructors/destructors
@remarks Constructs a XSpinButton
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner of the XSlider
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
<BR>
SP_LEFT
<BR>
SP_RIGHT
<BR>
SP_CENTER
<BR>
SP_NOBORDER
<BR>
SP_FAST
<BR>
SP_MASTER
<BR>
SP_SERVANT
<BR>
SP_READONLY
<BR>
SP_NUMERIC
<BR>
SP_CHAR
<BR>
SP_FILLZERO
<BR>
                                (can be or-ed)
            øchar * font            øfont to use, e.g. "8.Helv"
            </t>
*/
XSpinButton :: XSpinButton(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *string, const char *font):XControl(&rec, style, owner, string, WC_SPINBUTTON, id, font)
{
}


/*@ XSpinButton::SpinDown(const LONG units)
@group misc
@remarks Spin down window-content
@parameters LONG units to spin down
*/
void XSpinButton::SpinDown(const LONG units) const
{
   WinSendMsg(winhandle, SPBM_SPINDOWN, (MPARAM) units, 0);
}


/*@ XSpinButton::SpinUp(const LONG units)
@group misc
@remarks Spin up window-content
@parameters LONG units to spin up
*/
void XSpinButton::SpinUp(const LONG units) const
{
   WinSendMsg(winhandle, SPBM_SPINUP, (MPARAM) units, 0);
}


/*@ XSpinButton::SetTextLimit(const SHORT limit)
@group misc
@remarks Set maximum textlength
@parameters SHORT maximum length
*/
void XSpinButton::SetTextLimit(const SHORT limit) const
{
   WinSendMsg(winhandle, SPBM_SPINDOWN, MPFROMSHORT(limit), 0);
}


/*@ XSpinButton::SetMaster(const XWindow * master)
@group misc
@remarks
@parameters XWindow* master
@returns BOOL result
*/
BOOL XSpinButton::SetMaster(const XWindow * master) const
{
   return (BOOL) WinSendMsg(winhandle, SPBM_SETMASTER, (MPARAM) master->GetHandle(), 0);
}


/*@ XSpinButton::GetValue(void)
@group set/query values
@remarks Returns the current value of the spinbutton
*/
LONG XSpinButton::GetValue(void) const
{
   LONG l;

   WinSendMsg(winhandle, SPBM_QUERYVALUE, MPFROMP((VOID *) & l), 0);
   return l;
}


/*@ XSpinButton::SetLimits(const LONG lowerLimit, const LONG upperLimit)
@group limits
@remarks Set limits of the spinbutton
@parameters
<t 'ø' c=2>
øLONG øthe lower limit
øLONG øthe upper limit
</t>
*/
void XSpinButton::SetLimits(const LONG lowerLimit, const LONG upperLimit) const
{
   WinSendMsg(winhandle, SPBM_SETLIMITS, (MPARAM) upperLimit, (MPARAM) lowerLimit);
}


/*@ XSpinButton::SetValue(const LONG val)
@group set/query values
@remarks Set the value of the spinbutton
@parameters LONG the new value
*/
void XSpinButton::SetValue(const LONG val) const
{
   WinSendMsg(winhandle, SPBM_SETCURRENTVALUE, (MPARAM) val, 0);
}
