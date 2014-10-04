#define  INCL_ERRORS
#define  INCL_DOS
#define  INCL_WIN
#define  INCL_GPIBITMAPS

#include <os2.h>
#include "oolglib.h"
#include "oolgltk.h"
#include "oolgres.rh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/******************************************************************************/
USHORT GetLanguageBase () {
   COUNTRYINFO  countryInfo;
   COUNTRYCODE  countryCode = {0};
   ULONG        infoLen = 0;

   DosQueryCtryInfo (sizeof (countryInfo), &countryCode, 
      &countryInfo, &infoLen);
   switch (countryInfo.country) {
   case 49:
      return IDR_GERMAN_BASE;
   default:
      return IDR_ENGLISH_BASE;
   }
}

/******************************************************************************/
MRESULT XGLibDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   switch (msg) {
   case WM_COMMAND:
      if (NULL != param) 
         WinSendMsg (param->hwndDialog, msg, mp1, mp2);
      break;
   }
   return WinDefDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT FilterSetupWinProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   HWND hwndPage;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      break;
   case WM_COMMAND:
      hwndPage = (HWND) WinSendDlgItemMsg (hwnd, IDC_NOTEBOOK, BKM_QUERYPAGEWINDOWHWND, 
         MPFROMLONG (param->pageID [0]), 0L);
      WinSendMsg (hwndPage, WM_USER | msg, mp1, mp2);

      if (0 != param->pageID [1]) {
         hwndPage = (HWND) WinSendDlgItemMsg (hwnd, IDC_NOTEBOOK, BKM_QUERYPAGEWINDOWHWND, 
            MPFROMLONG (param->pageID [1]), 0L);
         WinSendMsg (hwndPage, WM_USER | msg, mp1, mp2);
      }

      break;
   }
   return WinDefDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
LONG GetMaxColorCount (UCHAR cBitCount, UCHAR cPalette, UCHAR cDithering) {
   switch (cBitCount) {
   case BFP_BPP_MONOCHROME:   return 2;
   case BFP_BPP_EGA:          return BFP_PALETTE_4BIT_8_COLORS == cPalette ? 8 : 16;
   case BFP_BPP_VGA:          return (BFP_DITHERING_OPTIMIZED | 
                                 BFP_DITHERING_MEDIANCUT) & cDithering ? 256 :
                                 BFP_PALETTE_8BIT_666 == cPalette ? 6*6*6 : 
                                 BFP_PALETTE_8BIT_784 == cPalette ? 7*8*4 : 256;
   case BFP_BPP_HIGHCOLOR:    return BFP_PALETTE_16BIT_555 == cPalette ? 32*32*32 : 32*64*32;
   case BFP_BPP_TRUECOLOR:    return 256 * 256 * 256;  
   case BFP_BPP_DIRECTCOLOR:  return 256 * 256 * 256;
   }
}

/******************************************************************************/
UCHAR GetBitCount (UCHAR bppID) {
   switch (bppID) {
   case 0: return BFP_BPP_MONOCHROME;
   case 1: return BFP_BPP_EGA;
   case 2: return BFP_BPP_VGA;
   case 3: return BFP_BPP_HIGHCOLOR;
   case 4: return BFP_BPP_TRUECOLOR;  
   case 5: return BFP_BPP_DIRECTCOLOR;
   }
}

/******************************************************************************/
void SetSpinButton (HWND hwnd, USHORT resID, LONG curr, LONG min, LONG max) {
   WinSendDlgItemMsg (hwnd, resID, SPBM_SETLIMITS, 
      MPFROMLONG (max), MPFROMLONG (min));
   WinSendDlgItemMsg (hwnd, resID, SPBM_SETCURRENTVALUE, MPFROMLONG (curr), 0);
}

/******************************************************************************/
void SelectColor (XBitmapFilterPageParameter* param, HWND hwndDlg) {
   UCHAR  cBitCount     = GetBitCount (param->bppID);
   USHORT paletteItem   = param->paletteID [param->bppID];
   USHORT ditheringItem = param->ditheringID [param->bppID];
   LONG   colorValue    = param->entryCount [param->bppID];

   ULONG  paletteMask   = GetPaletteMask (param->fImport ? -1 : 
      param->fltParam->cFilterID, cBitCount, ditheringItem);
   ULONG ditheringMask  = GetDitheringMask (param->fImport ? -1 : 
      param->fltParam->cFilterID, cBitCount);

   char  entryName [64];
   char  entryCount = 0;
   LONG  colorCount;
   int   n;

   HWND hwndCtrl = WinWindowFromID (hwndDlg, IDC_PALETTE);
   WinSendMsg (hwndCtrl, LM_DELETEALL, 0, 0);
   for (n = 0; n < BFP_PALETTE_COUNT; n++) if ((2 << n) & paletteMask) {
      WinLoadString (param->fltParam->hab, param->hmod, 
         param->profile->languageBase + IDR_PALETTE_GREYSCALE + n, 64, entryName);
      WinSendMsg (hwndCtrl, LM_INSERTITEM, MPFROMLONG (LIT_END), MPFROMP (entryName));
      WinSendMsg (hwndCtrl, LM_SETITEMHANDLE, MPFROMLONG (entryCount), MPFROMLONG (2 << n));
      if ((2 << n) == paletteItem)
	     WinSendMsg (hwndCtrl, LM_SELECTITEM, MPFROMSHORT (entryCount), MPFROMSHORT (TRUE));
      entryCount++;
   }
   if (0 == (paletteMask & paletteItem))
      WinSendMsg (hwndCtrl, LM_SELECTITEM, MPFROMSHORT (0), MPFROMSHORT (TRUE));

   for (n = 0; n < 6; n++) {
      WinEnableControl (hwndDlg, IDC_DITHERING_NEAREST + n, (1 << n) & ditheringMask);
      if ((1 << n) == ditheringItem)
      WinCheckButton (hwndDlg, IDC_DITHERING_NEAREST + n, TRUE);
   }

   SetSpinButton (hwndDlg, IDC_PALETTE_ENTRIES, colorValue, 
      2, GetMaxColorCount (cBitCount, paletteItem, ditheringItem));

   for (n = IDC_PRECISION_CAPTION; n <= IDC_PRECISION_BYTES; n++)
      WinEnableControl (hwndDlg, n, BFP_DITHERING_OPTIMIZED == ditheringItem);
}

