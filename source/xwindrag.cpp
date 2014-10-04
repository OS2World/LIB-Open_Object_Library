#include "XWindow.h"
#include "XWinDrag.h"
#include "XDragItm.h"
#include "XBitmap.h"
#include "XIcon.h"
#include "xprocess.h"
#include <stdlib.h>
#include <string.h>


XWindowDrag :: XWindowDrag(const XWindow * owner, const SHORT itemCount)
{
   count = itemCount;
   ownerWindow = (XWindow *) owner;
   pInfo = DrgAllocDraginfo(count);
   imageCount = 0;
   pImage = NULL;
}


XWindowDrag :: ~XWindowDrag()
{
/*
   free( pImage );
   DrgDeleteDraginfoStrHandles( pInfo );
   DrgFreeDraginfo( pInfo );
*/
}


OOL_WINDOWHANDLE XWindowDrag::PerformDrag(void)
{
   return DrgDrag(ownerWindow->GetHandle(), pInfo, pImage, imageCount, VK_BUTTON2, NULL);
}


void XWindowDrag::AddImage(const XIcon * icon, const SHORT style, const SHORT xOffset, const SHORT yOffset, const SHORT stretch)
{
   imageCount += 1;
   pImage = (DRAGIMAGE *) realloc(pImage, sizeof(DRAGIMAGE) * imageCount);
   pImage[imageCount - 1].cb = sizeof(DRAGIMAGE);
   pImage[imageCount - 1].cxOffset = xOffset;
   pImage[imageCount - 1].cyOffset = yOffset;
   pImage[imageCount - 1].hImage = icon->GetHandle();
   pImage[imageCount - 1].fl = style | IMG_ICON;
   pImage[imageCount - 1].sizlStretch.cx = pImage[imageCount - 1].sizlStretch.cy = stretch;
}


void XWindowDrag::AddImage(const XBitmap * bmp, const SHORT style, const SHORT xOffset, const SHORT yOffset, const SHORT stretch)
{
   imageCount += 1;
   pImage = (DRAGIMAGE *) realloc(pImage, sizeof(DRAGIMAGE) * imageCount);
   pImage[imageCount - 1].cb = sizeof(DRAGIMAGE);
   pImage[imageCount - 1].cxOffset = xOffset;
   pImage[imageCount - 1].cyOffset = yOffset;
   pImage[imageCount - 1].hImage = bmp->GetHandle();
   pImage[imageCount - 1].fl = style | IMG_BITMAP;
   pImage[imageCount - 1].sizlStretch.cx = pImage[imageCount - 1].sizlStretch.cy = stretch;
}


void XWindowDrag::SetDragItem(const XDragItem * item, const SHORT number)
{
   DrgSetDragitem(pInfo, item->item, sizeof(DRAGITEM), number);
}


void XWindowDrag::GetDragItem(XDragItem * item, const SHORT number)
{
//   DrgQueryDragitem(pInfo, sizeof(DRAGITEM), item->item, number);
   item->item = DrgQueryDragitemPtr(pInfo, number);
}


XDragItem :: XDragItem(const XWindow * sourceWindow, const SHORT operation, const LONG id, const char *type, const char *mechanism)
{
   trans = NULL;
   item = (DRAGITEM *) malloc(sizeof(DRAGITEM));
   memset(item, 0, sizeof(DRAGITEM));
   allocated = TRUE;
   item->hwndItem = sourceWindow->GetHandle();
   item->ulItemID = (ULONG) id;
   item->hstrType = DrgAddStrHandle((PSZ) type);
   item->hstrRMF = DrgAddStrHandle((PSZ) mechanism);
   item->fsControl = 0;
   item->fsSupportedOps = operation;
}


BOOL XDragItem :: RenderPrepare( const XWindow * target, const USHORT operation, const USHORT reply, const char * mechanism, const char * container, ULONG& support)
{
   if(trans == NULL)
   {
      trans = DrgAllocDragtransfer(1);
      trans->cb = sizeof(trans);
      trans->hwndClient = target->GetHandle();
      trans->pditem = item;
   }
   else
   {
      DrgDeleteStrHandle( trans->hstrSelectedRMF );
      DrgDeleteStrHandle( trans->hstrRenderToName );
   }

   trans->hstrSelectedRMF = DrgAddStrHandle( (char*) mechanism);
   trans->hstrRenderToName = DrgAddStrHandle( (char*) container );

   trans->usOperation = operation;
   trans->fsReply = reply;
   BOOL result = (BOOL) DrgSendTransferMsg( trans->pditem->hwndItem, DM_RENDERPREPARE, MPFROMP(trans), 0);
   support = trans->fsReply;

   return result;
}


