#ifndef __OOL_XCONTOBJECT_H__
#define __OOL_XCONTOBJECT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerObject                      */
/*                       derived from: XObject                               */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "xcontain.h"
#include "XBitmap.h"
#include "XIcon.h"
#include "xdate.h"
#include "xtime.h"
#include "xstring.h"


class XContainerControl;
class XString;
class XTime;
class XDate;

class _export_ XContainerObject: public XObject
{
      friend XContainerControl;
      friend MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
   private:
      RECORDCORE * core;
      char * GetColumnPointer( SHORT column) { return (char *) ((PBYTE) core + sizeof(RECORDCORE) + sizeof(void*) + column * sizeof(void*)); }
   protected:
      XString title;
   public:
      XContainerObject( const XContainerControl * owner, const SHORT columns = 0, const LONG attr = 0);
      virtual ~XContainerObject() { ;}
      virtual BOOL AllocMemory( char * oldText, SHORT newTextLength, XContainerColumn* ) { return FALSE;}
      LONG GetEmphasis( void ) const { return core->flRecordAttr; }
      void GetBitmap( XBitmap *);
      void GetIcon( XIcon * );
      void GetTitle( XString *);
      void SetBitmap( const XBitmap * b) { core->hbmBitmap = b->GetHandle(); }
      void SetColumnData( const SHORT column, const char * data) { memcpy( GetColumnPointer(column), &data, sizeof(PVOID)); }
      void SetColumnData( const SHORT column, const LONG data) { memcpy( GetColumnPointer(column), &data, sizeof(LONG)); }
      void SetColumnData( const SHORT column, const XBitmap*bitmap) { LONG buffer = bitmap->GetHandle(); memcpy( GetColumnPointer(column), &buffer, sizeof(LONG)); }
      void SetColumnData( const SHORT column, const XDate * data) { memcpy( GetColumnPointer(column), &data->d, 4); }
      void SetColumnData( const SHORT column, const XIcon * icon) { LONG buffer = icon->GetHandle(); memcpy( GetColumnPointer(column), &buffer, sizeof(LONG)); }
      void SetColumnData( const SHORT column, const XTime * data) { memcpy( GetColumnPointer(column), &data->t, 3); }
      void SetIcon( const XIcon * icon) { core->hptrIcon = icon->GetHandle(); }
      void SetMiniBitmap( const XBitmap* b) {core->hbmMiniBitmap = b->GetHandle();}
      void SetMiniIcon( const XIcon* icon) { core->hptrMiniIcon = icon->GetHandle(); }
      void SetTitle( char * t) { title = t; core->pszIcon = core->pszName = core->pszTree = core->pszText = (PSZ) (char*) title; }
      virtual SHORT Sort( const XContainerObject*) const { return 0; }
      virtual void TitleEdited( const char *, XContainerColumn* col = NULL) { ;}
};


#endif
