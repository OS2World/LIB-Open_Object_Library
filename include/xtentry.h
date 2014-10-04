#ifndef __OOL_XTIMEENTRYFIELD_H__
#define __OOL_XTIMEENTRYFIELD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XTimeEntryField                       */
/*                       derived from: XEntryField                           */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xentry.h"

class XTime;

class _export_ XTimeEntryField: public XEntryField
{
   private:
      BOOL checkTime;
   public:
      XTimeEntryField(const XWindow * owner, const XRect& rec= XRect(), const USHORT id = 0, const ULONG style= EN_BORDER | WIN_VISIBLE | EN_LEFT, const XTime * time=NULL, const char *font=NULL, BOOL checkT = TRUE);
      void GetText( XString * s) const { XEntryField::GetText(s); }
      BOOL GetText( XTime *) const;
      void SetText( const char * s) { XEntryField::SetText(s); }
      void SetText( const XTime *);
      virtual void FocusChanged( BOOL set );
};

#endif
