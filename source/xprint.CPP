/* Printer code. Code based on the Maverik class library by Fabrizio Aversa */
/* ported by Stefan von Brauk */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "XCheckBx.h"
#include "XListBox.h"
#include "XEntry.h"
#include "XCntrevn.h"
#include "xfont.h"
#include "XFrmwnd.h"
#include "xmsgbox.h"
#include "XReslib.h"
#include "XRes.h"
#include "XExcept.h"
#include "xcountry.h"
#include "xprint.h"
#include "xgraphob.h"
#include "xmodal.h"
#include "oolres.rh"

/*@
@class XPrinterDevice
@parent XGraphicDevice
@type overview
@remarks
XPrinterDevice gives you the capability to print. It is derived from XGraphicDevice,
you can draw text or graphic objects like on XGraphicDevice.<P>
For printing a dialog is displayed, this dialog is displayed with local language support
(currently only italic, francais, english and german). For applications which use XPrinterDevice it
is nessacary that the OOL-resourcelibrary OOLRES.DLL is in the libpath.<P>
To print follow these sample:
<PRE>
    XPrinterDevice printer(this); //this is a pointer to the owner-window
    XString queue, fileName;

    //setup the printer
    if( printer.SetupPrinter("Print", this, &queue, &fileName) == FALSE)
        return FALSE;

    //open a printer-job
    if( printer.OpenPrinterJob( "Test Job") == FALSE)
    {
        XMessageBox( "error, cannot create printer-job" );
        return FALSE;
    }

    XSize size;
    //query the size of the used sheet
    printer.GetPageSize( &size );

    //create a bitmap to draw
    XPoint rp( 300, 1500);
    XBitmap * bmp = new XBitmap( &rp);
    bmp->Load( "sample.bmp" );
    bmp->Draw(&printer);

    //close the job
    printer.ClosePrinterJob( );
</PRE>
<P>
If you need to draw multiple pages you need to follow this example:
<PRE>
   if( newPage)
   {
      priner.NewPage(); //add a new page
   }
   //continue here with drawing objects
</PRE>
@symbol _
*/
class PrinterDialog:public XModalDialog
{
   friend XPrinterDevice;
   public:
       PrinterDialog(const XFrameWindow *, PRINTERSETUP *, const XResource *);
      ~PrinterDialog();
   private:
      PRINTERSETUP backup;
      PPRINTERSETUP pSetup, pTarget;
      void FillFields();
      BOOL DoCommand(LONG com);
      void DoControl(XControlEvent *);
      XCheckBox *check;
      XEntryField *entry;
      XListBox *list;
};


PrinterDialog :: PrinterDialog(const XFrameWindow * owner, PRINTERSETUP * p1, const XResource * r): XModalDialog( r, owner)
{
   memcpy(&backup, p1, sizeof(PRINTERSETUP));
   pSetup = &backup;
   pTarget = p1;

   check = (XCheckBox *) GetWindow(IDC_TOFILE);
   entry = (XEntryField *) GetWindow(IDC_ENTRY);
   list = (XListBox *) GetWindow(IDC_LISTBOX);

   FillFields();
}


PrinterDialog :: ~PrinterDialog()
{
   delete check;
   delete entry;
   delete list;
}


void PrinterDialog::FillFields()
{
   LONG j, i, selected = 0;
   char * psz;

   for (i = 0; i < pSetup->cQueues; i++)
   {
      psz = (char*) (*pSetup->pQueueInfo[i].pszComment ?
         pSetup->pQueueInfo[i].pszComment : pSetup->pQueueInfo[i].pszName);

      for (j = 0; j < strlen(psz); j++)
         if (psz[j] == 13 || psz[j] == 10)
            psz[j] = ' ';

      list->InsertItem(psz);

      if (0 == strcmp( (char*)pSetup->pQueueInfo[i].pszName, pSetup->szPreferredQueue))
         selected = i;
   }

   if (list->GetCount())
      list->SelectItem(selected);

   if (!pSetup->fToFile)
   {
      check->Select(FALSE);
      entry->Enable(FALSE);
   }
   else
      check->Select();

   if (0 == strlen(pSetup->szFileName))
      strcpy(pSetup->szFileName, "PRINTER.OUT");

   entry->SetText(pSetup->szFileName);
}


