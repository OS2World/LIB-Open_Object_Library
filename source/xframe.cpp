#include "XFrmWnd.h"
#include "XFrame.h"
#include "XFrmCnt.h"
#include "XScrlBar.h"
#include "XRect.h"

#include <stdlib.h>


XWindow *XFrame::QueryWindow(const ULONG id) const
{
	if (id == WIN_PARENT)
		return frame;
	else
		return XWindow :: QueryWindow(id);
}


XFrame :: ~XFrame()
{
	SHORT i;

	for (i = 0; i < controls; i++)
		delete cont[i];
}


XFrame :: XFrame(const XFrameWindow * f):XControl(f->frame)
{
	cont = NULL;
	controls = 0;
	adds = 0;
	frame = (XFrameWindow *) f;
}


XFrame :: XFrame(const HWND hwnd):XControl(hwnd)
{
	cont = NULL;
	controls = 0;
	adds = 0;
	frame = NULL;
}


void XFrame::AddFrameControl(XFrameControl * f)
{
	controls += 1;
	cont = (XFrameControl **) realloc(cont, controls * sizeof(void *));

	if (f->style & TB_RIGHT || f->style & TB_LEFT)
		cont[controls - 1] = f;
	else
	{
		SHORT insert = controls - 1;

		for (int i = controls - 2; i > 0; i--)
		{
			BOOL swap = FALSE;

			if (cont[i]->style & TB_RIGHT || cont[i]->style & TB_LEFT)
			{
				insert = i;
				swap = TRUE;
			}
			if (swap)
				cont[i + 1] = cont[i];
		}
		cont[insert] = f;
	}
	if (frame->dlgHandle)
	{
		XRect rec1;

		frame->GetSize(&rec1);
		rec1.SetHeight(rec1.GetHeight() + f->cy);
		frame->SetSize(&rec1);
	}
}


void XFrame::RemoveFrameControl(XFrameControl * f)
{
	BOOL swap = FALSE, found = FALSE;

	for (int i = 0; i < controls; i++)
	{
		if (cont[i] == f)
		{
			if (i < controls)
				swap = TRUE;
			found = TRUE;
		}
		if (swap)
			cont[i] = cont[i + 1];
	}
	if (found)
	{
		controls -= 1;
		cont = (XFrameControl **) realloc(cont, controls * sizeof(void *));
	}
}


SHORT sly = 0;
SHORT slx = 0;


MRESULT XFrame::HandleMessage(ULONG msg, MPARAM mp1, MPARAM mp2)
{
	SHORT countSwp;

	switch (msg)
	{
      case WM_CLOSE:
         return 0;
      case WM_SYSCOMMAND:
         {
     		   if(frame)
            {
               if(frame->DoSysCommand(SHORT1FROMMP(mp1)) == TRUE)
                  return oldfunc(winhandle, msg, mp1, mp2);
            }
         }
   		return 0;
	case WM_QUERYFRAMECTLCOUNT:
		return MRFROMSHORT((ULONG) oldfunc(winhandle, msg, mp1, mp2) + controls + adds);
	case WM_FORMATFRAME:
		{
			SHORT i = 0;
			PSWP pswp, sav;

			if (slx == 0)
			{
				sly = WinQuerySysValue(HWND_DESKTOP, SV_CYHSCROLL);
				slx = WinQuerySysValue(HWND_DESKTOP, SV_CXVSCROLL);
			}

			countSwp = (int) oldfunc(winhandle, msg, mp1, mp2);

			pswp = (PSWP) mp1;
			sav = &pswp[countSwp - 1];

			for (i = 0; i < controls; i++)
				cont[i]->AdjustSize(&pswp[countSwp + i], sav);

			if (frame)
			{
				if (frame->vert)
				{
					sav->cx -= slx;
					SHORT y = sav->y;

					if (frame->horz)
						y += sly;
					pswp[countSwp + i].x = sav->cx + sav->x;
					pswp[countSwp + i].cx = slx;
					pswp[countSwp + i].y = y;
					pswp[countSwp + i].cy = sav->y + sav->cy - y;
					pswp[countSwp + i].hwnd = frame->vert->winhandle;
					pswp[countSwp + i].fl = SWP_SIZE | SWP_MOVE;
					i += 1;
				}

				if (frame->horz)
				{
					pswp[countSwp + i].x = sav->x;
					pswp[countSwp + i].cx = sav->cx;
					pswp[countSwp + i].y = sav->y;
					if (sav->cy > sly)
						pswp[countSwp + i].cy = sly;
					else
						pswp[countSwp + i].cy = sav->cy;
					pswp[countSwp + i].hwnd = frame->horz->winhandle;
					pswp[countSwp + i].fl = SWP_SIZE | SWP_MOVE;
					sav->y += pswp[countSwp + i].cy;
					sav->cy -= pswp[countSwp + i].cy;
					i++;
				}
				if (frame->dummy)
				{
					pswp[countSwp + i].x = sav->cx + sav->x;
					pswp[countSwp + i].cx = slx;
					pswp[countSwp + i].y = pswp[countSwp + i - 1].y;
					pswp[countSwp + i].cy = pswp[countSwp + i - 1].cy;
					pswp[countSwp + i].hwnd = frame->dummy;
					pswp[countSwp + i].fl = SWP_SIZE | SWP_MOVE;
					i++;
				}
				if (frame->clientWindow)
				{
					pswp[countSwp + i].x = sav->x;
					pswp[countSwp + i].cx = sav->cx;
					pswp[countSwp + i].y = sav->y;
					pswp[countSwp + i].cy = sav->cy;
					pswp[countSwp + i].hwnd = frame->clientWindow->winhandle;
					pswp[countSwp + i].fl = SWP_SIZE | SWP_MOVE;
				}
			}
		}
		return MRFROMSHORT(countSwp + controls + adds);
	default:
		return (MRESULT) FALSE;
	}
	return (MRESULT) FALSE;
}