/******************************************************************************/
void SetChangeFlag (HWND hwnd, BOOL change, UCHAR bpps) {
   change = TRUE;
   WinEnableControl (hwnd, IDC_BPP, change);
   WinEnableControl (hwnd, IDC_BPP_MONOCHROME,  change && (bpps & BFF_PROCESS_1BPP));
   WinEnableControl (hwnd, IDC_BPP_EGA,         change && (bpps & BFF_PROCESS_4BPP));
   WinEnableControl (hwnd, IDC_BPP_VGA,         change && (bpps & BFF_PROCESS_8BPP));
   WinEnableControl (hwnd, IDC_BPP_HIGHCOLOR,   change && (bpps & BFF_PROCESS_16BPP));
   WinEnableControl (hwnd, IDC_BPP_TRUECOLOR,   change && (bpps & BFF_PROCESS_24BPP));
   WinEnableControl (hwnd, IDC_BPP_DIRECTCOLOR, change && (bpps & BFF_PROCESS_32BPP));
}

/******************************************************************************/
MRESULT ColorSetupWinProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   HWND           hwndCtrl;
   MRESULT        rc;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;

/* Getting color palette */
      param->paletteID [0] = BFP_PALETTE_1BIT_2_COLORS;
      param->paletteID [1] = BFP_PALETTE_4BIT_16_COLORS;
      param->paletteID [2] = BFP_PALETTE_8BIT_666;
      param->paletteID [3] = BFP_PALETTE_16BIT_555;
      param->paletteID [4] = BFP_PALETTE_TRUECOLORS;
      param->paletteID [5] = BFP_PALETTE_TRUECOLORS;

/* Getting color palette */
      param->ditheringID [0] = /*BFP_DITHERING_ERRORDIFFUSION;*/
      param->ditheringID [1] = /*BFP_DITHERING_ERRORDIFFUSION;*/
      param->ditheringID [2] = /*BFP_DITHERING_OPTIMIZED;*/
      param->ditheringID [3] = /*BFP_DITHERING_ERRORDIFFUSION;*/
      param->ditheringID [4] = /*BFP_DITHERING_NONE;*/
      param->ditheringID [5] = BFP_DITHERING_NONE;
      param->entryCount [0] = GetMaxColorCount (BFP_BPP_MONOCHROME,  param->paletteID [0], param->ditheringID [0]);
      param->entryCount [1] = GetMaxColorCount (BFP_BPP_EGA,         param->paletteID [1], param->ditheringID [1]);
      param->entryCount [2] = GetMaxColorCount (BFP_BPP_VGA,         param->paletteID [2], param->ditheringID [2]);
      param->entryCount [3] = GetMaxColorCount (BFP_BPP_HIGHCOLOR,   param->paletteID [3], param->ditheringID [3]);
      param->entryCount [4] = GetMaxColorCount (BFP_BPP_TRUECOLOR,   param->paletteID [4], param->ditheringID [4]);
      param->entryCount [5] = GetMaxColorCount (BFP_BPP_DIRECTCOLOR, param->paletteID [5], param->ditheringID [5]);

