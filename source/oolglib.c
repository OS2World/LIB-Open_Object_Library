#define  INCL_DOSERRORS
#define  INCL_DOSMEMMGR
#define  INCL_GPIBITMAPS
#define  INCL_WINSTDFILE
#define  INCL_WINSTDFILE
#define  INCL_WINLISTBOXES
#define  INCL_WINWINDOWMGR

#include <os2.h>
#include "gbm.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "oolglib.h"
#include "oolgltk.h"
#include "oolgres.rh"

#include <stdio.h>

void    trunc_pal_8G (GBMRGB*);
void    trunc_pal_8Red (GBMRGB*);
void    trunc_pal_8Green (GBMRGB*);
void    trunc_pal_8Blue (GBMRGB*);

/* ULONG OOL_GLIB_INIT (XBitmapFilterProfile*);                               */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_INIT (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   XBitmapFilterProfile* profile = (XBitmapFilterProfile*) h;

   char* pMemory = ((char*) profile) + sizeof (XBitmapFilterProfile);
   GBMFT gbmInfo;
   int   n;
   char* s;

   if (NULL == profile) return (MRESULT) GetProfileSize ();
   
   InitFileTypeMapping (profile);

   profile->maxFilterSize = 0;
   for (n = 0; n < BFT_COUNT; n++) {
      if (GetImportFilterSize (n) > profile->maxFilterSize) 
         profile->maxFilterSize = GetImportFilterSize (n);
      if (GetExportFilterSize (n) > profile->maxFilterSize) 
         profile->maxFilterSize = GetExportFilterSize (n);

      gbm_query_filetype (profile->gbmFileType [n], &gbmInfo);

      profile->pszExtension [n] = pMemory;
      *pMemory++ = '*'; *pMemory++ = '.';
      for (s = gbmInfo.extensions; *s; s++)
         if (' ' == *s) {
            *pMemory++ = ';'; *pMemory++ = '*'; *pMemory++ = '.';
         } else 
            *pMemory++ = *s;
         *pMemory++ = '\0';

      for (s = profile->pszExtension [n]; *s; s++)
      profile->pszName [n] = pMemory;
      strcpy (profile->pszName [n], gbmInfo.long_name);
      pMemory+= strlen (profile->pszName [n]) + 1;

      profile->readingModes [n] = 15 & gbmInfo.flags;
      profile->writingModes [n] = 15 & (gbmInfo.flags >> 4);

      if (gbmInfo.flags & GBM_FT_R24) 
         profile->readingModes [n] |= BFF_PROCESS_16BPP;
      if (gbmInfo.flags & GBM_FT_W24) 
         profile->writingModes [n] |= BFF_PROCESS_16BPP;
   }
   profile->readingModes [BFT_Targa] |= BFF_PROCESS_32BPP;
   profile->writingModes [BFT_Targa] |= BFF_PROCESS_32BPP;

   profile->pszExtension [n] = NULL;
   profile->pszName      [n] = NULL;

   profile->languageBase = GetLanguageBase ();

   return (MRESULT) BFE_OK;
}

/* ULONG OOL_GLIB_IMP_INIT (PSZ filename, XBitmapFilterProfile* profile,      */ 
/*    XBitmapFilter* param);                                                  */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_IMP_INIT (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   char*                 filename = (char*) h;
   XBitmapFilterProfile* profile  = (XBitmapFilterProfile*) m;
   XBitmapFilter*        param    = (XBitmapFilter*) mp1;

   ULONG cbFix;

   if (NULL == filename) returnMR (BFE_ZERO_FILENAME);
   if (NULL == profile)  returnMR (BFE_ZERO_PROFILE);

   if (0 == param->cbFix) return (MRESULT*) GetImportFilterSize (GetFilterID (filename, profile));
   if (param->cbFix != (cbFix = GetImportFilterSize (GetFilterID (filename, profile))))
      returnExtMR (BFE_INVALID_CBFIX, param->cbFix, cbFix);

   param->hps        = NULLHANDLE;
   param->hbm        = NULLHANDLE;
   param->cBitCount  = BFP_BPP_DONT_TOUCH;
   param->cDithering = BFP_DITHERING_NONE;
   param->cPalette   = BFP_PALETTE_DEFAULT;
   param->cFilterID  = GetFilterID (filename, profile);

   switch (param->cFilterID) {
   case BFT_Bitmap:   return (MRESULT)InitBitmapImportFilter (param, filename);
   case BFT_GEM:      return (MRESULT)InitGEMImportFilter (param, filename);
   case BFT_GIF:      return (MRESULT)InitGIFImportFilter (param, filename);
   case BFT_Greymap:  return (MRESULT)InitGreymapImportFilter (param, filename);
   case BFT_IAX:      return (MRESULT)InitIAXImportFilter (param, filename);
   case BFT_ILBM:     return (MRESULT)InitILBMImportFilter (param, filename);
   case BFT_JPEG:     return (MRESULT)InitJPEGImportFilter (param, filename);
   case BFT_KIPS:     return (MRESULT)InitKIPSImportFilter (param, filename);
   case BFT_PCX:      return (MRESULT)InitPCXImportFilter (param, filename);
   case BFT_Pixmap:   return (MRESULT)InitPixmapImportFilter (param, filename);
   case BFT_Portrait: return (MRESULT)InitPortraitImportFilter (param, filename);
   case BFT_PSeg:     return (MRESULT)InitPSegImportFilter (param, filename);
   case BFT_Sprite:   return (MRESULT)InitSpriteImportFilter (param, filename);
   case BFT_Targa:    return (MRESULT)InitTargaImportFilter (param, filename);
   case BFT_TIFF:     return (MRESULT)InitTIFFImportFilter (param, filename);
   case BFT_XBitmap:  return (MRESULT)InitXBitmapImportFilter (param, filename);
   case BFT_YUV12C:   return (MRESULT)InitYUV12CImportFilter (param, filename);
   default:           return (MRESULT)BFE_OK;
   }
}

