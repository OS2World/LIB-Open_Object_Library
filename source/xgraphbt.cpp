#include "xgraphbt.h"

#include "string.h"
#include "stdlib.h"
#include "XColor.h"

#ifdef OOL_FOR_OS2_X86
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MMIO_CODEC
#define INCL_MCIOS2
#define INCL_MACHDR

#include "os2me.h"
#endif

#ifndef WC_GRAPHICBUTTON

#define GB_ERROR           -1
#define GB_UP               1
#define GB_DOWN             2
#define GB_DISABLE          3
#define GB_HILITE           4
#define GB_OUTOFHILITE      5
#define GB_TOGGLE          10
#define GB_CURRENTSTATE    11
#define GB_ANIMATIONBEGIN  12
#define GB_ANIMATIONEND    13
#define GB_MAXINDEX        14
#define GB_TEXTBELOW        1
#define GB_TEXTABOVE        2
#define GB_INDEX_FORWARD   -1
#define GB_INDEX_BACKWARD  -2
#define GB_INDEX_FIRST     -3
#define GB_INDEX_LAST      -4
#define GB_RESOURCE         1
#define GB_STRUCTURE        0

#define GBM_SETGRAPHICDATA        0x052A
#define GBM_ANIMATE               0x052B
#define GBM_SETANIMATIONRATE      0x052C
#define GBM_QUERYANIMATIONACTIVE  0x052D
#define GBM_QUERYANIMATIONRATE    0x052E
#define GBM_SETBITMAPINDEX        0x052F
#define GBM_QUERYBITMAPINDEX      0x0530
#define GBM_SETSTATE              0x0531
#define GBM_QUERYSTATE            0x0532
#define GBM_SETTEXTPOSITION       0x0533
#define GBM_QUERYTEXTPOSITION     0x0534

#define GBN_BUTTONUP      0x0524
#define GBN_BUTTONDOWN    0x0525
#define GBN_BUTTONHILITE  0x0526

#define GBS_TWOSTATE         0x1000
#define GBS_AUTOTWOSTATE     0X2000
#define GBS_ANIMATION        0x4000
#define GBS_AUTOANIMATION    0x8000
#define GBS_DISABLEBITMAP    0x0010
#define GBS_HILITEBITMAP     0x0020
#define GBS_3D_TEXTRECESSED  0x0040
#define GBS_3D_TEXTRAISED    0x0080

#define WC_GRAPHICBUTTON ((PSZ)0xffff0040L)

 /* use 8-bit alignment */
#pragma pack(1)

typedef struct _GBTNCDATA
{
   USHORT usReserved;
   PSZ pszText;
   HMODULE hmod;
   USHORT cBitmaps;
   USHORT aidBitmap[1];
}   GBTNCDATA;

typedef GBTNCDATA *PGBTNCDATA;

 /* use 32-bit alignment again */
#pragma pack(4)

BOOL WinRegisterGraphicButton(VOID);

#endif


/*@ XGraphicButton::Select(BOOL select)
@group misc
@remarks Select/deselect the button
@parameters BOOL select    TRUE=select, FALSE=deselect
*/
void XGraphicButton::Select(const BOOL select)
{
   SHORT sel = select ? 2 : 1;

   WinSendMsg(winhandle, GBM_SETSTATE, MPFROMSHORT(sel), 0);
}


/*@ XGraphicButton::IsSelected(void)
@group misc
@remarks Query if the button is selected
@returns BOOL result
*/
BOOL XGraphicButton::IsSelected(void) const
{
   if (SHORT1FROMMR(WinSendMsg(winhandle, GBM_QUERYSTATE, 0, 0)) == 2)
      return TRUE;
   else
      return FALSE;
}


/*@ XGraphicButton::Toggle(void)
@group misc
@remarks Toggles the button
*/
void XGraphicButton::Toggle(void) const
{
   WinSendMsg(winhandle, GBM_SETSTATE, MPFROMSHORT(10), 0);
}


XGraphicButton :: XGraphicButton(HWND hwnd):XSettingButton(hwnd)
{
}