/* Getting color depth */
      if (BFP_BPP_DONT_TOUCH == param->fltParam->cBitCount)
         param->fltParam->cBitCount = AdjustColorBits 
            (8, param->profile->writingModes [param->fltParam->cFilterID]);

      if (param->fltParam->cBitCount > 24) param->bppID = 5;
      else if (param->fltParam->cBitCount > 16) param->bppID = 4;
      else if (param->fltParam->cBitCount >  8) param->bppID = 3;
      else if (param->fltParam->cBitCount >  4) param->bppID = 2;
      else if (param->fltParam->cBitCount >  2) param->bppID = 1;
      else param->bppID = 0;

      if (BFP_PALETTE_DEFAULT != param->fltParam->cPalette)
         param->paletteID [param->bppID] = param->fltParam->cPalette;
      param->ditheringID [param->bppID] = param->fltParam->cDithering;

      param->modifyColors = BFP_BPP_DONT_TOUCH != param->fltParam->cBitCount;
      WinCheckButton (hwnd, IDC_BPP_CHANGE, param->modifyColors);
      SetChangeFlag (hwnd, param->modifyColors, param->fImport ?
         BFF_PROCESS_ALL : param->profile->writingModes 
         [param->fltParam->cFilterID]);

      WinCheckButton (hwnd, IDC_BPP_MONOCHROME + param->bppID, TRUE);
      SelectColor (param, hwnd);

      SetSpinButton (hwnd, IDC_PRECISION_RED,   8, 0, 8);
      SetSpinButton (hwnd, IDC_PRECISION_GREEN, 8, 0, 8);
      SetSpinButton (hwnd, IDC_PRECISION_BLUE,  8, 0, 8);
      break;
   case WM_CONTROL:
      hwndCtrl = (HWND) mp2;
      switch (SHORT1FROMMP (mp1)) {
/*      case IDC_BPP_CHANGE:*/
/*         param->modifyColors = WinQueryButtonCheckstate (hwnd, IDC_BPP_CHANGE);*/
/*         SetChangeFlag (hwnd, param->modifyColors, param->bppMask);*/
/*         break;*/
      case IDC_PALETTE:
         param->paletteID [param->bppID] = SHORT1FROMMR 
            (WinSendMsg (hwndCtrl, LM_QUERYITEMHANDLE, MPFROMSHORT (SHORT1FROMMR 
            (WinSendMsg (hwndCtrl, LM_QUERYSELECTION, 0, 0))), 0));
         break;
      case IDC_BPP_MONOCHROME:
         param->bppID = 0;
         SelectColor (param, hwnd);
         break;
      case IDC_BPP_EGA:
         param->bppID = 1;
         SelectColor (param, hwnd);
         break;
      case IDC_BPP_VGA:
         param->bppID = 2;
         SelectColor (param, hwnd);
         break;
      case IDC_BPP_HIGHCOLOR:
         param->bppID = 3;
         SelectColor (param, hwnd);
         break;
      case IDC_BPP_TRUECOLOR:
         param->bppID = 4;
         SelectColor (param, hwnd);
         break;
      case IDC_BPP_DIRECTCOLOR:
         param->bppID = 5;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_NEAREST:
         param->ditheringID [param->bppID] = BFP_DITHERING_NONE;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_ERRORDIFFUSION:
         param->ditheringID [param->bppID] = BFP_DITHERING_ERRORDIFFUSION;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_SMALL_HALFTONE:
         param->ditheringID [param->bppID] = BFP_DITHERING_SMALL_HALFTONE;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_LARGE_HALFTONE:
         param->ditheringID [param->bppID] = BFP_DITHERING_LARGE_HALFTONE;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_OPTIMIZED:
         param->ditheringID [param->bppID] = BFP_DITHERING_OPTIMIZED;
         SelectColor (param, hwnd);
         break;
      case IDC_DITHERING_MEDIANCUT:
         param->ditheringID [param->bppID] = BFP_DITHERING_MEDIANCUT;
         SelectColor (param, hwnd);
         break;
      case IDC_PALETTE_ENTRIES:
         if ((BFP_DITHERING_OPTIMIZED | BFP_DITHERING_MEDIANCUT) & 
            param->ditheringID [param->bppID]) {
            rc = WinDefDlgProc (hwnd, msg, mp1, mp2);
            WinSendMsg (hwndCtrl, SPBM_QUERYVALUE, MPFROMP (param->entryCount + param->bppID), 0);
            return rc;
         }
         break;
      }

      WinEnableControl (hwnd, IDC_PALETTE, 
         !((BFP_DITHERING_OPTIMIZED | BFP_DITHERING_MEDIANCUT) & 
         param->ditheringID [param->bppID]) &&
         SHORT1FROMMR (WinSendDlgItemMsg (hwnd, IDC_PALETTE, LM_QUERYITEMCOUNT, 0, 0)) > 1);
      WinEnableControl (hwnd, IDC_PALETTE_ENTRIES, 
         (BFP_DITHERING_OPTIMIZED | BFP_DITHERING_MEDIANCUT) & 
         param->ditheringID [param->bppID]);

      if ((BFP_DITHERING_OPTIMIZED | BFP_DITHERING_MEDIANCUT) & 
         param->ditheringID [param->bppID])
         SetSpinButton (hwnd, IDC_PALETTE_ENTRIES, param->entryCount [param->bppID], 2,
            GetMaxColorCount (GetBitCount (param->bppID), param->paletteID [param->bppID], param->ditheringID [param->bppID]));
       else
         SetSpinButton (hwnd, IDC_PALETTE_ENTRIES,
            GetMaxColorCount (GetBitCount (param->bppID), param->paletteID [param->bppID], param->ditheringID [param->bppID]), 2,
            GetMaxColorCount (GetBitCount (param->bppID), param->paletteID [param->bppID], param->ditheringID [param->bppID]));
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) 
         if WinQueryButtonCheckstate (hwnd, IDC_BPP_CHANGE) {
            param->fltParam->cBitCount  = GetBitCount (param->bppID);
            param->fltParam->cPalette   = param->paletteID   [param->bppID];
            param->fltParam->cDithering = param->ditheringID [param->bppID];
            if (BFP_DITHERING_OPTIMIZED == param->ditheringID [param->bppID]) {
               LONG tmpVal;
               param->fltParam->cClrEntries = param->entryCount  [param->bppID];
               WinSendDlgItemMsg (hwnd, IDC_PRECISION_RED, SPBM_QUERYVALUE, MPFROMP (&tmpVal), 0);
               param->fltParam->cRedBits    = 0xff00 >> tmpVal;
               WinSendDlgItemMsg (hwnd, IDC_PRECISION_GREEN, SPBM_QUERYVALUE, MPFROMP (&tmpVal), 0);
               param->fltParam->cGreenBits  = 0xff00 >> tmpVal;
               WinSendDlgItemMsg (hwnd, IDC_PRECISION_RED, SPBM_QUERYVALUE, MPFROMP (&tmpVal), 0);
               param->fltParam->cBlueBits   = 0xff00 >> tmpVal;
            }
            if (BFP_DITHERING_MEDIANCUT == param->ditheringID [param->bppID])
               param->fltParam->cClrEntries  = param->entryCount  [param->bppID];
         } else {
            param->fltParam->cBitCount  = BFP_BPP_DONT_TOUCH;
            param->fltParam->cPalette   = BFP_PALETTE_DEFAULT;
            param->fltParam->cDithering = BFP_DITHERING_NONE;
         }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT BitmapImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XBitmapImportFilter* fltParam = NULL != param ? (XBitmapImportFilter*) param->fltParam : NULL;
   int n;

   switch (msg) {
   case WM_PAINT:
      for (n = IDC_BITMAP_PALETTE; n <= IDC_BITMAP_INVERSE_BITMAP; n++)
         WinEnableControl (hwnd, n, BFP_BPP_MONOCHROME == GetBitCount (param->bppID));
      break;
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XBitmapImportFilter*) param->fltParam;

      SetSpinButton (hwnd, IDC_BITMAP_INDEX, fltParam->uIndex, 0, 65535);
      WinCheckButton (hwnd, IDC_BITMAP_INVERSE_NOTHING + 
         fltParam->cInversionMode, TRUE);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
/* Bitmapindex abfragen */
         WinSendDlgItemMsg (hwnd, IDC_BITMAP_INDEX, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uIndex), 0);

