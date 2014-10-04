#ifndef __OOL_XCONTEDITEVENT_H__
#define __OOL_XCONTEDITEVENT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerEditEvent                   */
/*                       derived from: XContainerEvent                       */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcntevnt.h"

class XString;
class XContainerColumn;
class XContainerObject;

class _export_  XContainerEditEvent: public XContainerEvent
{
      friend MRESULT HandleFrameDefault( XFrameWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
      friend MRESULT HandleDefault( XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL& handled);
   private:
      XContainerEditEvent( const XWindow *, const MPARAM, const MPARAM);
      CNREDITDATA * cnEdit;
   public:
      void GetText( XString * s) { memcpy( s->GetBuffer( cnEdit->cbText + 1), *cnEdit->ppszText, cnEdit->cbText + 1); s->ReleaseBuffer( cnEdit->cbText ); }
      ULONG GetTextLength( void ) const { return cnEdit->cbText; }
      XContainerColumn * GetColumn( void ) const { return cnEdit->pFieldInfo ? (XContainerColumn*) cnEdit->pFieldInfo->pUserData : NULL; }
      XContainerObject * GetObject(void);
};


#endif
