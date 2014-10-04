#ifndef __OOL_XCONTCOL_H__
#define __OOL_XCONTCOL_H__


/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerColumn                      */
/*                       derived from: XObject                               */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"

class XContainerControl;
#include "XIcon.h"
#include "XBitmap.h"


#ifdef OOL_FOR_OS2_X86

   //settings
   #define COL_LEFT         CFA_LEFT
   #define COL_RIGHT         CFA_RIGHT
   #define COL_CENTER      CFA_CENTER

   #define COL_TOP            CFA_TOP
   #define COL_VCENTER         CFA_VCENTER
   #define COL_BOTTOM         CFA_BOTTOM

   #define COL_INVISIBLE         CFA_INVISIBLE

   #define COL_SEPARATOR         CFA_SEPARATOR
   #define COL_HORZSEPARATOR      CFA_HORZSEPARATOR

   #define COL_BITMAPORICON      CFA_BITMAPORICON
   #define COL_STRING         CFA_STRING
   #define COL_DATE         CFA_DATE
   #define COL_TIME         CFA_TIME
   #define COL_ULONG         CFA_ULONG

   #define COL_FIREADONLY         CFA_FIREADONLY
   #define COL_FITITLEREADONLY      CFA_FITITLEREADONLY
   #define COL_OWNERDRAW   CFA_OWNER
#endif

class _export_ XContainerColumn: public XObject
{
      friend XContainerControl;
      friend class XContainerInfo;
      friend MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
   private:
      FIELDINFO * info;
      XContainerControl * owner;
      SHORT col;
      XString title;
   public:
      XContainerColumn( XContainerControl * owner, void * titleData, SHORT dataColumn, LONG dataSettings = CFA_STRING|CFA_FIREADONLY , LONG titleSettings = CFA_LEFT | CFA_FITITLEREADONLY, LONG wight = 0);
      virtual ~XContainerColumn();
      XContainerColumn * GetNextColumn( void ) const { return info->pNextFieldInfo ? (XContainerColumn*) info->pNextFieldInfo->pUserData : 0; }
      LONG GetDataAttributes( void ) const { return info->flData; }
      SHORT GetColumnNumber(void) const { return col; }
      LONG GetTitleAttributes( void ) const { return info->flTitle; }
      void GetTitle( XString *);
      SHORT GetWidth() const { return info->cxWidth; }
      void SetDataAttributes( const LONG attr ) { info->flData = attr; }
      void SetTitleAttributes( const LONG attr ) { info->flTitle = attr; }
      void SetTitle( const char * p) { title = p; info->pTitleData = (char*) title; }
      void SetTitle( const XBitmap * p) { info->pTitleData = (void*) p->GetHandle(); }
      void SetTitle( const XIcon * p) { info->pTitleData = (void*) p->GetHandle(); }
      virtual void TitleEdited( const char *) { ;}
};

#endif