/* ggf. Palettentransformation abfragen */
         if (BFP_BPP_MONOCHROME == GetBitCount (param->bppID))
            for (n = IDC_BITMAP_INVERSE_NOTHING; n <= IDC_BITMAP_INVERSE_BITMAP; n++)
               if (WinQueryButtonCheckstate (hwnd, n))
                  fltParam->cInversionMode = n - IDC_BITMAP_INVERSE_NOTHING;
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT BitmapExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XBitmapExportFilter* fltParam = NULL != param ? (XBitmapExportFilter*) param->fltParam : NULL;
   int n;

   switch (msg) {
   case WM_PAINT:
      for (n = IDC_BITMAP_PALETTE; n <= IDC_BITMAP_FORCE_LIGHTFG; n++)
         WinEnableControl (hwnd, n, BFP_BPP_MONOCHROME == GetBitCount (param->bppID));
      break;
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XBitmapExportFilter*) param->fltParam;

      switch (fltParam->cVersion) {
      case BFP_BITMAP_OS2_11: 
         WinCheckButton (hwnd, IDC_BITMAP_OS2_11, TRUE); break;
      case BFP_BITMAP_OS2_20: 
         WinCheckButton (hwnd, IDC_BITMAP_OS2_20, TRUE); break;
      }

      WinCheckButton (hwnd, IDC_BITMAP_INVERSE_NOTHING + 
         fltParam->cInversionMode, TRUE);

      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
      /* Bitmapversion abfragen */
         if (TRUE == WinQueryButtonCheckstate (hwnd, IDC_BITMAP_OS2_11))
            fltParam->cVersion = BFP_BITMAP_OS2_11;
         else if (TRUE == WinQueryButtonCheckstate (hwnd, IDC_BITMAP_OS2_20))
            fltParam->cVersion = BFP_BITMAP_OS2_20;

      /* ggf. Palettentransformation abfragen */
         if (BFP_BPP_MONOCHROME == GetBitCount (param->bppID))
            for (n = IDC_BITMAP_INVERSE_NOTHING; n <= IDC_BITMAP_FORCE_LIGHTFG; n++)
               if (TRUE == WinQueryButtonCheckstate (hwnd, n))
                  fltParam->cInversionMode =
                     n - IDC_BITMAP_INVERSE_NOTHING;
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT GEMExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XGEMExportFilter* fltParam = NULL != param ? (XGEMExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XGEMExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_GEM_GREYSCALE, fltParam->fGreyscaled);
      WinCheckButton (hwnd, IDC_GEM_XIMG, fltParam->fWritePalette);
      SetSpinButton (hwnd, IDC_GEM_PIXEL_WIDTH, fltParam->uPixelWidth, 0, 255);
      SetSpinButton (hwnd, IDC_GEM_PIXEL_HEIGHT, fltParam->uPixelHeight, 0, 255);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
         fltParam->fGreyscaled = 
            WinQueryButtonCheckstate (hwnd, IDC_GEM_GREYSCALE);
         fltParam->fWritePalette = 
            WinQueryButtonCheckstate (hwnd, IDC_GEM_XIMG);

         WinSendDlgItemMsg (hwnd, IDC_GEM_PIXEL_WIDTH, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uPixelWidth), 0);
         WinSendDlgItemMsg (hwnd, IDC_GEM_PIXEL_HEIGHT, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uPixelHeight), 0);
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT GIFImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XGIFImportFilter* fltParam = NULL != param ? (XGIFImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XGIFImportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_GIF_IGNORE_ERRORS, fltParam->fIgnoreErrors);
      SetSpinButton (hwnd, IDC_GIF_INDEX, fltParam->uIndex, 0, 65535);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
         fltParam->fIgnoreErrors = 
            WinQueryButtonCheckstate (hwnd, IDC_GIF_IGNORE_ERRORS);
         WinSendDlgItemMsg (hwnd, IDC_GIF_INDEX, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uIndex), 0);
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT GIFExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XGIFExportFilter* fltParam = NULL != param ? (XGIFExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_PAINT:
      if (NULL != param) {
         LONG tColor;
         UCHAR max = (1 << GetBitCount (param->bppID)) - 1;

         WinSendDlgItemMsg (hwnd, IDC_GIF_TRANSINDEX, SPBM_QUERYVALUE, MPFROMP (&tColor), 0);
         if (tColor > max) max = 0;
         SetSpinButton (hwnd, IDC_GIF_TRANSINDEX, tColor, 0, max);
         WinSendDlgItemMsg (hwnd, IDC_GIF_BKGNDINDEX, SPBM_QUERYVALUE, MPFROMP (&tColor), 0);
         if (tColor > max) max = 0;
         SetSpinButton (hwnd, IDC_GIF_BKGNDINDEX, tColor, 0, max);
      }
      break;
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XGIFExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_GIF_INTERLACED, fltParam->fInterlaced);

      WinCheckButton (hwnd, IDC_GIF_TRANS_CAPTION, 
         BFP_GIF_TRANSCOLOR_NONE != fltParam->cTransColor);
      SetSpinButton (hwnd, IDC_GIF_TRANSINDEX, fltParam->cTransColor >= 0 ?
         fltParam->cTransColor : 0, 0, (1 << GetBitCount (param->bppID)) - 1);
      WinCheckButton (hwnd, IDC_GIF_TRANSAUTO, 
         BFP_GIF_TRANSCOLOR_NONE != fltParam->cTransColor ?
         BFP_GIF_TRANSCOLOR_EDGE == fltParam->cTransColor : TRUE);

      SetSpinButton (hwnd, IDC_GIF_BKGNDINDEX, fltParam->cBackColor, 0, 
         (1 << GetBitCount (param->bppID)) - 1);
      SetSpinButton (hwnd, IDC_GIF_POS_X, fltParam->ulPosX, 0, 65535);
      SetSpinButton (hwnd, IDC_GIF_POS_Y, fltParam->ulPosY, 0, 65535);
      SetSpinButton (hwnd, IDC_GIF_SCREEN_WIDTH, fltParam->ulScreenWidth, 0, 65535);
      SetSpinButton (hwnd, IDC_GIF_SCREEN_HEIGHT, fltParam->ulScreenHeight, 0, 65535);
      break;
   case WM_CONTROL:
      WinEnableControl (hwnd, IDC_GIF_TRANSINDEX, 
         WinQueryButtonCheckstate (hwnd, IDC_GIF_TRANS_CAPTION) &&
         !WinQueryButtonCheckstate (hwnd, IDC_GIF_TRANSAUTO));
      WinEnableControl (hwnd, IDC_GIF_TRANSAUTO, 
         WinQueryButtonCheckstate (hwnd, IDC_GIF_TRANS_CAPTION));
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1)) {
/* Interlaced GIF? */
         fltParam->fInterlaced = 
            WinQueryButtonCheckstate (hwnd, IDC_GIF_INTERLACED);
/* TransColor */
         if (WinQueryButtonCheckstate (hwnd, IDC_GIF_TRANS_CAPTION))
            fltParam->cTransColor = BFP_GIF_TRANSCOLOR_NONE;
         else if (WinQueryButtonCheckstate (hwnd, IDC_GIF_TRANSAUTO))
            fltParam->cTransColor = BFP_GIF_TRANSCOLOR_EDGE;
         else
            WinSendDlgItemMsg (hwnd, IDC_GIF_TRANSINDEX, SPBM_QUERYVALUE, 
               MPFROMP (&fltParam->cTransColor), 0);
/* Backgroundcolor */
         WinSendDlgItemMsg (hwnd, IDC_GIF_BKGNDINDEX, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->cBackColor), 0);
/* Position */
         WinSendDlgItemMsg (hwnd, IDC_GIF_POS_X, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosX), 0);
         WinSendDlgItemMsg (hwnd, IDC_GIF_POS_Y, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosY), 0);