/* ULONG OOL_GLIB_EXP_INIT (PSZ filename, XBitmapFilterProfile* profile,      */
/*    XBitmapFilter* param);                                                  */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_EXP_INIT (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   char*                 filename = (char*) h;
   XBitmapFilterProfile* profile  = (XBitmapFilterProfile*) m;
   XBitmapFilter*        param    = (XBitmapFilter*) mp1;

   if (NULL == filename) returnMR (BFE_ZERO_FILENAME);
   if (NULL == profile)  returnMR (BFE_ZERO_PROFILE);

   if (0 == param->cbFix) return (MRESULT*) GetExportFilterSize (GetFilterID (filename, profile));
   if (param->cbFix != GetExportFilterSize (GetFilterID (filename, profile)))
      returnMR (BFE_INVALID_CBFIX);

   param->hps        = NULLHANDLE;
   param->hbm        = NULLHANDLE;
   param->cBitCount  = BFP_BPP_DONT_TOUCH;
   param->cDithering = BFP_DITHERING_NONE;
   param->cPalette   = BFP_PALETTE_DEFAULT;
   param->cFilterID  = GetFilterID (filename, profile);

   switch (param->cFilterID) {
   case BFT_Bitmap:   return (MRESULT) InitBitmapExportFilter (param, filename);
   case BFT_GEM:      return (MRESULT) InitGEMExportFilter (param, filename);
   case BFT_GIF:      return (MRESULT) InitGIFExportFilter (param, filename);
   case BFT_Greymap:  return (MRESULT) InitGreymapExportFilter (param, filename);
   case BFT_IAX:      return (MRESULT) InitIAXExportFilter (param, filename);
   case BFT_ILBM:     return (MRESULT) InitILBMExportFilter (param, filename);
   case BFT_JPEG:     return (MRESULT) InitJPEGExportFilter (param, filename);
   case BFT_KIPS:     return (MRESULT) InitKIPSExportFilter (param, filename);
   case BFT_PCX:      return (MRESULT) InitPCXExportFilter (param, filename);
   case BFT_Pixmap:   return (MRESULT) InitPixmapExportFilter (param, filename);
   case BFT_Portrait: return (MRESULT) InitPortraitExportFilter (param, filename);
   case BFT_PSeg:     return (MRESULT) InitPSegExportFilter (param, filename);
   case BFT_Sprite:   return (MRESULT) InitSpriteExportFilter (param, filename);
   case BFT_Targa:    return (MRESULT) InitTargaExportFilter (param, filename);
   case BFT_TIFF:     return (MRESULT) InitTIFFExportFilter (param, filename);
   case BFT_XBitmap:  return (MRESULT) InitXBitmapExportFilter (param, filename);
   case BFT_YUV12C:   return (MRESULT) InitYUV12CExportFilter (param, filename);
   default:           return (MRESULT) BFE_OK;
   }
}

