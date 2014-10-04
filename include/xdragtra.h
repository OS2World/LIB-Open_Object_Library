#ifndef __OOL_XDRAGTRANSFER_H__
#define __OOL_XDRAGTRANSFER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDragTraansfer                        */
/*                       derived from: XObject                               */
/*                        last update: 12/97                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstring.h"
#include "xdragitm.h"
#include "xprocess.h"
class _export_ XDragTransfer: public XObject
{
      friend MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
   private:
      PDRAGTRANSFER trans;
      XDragTransfer(PDRAGTRANSFER p, ULONG i, ULONG r) {trans = p;id = i;renderValue = r;}
      ULONG id;
      ULONG renderValue;
   public:
      ULONG GetTransID() const {return id;}
      OOL_WINDOWHANDLE GetClientHandle() const {return trans->hwndClient;}
      void GetDragItem(XDragItem * it) {it->item = trans->pditem; }
      USHORT GetOperation() const {return trans->usOperation; }
      USHORT GetReply() const {return trans->fsReply; }
      ULONG GetRenderOption() const { return renderValue; }
      void GetRenderToName( XString * target) const
         {
            target->ReleaseBuffer(DrgQueryStrName(trans->hstrRenderToName, 512, (PSZ) target->GetBuffer(512)));
         }
      void GetRenderFormat( XString * target) const
         {
            target->ReleaseBuffer(DrgQueryStrName(trans->hstrSelectedRMF, 512, (PSZ) target->GetBuffer(512)));
         }
      void EndConversation( const OOL_WINDOWHANDLE hwnd, const ULONG id, const BOOL success = TRUE) const
         {
            DrgSendTransferMsg( hwnd, DM_ENDCONVERSATION, (MPARAM) id, (MPARAM) (success == TRUE ? DMFL_TARGETSUCCESSFUL : DMFL_TARGETFAIL));
         }
      void FreeTrans() const
         {
            DrgDeleteStrHandle( trans->hstrSelectedRMF );
            DrgDeleteStrHandle( trans->hstrRenderToName );
            DrgFreeDragtransfer(trans);
         }
      BOOL RenderComplete(const SHORT success, const BOOL retry = TRUE) const
         {
            return DrgPostTransferMsg(GetClientHandle(), DM_RENDERCOMPLETE, trans, success, 0, retry);
         }
};

#endif