BOOL PrinterDialog::DoCommand(LONG command)
{
   CHAR szDeviceName[48];
   CHAR szDriverName[64];
   PPRQINFO3 pqi;

   pqi = &pSetup->pQueueInfo[list->GetSelection()];
   char *pch;

   switch (command)
   {
   case DID_OK:
      strcpy(pSetup->szPreferredQueue, (char*)pqi->pszName);
      if (check->IsSelected())
      {
         pSetup->fToFile = TRUE;
         XString buffer;

         entry->GetText(&buffer);
         strcpy(pSetup->szFileName, (char *) buffer);
      }
      else
      {
         pSetup->fToFile = FALSE;
         *pSetup->szFileName = 0;
      }
      memcpy(pTarget, pSetup, sizeof(PRINTERSETUP));
      break;
   case DID_CANCEL:
      break;
   case IDC_JOBPROP:
      strcpy(szDriverName, (char*)pqi->pszDriverName);
      pch = strchr(szDriverName, '.');
      if (pch)
      {
         strcpy(szDeviceName, pch + 1);
         *pch = 0;
      }
      else
         *szDeviceName = 0;

      pch = strchr( (char*) pqi->pszPrinters, ',');
      if (pch)
         *pch = 0;

      if (DevPostDeviceModes(pSetup->hab, pqi->pDriverData, (PSZ) szDriverName, (PSZ) szDeviceName, pqi->pszPrinters, DPDM_POSTJOBPROP) == DPDM_ERROR)
      {
         XMessageBox msgbox("Impostazione stampante", "DevPostDeviceModes", MB_OK | MB_ERROR | MB_MOVEABLE, this);
      }
      return FALSE;
   }
   return TRUE;
}


void PrinterDialog::DoControl(XControlEvent * event)
{
   switch (event->GetWindowID())
   {
      case IDC_TOFILE:
      if (check->IsSelected())
         entry->Enable();
      else
         entry->Enable(FALSE);
      break;
   }
}