/* OOL_GLIB_IMP (PSZ, XBitmapFilterProfile*, XBitmapFilter*);                 */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_IMP (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   PSZ                   filename = (PSZ) h;
   XBitmapFilterProfile* profile  = (XBitmapFilterProfile*) m;
   XBitmapFilter*        param    = (XBitmapFilter*) mp1;

   int            stream;
   int            filetype;

   GBM            GBMHeader;
   int            palettesize;
   GBMRGB         palette [256];

   PBITMAPINFO2   OS2Bitmap;
   PBITMAPINFO2   finalBitmap;

   int            rc;
   int            n;

   fprintf (stderr, "OOL_GLIB_IMP: %s\n", filename);

/* parameters */
   if (NULL == filename) returnMR (BFE_ZERO_FILENAME);
   if (NULL == param)    returnMR (BFE_ZERO_PARARAMETER);
   if (BFE_OK != (rc = CheckImportFilter (param))) return (MRESULT) rc;

/* init */
   if (GBM_ERR_OK != (rc = gbm_guess_filetype (filename, &filetype)))
      returnMR (BFE_GBM | rc);
   if (0 > (stream = gbm_io_open (filename, O_RDONLY | O_BINARY)))
      returnMR (BFE_GBM | rc);
   /* STREAM has to be closed */

/* bitmap info */
fprintf (stderr, "ImportFilterString: %s\n", GetImportFilterString (param));
   if (GBM_ERR_OK != (rc = gbm_read_header(filename, stream, filetype,
      &GBMHeader, GetImportFilterString (param)))) {
      gbm_io_close (stream);
      returnMR (BFE_GBM | rc);
   }
   if (BFE_OK != (rc = CreateBitmapInfo (&OS2Bitmap, GBMHeader.w, GBMHeader.h, GBMHeader.bpp))) {
      gbm_io_close (stream);
      returnMR (rc);
   }
   /* OS2BITMAP has to be closed */

/* palette */
   if (GBM_ERR_OK != (rc = gbm_read_palette(stream, filetype, &GBMHeader,
      palette))) {
      free (OS2Bitmap);
      gbm_io_close (stream);
      returnMR (BFE_GBM | rc);
   }
   if (BFT_Greymap == param->cFilterID)
      switch (((XGreymapImportFilter*) param)->cShadingType) {
      case BFP_GREYMAP_GREY_SHADE:  trunc_pal_8G     (palette); break;
      case BFP_GREYMAP_RED_SHADE:   trunc_pal_8Red   (palette); break;
      case BFP_GREYMAP_GREEN_SHADE: trunc_pal_8Green (palette); break;
      case BFP_GREYMAP_BLUE_SHADE:  trunc_pal_8Blue  (palette); break;
      }
   if (BFT_IAX == param->cFilterID)
      switch (((XIAXImportFilter*) param)->cShadingType) {
      case BFP_IAX_GREY_SHADE:  trunc_pal_8G     (palette); break;
      case BFP_IAX_RED_SHADE:   trunc_pal_8Red   (palette); break;
      case BFP_IAX_GREEN_SHADE: trunc_pal_8Green (palette); break;
      case BFP_IAX_BLUE_SHADE:  trunc_pal_8Blue  (palette); break;
      }

   palettesize = PALETTESIZE (GBMHeader.bpp);
   for (n = 0; n < palettesize; n++) {
       OS2Bitmap->argbColor [n].bRed   = palette [n].r;
       OS2Bitmap->argbColor [n].bGreen = palette [n].g;
       OS2Bitmap->argbColor [n].bBlue  = palette [n].b;
   }

/* the bitmap */
   if (GBM_ERR_OK != (rc = gbm_read_data (stream, filetype, &GBMHeader,
      (UCHAR*) (OS2Bitmap->argbColor + palettesize)))) {
      free (OS2Bitmap);
      close(stream);
      returnMR (BFE_GBM | rc);
   }
   gbm_io_close (stream);
   /* STREAM closed */

   finalBitmap = NULL;
   if (BFE_OK != (rc = Remap (OS2Bitmap, &finalBitmap, param))) {
      free (OS2Bitmap);
      return (MRESULT) rc;
   }

   if (OS2Bitmap != finalBitmap) free (OS2Bitmap);
   /* OS2BITMAP closed */
   /* FINALBITMAP has to be closed */
   gbm_deinit ();
/* GPI bitmap */
   if (NULLHANDLE == (param->hbm = GpiCreateBitmap (param->hps,
      (PBITMAPINFOHEADER2) finalBitmap, CBM_INIT,
      (PBYTE) (finalBitmap->argbColor + PALETTESIZE (finalBitmap->cBitCount)), 
      finalBitmap))) {
      free (finalBitmap);
      returnMR (BFE_OS2 | WinGetLastError (param->hab));
   }
   free (finalBitmap);
   /* FINALBITMAP closed */
   return (MRESULT) BFE_OK;
}

