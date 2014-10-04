#ifndef __OOL_XDATEENTRYFIELD_H__
#define __OOL_XDATEENTRYFIELD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDateEntryField                       */
/*                       derived from: XEntryField                           */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xentry.h"

class XDate;
class XControlEvent;

class _export_ XDateEntryField: public XEntryField
{
   private:
      BOOL dateMode, checkDate;
   public:
      XDateEntryField(const XWindow * owner, const XRect& rec= XRect(), const USHORT id = 0, const ULONG style= EN_BORDER | WIN_VISIBLE | EN_LEFT, const XDate * date=NULL, const char *font=NULL, BOOL checkD = TRUE);
      void GetText( XString * s) const { XEntryField::GetText(s); }
      BOOL GetText( XDate *) const;
      void SetText( const char * s) { XEntryField::SetText(s); }
      void SetText( const XDate *);
      virtual void FocusChanged( BOOL set );
};

#endif