/*@ XPrinterDevice::SetupPrinter(char *title, XFrameWindow * owner, XString * queueName, XString * fileName)
@remarks Let the user select the printer-queue in a dialog. The dialog is loaded from OOLRES.DLL which must be installed.
@parameters
<t 'ø' c=2>
øchar *title            øthe title of the dialog
øXFrameWindow * owner   øowner window. If NULL, the dialog for the printer-setup is not opened
                         and the  queue given in parameter 3 is initialized directly
øXString * queueName      ødefault queue-name (can be null)
øXString * fileName      øbuffer for a fileName if the user wants to print to a file (if NULL no fiflename is stored)
</t>
@returns BOOL success
*/
BOOL XPrinterDevice::SetupPrinter(const char *title, const XFrameWindow * owner, XString * queueName, XString * fileName)
{
   BOOL fOK;
   CHAR szDefaultQueue[196];
   CHAR szSavedQueue[196];
   CHAR szWork[196];
   PCHAR pch;
   PPRQINFO3 pqi;
   SIZEL sizel;
   ULONG cReturned;
   ULONG cTotal;
   ULONG cbNeeded;
   ULONG ul;
   ULONG ulrc;

   // Caller must set these items before calling.
   if (!pSetup->hab || !pSetup->lWorldCoordinates)
      return FALSE;

   // no good unless I can open a PS
   pSetup->pDevOpenData = NULL;

   // Close the info DC's and PS's from any previous call.
   if (pSetup->hpsPrinterInfo)
   {
      GpiAssociate(pSetup->hpsPrinterInfo, (HDC) 0);
      GpiDestroyPS(pSetup->hpsPrinterInfo);
      pSetup->hpsPrinterInfo = (HPS) 0;
   }

   if (pSetup->hdcPrinterInfo)
   {
      DevCloseDC(pSetup->hdcPrinterInfo);
      pSetup->hdcPrinterInfo = (HDC) 0;
   }

   if (pSetup->pQueueInfo)
   {
      // Free the array of PRQINFO3 from previous call.
      free(pSetup->pQueueInfo);
      pSetup->pQueueInfo = NULL;
   }

   // Query how many queues exist on this computer and the
   // number of bytes needed to hold the array.
   ul = SplEnumQueue(NULL, 3, NULL, 0, &cReturned, &cTotal, &cbNeeded, NULL);
   if (cTotal == 0)
   {
      // There are no queues on this computer!
      pSetup->cQueues = 0;
      return FALSE;
   }

   // Allocate memory to store the newly enumerated queue information.
   pSetup->pQueueInfo = (PRQINFO3 *) malloc(cbNeeded);
   if (!pSetup->pQueueInfo)
      return FALSE;

   // Call system again to get the array of PRQINFO3 structures.
   ul = SplEnumQueue(NULL, 3, pSetup->pQueueInfo, cbNeeded, &cReturned, &cTotal, &cbNeeded, NULL);
   if (ul != 0 ||
      cReturned != cTotal)
      return FALSE;
   pSetup->cQueues = cReturned;

   // Establish a default queue -- might need it.
   // Profiled queue name ends with a semicolon.
   ul = PrfQueryProfileString(HINI_PROFILE, (PSZ) "PM_SPOOLER", (PSZ) "QUEUE", NULL, szDefaultQueue, 196);
   if (ul > 1)
   {
      // Trim off semicolon.
      pch = strchr(szDefaultQueue, ';');
      *pch = 0;
   }
   else
   {
      // Hmmmm. Use the first one queue from the enumeration.
      strcpy(szDefaultQueue, (char*) pSetup->pQueueInfo->pszName);
   }
   if (!strlen(szDefaultQueue))
      return FALSE;

   if (0 == strlen(pSetup->szPreferredQueue))
   {
      // No queue preference; use default.
      strcpy(pSetup->szPreferredQueue, szDefaultQueue);

      // Don't expect to see DRIVDATA without queue name.
      // if(! pSetup->pDriverData ) return FALSE;
   }

   if (queueName)
   {
      if (!queueName->IsEmpty())
      {
         pSetup->fToFile = FALSE;
         strcpy(pSetup->szPreferredQueue, (char *) *queueName);
      }
      if (fileName)
      {
         if (!fileName->IsEmpty())
         {
            pSetup->fToFile = TRUE;
            strcpy(pSetup->szFileName, (char *) *fileName);
         }
      }
   }

   pqi = FindQueue(pSetup);
   if (!pqi)
   {
      strcpy(pSetup->szPreferredQueue, szDefaultQueue);
      if (pSetup->pDriverData)
      {
         free(pSetup->pDriverData);
         pSetup->pDriverData = NULL;
      }
   }
   else
   {
      fOK = TRUE;

      if (pSetup->pDriverData)
      {
         fOK = fOK && (pqi->pDriverData->cb == pSetup->pDriverData->cb);
         fOK = fOK && (0 == strcmp(pqi->pDriverData->szDeviceName, pSetup->pDriverData->szDeviceName));
      }

      if (!fOK)
      {
         free(pSetup->pDriverData);
         pSetup->pDriverData = NULL;
      }
   }

   pqi = FindQueue(pSetup);

   if (!pSetup->pDriverData)
   {
      pSetup->pDriverData = (DRIVDATA *) malloc(pqi->pDriverData->cb);
      if (!pSetup->pDriverData)
      {
         ulrc = FALSE;
         return ulrc;
      }
      memcpy(pSetup->pDriverData, pqi->pDriverData, pqi->pDriverData->cb);
   }

   if (!pSetup->pDriverData || pSetup->pDriverData->cb <= 0 || pSetup->pDriverData->cb != pqi->pDriverData->cb || strcmp(pqi->pDriverData->szDeviceName, pSetup->pDriverData->szDeviceName))
      return FALSE;

   memcpy(pqi->pDriverData, pSetup->pDriverData, pSetup->pDriverData->cb);

   strcpy(szSavedQueue, pSetup->szPreferredQueue);

   if (owner)
   {
      XCountryInfo info;
      XResourceLibrary lib( "oolres");
      LONG dlgID;

      switch (info.GetCountry())
      {
      case 39:            // italy
         dlgID = IDD_SELPRINT_ITA;
         break;
      case 2:             // can francais
      case 33:            // france
      case 32:            // belgien
         dlgID = IDD_SELPRINT_FRA;
         break;
      case 49:            // german
         dlgID = IDD_SELPRINT_GER;
         break;
      default:            // english
         dlgID = IDD_SELPRINT_ENG;
      }
      XResource res(dlgID, &lib);

      PrinterDialog *printerDialog = new PrinterDialog(owner, pSetup, &res);
//      printerDialog->SetText((char*) title);
      LONG result = printerDialog->Start();
      if (result == DID_CANCEL)
         return FALSE;
   }
   else
   {
      if (queueName)
      {
         pSetup->fToFile = FALSE;
         strcpy(pSetup->szPreferredQueue, (char *) *queueName);

         if (fileName)
         {
            pSetup->fToFile = TRUE;
            strcpy(pSetup->szFileName, (char *) *fileName);
         }
      }
   }

   *queueName = "";
   *fileName = "";

   pqi = FindQueue(pSetup);
   if (!pqi)
      return FALSE;

   if (0 != strcmp(szSavedQueue, pSetup->szPreferredQueue))
   {
      if (!pSetup->pDriverData)
         return FALSE;
      free(pSetup->pDriverData);

      pSetup->pDriverData = (DRIVDATA *) malloc(pqi->pDriverData->cb);
      if (!pSetup->pDriverData)
      {
         ulrc = FALSE;
         return ulrc;
      }
      pSetup->pDriverData->cb = pqi->pDriverData->cb;
   }

   if (!pSetup->pDriverData || !pSetup->pDriverData->cb == pqi->pDriverData->cb)
      return FALSE;
   memcpy(pSetup->pDriverData, pqi->pDriverData, pqi->pDriverData->cb);

   if (pSetup->fToFile)
   {
      pSetup->lDCType = OD_DIRECT;
      pSetup->devopenstruc.pszLogAddress = (PSZ) pSetup->szFileName;
   }
   else
   {
      pSetup->lDCType = OD_QUEUED;
      pSetup->devopenstruc.pszLogAddress = (PSZ) pSetup->szPreferredQueue;
   }

   strcpy(szWork, (char*) pqi->pszDriverName);
   pch = strchr(szWork, '.');
   if (pch)
      *pch = 0;

   if (pSetup->devopenstruc.pszDriverName)
      free(pSetup->devopenstruc.pszDriverName);

   pSetup->devopenstruc.pszDriverName = (PSZ) malloc(1 + strlen(szWork));
   if (!pSetup->devopenstruc.pszDriverName)
      return FALSE;

   strcpy( (char*) pSetup->devopenstruc.pszDriverName, szWork);

   pSetup->devopenstruc.pdriv = pSetup->pDriverData;
   pSetup->devopenstruc.pszDataType = (PSZ) "PM_Q_STD";

   pSetup->hdcPrinterInfo = DevOpenDC(pSetup->hab, OD_INFO, (PSZ) "*", 4, (PDEVOPENDATA) & pSetup->devopenstruc, (HDC) 0);
   if (!pSetup->hdcPrinterInfo)
      return FALSE;

   sizel.cx = 0;
   sizel.cy = 0;
   pSetup->hpsPrinterInfo = GpiCreatePS(pSetup->hab, pSetup->hdcPrinterInfo, &sizel, pSetup->lWorldCoordinates | GPIA_ASSOC);

   if (GPI_ERROR == pSetup->hpsPrinterInfo)
   {
      DevCloseDC(pSetup->hdcPrinterInfo);
      pSetup->hdcPrinterInfo = (HDC) 0;
      pSetup->hpsPrinterInfo = (HPS) 0;
      return FALSE;
   }

   pSetup->pDevOpenData = (PDEVOPENDATA) & pSetup->devopenstruc;

   *queueName = pSetup->szPreferredQueue;
   if (pSetup->fToFile)
      *fileName = pSetup->szFileName;
   return TRUE;
}