/* OOL_GLIB_EXP (PSZ, XBitmapFilterProfile*, XBitmapFilter*);                 */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_EXP (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   PSZ                   filename = (PSZ) h;
   XBitmapFilterProfile* profile  = (XBitmapFilterProfile*) m;
   XBitmapFilter*        param    = (XBitmapFilter*) mp1;

   int                stream;
   int                filetype;

   GBM                GBMHeader;
   GBMRGB             palette [256];
   int                palettesize;

   size_t             bytes;
   PBITMAPINFOHEADER2 header;
   PBITMAPINFO2       OS2Bitmap;
   PBITMAPINFO2       finalBitmap;

   int                rc;
   int                n;

   fprintf (stderr, "OOL_GLIB_EXP: %s\n", filename);

/* parameters */
   if (NULL == filename) returnMR (BFE_ZERO_FILENAME);
   if (NULL == param)    returnMR (BFE_ZERO_PARARAMETER);
   if (BFE_OK != (rc = CheckExportFilter (param))) return (MRESULT) rc;

/* init */
   if (GBM_ERR_OK != (rc = gbm_guess_filetype (filename, &filetype)))
      returnMR (BFE_GBM | rc);

/* info HEADER */
   if (NULL == (header = (PBITMAPINFOHEADER2)
      malloc (sizeof (BITMAPINFOHEADER2)))) {
      returnMR (BFE_MEMORY | sizeof (BITMAPINFOHEADER2));
   }
   header->cbFix = sizeof (BITMAPINFOHEADER2);
   /* HEADER has to be closed */

/* getting GPI info HEADER */
   if (GPI_ALTERROR == GpiSetBitmap(param->hps, param->hbm)) {
      free (header);
      returnMR (BFE_OS2 | WinGetLastError (param->hab));
   }
   if (GPI_ALTERROR == GpiQueryBitmapInfoHeader (param->hbm, header)) {
      free (header);
      returnMR (BFE_OS2 | WinGetLastError (param->hab));
   }

   if (BFE_OK != (rc = CreateBitmapInfo (&OS2Bitmap, header->cx,
      header->cy, header->cBitCount))) {
      free (header);
      return (MRESULT) rc;
   }
   free (header);
   /* HEADER closed */
   /* OS2BITMAP has to be closed */

/* getting GPI info BITMAP */
   if (GPI_ALTERROR == GpiQueryBitmapBits (param->hps, 0, OS2Bitmap->cy,
      ((PBYTE) OS2Bitmap) + DATAOFFSET(OS2Bitmap->cBitCount), OS2Bitmap)) {
      free (OS2Bitmap);
      returnMR (BFE_OS2 | WinGetLastError (param->hab));
   }

/* remapping */
   if (BFP_BPP_DONT_TOUCH == param->cBitCount)
      param->cBitCount = AdjustColorBits (
         OS2Bitmap->cBitCount, profile->writingModes [param->cFilterID]);

   if (BFE_OK != (rc = Remap (OS2Bitmap, &finalBitmap, param))) {
      free (OS2Bitmap);
      return (MRESULT) rc;
   }
   if (OS2Bitmap != finalBitmap) free (OS2Bitmap);
   /* OS2BITMAP closed */
   /* FINALBITMAP has to be closed */

   GBMHeader.w   = finalBitmap->cx;
   GBMHeader.h   = finalBitmap->cy;
   GBMHeader.bpp = finalBitmap->cBitCount;
   for (n = 0; n < PALETTESIZE (finalBitmap->cBitCount); n++) {
      palette [n].r = finalBitmap->argbColor [n].bRed;
      palette [n].g = finalBitmap->argbColor [n].bGreen;
      palette [n].b = finalBitmap->argbColor [n].bBlue;
   }

/* automatic transparency for ILBM files */
   if (BFT_ILBM == param->cFilterID && 
      BFP_ILBM_TRANSCOLOR_EDGE == ((XILBMExportFilter*) param)->cTransColor) 
      ((XILBMExportFilter*) param)->cTransColor = 
         ((PBYTE) finalBitmap) [DATAOFFSET (finalBitmap->cBitCount)];

/* writing */
   umask (0);
   if (0 > (stream = open (filename, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE)))
      returnMR (BFE_GBM | rc);
/* if (0 > (stream = gbm_io_open (filename, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC))) */
/*    returnMR (BFE_GBM | rc);*/
   /* STREAM has to be closed */

fprintf (stderr, "ExportFilterString: %s\n", GetExportFilterString (param));
   if (BFE_OK != (rc = gbm_write (filename, stream, filetype, &GBMHeader,
      palette, ((PBYTE) finalBitmap) + DATAOFFSET (finalBitmap->cBitCount),
      GetExportFilterString (param)))) {
      free (finalBitmap);
      gbm_io_close (stream);
fprintf (stderr, "rc=%s\n", gbm_err (rc));
      returnMR (BFE_GBM | rc);
   }

   free (finalBitmap);
   gbm_io_close (stream);
//   chmod (filename, S_IREAD | S_IWRITE);

/* FINALBITMAP closed */
/* STREAM closed */

   return (MRESULT) BFE_OK;
}