/* Screensize */
         WinSendDlgItemMsg (hwnd, IDC_GIF_SCREEN_WIDTH, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulScreenWidth), 0);
         WinSendDlgItemMsg (hwnd, IDC_GIF_SCREEN_HEIGHT, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulScreenHeight), 0);
      }
      break;
   }

   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT GreymapImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XGreymapImportFilter* fltParam = NULL != param ? (XGreymapImportFilter*) param->fltParam : NULL;
   int n;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XGreymapImportFilter*) param->fltParam;

      for (n = 0; n < 4; n++) 
         if ("krgb" [n] == fltParam->cShadingType) 
            WinCheckButton (hwnd, IDC_GREYMAP_GREY + n, TRUE);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         for (n = 0; n < 4; n++) 
            if (WinQueryButtonCheckstate (hwnd, IDC_GREYMAP_GREY + n))
               fltParam->cShadingType = "krgb" [n];
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT IAXImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XIAXImportFilter* fltParam = NULL != param ? (XIAXImportFilter*) param->fltParam : NULL;
   int n;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XIAXImportFilter*) param->fltParam;

      SetSpinButton (hwnd, IDC_IAX_WIDTH, fltParam->ulWidth, 0, 65535);
      for (n = 0; n < 4; n++) 
         if ("krgb" [n] == fltParam->cShadingType) 
            WinCheckButton (hwnd, IDC_IAX_GREY + n, TRUE);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1)) {
         WinSendDlgItemMsg (hwnd, IDC_IAX_WIDTH, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulWidth), 0);

         for (n = 0; n < 4; n++) 
            if (WinQueryButtonCheckstate (hwnd, IDC_IAX_GREY + n))
               fltParam->cShadingType = "krgb" [n];
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT ILBMExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XILBMExportFilter* fltParam = NULL != param ? (XILBMExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_PAINT:
      if (NULL != param) {
         LONG tColor;
         UCHAR max = (1 << GetBitCount (param->bppID)) - 1;

         WinSendDlgItemMsg (hwnd, IDC_ILBM_TRANSINDEX, SPBM_QUERYVALUE, MPFROMP (&tColor), 0);
         if (tColor > max) max = 0;
         SetSpinButton (hwnd, IDC_ILBM_TRANSINDEX, tColor, 0, max);
      }
      break;
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XILBMExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_ILBM_HAM6, fltParam->fHAM6);

      WinCheckButton (hwnd, IDC_ILBM_TRANS_CAPTION, 
         BFP_ILBM_TRANSCOLOR_NONE != fltParam->cTransColor);
      SetSpinButton (hwnd, IDC_ILBM_TRANSINDEX, fltParam->cTransColor >= 0 ?
         fltParam->cTransColor : 0, 0, (1 << GetBitCount (param->bppID)) - 1);
      WinCheckButton (hwnd, IDC_ILBM_TRANSAUTO, 
         BFP_ILBM_TRANSCOLOR_NONE != fltParam->cTransColor ?
         BFP_ILBM_TRANSCOLOR_EDGE == fltParam->cTransColor : TRUE);

      SetSpinButton (hwnd, IDC_ILBM_POS_X, fltParam->ulPosX, 0, 65535);
      SetSpinButton (hwnd, IDC_ILBM_POS_Y, fltParam->ulPosY, 0, 65535);
      SetSpinButton (hwnd, IDC_ILBM_SCREEN_WIDTH, fltParam->ulScreenWidth, 0, 65535);
      SetSpinButton (hwnd, IDC_ILBM_SCREEN_HEIGHT, fltParam->ulScreenHeight, 0, 65535);
      SetSpinButton (hwnd, IDC_ILBM_ASPECT_X, fltParam->uXAspect, 1, 65535);
      SetSpinButton (hwnd, IDC_ILBM_ASPECT_Y, fltParam->uYAspect, 1, 65535);
      break;
   case WM_CONTROL:
      WinEnableControl (hwnd, IDC_ILBM_TRANSINDEX, 
         WinQueryButtonCheckstate (hwnd, IDC_ILBM_TRANS_CAPTION) &&
         !WinQueryButtonCheckstate (hwnd, IDC_ILBM_TRANSAUTO) &&
         GetBitCount (param->bppID) <= 8);

      WinEnableControl (hwnd, IDC_ILBM_TRANSAUTO, 
         WinQueryButtonCheckstate (hwnd, IDC_ILBM_TRANS_CAPTION));
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1)) {
/* HAM6? */
         fltParam->fHAM6 = 
            WinQueryButtonCheckstate (hwnd, IDC_ILBM_HAM6);
/* TransColor */
         if (WinQueryButtonCheckstate (hwnd, IDC_ILBM_TRANS_CAPTION))
            fltParam->cTransColor = BFP_ILBM_TRANSCOLOR_NONE;
         else if (WinQueryButtonCheckstate (hwnd, IDC_ILBM_TRANSAUTO))
            fltParam->cTransColor = BFP_ILBM_TRANSCOLOR_EDGE;
         else
            WinSendDlgItemMsg (hwnd, IDC_ILBM_TRANSINDEX, SPBM_QUERYVALUE, 
               MPFROMP (&fltParam->cTransColor), 0);
/* Position */
         WinSendDlgItemMsg (hwnd, IDC_ILBM_POS_X, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosX), 0);
         WinSendDlgItemMsg (hwnd, IDC_ILBM_POS_Y, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosY), 0);
/* Screensize */
         WinSendDlgItemMsg (hwnd, IDC_ILBM_SCREEN_WIDTH, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulScreenWidth), 0);
         WinSendDlgItemMsg (hwnd, IDC_ILBM_SCREEN_HEIGHT, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulScreenHeight), 0);