PPRQINFO3 XPrinterDevice::FindQueue(PPRINTERSETUP pSetup)
{
   for (LONG i = 0; i < pSetup->cQueues; i++)
   {
      if (0 == strcmp(pSetup->szPreferredQueue, (char*) pSetup->pQueueInfo[i].pszName))
         return &pSetup->pQueueInfo[i];
   }
   return NULL;
}


void XPrinterDevice::CleanupPrinter()
{
   if (pSetup->hpsPrinterInfo)
   {
      GpiAssociate(pSetup->hpsPrinterInfo, (HDC) 0);
      GpiDestroyPS(pSetup->hpsPrinterInfo);
      pSetup->hpsPrinterInfo = (HPS) 0;
   }

   if (pSetup->hdcPrinterInfo)
   {
      DevCloseDC(pSetup->hdcPrinterInfo);
      pSetup->hdcPrinterInfo = (HDC) 0;
   }

   if (pSetup->pQueueInfo)
   {
      free(pSetup->pQueueInfo);
      pSetup->pQueueInfo = NULL;
   }

   if (pSetup->pDriverData)
   {
      free(pSetup->pDriverData);
      pSetup->pDriverData = NULL;
   }

   if (pSetup->devopenstruc.pszDriverName)
   {
      free(pSetup->devopenstruc.pszDriverName);
      pSetup->devopenstruc.pszDriverName = NULL;
   }
}