/* ULONG OOL_GLIB_IMP_SETUP (HWND owner, PSZ filename,                        */
/*    XBitmapFilterProfile* profile, XBitmapFilter* param);                   */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_IMP_SETUP (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   HWND                  owner    = (HWND) h;

   XBitmapFilterPageParameter pageParams;
   pageParams.fltParam = (XBitmapFilter*) mp2;
   pageParams.profile  = (XBitmapFilterProfile*) mp1;
   pageParams.filename = (char*) m;
   pageParams.fImport  = TRUE;

   return BitmapFilterDialog (owner, &pageParams, 
      BFT_COUNT == pageParams.fltParam->cFilterID ? 0 :
         pageParams.profile->languageBase + IDR_IMPORTFILTER_BASE + 
         pageParams.fltParam->cFilterID, 
      GetImportWinProc (pageParams.fltParam->cFilterID));
}

/* ULONG OOL_GLIB_EXP_SETUP (HWND owner, PSZ filename,                        */
/*    XBitmapFilterProfile* profile, XBitmapFilter* param);                   */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_EXP_SETUP (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   HWND                  owner    = (HWND) h;
   char*                 filename = (char*) m;
   XBitmapFilterProfile* profile  = (XBitmapFilterProfile*) mp1;
   XBitmapFilter*        param    = (XBitmapFilter*) mp2;

   XBitmapFilterPageParameter pageParams;
   pageParams.fltParam = (XBitmapFilter*) mp2;
   pageParams.profile  = (XBitmapFilterProfile*) mp1;
   pageParams.filename = (char*) m;
   pageParams.fImport  = FALSE;

   return BitmapFilterDialog (owner, &pageParams,
      BFT_COUNT == pageParams.fltParam->cFilterID ? 0 :
         pageParams.profile->languageBase + IDR_EXPORTFILTER_BASE + 
         pageParams.fltParam->cFilterID,
      GetExportWinProc (pageParams.fltParam->cFilterID));
}