/* Aspect */
         WinSendDlgItemMsg (hwnd, IDC_ILBM_ASPECT_X, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uXAspect), 0);
         WinSendDlgItemMsg (hwnd, IDC_ILBM_ASPECT_Y, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uYAspect), 0);
      }
      break;
   }

   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT JPEGExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XJPEGExportFilter* fltParam = NULL != param ? (XJPEGExportFilter*) param->fltParam : NULL;
   char* scale [] = {"0", "25", "50", "75", "100"};
   char  s [4];
   int   n;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XJPEGExportFilter*) param->fltParam;

      WinSendDlgItemMsg (hwnd, IDC_JPEG_QUALITY, 
         SLM_SETTICKSIZE, MPFROM2SHORT (SMA_SETALLTICKS, 2), 0);
      for (n = 0; n < 100; n+= 10) {
         WinSendDlgItemMsg (hwnd, IDC_JPEG_QUALITY, 
            SLM_SETTICKSIZE, MPFROM2SHORT (n, 4), 0);
         WinSendDlgItemMsg (hwnd, IDC_JPEG_QUALITY, 
            SLM_SETTICKSIZE, MPFROM2SHORT (n + 5, 7), 0);
      }

      for (n = 0; n < 5; n++)
         WinSendDlgItemMsg (hwnd, IDC_JPEG_QUALITY, 
            SLM_SETSCALETEXT, MPFROMSHORT (n * 25), 
            MPFROMP (scale [n]));

      WinSendDlgItemMsg (hwnd, IDC_JPEG_QUALITY, 
         SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), 
         MPFROMCHAR (fltParam->cQuality));

      WinCheckButton (hwnd, IDC_JPEG_PROGRESSIVE, fltParam->fProgressive);
   
      break;
   case WM_CONTROL:
      switch (SHORT1FROMMP (mp1)) {
      case IDC_JPEG_QUALITY:
         WinDefWindowProc (hwnd, msg, mp1, mp2);
         sprintf (s, "%lu", (ULONG) WinSendDlgItemMsg (hwnd, 
            IDC_JPEG_QUALITY, SLM_QUERYSLIDERINFO, 
            MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), 0));
         WinSetDlgItemText (hwnd, IDC_JPEG_QUALITY_VALUE, s);
         break;
      }
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1)) {
         fltParam->fProgressive = 
            WinQueryButtonCheckstate (hwnd, IDC_JPEG_PROGRESSIVE);
         fltParam->cQuality = (UCHAR) (ULONG) WinSendDlgItemMsg (hwnd, 
            IDC_JPEG_QUALITY, SLM_QUERYSLIDERINFO, 
            MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), 0);
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT KIPSImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {    
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XKIPSImportFilter* fltParam = NULL != param ? (XKIPSImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XKIPSImportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_KIPS_PAL + 
         fltParam->cPaletteFileExtension, TRUE);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         if (WinQueryButtonCheckstate (hwnd, IDC_KIPS_PAL))
            fltParam->cPaletteFileExtension = BFP_KIPS_PAL_EXTENSION;
         else
            fltParam->cPaletteFileExtension = BFP_KIPS_KPL_EXTENSION;
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT KIPSExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {    
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XKIPSExportFilter* fltParam = NULL != param ? (XKIPSExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XKIPSExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_KIPS_PAL + 
         fltParam->cPaletteFileExtension, TRUE);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         if (WinQueryButtonCheckstate (hwnd, IDC_KIPS_PAL))
            fltParam->cPaletteFileExtension = BFP_KIPS_PAL_EXTENSION;
         else
            fltParam->cPaletteFileExtension = BFP_KIPS_KPL_EXTENSION;
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT PCXImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XPCXImportFilter* fltParam = NULL != param ? (XPCXImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XPCXImportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_PCX_FIXLINES, fltParam->fFixLines);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         fltParam->fFixLines = 
            WinQueryButtonCheckstate (hwnd, IDC_PCX_FIXLINES);
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT PSegImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XPSegImportFilter* fltParam = NULL != param ? (XPSegImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XPSegImportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_PSEG_FIXRECORDS, fltParam->fFixRecords);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         fltParam->fFixRecords = 
            WinQueryButtonCheckstate (hwnd, IDC_PSEG_FIXRECORDS);
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT PSegExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XPSegExportFilter* fltParam = NULL != param ? (XPSegExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XPSegExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_PSEG_DOUBLE, fltParam->fDoubleSized);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         fltParam->fDoubleSized = 
            WinQueryButtonCheckstate (hwnd, IDC_PSEG_DOUBLE);
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT SpriteImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XSpriteImportFilter* fltParam = NULL != param ? (XSpriteImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XSpriteImportFilter*) param->fltParam;

      SetSpinButton (hwnd, IDC_SPRITE_INDEX, fltParam->uIndex, 0, 65535);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1))
         WinSendDlgItemMsg (hwnd, IDC_SPRITE_INDEX, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uIndex), 0);
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT TargaExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {   
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XTargaExportFilter* fltParam = NULL != param ? (XTargaExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XTargaExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_TARGA_YDOWN + 
         fltParam->cDirection, TRUE);
      break;
   case WM_USER | WM_COMMAND: 
      if (DID_OK == SHORT1FROMMP (mp1))
         if (WinQueryButtonCheckstate (hwnd, IDC_TARGA_YDOWN))
            fltParam->cDirection = BFP_TARGA_START_AT_BOTTOM;
         else
            fltParam->cDirection = BFP_TARGA_START_AT_TOP;
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT TIFFImportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XTIFFImportFilter* fltParam = NULL != param ? (XTIFFImportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XTIFFImportFilter*) param->fltParam;

      SetSpinButton (hwnd, IDC_TIFF_INDEX, fltParam->uIndex, 0, 65535);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1))
         WinSendDlgItemMsg (hwnd, IDC_TIFF_INDEX, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->uIndex), 0);
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT TIFFExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XTIFFExportFilter* fltParam = NULL != param ? (XTIFFExportFilter*) param->fltParam : NULL;
   static char* tagValue [BFP_TIFF_TAGCOUNT];
   static int   tagID;
   char tagName [64];

   switch (msg) {
   case WM_PAINT:
      WinEnableControl (hwnd, 
         IDC_TIFF_MONOPAL, BFP_BPP_MONOCHROME == GetBitCount (param->bppID));
      break;
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XTIFFExportFilter*) param->fltParam;

      WinCheckButton (hwnd, IDC_TIFF_LZW, fltParam->fLZW);
      WinCheckButton (hwnd, IDC_TIFF_MONOPAL, fltParam->fMonoPal);

      for (tagID = 0; tagID < BFP_TIFF_TAGCOUNT; tagID++)  {
         WinLoadString (fltParam->hab, param->hmod, 
            param->profile->languageBase + IDR_ARTIST + tagID, 64, tagName);
         WinSendDlgItemMsg (hwnd, IDC_TIFF_TAG, LM_INSERTITEM, 
            MPFROMLONG (LIT_END), MPFROMP (tagName));
         if (NULL != fltParam->pszTagValue [tagID])
            tagValue [tagID] = strdup ((char*) fltParam->pszTagValue [tagID]);
         else
            tagValue [tagID] = strdup ("");
      }
	   WinSendDlgItemMsg (hwnd, IDC_TIFF_TAG, LM_SELECTITEM, 
         MPFROMSHORT (tagID = 0), MPFROMSHORT (TRUE));
      WinSetDlgItemText (hwnd, IDC_TIFF_TAG_TEXT, tagValue [tagID]);

