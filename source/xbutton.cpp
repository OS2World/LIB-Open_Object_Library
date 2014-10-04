#include "xcheckbx.h"
#include "xpushbtn.h"
#include "xradio.h"
#include "xuserbtn.h"
#include "xstring.h"

#include <stdlib.h>


/*@
@class XButton
@parent XControl
@type overview
@symbol _
*/

XButton :: XButton(const XWindow * owner, const XRect * rec, const USHORT id, const ULONG style, const char *s, const unsigned char *classStyle, const char *font):XControl(rec, style, owner, (const char *) s, (PSZ) classStyle, id, font)
{
}


/*@
@class XPushButton
@parent XButton
@type overview
@symbol _
*/

/*@ XPushButton :: XPushButton( const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char * s, const char*font)
@group constructors/destructors
@remarks Constructs a pushbuttom
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner
            øXRect * rect         øthe rectangle
            øUSHORT id            øid of the window
            øULONG style          østyle, valid values are:
<BR>
BU_TEXT
<BR>
BU_HELP
<BR>
BU_DEFAULT
<BR>
BU_NOPOINTERFOCUS
<BR>
BU_NOBORDER
<BR>
BU_NOCURSORSELECT
<BR>
BU_AUTOSIZE
<BR>
(can be or-ed). If BU_ICON or BU_BITMAP is set, an icon/bitmap must be avaible in the programs resources with the id given in id (third parameter).
            øconst char * string  øtext to display
            </t>
*/
XPushButton :: XPushButton(const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *s, const char *font):XButton(owner, &rec, id, style | BS_PUSHBUTTON, (const char *) s, (unsigned char *) WC_BUTTON, font)
{
}


/*@
@class XSettingButton
@parent XButton
@type overview
@symbol _
*/
XSettingButton :: XSettingButton(const XWindow * owner, const XRect * rec, const USHORT id, const ULONG style, const char *s, const unsigned char *classStyle, const char *font):XButton(owner, rec, id, style, (const char *) s, (unsigned char *) classStyle, font)
{
}


/*@ XSettingButton :: Select( BOOL select)
@group misc
@remarks Select/deselect a XSettingButton
@parameters BOOL select    TRUE=select, FALSE=deselect
*/
void XSettingButton::Select(const BOOL select)
{
   WinSendMsg(winhandle, BM_SETCHECK, MPFROMSHORT(select), 0);
}


/*@ XSettingButton :: IsSelected( void )
@group misc
@remarks Query if a button is selected
@returns BOOL result
*/
BOOL XSettingButton::IsSelected(void) const
{
   return SHORT1FROMMR(WinSendMsg(winhandle, BM_QUERYCHECK, 0, 0));
}


/*@
@class XCheckBox
@parent XSettingButton
@type overview
@symbol _
*/


/*@ XCheckBox::XCheckBox( const XWindow * owner, const XRect& rec, const USHORT id, const LONG style, const char * s, const char*font)
@group constructors/destructors
@remarks Constructs a XCheckBox
@parameters <t 'ø' c=2>
         øXWindow * owner      øthe owner
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øLONG style           øthe style
            øconst char * string  øtext to display
         </t>
*/
XCheckBox :: XCheckBox(const XWindow * owner, const XRect& rec, const USHORT id, const LONG style, const char *s, const char *font):XSettingButton(owner, &rec, id, style | BS_AUTOCHECKBOX, (const char *) s, (unsigned char *) WC_BUTTON, font)
{
}


/*@
@class XRadioButton
@parent XSettingButton
@type overview
@symbol _
*/


/*@ XRadioButton::XRadioButton( const XWindow * owner, const XRect& rec, const USHORT id, const LONG style, const char * s, const char * font)
@group constructors/destructors
@remarks Constructs a XCheckBox
@parameters <t 'ø' c=2>
            øXWindow * owner      øthe owner
            øXRect& rect         øthe rectangle
            øUSHORT id            øid of the window
            øLONG style           øthe style
            øconst char * string  øtext to display
            </t>
*/
XRadioButton :: XRadioButton(const XWindow * owner, const XRect& rec, const USHORT id, const LONG style, const char *s, const char *font):XSettingButton(owner, &rec, id, style | BS_AUTORADIOBUTTON, (const char *) s, (unsigned char *) WC_BUTTON, font)
{
}



