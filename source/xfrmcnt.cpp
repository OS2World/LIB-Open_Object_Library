#include "XFrmCnt.h"
#include "XFrmwnd.h"
#include "XFrame.h"

#include "string.h"


XFrameControl :: XFrameControl(const XFrameWindow * w, const LONG st, const ULONG id):XUserWindow(w->frame)
{
	style = st;
	w->frameWin->AddFrameControl(this);
	WinSetWindowUShort(winhandle, QWS_ID, id);
}


void XFrameControl::AdjustSize(PSWP pswpToolbar, PSWP pswpClient)
{
///   if( pswpToolbar->fl & SWP_HIDE)
	if (!(IsVisible()))
	{
		memset(pswpToolbar, 0, sizeof(SWP));
		pswpToolbar->hwnd = winhandle;
		pswpToolbar->fl = SWP_HIDE;
		return;
	}

	pswpToolbar->x = pswpClient->x;

	if (style & TB_TOP)
	{
		pswpToolbar->cx = pswpClient->cx;
		if (pswpClient->cy > cy)
		{
			pswpClient->cy -= cy;
			pswpToolbar->cy = cy;
		}
		else
		{
			pswpToolbar->cy = pswpClient->cy;
			pswpClient->cy -= pswpToolbar->cy;
		}
		pswpToolbar->y = (pswpClient->y + pswpClient->cy);
	}
	else if (style & TB_BOTTOM)
	{
		pswpToolbar->cx = pswpClient->cx;
		pswpToolbar->y = pswpClient->y;
		if (pswpClient->cy > cy)
			pswpToolbar->cy = cy;
		else
			pswpToolbar->cy = pswpClient->cy;
		pswpClient->cy -= pswpToolbar->cy;
		pswpClient->y += pswpToolbar->cy;
	}
	else if (style & TB_LEFT)
	{
		pswpToolbar->y = pswpClient->y;
		if (pswpClient->cx > cx)
			pswpToolbar->cx = cx;
		else
			pswpToolbar->cx = pswpClient->cx;
		pswpToolbar->cy = pswpClient->cy;
		pswpClient->cx -= pswpToolbar->cx;
		pswpClient->x += pswpToolbar->cx;
	}
	else if (style & TB_RIGHT)
	{
		pswpToolbar->y = pswpClient->y;
		if (pswpClient->cx > cx)
			pswpToolbar->cx = cx;
		else
			pswpToolbar->cx = pswpClient->cx;
		pswpToolbar->x = pswpClient->x + pswpClient->cx - pswpToolbar->cx;
		pswpToolbar->cy = pswpClient->cy;
		pswpClient->cx -= pswpToolbar->cx;
	}

	pswpToolbar->hwnd = winhandle;

	pswpToolbar->fl = SWP_SIZE | SWP_MOVE | SWP_SHOW;
	Invalidate(TRUE);
}