/*      if (NULL == fltParam->pszTagValue [BFP_TIFF_MAKE]) {
         time_t t;
         free (tagValue [BFP_TIFF_MAKE]);
         time (&t);
         tagValue [BFP_TIFF_MAKE] = strdup (ctime (&t));
      } */
      if (NULL == fltParam->pszTagValue [BFP_TIFF_DOCNAME]) {
         time_t t;
         free (tagValue [BFP_TIFF_DOCNAME]);
         tagValue [BFP_TIFF_DOCNAME] = strdup (param->filename);
      }

      WinCheckButton (hwnd, IDC_TIFF_REPLACE, TRUE);
      break;
   case WM_CONTROL:
      switch (SHORT1FROMMP (mp1)) {
      case IDC_TIFF_REPLACE:
         WinEnableControl (hwnd, IDC_TIFF_REPLACEMENT, 
            WinQueryButtonCheckstate (hwnd, IDC_TIFF_REPLACE));
         break;
      case IDC_TIFF_TAG:
         if (LN_SELECT == SHORT2FROMMP (mp1)) {
            ULONG l;

            free (tagValue [tagID]);
            l = WinQueryDlgItemTextLength (hwnd, IDC_TIFF_TAG_TEXT) + 1;
            tagValue [tagID] = malloc (l);
            WinQueryDlgItemText (hwnd, IDC_TIFF_TAG_TEXT, l, tagValue [tagID]);
               
            WinDefWindowProc (hwnd, msg, mp1, mp2);
            tagID = SHORT1FROMMR (WinSendDlgItemMsg (hwnd, IDC_TIFF_TAG,
               LM_QUERYSELECTION, 0, 0));
            WinSetDlgItemText (hwnd, IDC_TIFF_TAG_TEXT, tagValue [tagID]);
         }
         break;
      }
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
         BOOL fReplace = WinQueryButtonCheckstate (hwnd, IDC_TIFF_REPLACE);
         char rValue [2];
         WinQueryDlgItemText (hwnd, IDC_TIFF_REPLACEMENT, 2, rValue);

         fltParam->fLZW = WinQueryButtonCheckstate (hwnd, IDC_TIFF_LZW);
         if (BFP_BPP_MONOCHROME == GetBitCount (param->bppID))
            fltParam->fMonoPal = WinQueryButtonCheckstate (hwnd, IDC_TIFF_MONOPAL);
         for (tagID = 0; tagID < BFP_TIFF_TAGCOUNT; tagID++)
            if (strlen (tagValue [tagID])) {
               char* s;
               if (fReplace) for (s = tagValue [tagID]; *s; s++)
                  if (' ' == *s) *s = *rValue;
               fltParam->pszTagValue [tagID] = tagValue [tagID];
            } else {
               free (tagValue [tagID]);
               fltParam->pszTagValue [tagID] = NULL;
            }
      }
      break;
   }

   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

/******************************************************************************/
MRESULT YUV12CExportWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterPageParameter* param = (XBitmapFilterPageParameter*) WinQueryWindowPtr (hwnd, 0);
   XYUV12CExportFilter* fltParam = NULL != param ? (XYUV12CExportFilter*) param->fltParam : NULL;

   switch (msg) {
   case WM_INITDLG:
      WinSetWindowPtr (hwnd, 0, (PVOID*) mp2);
      param = (XBitmapFilterPageParameter*) mp2;
      fltParam = (XYUV12CExportFilter*) param->fltParam;

      SetSpinButton (hwnd, IDC_YUV12C_POSITION_X, fltParam->ulPosX, 0, 65535);
      SetSpinButton (hwnd, IDC_YUV12C_POSITION_Y, fltParam->ulPosY, 0, 65535);
      break;
   case WM_USER | WM_COMMAND:
      if (DID_OK == SHORT1FROMMP (mp1)) {
         WinSendDlgItemMsg (hwnd, IDC_YUV12C_POSITION_X, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosX), 0);
         WinSendDlgItemMsg (hwnd, IDC_YUV12C_POSITION_Y, SPBM_QUERYVALUE, 
            MPFROMP (&fltParam->ulPosY), 0);
      }
      break;
   }
   return XGLibDlgProc (hwnd, msg, mp1, mp2);
}

PFNWP GetImportWinProc (UCHAR cFilterID) {
   switch (cFilterID) {
   case BFT_Bitmap:   return BitmapImportWndProc;
/* case BFT_GEM:      return GEMImportWndProc;     */
   case BFT_GIF:      return GIFImportWndProc;
   case BFT_Greymap:  return GreymapImportWndProc;
   case BFT_IAX:      return IAXImportWndProc;
/* case BFT_ILBM:     return ILBMImportWndProc;    */
/* case BFT_JPEG:     return JPEGImportWndProc;    */
   case BFT_KIPS:     return KIPSImportWndProc;    
   case BFT_PCX:      return PCXImportWndProc;
/* case BFT_Pixmap:   return PixmapImportWndProc;  */
/* case BFT_Portrait: return PortraitImportWndProc;*/
   case BFT_PSeg:     return PSegImportWndProc;
   case BFT_Sprite:   return SpriteImportWndProc;
/*   case BFT_Targa:    return TargaImportWndProc; */
   case BFT_TIFF:     return TIFFImportWndProc;
/* case BFT_XBitmap:  return XBitmapImportWndProc; */
/* case BFT_YUV12C:   return YUV12CImportWndProc;  */
   default:           return NULL;
   }
}

PFNWP GetExportWinProc (UCHAR cFilterID) {
   switch (cFilterID) {
   case BFT_Bitmap:   return BitmapExportWndProc;
   case BFT_GEM:      return GEMExportWndProc;
   case BFT_GIF:      return GIFExportWndProc;
/*   case BFT_Greymap:  return GreymapExportWndProc; */
/*   case BFT_IAX:      return IAXExportWndProc; */
   case BFT_ILBM:     return ILBMExportWndProc;
   case BFT_JPEG:     return JPEGExportWndProc;
   case BFT_KIPS:     return KIPSExportWndProc;
/* case BFT_PCX:      return PCXExportWndProc;     */
/* case BFT_Pixmap:   return PixmapExportWndProc;  */
/* case BFT_Portrait: return PortraitExportWndProc;*/
   case BFT_PSeg:     return PSegExportWndProc;
/* case BFT_Sprite:   return SpriteExportWndProc;  */
   case BFT_Targa:    return TargaExportWndProc;   
   case BFT_TIFF:     return TIFFExportWndProc;
/* case BFT_XBitmap:  return XBitmapExportWndProc; */
   case BFT_YUV12C:   return YUV12CExportWndProc;
   default:           return NULL;
   }
}