BOOL XDragItem :: Render( const XWindow * target, const USHORT operation, const USHORT reply, const char * mechanism, const char * container, ULONG& support)
{
   if(trans ==  NULL)
   {
      trans = DrgAllocDragtransfer(1);
      trans->cb = sizeof(trans);
      trans->hwndClient = target->GetHandle();
      trans->pditem = item;
   }
   else
   {
      DrgDeleteStrHandle( trans->hstrSelectedRMF );
      DrgDeleteStrHandle( trans->hstrRenderToName );
   }

   trans->hstrSelectedRMF = DrgAddStrHandle( (char*) mechanism);
   trans->hstrRenderToName = DrgAddStrHandle( (char*) container );
   trans->usOperation = operation;
   trans->fsReply = reply;

   BOOL result = (BOOL) DrgSendTransferMsg( item->hwndItem, DM_RENDER, MPFROMP(trans), 0);
   support = trans->fsReply;
   return result;
}

void XDragItem :: SetControlOps( const USHORT ops)
{
   item->fsControl = ops;
}


USHORT XDragItem :: GetControlOps( ) const
{
   return item->fsControl;
}


void XDragItem :: SetSupportedOps( const USHORT ops)
{
   item->fsSupportedOps = ops;
}


void XDragItem::SetMechanism(const char *m)
{
   item->hstrRMF = DrgAddStrHandle((PSZ) m);
}


void XDragItem::EndConversation(const BOOL success)
{
   LONG t = DMFL_TARGETSUCCESSFUL;

   if (success == FALSE)
      t = DMFL_TARGETFAIL;
   DrgSendTransferMsg(item->hwndItem, DM_ENDCONVERSATION, MPFROMLONG(item->ulItemID), MPFROMLONG(t));
}


void XDragItem::GetTargetName(XString * target) const
{
   target->ReleaseBuffer(DrgQueryStrName(item->hstrTargetName, 512, (PSZ) target->GetBuffer(512)));
}


void XDragItem::GetName(XString * sourceName) const
{
   sourceName->ReleaseBuffer(DrgQueryStrName(item->hstrSourceName, 512, (PSZ) sourceName->GetBuffer(512)));
}


void XDragItem::GetMechanism(XString * mechanism) const
{
   mechanism->ReleaseBuffer(DrgQueryStrName(item->hstrRMF, 512, (PSZ) mechanism->GetBuffer(512)));
}


void XDragItem::GetType(XString * type) const
{
   type->ReleaseBuffer(DrgQueryStrName(item->hstrType, 512, (PSZ) type->GetBuffer(512)));
}


void XDragItem::SetType(const char * type)
{
     item->hstrType = DrgAddStrHandle((PSZ) type);
}


void XDragItem::GetContainerName(XString * containerName) const
{
   containerName->ReleaseBuffer(DrgQueryStrName(item->hstrContainerName, 512, (PSZ) containerName->GetBuffer(512)));
}


void XDragItem::Accept(const BOOL accept)
{
   if (accept)
      DrgSendTransferMsg(item->hwndItem, DM_ENDCONVERSATION, (MPARAM) item->ulItemID, MPFROMLONG(DMFL_TARGETSUCCESSFUL));
   else
      DrgSendTransferMsg(item->hwndItem, DM_ENDCONVERSATION, (MPARAM) item->ulItemID, MPFROMLONG(DMFL_TARGETFAIL));
}


BOOL XDragItem::VerifyItemType(const char *mechanism, const char *format)
{
   return DrgVerifyRMF(item, (PSZ) mechanism, (PSZ) format);
}


void XDragItem::SetContainerName(const char *containerName)
{
   item->hstrContainerName = DrgAddStrHandle((PSZ) containerName);
}


void XDragItem::SetName(const char *name)
{
   item->hstrSourceName = DrgAddStrHandle((PSZ) name);
}


void XDragItem::SetTargetName(const char *target)
{
   item->hstrTargetName = DrgAddStrHandle((PSZ) target);
}


XDragItem :: ~XDragItem()
{
/*
   if(allocated)
      free( item );

   if(trans)
      DrgFreeDragtransfer(trans);
*/
}