/* ULONG OOL_GLIB_ERR (PULONG rc, XBitmapFilter* param, PSZ buffer,           */
/*    ULONG bufLen);                                                          */
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_ERR (HWND h, ULONG m, MPARAM mp1, MPARAM mp2) {
   PULONG         rc     = (PULONG) h;
   XBitmapFilter* param  = (XBitmapFilter*) m;
   PSZ            buffer = (PSZ) mp1;
   ULONG          bufLen = (ULONG) mp2;
   
   *rc = param->errorCode;
   snprintf (buffer, bufLen, "An error occured in bitmap filter OOLGLIB at file %s, line %u:\n", param->errorFile, param->errorLine);

   switch (param->errorCode & 0xf0000000) {
   case BFE_OK:      strncat (buffer, "No Error", bufLen); break;
   case BFE_OOLGLIB: switch (param->errorCode) {
      case BFE_ZERO_FILENAME:          strncat (buffer, "Internal error: NULL == filename",       bufLen - strlen (buffer)); break;
      case BFE_ZERO_PROFILE:           strncat (buffer, "Internal error: NULL == profile",        bufLen - strlen (buffer)); break;
      case BFE_ZERO_PARARAMETER:       strncat (buffer, "Internal error: NULL == parameter",      bufLen - strlen (buffer)); break;
      case BFE_INVALID_CBFIX:          strncat (buffer, "Internal error: invalid cbFix",          bufLen - strlen (buffer)); break;
      case BFE_INVALID_PALETTE:        strncat (buffer, "Internal error: invalid palette mode",   bufLen - strlen (buffer)); break;
      case BFE_INVALID_DITHERING:      strncat (buffer, "Internal error: invalid dithering mode", bufLen - strlen (buffer)); break;
      case BFE_INVALID_INVERSION:      strncat (buffer, "Internal error: invalid inversion mode", bufLen - strlen (buffer)); break; 
      case BFE_INVALID_BITMAP_VERSION: strncat (buffer, "Internal error: invalid bitmap version", bufLen - strlen (buffer)); break;
      case BFE_INVALID_COLOR_INDEX:    strncat (buffer, "Internal error: invalid color index",    bufLen - strlen (buffer)); break;
      case BFE_INVALID_SHADE:          strncat (buffer, "Internal error: invalid shading mode",   bufLen - strlen (buffer)); break;
      case BFE_INVALID_QUALITY:        strncat (buffer, "Internal error: invalid quality",        bufLen - strlen (buffer)); break;
      case BFE_INVALID_EXTENSION:      strncat (buffer, "Internal error: invalid extension",      bufLen - strlen (buffer)); break;
      case BFE_INVALID_DIRECTION:      strncat (buffer, "Internal error: invalid direction",      bufLen - strlen (buffer)); break;
      case BFE_MAPPING_ERROR:          strncat (buffer, "Internal error: mapping error",          bufLen - strlen (buffer)); break;
      }
      if (param->errorExtInfo) 
         snprintf (buffer + strlen (buffer), bufLen - strlen (buffer), " (%i vs %i)", param->errorVal1, param->errorVal2);
      break;
   case BFE_OS2:
      snprintf (buffer + strlen (buffer), bufLen - strlen (buffer), "OS/2 API error %04x\n", param->errorCode & 0x0fffffff);
      break;
   case BFE_MEMORY:
      snprintf (buffer + strlen (buffer), bufLen - strlen (buffer), "Could not allocate %u bytes of memory.", param->errorCode & 0x0fffffff);
      break;
   }
   return BFE_OK;
};

ULONG BitmapFilterError (ULONG rc, char* file, ULONG line, XBitmapFilter* param) {
   param->errorCode    = rc;
   param->errorFile    = file;
   param->errorLine    = line;
   param->errorExtInfo = FALSE;
   return rc;
}

ULONG BitmapFilterExtError (ULONG rc, char* file, ULONG line, XBitmapFilter* param, ULONG v1, ULONG v2) {
   param->errorCode    = rc;
   param->errorFile    = file;
   param->errorLine    = line;
   param->errorExtInfo = TRUE;
   param->errorVal1    = v1;
   param->errorVal2    = v2;
   return rc;
}