ULONG AddBitmapFilterPage (USHORT resID, PFNWP WinProc, XBitmapFilterPageParameter* param, int num) {
   HWND  hwndPage;
   UCHAR title [512];

   if (NULLHANDLE == (hwndPage = WinLoadDlg (param->hwndNoteBook, 
      param->hwndNoteBook, WinProc, param->hmod, resID, param))) {
      WinDestroyWindow (param->hwndDialog);
      returnUL (BFE_OS2 | WinGetLastError (param->fltParam->hab));
   }

   if (0 == (param->pageID [num] = LONGFROMMR (WinSendMsg (param->hwndNoteBook, 
         BKM_INSERTPAGE, MPFROMLONG (0), 
         MPFROM2SHORT (BKA_MAJOR | BKA_STATUSTEXTON, BKA_LAST))))) {
      WinDestroyWindow (param->hwndDialog);
      returnUL (BFE_OS2 | WinGetLastError (param->fltParam->hab));
   }

   if (FALSE == WinSendMsg (param->hwndNoteBook, BKM_SETPAGEWINDOWHWND, 
      MPFROMLONG (param->pageID [num]), MPFROMHWND (hwndPage)))  {
      WinDestroyWindow (param->hwndDialog);
      returnUL (BFE_OS2 | WinGetLastError (param->fltParam->hab));
   }

   if (0 == WinLoadString (param->fltParam->hab, param->hmod, resID, 512, title)) {
      WinDestroyWindow (param->hwndDialog);
      returnUL (BFE_OS2 | WinGetLastError (param->fltParam->hab));
   }
   WinSendMsg (param->hwndNoteBook, BKM_SETTABTEXT, MPFROMLONG (param->pageID [num]), MPFROMP (title));

   return BFE_OK;
}

/******************************************************************************/
MRESULT BitmapFilterDialog (HWND owner, XBitmapFilterPageParameter* pageParams, 
   USHORT fltResID, PFNWP proc) {
   XBitmapFilter* param = pageParams->fltParam; /* fr returnMR */ 
   ULONG  rc;
   UCHAR  buffer [512];
   ULONG  osMajor, osMinor;
   USHORT resID;
   SWP    swp;
 
   DosQuerySysInfo (QSV_VERSION_MAJOR, QSV_VERSION_MAJOR, &osMajor, sizeof (osMajor));
   DosQuerySysInfo (QSV_VERSION_MINOR, QSV_VERSION_MINOR, &osMinor, sizeof (osMinor));
   resID = pageParams->profile->languageBase + 
      ((osMajor > 20 || (osMajor == 20 && osMinor >= 40)) ?
       IDR_FILTER_PARAMETERS_WARP4 : IDR_FILTER_PARAMETERS);

   if (NO_ERROR != (rc = DosLoadModule (buffer, 512, (PSZ) "OOLRES", &pageParams->hmod)))
      returnMR (BFE_OS2 | rc);
printf("%i", resID);
   if (NULLHANDLE == (pageParams->hwndDialog = WinLoadDlg (HWND_DESKTOP, owner, 
      FilterSetupWinProc, pageParams->hmod, resID, pageParams)))
      returnMR (BFE_OS2 | WinGetLastError (param->hab));
   strncpy (buffer, pageParams->filename, 511);
   WinQueryWindowText (pageParams->hwndDialog, 512 - strlen (buffer), buffer + strlen (buffer));
   WinSetWindowText (pageParams->hwndDialog, buffer);

   pageParams->hwndNoteBook = WinWindowFromID (pageParams->hwndDialog, IDC_NOTEBOOK);

   WinSendMsg (pageParams->hwndNoteBook, BKM_SETNOTEBOOKCOLORS, 
      MPFROMLONG (WinQuerySysColor (HWND_DESKTOP, SYSCLR_DIALOGBACKGROUND, 0)),
      MPFROMLONG (BKA_BACKGROUNDPAGECOLOR));

   if (BFE_OK != (rc = AddBitmapFilterPage (pageParams->profile->languageBase + 
      IDR_COLOR_CONV_PAGE, ColorSetupWinProc, pageParams, 0)))
      return (MRESULT) rc;
   if (NULL != proc) {
      if (BFE_OK != (rc = AddBitmapFilterPage (fltResID, proc, pageParams, 1)))
         return (MRESULT) rc;
   } else
      pageParams->pageID [1] = 0;

/* Richtige Breite fr Warp3-Tabs... */
   if (pageParams->profile->languageBase + IDR_FILTER_PARAMETERS == resID) {
      int    tabCX, tabCY;
      HPS    hpsNB;
      POINTL textBox [4];
   
      hpsNB = WinGetPS (pageParams->hwndNoteBook);
      WinLoadString (pageParams->fltParam->hab, pageParams->hmod,
         pageParams->profile->languageBase + IDR_COLOR_CONV_PAGE, 512, buffer);
      GpiQueryTextBox (hpsNB, strlen (buffer), buffer, 4, textBox);
      tabCX = textBox [2].x - textBox [0].x + 1;
      tabCY = textBox [0].y - textBox [1].y + 11;
      if (NULL != proc) {
         WinLoadString (pageParams->fltParam->hab, pageParams->hmod,
            fltResID, 512, buffer);
         GpiQueryTextBox (hpsNB, strlen (buffer), buffer, 4, textBox);
         if (tabCX < (textBox [2].x - textBox [0].x + 1))
            tabCX = textBox [2].x - textBox [0].x + 1;
      }
      WinReleasePS (hpsNB);
      WinSendMsg (pageParams->hwndNoteBook, BKM_SETDIMENSIONS, 
         MPFROM2SHORT (tabCX, tabCY), (MPARAM) BKA_MAJORTAB);

      WinQueryWindowPos (pageParams->hwndDialog, &swp);
      WinSetWindowPos (pageParams->hwndDialog, NULLHANDLE, 0, 0,
         swp.cx + tabCX, swp.cy, SWP_SIZE);
      WinQueryWindowPos (pageParams->hwndNoteBook, &swp);
      WinSetWindowPos (pageParams->hwndNoteBook, NULLHANDLE, 0, 0,
         swp.cx + tabCX, swp.cy, SWP_SIZE);
   }
   WinQueryWindowPos (pageParams->hwndDialog, &swp);
   WinSetWindowPos (pageParams->hwndDialog, NULLHANDLE,
      (WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN) - swp.cx) >> 1,
      (WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN) - swp.cy) >> 1,
      0, 0, SWP_MOVE);

   param->command = WinProcessDlg (pageParams->hwndDialog);
   WinDestroyWindow (pageParams->hwndDialog);

   DosFreeModule (pageParams->hmod);
   return (MRESULT) BFE_OK;
}