/*@
@class XGraphicButton
@parent XSettingButton
@type overview
@symbol _
*/

/*@ XGraphicButton :: XGraphicButton( const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char * s, const SHORT bitmapIdCount, const SHORT * bitmapIdList);
@group constructors/destructors
@remarks Construct a graphic-button. To use this control you must invoke the multimedia-library!
@parameters <t 'ø' c=2>
            øXWindow * owner      øowner of the button
            øXRect& rectangle    ørectangle for position/size
            øUSHORT id            øid of the window
            øULONG style          østyle of the button:
                                 <t 'ø' c=1>
                                    øGB_TWOSTATE
                                    øGB_AUTOTWOSTATE
                                    øGB_ANIMATION
                                    øGB_AUTOANIMATION
                                    øGB_DISABLEBITMAP
                                    øGB_HILITEBITMAP
                                    øGB_3D_TEXTRECESSED
                                    øGB_3D_TEXTRAISED
                                 </t>
            øconst char * title   øtext to display
            øSHORT bitmapCount    ønumber of bitmap-ids in parameter bitmapList
            øSHORT * bitmapList   øarray of bitmap-ids to use for an animation. Bitmaps
                                 with this ids must be in the programs resources (not in DLLs).
            </t>
*/
XGraphicButton :: XGraphicButton( const XWindow * owner, const XRect& rec, const USHORT id, const ULONG style, const char *s, const SHORT bitmapIdCount, const SHORT * bitmapIdList):XSettingButton(owner, &rec, id, style, "", (unsigned char *) WC_GRAPHICBUTTON)
{
   PGBTNCDATA pgbtn;
   LONG lSize;
   SHORT i;

   lSize = sizeof(GBTNCDATA) + sizeof(USHORT) * (bitmapIdCount - 1);
   pgbtn = (PGBTNCDATA) malloc(lSize);

   memset(pgbtn, 0, lSize);

   pgbtn->pszText = (PSZ) s;
   pgbtn->cBitmaps = bitmapIdCount;
   for (i = 0; i < bitmapIdCount; i++)
      pgbtn->aidBitmap[i] = bitmapIdList[i];

   WinSendMsg(winhandle, GBM_SETGRAPHICDATA, MPFROMP(pgbtn), 0);

   XColor color;

   owner->GetBackgroundColor(&color);
   SetBackgroundColor(&color);
}


/*@ XGraphicButton::Animate(BOOL start, BOOL fromBeginning)
@group animation
@remarks Start/stop to display the animation (if GB_ANIMATION is set)
@parameters BOOL start        TRUE=start, FALSE=stop<BR>
            BOOL fromBegining TRUE=display from beginning,
                              FALSE=display from current position
@returns    BOOL success
*/
BOOL XGraphicButton::Animate(const BOOL start, const BOOL fromBeginning)
{
   return (BOOL) WinSendMsg(winhandle, GBM_ANIMATE, MPFROMSHORT(start), MPFROMSHORT(fromBeginning));
}


/*@ XGraphicButton::IsAnimationActive(void)
@group animation
@remarks Query if the animation is active
@returns BOOL result
*/
BOOL XGraphicButton::IsAnimationActive(void)
{
   return (BOOL) WinSendMsg(winhandle, GBM_QUERYANIMATIONACTIVE, 0, 0);
}


/*@ XGraphicButton::GetAnimationRate(void)
@group animation
@remarks Query the speed of the animation
@returns LONG  speed
*/
LONG XGraphicButton::GetAnimationRate(void) const
{
   return (LONG) WinSendMsg(winhandle, GBM_QUERYANIMATIONRATE, 0, 0);
}


/*@ XGraphicButton::SetAnimationRate(LONG rate)
@group animation
@remarks Set the speed of animation
@parameters LONG rate   speed
@returns    BOOL result
*/
BOOL XGraphicButton::SetAnimationRate(const LONG rate) const
{
   return (BOOL) WinSendMsg(winhandle, GBM_SETANIMATIONRATE, (MPARAM) rate, 0);
}