/*@ XPrinterDevice :: XPrinterDevice(XFrameWindow * w, LONG m)
@remarks Create a printer-device
@parameters
<t 'ø' c=2>
øXFrameWindow * øowner-window
øLONG øresolution, see XGraphicDevice::XGraphicDevice() for details
</t>
*/
XPrinterDevice :: XPrinterDevice(const XFrameWindow * w, const LONG m):XGraphicDevice(m)
{
   pSetup = (PRINTERSETUP *) calloc(sizeof(PRINTERSETUP), 1);

   pSetup->lWorldCoordinates = m;
   pSetup->hab = XApplication::GetApplication()->GetAnchorBlock();

   width = 0;
   height = 0;
}


XPrinterDevice :: ~XPrinterDevice()
{
   free(pSetup);
}


/*@ XPrinterDevice::OpenPrinterJob(char *pszJobTitle)
@remarks Open a printer job
@parameters char * title      title of the printer job
@returns BOOL success
*/
BOOL XPrinterDevice::OpenPrinterJob(const char *pszJobTitle)
{
   hdc = DevOpenDC(pSetup->hab, pSetup->lDCType, (PSZ) "*", 9, pSetup->pDevOpenData, (HDC) 0);

   if (hdc == NULLHANDLE)
      return FALSE;

   SIZEL sizel;

   sizel.cx = 0;
   sizel.cy = 0;
   hps = GpiCreatePS(pSetup->hab, hdc, &sizel, pSetup->lWorldCoordinates | GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC);

   if (GPI_ERROR == hps)
   {
      DevCloseDC(hdc);
      hdc = (HDC) 0;
      hps = (HPS) 0;
      return FALSE;
   }
   DevEscape(hdc, DEVESC_STARTDOC, strlen(pszJobTitle), (PSZ) pszJobTitle, NULL, NULL);
   if (!GpiSetCharMode(hps, CM_MODE2))
      return FALSE;
   if (!GpiSetTextAlignment(hps, TA_NORMAL_HORIZ, TA_NORMAL_VERT))
      return FALSE;

   SIZEL sizPage;

   if (!GpiQueryPS(hps, &sizPage))
      return FALSE;
   width = sizPage.cx;
   height = sizPage.cy;

   return TRUE;
}


/*@ XPrinterDevice::ClosePrinterJob()
@remarks Close the job so it can be printed.
*/
void XPrinterDevice::ClosePrinterJob()
{
   DevEscape(hdc, DEVESC_ENDDOC, 0, NULL, NULL, NULL);

   if (hps)
   {
      GpiSetCharSet(hps, 0);
      GpiAssociate(hps, (HDC) 0);
      GpiDestroyPS(hps);
      hps = (HPS) 0;
   }

   if (hdc)
   {
      DevCloseDC(hdc);
      hdc = (HDC) 0;
   }
}


/*@ XPrinterDevice::KillPrinterJob()
@remarks Remove the printer job from the printer queue.
*/
void XPrinterDevice::KillPrinterJob()
{
   DevEscape(hdc, DEVESC_ABORTDOC, 0, NULL, NULL, NULL);

   if (hps)
   {
      GpiSetCharSet(hps, 0);
      GpiAssociate(hps, (HDC) 0);
      GpiDestroyPS(hps);
      hps = (HPS) 0;
   }

   if (hdc)
   {
      DevCloseDC(hdc);
      hdc = (HDC) 0;
   }
}


/*@ XPrinterDevice::NewPage()
@remarks Add a new page to the printer-job.
@returns BOOL success
*/
BOOL XPrinterDevice::NewPage() const
{
   if (hdc)
   {
      if (DevEscape(hdc, DEVESC_NEWFRAME, 0, NULL, NULL, NULL) == DEV_OK)
         return TRUE;
   }
   return FALSE;
}