/* MRESULT OOL_GLIB_FDLG_PROC (HWND, ULONG, MPARAM, MPARAM);                  */ 
/******************************************************************************/
MRESULT _export_ _System OOL_GLIB_FDLG_PROC (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   static UCHAR                 buffer [260];
   static char                  pszAllFiles [260];
   static char                  pszAllSupportedFiles [260];
   static XBitmapFilterProfile* profile;
   static XBitmapFilter*        filter [BFT_COUNT + 1];
   static ULONG                 cFilterID = BFT_COUNT;
   PFILEDLG  pfild = (PFILEDLG) WinQueryWindowPtr (hwnd, QWL_USER);
   ULONG     rc;

   switch (msg) {
   case WM_INITDLG: {
      HAB     hab = WinInitialize (0);
      HMODULE hmod;
      ULONG   size;
      int     n;

      size = (ULONG) OOL_GLIB_INIT ((HWND) NULL, 0, 0, 0);
      DosAllocMem ((PPVOID) &profile, size, PAG_READ | PAG_WRITE | PAG_COMMIT);
      OOL_GLIB_INIT ((HWND) profile, 0, 0, 0);

      for (n = 0; n <= BFT_COUNT; n++) filter [n] = NULL;
      pfild->ulUser = (ULONG) NULL;

      if (NO_ERROR != (rc = DosLoadModule (buffer, 512, (PSZ) "OOLRES", &hmod)))
         return (MRESULT) FALSE;
      WinLoadString (hab, hmod, profile->languageBase + profile->languageBase + IDR_XGFD_ALLFILES, sizeof (pszAllFiles), pszAllFiles);
      WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_SETITEMTEXT, 0, pszAllFiles);
      WinLoadString (hab, hmod, profile->languageBase + IDR_XGFD_ALLSUPPORTED, sizeof (pszAllSupportedFiles), pszAllSupportedFiles);
      WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_INSERTITEM, 1, pszAllSupportedFiles);
      WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_SELECTITEM, MPFROMSHORT (1), MPFROMSHORT (TRUE));

      if (NULL == pfild->pszTitle) {
         WinLoadString (hab, hmod, pfild->fl & FDS_OPEN_DIALOG ? 
            profile->languageBase + IDR_XGFD_OPEN_TITLE : profile->languageBase + IDR_XGFD_SAVE_TITLE, sizeof (buffer), buffer);
         WinSetWindowText (hwnd, buffer);
      }
      WinLoadString (hab, hmod, pfild->fl & FDS_OPEN_DIALOG ? 
         profile->languageBase + IDR_XGFD_OPEN : profile->languageBase + IDR_XGFD_SAVE, sizeof (buffer), buffer);
      WinSetDlgItemText (hwnd, DID_OK_PB, buffer);
      WinLoadString (hab, hmod, profile->languageBase + IDR_XGFD_CANCEL, sizeof (buffer), buffer);
      WinSetDlgItemText (hwnd, DID_CANCEL_PB, buffer);
      WinLoadString (hab, hmod, profile->languageBase + IDR_XGFD_OPTIONS, sizeof (buffer), buffer);
      WinSetDlgItemText (hwnd, DID_APPLY_PB, buffer);
      WinLoadString (hab, hmod, profile->languageBase + IDR_XGFD_HELP, sizeof (buffer), buffer);
      WinSetDlgItemText (hwnd, DID_HELP_PB, buffer);
      DosFreeModule (hmod);      
   }  break;
   case WM_CONTROL:
      switch (SHORT1FROMMP (mp1)) {
      case DID_FILENAME_ED:
      case DID_FILTER_CB:
         WinQueryDlgItemText (hwnd, DID_FILENAME_ED, sizeof (buffer), buffer);
         WinEnableControl (hwnd, DID_APPLY_PB, 
            (GBM_ERR_OK == gbm_guess_filetype (buffer, (int*) &rc)) ||
            (WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_QUERYSELECTION, 0, 0) > 1));
         break;
      }
      break;
   case WM_COMMAND:
      switch (SHORT1FROMMP (mp1)) {
      case DID_OK_PB: {
         UCHAR pszUsersFileName [260];
         int   fileType;
         WinQueryDlgItemText (hwnd, DID_FILENAME_ED, sizeof (pszUsersFileName), pszUsersFileName);
      /* if gbm cannot guess filetype through user's input */
         if (GBM_ERR_OK != gbm_guess_filetype (pszUsersFileName, &fileType)) {
            SHORT fltIndex = WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_QUERYSELECTION, 0, 0);
      /* when a filetype is selected */
            if (fltIndex > 1) {
               int idx, ftCount;
               GBMFT gbmInfo;
      
      /* determinate gbm filetype of listbox selection */
               idx = 0;
               gbm_query_n_filetypes (&ftCount);
               WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_QUERYITEMTEXT, 
                  MPFROM2SHORT (fltIndex, sizeof (buffer)), MPFROMP (buffer));
               do {
                  gbm_query_filetype (idx, &gbmInfo);
               } while (strcmp (gbmInfo.long_name, buffer) && ++idx < ftCount);

      /* add default (=first) extension of the selected filetype do user's input */
               if (idx < ftCount) {
                  char*s = strchr (strcpy (buffer, gbmInfo.extensions), ' ');
                  
                  if (NULL != s) *s = '\0';
                  strncat (pszUsersFileName, ".", sizeof (pszUsersFileName));
                  strncat (pszUsersFileName, buffer, sizeof (pszUsersFileName));
                  WinSetDlgItemText (hwnd, DID_FILENAME_ED, pszUsersFileName);
               }
            }
         }
         if (DID_APPLY_PB == SHORT2FROMMP (mp1)) return (MRESULT) FALSE;
      } break;
      case DID_APPLY_PB: {
         UCHAR pszUsersFileName [260];
         UCHAR cLastFilter = cFilterID;
 
         OOL_GLIB_FDLG_PROC (hwnd, WM_COMMAND, MPFROM2SHORT (DID_OK_PB, DID_APPLY_PB), 0);
         WinQueryDlgItemText (hwnd, DID_FILENAME_ED, sizeof (pszUsersFileName), pszUsersFileName);
         cFilterID = GetFilterID (pszUsersFileName, profile);
         if (BFT_COUNT == cFilterID) {
            WinQueryDlgItemText (hwnd, DID_FILTER_CB, sizeof (buffer), buffer);
            for (cFilterID = 0; cFilterID < BFT_COUNT && 
               strcmp (profile->pszName [cFilterID], buffer); cFilterID++);
         }

         if (pfild->fl & FDS_OPEN_DIALOG) {
            if (NULL == filter [cFilterID]) {
               ULONG cbFix = GetImportFilterSize (cFilterID);
               DosAllocMem ((PPVOID) filter + cFilterID, cbFix, PAG_READ | PAG_WRITE | PAG_COMMIT);
               filter [cFilterID]->cbFix = cbFix;
               OOL_GLIB_IMP_INIT ((HWND) pszUsersFileName, (ULONG) profile, (MPARAM) filter [cFilterID], 0L);

               if (filter [cLastFilter]) {
                  filter [cFilterID]->cBitCount  = filter [cLastFilter]->cBitCount;
                  filter [cFilterID]->cDithering = filter [cLastFilter]->cDithering;
                  filter [cFilterID]->cPalette   = filter [cLastFilter]->cPalette;
               }
            }
            OOL_GLIB_IMP_SETUP (hwnd, (ULONG) pszUsersFileName, (MPARAM) profile, (MPARAM) filter [cFilterID]);
            if (DID_OK == filter [cFilterID]->command) {
               int n; 
               for (n = 0; n <= BFT_COUNT; n++) if (filter [n]) {
                  filter [n]->cBitCount  = filter [cFilterID]->cBitCount;
                  filter [n]->cDithering = filter [cFilterID]->cDithering;
                  filter [n]->cPalette   = filter [cFilterID]->cPalette;
               }
               pfild->ulUser =  (ULONG) filter [cFilterID];
            } else
               pfild->ulUser =  (ULONG) NULL;
         } else {
            if (NULL == filter [cFilterID]) {
               ULONG cbFix = GetExportFilterSize (cFilterID);
               DosAllocMem ((PPVOID) filter + cFilterID, cbFix, PAG_READ | PAG_WRITE | PAG_COMMIT);
               filter [cFilterID]->cbFix = cbFix;
               OOL_GLIB_EXP_INIT ((HWND) pszUsersFileName, (ULONG) profile, (MPARAM) filter [cFilterID], 0L);
            }
            OOL_GLIB_EXP_SETUP (hwnd, (ULONG) pszUsersFileName, (MPARAM) profile, (MPARAM) filter [cFilterID]);
            if (DID_OK == filter [cFilterID]->command)
               pfild->ulUser =  (ULONG) filter [cFilterID];
            else
               pfild->ulUser =  (ULONG) NULL;
         }
         return (MRESULT) FALSE;
      }
   }  break;
   case FDM_FILTER: {
      UCHAR pszUsersFileName [260];
      UCHAR pszFullFileName [260];
      PSZ   currFile = (char*) mp1;
      int   fileType;
      GBMFT gbmInfo;
      SHORT fltIndex;

      WinQueryDlgItemText (hwnd, DID_FILENAME_ED, sizeof (pszUsersFileName), pszUsersFileName);
   	if (0 != strlen (pszUsersFileName))
   /* apply user's filter to all filenames... */
         if (0 == DosEditName (1, currFile, pszUsersFileName, 
            pszFullFileName, sizeof (pszFullFileName)))
   /* ...if result differs the filter does not match! */
            if (stricmp (pszUsersFileName, pszFullFileName) && 
                stricmp (currFile, pszFullFileName))
               return (MRESULT) FALSE;

         fltIndex = WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_QUERYSELECTION, 0, 0);

   /* no Filter/<all Files> */      
         if (1 > fltIndex)
            return (MRESULT)TRUE;

   /* <all supported Files>/specific Filter -> GBM has to recognize it! */      
         if (GBM_ERR_OK != gbm_guess_filetype (currFile, &fileType))
            return (MRESULT) FALSE;

   /* <all supported Files> -> recognision is enough! */      
         if (1 == fltIndex)
            return (MRESULT) TRUE;

   /* filter text and long filetype text have to match */      
	      WinSendDlgItemMsg (hwnd, DID_FILTER_CB, LM_QUERYITEMTEXT, 
            MPFROM2SHORT (fltIndex, sizeof (buffer)), MPFROMP (buffer));
         gbm_query_filetype (fileType, &gbmInfo);
         return (MRESULT) !stricmp (buffer, gbmInfo.long_name);
   }  break;
   case WM_CLOSE: {
      int n; for (n = 0; n <= BFT_COUNT; n++)
         if ((NULL != filter [n]) && (pfild->ulUser != (ULONG) filter [n]))
            DosFreeMem (filter [n]);
      DosFreeMem (profile);
   }  break;
   } /* endswitch */
	return WinDefFileDlgProc (hwnd, msg, mp1, mp2);
}
