#ifndef __OOL_XDRAGITEM_H__
#define __OOL_XDRAGITEM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDragItem                             */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstring.h"
#include "xdragevn.h"


#ifdef OOL_FOR_OS2_X86

   //mechanism information
   #define DRG_M_FILE    "DRM_OS2FILE"
   #define DRG_M_OBJECT  "DRM_OBJECT"

   //format information
   #define DRG_F_TEXT    "DRF_TEXT"

#endif


class _export_ XDragItem: public XObject
{
      friend class XDragEvent;
      friend class XContainerDragEvent;
      friend class XWindowDrag;
      friend class XDragTransfer;
   private:
      PDRAGITEM item;
      BOOL allocated;
      PDRAGTRANSFER trans;
   public:
      XDragItem() { allocated = FALSE; trans = NULL; }
      XDragItem( const XWindow * sourceWindow, const SHORT operation = DRG_COPY, const LONG id = 0, const char * type = DRG_M_OBJECT , const char * mechanism = DRG_F_TEXT);
      ~XDragItem();
      void Accept( const BOOL accept = TRUE );
      void EndConversation( const BOOL success = TRUE);
      OOL_WINDOWHANDLE GetSourceWindow(void) const { return item->hwndItem; }
      USHORT GetControlOps() const;
      void GetContainerName( XString*) const;
      LONG GetID(void) const {return item->ulItemID;}
      void GetMechanism( XString *) const;
      void GetName( XString * ) const;
      void GetTargetName( XString *) const;
      void GetType( XString *) const;
      BOOL RenderPrepare( const XWindow * target, const USHORT operation, const USHORT reply, const char * mechanism, const char * name, ULONG& suportedOps);
      void SetContainerName( const char *);
      void SetControlOps( const USHORT );
      void SetMechanism( const char *);
      void SetName( const char *);
      void SetSupportedOps( const USHORT ops);
      void SetTargetName( const char *);
      void SetType(const char * type);
      BOOL VerifyItemType( const char * mechanism, const char * format = NULL);
      BOOL Render( const XWindow * target, const USHORT operation, const USHORT reply, const char * mechanism, const char * name, ULONG& suportedOps);
};

#endif
