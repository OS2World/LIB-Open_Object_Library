#define  INCL_GPIBITMAPS
#define  INCL_WINSYS
#include <os2.h>

#include "oolglib.h"
#include "oolgltk.h"

#include "gbm.h"
#include "gbmtrunc.h"
#include "gbmerr.h"
#include "gbmht.h"
#include "gbmhist.h"
#include "gbmmcut.h"

#include <stdio.h>
#include <string.h>

#define       __XGLIBpstrLen__ 65536
static char   __XGLIBpstr__ [__XGLIBpstrLen__];

ULONG CreateBitmapInfo (PBITMAPINFO2* os2, ULONG w, ULONG h, UCHAR bpp) {
   ULONG bytes;

   if (bpp <= 1) bpp = 1; else
   if (bpp <= 4) bpp = 4; else
   if (bpp <= 8) bpp = 8; else bpp = 24;

   bytes = DATAOFFSET (bpp) + h * SCANLINEBYTES (w, bpp);
   if (NULL == (*os2 = (PBITMAPINFO2) malloc (bytes)))
      return (BFE_MEMORY | bytes);

   memset (*os2, 0, sizeof (BITMAPINFOHEADER2));
   (*os2)->cbFix     = sizeof (BITMAPINFOHEADER2);
   (*os2)->cx        = w;
   (*os2)->cy        = h;
   (*os2)->cPlanes   = 1;
   (*os2)->cBitCount = bpp;
   (*os2)->cbImage   = h * SCANLINEBYTES (w, bpp);
   
   return BFE_OK;
}

void trunc_pal_8G (GBMRGB* pal) {
   int n;
   for (n = 0; n < 256; n++)
      pal [n].r = pal [n].g = pal [n].b = n;
}

void trunc_8G (GBM* gbm, PBYTE src24, PBYTE dst8) {
   PBYTE srcData, srcLine = src24, 
         dstData, dstLine = dst8;
   int   srcDelta = SCANLINEBYTES (gbm->w, 24),
         dstDelta = SCANLINEBYTES (gbm->w, 8);
   int   x, y;

   for (y = 0; y < gbm->h; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
      srcData = srcLine;
      dstData = dstLine; 
      for (x = 0; x < gbm->w; x++)
         *dstData++ = (*srcData++ * 154 + *srcData++ * 307 + *srcData++ *  51) >> 9;
   }
}

void trunc_pal_8Red (GBMRGB* pal) {
   int n;
   for (n = 0; n < 256; n++) {
      pal [n].r = n; pal [n].g = pal [n].b = 0;
   }
}

void trunc_8Red (GBM* gbm, PBYTE src24, PBYTE dst8) {
   PBYTE srcData, srcLine = src24, 
         dstData, dstLine = dst8;
   int   srcDelta = SCANLINEBYTES (gbm->w, 24),
         dstDelta = SCANLINEBYTES (gbm->w, 8);
   int   x, y;

   for (y = 0; y < gbm->h; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
      srcData = srcLine;
      dstData = dstLine; 
      for (x = 0; x < gbm->w; x++, srcData+= 3)
         *dstData++ = ((RGB*) srcData)->bRed;
   }
}

void trunc_pal_8Green (GBMRGB* pal) {
   int n;
   for (n = 0; n < 256; n++) {
      pal [n].g = n; pal [n].r = pal [n].b = 0;
   }
}

void trunc_8Green (GBM* gbm, PBYTE src24, PBYTE dst8) {
   PBYTE srcData, srcLine = src24, 
         dstData, dstLine = dst8;
   int   srcDelta = SCANLINEBYTES (gbm->w, 24),
         dstDelta = SCANLINEBYTES (gbm->w, 8);
   int   x, y;

   for (y = 0; y < gbm->h; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
      srcData = srcLine;
      dstData = dstLine; 
      for (x = 0; x < gbm->w; x++, srcData+= 3)
         *dstData++ = ((RGB*) srcData)->bGreen;
   }
}

void trunc_pal_8Blue (GBMRGB* pal) {
   int n;
   for (n = 0; n < 256; n++) {
      pal [n].b = n; pal [n].r = pal [n].g = 0;
   }
}

void trunc_8Blue (GBM* gbm, PBYTE src24, PBYTE dst8) {
   PBYTE srcData, srcLine = src24, 
         dstData, dstLine = dst8;
   int   srcDelta = SCANLINEBYTES (gbm->w, 24),
         dstDelta = SCANLINEBYTES (gbm->w, 8);
   int   x, y;

   for (y = 0; y < gbm->h; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
      srcData = srcLine;
      dstData = dstLine; 
      for (x = 0; x < gbm->w; x++, srcData+= 3)
         *dstData++ = ((RGB*) srcData)->bBlue;
   }
}

ULONG TrueColorExpansion (PBITMAPINFO2* newBitmap, PBITMAPINFO2 oldBitmap) {
   ULONG rc;
   PBYTE srcData, dstData, srcLine, dstLine;
   int   srcDelta = SCANLINEBYTES (oldBitmap->cx, oldBitmap->cBitCount),
         dstDelta = SCANLINEBYTES (oldBitmap->cx, 24);

   if (BFP_BPP_TRUECOLOR == oldBitmap->cBitCount) {
      *newBitmap = oldBitmap;
      return BFE_OK;
   }

   if (BFE_OK != (rc = CreateBitmapInfo (newBitmap, oldBitmap->cx, oldBitmap->cy, 24)))
      return rc;

   srcLine = (PBYTE) (oldBitmap->argbColor + PALETTESIZE (oldBitmap->cBitCount));
   dstLine = (PBYTE) ((*newBitmap)->argbColor);

   switch (oldBitmap->cBitCount) {
   case BFP_BPP_MONOCHROME: {
      int x, y;
      for (y = 0; y < oldBitmap->cy; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
         BYTE b;
         srcData = srcLine;
         dstData = dstLine; 
         for (x = 0; x < oldBitmap->cx; x++) {
            if (0 == (x & 7))
               b = *srcData++;
            else
               b <<= 1;

            *dstData++ = oldBitmap->argbColor [b >> 7].bRed;
            *dstData++ = oldBitmap->argbColor [b >> 7].bGreen;
            *dstData++ = oldBitmap->argbColor [b >> 7].bBlue;
         }
         ;
      }
      break; }
   case BFP_BPP_EGA: {
      int x, y;
      for (y = 0; y < oldBitmap->cy; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
         srcData = srcLine;
         dstData = dstLine; 
         for (x = 0; x + 1 < oldBitmap->cx; x+=2, srcData++) {
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bBlue;
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bGreen;
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bRed;
            *dstData++ = oldBitmap->argbColor [*srcData & 15].bBlue;
            *dstData++ = oldBitmap->argbColor [*srcData & 15].bGreen;
            *dstData++ = oldBitmap->argbColor [*srcData & 15].bRed;
         }
         if (oldBitmap->cx & 1) {
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bBlue;
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bGreen;
            *dstData++ = oldBitmap->argbColor [*srcData >> 4].bRed;
         }
      }
      break; }
   case BFP_BPP_VGA: {
      int x, y;
      for (y = 0; y < oldBitmap->cy; y++, srcLine+= srcDelta, dstLine+= dstDelta) {
         srcData = srcLine;
         dstData = dstLine; 
         for (x = 0; x < oldBitmap->cx; x++, srcData++) {
            *dstData++ = oldBitmap->argbColor [*srcData].bBlue;
            *dstData++ = oldBitmap->argbColor [*srcData].bGreen;
            *dstData++ = oldBitmap->argbColor [*srcData].bRed;
         }
      }
      break; }
   }
   
   return BFE_OK;
}

UCHAR AdjustColorBits (UCHAR bppValue, UCHAR bppMask) {
/* first try identical or next higher bpp-value */
   if (BFP_BPP_MONOCHROME == bppValue && (BFF_PROCESS_1BPP & bppMask))
      return BFP_BPP_MONOCHROME;
   if (BFP_BPP_EGA        >= bppValue && (BFF_PROCESS_4BPP & bppMask))
      return BFP_BPP_EGA;
   if (BFP_BPP_VGA        >= bppValue && (BFF_PROCESS_8BPP & bppMask))
      return BFP_BPP_VGA;
   if (BFP_BPP_TRUECOLOR  >= bppValue && (BFF_PROCESS_24BPP & bppMask))
      return BFP_BPP_TRUECOLOR;

/* and now next lower bpp-value */
   if (BFP_BPP_VGA < bppValue && (BFF_PROCESS_8BPP & bppMask))
      return BFP_BPP_VGA;
   if (BFP_BPP_EGA < bppValue && (BFF_PROCESS_4BPP & bppMask))
      return BFP_BPP_EGA;
   return BFP_BPP_MONOCHROME;
}

ULONG Remap (PBITMAPINFO2 source, PBITMAPINFO2* dest, XBitmapFilter* param) {
   PBITMAPINFO2 trueColor;
   GBMRGB       newPal [256];
   GBM          gbmInfo;
   PBYTE        srcData;
   PBYTE        dstData;
   ULONG        rc;
   int          n;

   if (BFP_BPP_DONT_TOUCH == param->cBitCount) {
      *dest = source;
      return BFE_OK;
   }

   if (BFP_PALETTE_DEFAULT == param->cPalette)
      param->cPalette = 
         GetDefaultPalette (param->cFilterID, param->cBitCount);

   if (!(GetDitheringMask (param->cFilterID, param->cBitCount) &
      param->cDithering))
      param->cDithering = BFP_DITHERING_NONE;

fprintf (stderr, "Remap bpp: %d->%d, pal: %d, alg: %d\n",
   source->cBitCount, param->cBitCount, param->cPalette, param->cDithering);
   if (BFE_OK != (rc = TrueColorExpansion (&trueColor, source))) {
      return rc;
   }

   if (BFE_OK != (rc = CreateBitmapInfo (dest, source->cx, source->cy, param->cBitCount))) {
      return rc;
   }
   gbmInfo.w   = trueColor->cx;
   gbmInfo.h   = trueColor->cy;
   gbmInfo.bpp = trueColor->cBitCount;
   dstData = (PBYTE) ((*dest)->argbColor + PALETTESIZE ((*dest)->cBitCount));
   srcData = (PBYTE) (trueColor->argbColor);

   rc = TRUE;
   switch (param->cBitCount) {
   case BFP_BPP_MONOCHROME: /**************************************************/
      switch (param->cDithering) {
      case BFP_DITHERING_NONE:            /* MONO->Truncate */
         gbm_trunc_pal_BW (newPal);
         newPal [2] = newPal [1]; newPal [1] = newPal [0]; newPal [0] = newPal [2];
         gbm_trunc_BW (&gbmInfo, srcData, dstData);
         break;
      case BFP_DITHERING_ERRORDIFFUSION:  /* MONO->Errordiffusion */
         gbm_errdiff_pal_BW (newPal);
         newPal [2] = newPal [1]; newPal [1] = newPal [0]; newPal [0] = newPal [2];
         rc = gbm_errdiff_BW (&gbmInfo, srcData, dstData);
         break;
      }
      break;
   case BFP_BPP_EGA: /*********************************************************/
      switch (param->cDithering) {
      case BFP_DITHERING_NONE:            /* EGA->Truncate */
         switch (param->cPalette) {
         case BFP_PALETTE_GREYSCALE:
            gbm_trunc_pal_4G (newPal);
            gbm_trunc_4G (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_8_COLORS:
            gbm_trunc_pal_8 (newPal);
            gbm_trunc_8 (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_16_COLORS:
            gbm_trunc_pal_VGA (newPal);
            gbm_trunc_VGA (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_ERRORDIFFUSION:  /* EGA->Errordiffusion */
         switch (param->cPalette) {
         case BFP_PALETTE_GREYSCALE:
            gbm_errdiff_pal_4G (newPal);
            rc = gbm_errdiff_4G (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_8_COLORS:            gbm_errdiff_pal_8 (newPal);
            rc = gbm_errdiff_8 (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_16_COLORS:
            gbm_errdiff_pal_VGA (newPal);
            rc = gbm_errdiff_VGA (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_SMALL_HALFTONE:  /* EGA->smallHalfTone */
         switch (param->cPalette) {
         case BFP_PALETTE_4BIT_8_COLORS:
            gbm_ht_pal_8 (newPal);
            gbm_ht_8_2x2 (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_16_COLORS:
            gbm_ht_pal_VGA (newPal);
            gbm_ht_VGA_2x2 (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_LARGE_HALFTONE:  /* EGA->largeHalfTone */
         switch (param->cPalette) {
         case BFP_PALETTE_4BIT_8_COLORS:
            gbm_ht_pal_8 (newPal);
            gbm_ht_8_3x3 (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_4BIT_16_COLORS:
            gbm_ht_pal_VGA (newPal);
            gbm_ht_VGA_3x3 (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      }
      break;
   case BFP_BPP_VGA: /*********************************************************/
      switch (param->cDithering) {
      case BFP_DITHERING_NONE:            /* VGA->Truncate */
         switch (param->cPalette) {
         case BFP_PALETTE_GREYSCALE:
            trunc_pal_8G (newPal);
            trunc_8G (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_REDSCALE:
            trunc_pal_8Red (newPal);
            trunc_8Red (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_GREENSCALE:
            trunc_pal_8Green (newPal);
            trunc_8Green (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_BLUESCALE:
            trunc_pal_8Blue (newPal);
            trunc_8Blue (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_8BIT_666:
            gbm_trunc_pal_6R6G6B (newPal);
            gbm_trunc_6R6G6B (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_8BIT_784:
            gbm_trunc_pal_7R8G4B (newPal);
            gbm_trunc_7R8G4B (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_ERRORDIFFUSION:  /* VGA->ErrorDiffusion */
         switch (param->cPalette) {
         case BFP_PALETTE_8BIT_666:
            gbm_errdiff_pal_6R6G6B (newPal);
            rc = gbm_errdiff_6R6G6B (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_8BIT_784:
            gbm_errdiff_pal_7R8G4B (newPal);
            rc = gbm_errdiff_7R8G4B (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_SMALL_HALFTONE:  /* VGA->SmallHalfTone */
         switch (param->cPalette) {
         case BFP_PALETTE_8BIT_666:
            gbm_ht_pal_6R6G6B (newPal);
            gbm_ht_6R6G6B_2x2 (&gbmInfo, srcData, dstData);
            break;
         case BFP_PALETTE_8BIT_784:
            gbm_ht_pal_7R8G4B (newPal);
            gbm_ht_7R8G4B_2x2 (&gbmInfo, srcData, dstData);
            break;
         }
         break;
      case BFP_DITHERING_OPTIMIZED:       /* VGA->HistoGraph */
         rc = gbm_hist (&gbmInfo, srcData, newPal, dstData, param->cClrEntries, 
            param->cRedBits, param->cGreenBits, param->cBlueBits);
         break;
      case BFP_DITHERING_MEDIANCUT:       /* VGA->MedianCut */
         rc = gbm_mcut (&gbmInfo, srcData, newPal, dstData, param->cClrEntries);
         break;
      }
      break;
   case BFP_BPP_HIGHCOLOR: /**************************************************/
      switch (param->cDithering) {
      case BFP_DITHERING_NONE:
         switch (param->cPalette) {       /* HighCol->Truncate */
         case BFP_PALETTE_16BIT_555:
            gbm_trunc_24 (&gbmInfo, srcData, dstData, 248, 248, 248);
            break;
         case BFP_PALETTE_16BIT_565:
            gbm_trunc_24 (&gbmInfo, srcData, dstData, 248, 252, 248);
            break;
         }
         break;
      case BFP_DITHERING_ERRORDIFFUSION:
         switch (param->cPalette) {       /* HighCol->ErrorDiffusion */
         case BFP_PALETTE_16BIT_555:
            rc = gbm_errdiff_24 (&gbmInfo, srcData, dstData, 248, 248, 248);
            break;
         case BFP_PALETTE_16BIT_565:
            rc = gbm_errdiff_24 (&gbmInfo, srcData, dstData, 248, 252, 248);
            break;
         }
         break;
      case BFP_DITHERING_SMALL_HALFTONE:  /* HighCol->SmallHalfTone */
         switch (param->cPalette) {       
         case BFP_PALETTE_16BIT_555:
            gbm_ht_24_2x2 (&gbmInfo, srcData, dstData, 248, 248, 248);
            break;
         case BFP_PALETTE_16BIT_565:
            gbm_ht_24_2x2 (&gbmInfo, srcData, dstData, 248, 252, 248);
            break;
         }
         break;
      }
      break;
   case BFP_BPP_TRUECOLOR:
   case BFP_BPP_DIRECTCOLOR:
      free (*dest);
      *dest = trueColor;
      trueColor = NULL; /* free() has to be able to handle this */
      break;
   }
   if (FALSE == rc) {
      if (source != trueColor) free (trueColor);
      free (*dest);
      return (BFE_MAPPING_ERROR);
   }

   for (n = 0; n < PALETTESIZE ((*dest)->cBitCount); n++) {
      (*dest)->argbColor [n].bRed   = newPal [n].r;
      (*dest)->argbColor [n].bGreen = newPal [n].g;
      (*dest)->argbColor [n].bBlue  = newPal [n].b;
   }

   if (source != trueColor) free (trueColor);
   return BFE_OK;
}

void InitFileTypeMapping (XBitmapFilterProfile* profile) {
   gbm_guess_filetype ("oolglib.bmp", &profile->gbmFileType [BFT_Bitmap]);
   gbm_guess_filetype ("oolglib.img", profile->gbmFileType + BFT_GEM);
   gbm_guess_filetype ("oolglib.gif", profile->gbmFileType + BFT_GIF);
   gbm_guess_filetype ("oolglib.pgm", profile->gbmFileType + BFT_Greymap);
   gbm_guess_filetype ("oolglib.iax", profile->gbmFileType + BFT_IAX);
   gbm_guess_filetype ("oolglib.lbm", profile->gbmFileType + BFT_ILBM);
   gbm_guess_filetype ("oolglib.jpg", profile->gbmFileType + BFT_JPEG);
   gbm_guess_filetype ("oolglib.kps", profile->gbmFileType + BFT_KIPS);
   gbm_guess_filetype ("oolglib.pcx", profile->gbmFileType + BFT_PCX);
   gbm_guess_filetype ("oolglib.ppm", profile->gbmFileType + BFT_Pixmap);
   gbm_guess_filetype ("oolglib.cvp", profile->gbmFileType + BFT_Portrait);
   gbm_guess_filetype ("oolglib.pse", profile->gbmFileType + BFT_PSeg);
   gbm_guess_filetype ("oolglib.spr", profile->gbmFileType + BFT_Sprite);
   gbm_guess_filetype ("oolglib.tga", profile->gbmFileType + BFT_Targa);
   gbm_guess_filetype ("oolglib.tif", profile->gbmFileType + BFT_TIFF);
   gbm_guess_filetype ("oolglib.xbm", profile->gbmFileType + BFT_XBitmap);
   gbm_guess_filetype ("oolglib.vid", profile->gbmFileType + BFT_YUV12C);
}

ULONG GetProfileSize () {
   int                  n;
   ULONG                ulMemSize = sizeof (XBitmapFilterProfile);
   GBMFT                gbmInfo;
   XBitmapFilterProfile tmpProf;

   InitFileTypeMapping (&tmpProf);
   for (n = 0; n < BFT_COUNT; n++) {
      gbm_query_filetype (tmpProf.gbmFileType [n], &gbmInfo);
      ulMemSize+= strlen (gbmInfo.extensions) +
                  strlen (gbmInfo.long_name) + 2;
   }

   return ulMemSize;
}

UCHAR GetFilterID (PSZ filename, XBitmapFilterProfile* profile) {
   int   gbmType;
   UCHAR cFilterID;
   
   if (GBM_ERR_OK != gbm_guess_filetype (filename, &gbmType))
      return BFT_COUNT;
   for (cFilterID = 0; profile->gbmFileType [cFilterID] != gbmType &&
      cFilterID < BFT_COUNT; cFilterID++);
   return cFilterID;
}

ULONG GetDefaultPalette (UCHAR cFilterID, UCHAR cBitCount) {
   switch (cFilterID) {
   case BFT_Greymap:
   case BFT_IAX:
      return BFP_PALETTE_GREYSCALE;
   default:
      switch (cBitCount) {
      case BFP_BPP_MONOCHROME:  return BFP_PALETTE_1BIT_2_COLORS;
      case BFP_BPP_EGA:         return BFP_PALETTE_4BIT_16_COLORS;
      case BFP_BPP_VGA:         return BFP_PALETTE_8BIT_666;
      case BFP_BPP_HIGHCOLOR:   return BFP_PALETTE_16BIT_555;
      case BFP_BPP_TRUECOLOR:   return BFP_PALETTE_TRUECOLORS;
      case BFP_BPP_DIRECTCOLOR: return BFP_PALETTE_TRUECOLORS;
      }
   }
}

ULONG GetPaletteMask (UCHAR cFilterID, UCHAR cBitCount, UCHAR cDitheringID) {
   switch (cFilterID) {
   case BFT_Greymap:
   case BFT_IAX:
      switch (cBitCount) {
      case BFP_BPP_DONT_TOUCH:  return BFP_PALETTE_DEFAULT;
      case BFP_BPP_VGA:         return BFP_PALETTE_DEFAULT | BFP_PALETTE_GREYSCALE |
         BFP_PALETTE_REDSCALE | BFP_PALETTE_GREENSCALE | BFP_PALETTE_BLUESCALE;
      default: return 0;
      }
   default:
      switch (cBitCount) {
      case BFP_BPP_DONT_TOUCH:  return BFP_PALETTE_DEFAULT;
      case BFP_BPP_MONOCHROME:  return BFP_PALETTE_DEFAULT | BFP_PALETTE_1BIT_2_COLORS;
      case BFP_BPP_EGA:         return BFP_PALETTE_DEFAULT | BFP_PALETTE_4BIT_8_COLORS | BFP_PALETTE_4BIT_16_COLORS
         | ((BFP_DITHERING_NONE | BFP_DITHERING_ERRORDIFFUSION) & cDitheringID ? BFP_PALETTE_GREYSCALE : 0);
      case BFP_BPP_VGA:         return BFP_PALETTE_DEFAULT | BFP_PALETTE_8BIT_666 | BFP_PALETTE_8BIT_784
         | (BFP_DITHERING_NONE == cDitheringID ? BFP_PALETTE_GREYSCALE : 0);
      case BFP_BPP_HIGHCOLOR:   return BFP_PALETTE_DEFAULT | BFP_PALETTE_16BIT_555 | BFP_PALETTE_16BIT_565;
      case BFP_BPP_TRUECOLOR:   return BFP_PALETTE_DEFAULT | BFP_PALETTE_TRUECOLORS;
      case BFP_BPP_DIRECTCOLOR: return BFP_PALETTE_DEFAULT | BFP_PALETTE_TRUECOLORS;
      default: return 0;
      }
   }
}

ULONG GetDitheringMask (UCHAR cFilterID, UCHAR cBitCount) {
   switch (cFilterID) {
   case BFT_Greymap:
   case BFT_IAX:
      return BFP_DITHERING_NONE;
   default:
      switch (cBitCount) {
      case BFP_BPP_DONT_TOUCH:  return BFP_DITHERING_NONE;
      case BFP_BPP_MONOCHROME:  return BFP_DITHERING_NONE | BFP_DITHERING_ERRORDIFFUSION;
      case BFP_BPP_EGA:         return BFP_DITHERING_NONE | BFP_DITHERING_ERRORDIFFUSION  | BFP_DITHERING_SMALL_HALFTONE | BFP_DITHERING_LARGE_HALFTONE;
      case BFP_BPP_VGA:         return BFP_DITHERING_NONE | BFP_DITHERING_ERRORDIFFUSION  | BFP_DITHERING_SMALL_HALFTONE | BFP_DITHERING_OPTIMIZED | BFP_DITHERING_MEDIANCUT;
      case BFP_BPP_HIGHCOLOR:   return BFP_DITHERING_NONE | BFP_DITHERING_ERRORDIFFUSION  | BFP_DITHERING_SMALL_HALFTONE;
      case BFP_BPP_TRUECOLOR:   return BFP_DITHERING_NONE;
      case BFP_BPP_DIRECTCOLOR: return BFP_DITHERING_NONE;
      default:                  return 0;
      }
   }
}

ULONG CheckImportFilter (XBitmapFilter* param) {
   int n, c;

   if (param->cbFix != GetImportFilterSize (param->cFilterID))
      returnExtUL (BFE_INVALID_CBFIX, param->cbFix, GetImportFilterSize (param->cFilterID));
   for (n = 0, c = 0; n < BFP_PALETTE_COUNT; n++)
      if ((1 << n) & param->cPalette) c++;
   if (c != 1 || param->cPalette >= (1 << BFP_PALETTE_COUNT))
      returnUL (BFE_INVALID_PALETTE);
   for (n = 0, c = 0; n < BFP_DITHERING_COUNT; n++)
      if ((1 << n) & param->cDithering) c++;
   if (c != 1 || param->cDithering >= (1 << BFP_DITHERING_COUNT))
      returnUL (BFE_INVALID_DITHERING);

   switch (param->cFilterID) {
   case BFT_Bitmap:   return CheckBitmapImportFilter (param);
   case BFT_GEM:      return CheckGEMImportFilter (param);
   case BFT_GIF:      return CheckGIFImportFilter (param);
   case BFT_Greymap:  return CheckGreymapImportFilter (param);
   case BFT_IAX:      return CheckIAXImportFilter (param);
   case BFT_ILBM:     return CheckILBMImportFilter (param);
   case BFT_JPEG:     return CheckJPEGImportFilter (param);
   case BFT_KIPS:     return CheckKIPSImportFilter (param);
   case BFT_PCX:      return CheckPCXImportFilter (param);
   case BFT_Pixmap:   return CheckPixmapImportFilter (param);
   case BFT_Portrait: return CheckPortraitImportFilter (param);
   case BFT_PSeg:     return CheckPSegImportFilter (param);
   case BFT_Sprite:   return CheckSpriteImportFilter (param);
   case BFT_Targa:    return CheckTargaImportFilter (param);
   case BFT_TIFF:     return CheckTIFFImportFilter (param);
   case BFT_XBitmap:  return CheckXBitmapImportFilter (param);
   case BFT_YUV12C:   return CheckYUV12CImportFilter (param);
   default:           return BFE_OK;
   }
}

ULONG CheckExportFilter (XBitmapFilter* param) {
   ULONG mask;

   if (param->cbFix != GetExportFilterSize (param->cFilterID))
      returnExtUL (BFE_INVALID_CBFIX, param->cbFix, GetExportFilterSize (param->cFilterID));
   mask = GetPaletteMask (param->cFilterID, param->cBitCount, param->cDithering);
   if (0 == (param->cPalette & mask)) returnExtUL (BFE_INVALID_PALETTE, param->cPalette, mask);
   mask = GetDitheringMask (param->cFilterID, param->cBitCount);
   if (0 == (param->cDithering & mask)) returnExtUL (BFE_INVALID_DITHERING, param->cDithering, mask);

   switch (param->cFilterID) {
   case BFT_Bitmap:   return CheckBitmapExportFilter (param);
   case BFT_GEM:      return CheckGEMExportFilter (param);
   case BFT_GIF:      return CheckGIFExportFilter (param);
   case BFT_Greymap:  return CheckGreymapExportFilter (param);
   case BFT_IAX:      return CheckIAXExportFilter (param);
   case BFT_ILBM:     return CheckILBMExportFilter (param);
   case BFT_JPEG:     return CheckJPEGExportFilter (param);
   case BFT_KIPS:     return CheckKIPSExportFilter (param);
   case BFT_PCX:      return CheckPCXExportFilter (param);
   case BFT_Pixmap:   return CheckPixmapExportFilter (param);
   case BFT_Portrait: return CheckPortraitExportFilter (param);
   case BFT_PSeg:     return CheckPSegExportFilter (param);
   case BFT_Sprite:   return CheckSpriteExportFilter (param);
   case BFT_Targa:    return CheckTargaExportFilter (param);
   case BFT_TIFF:     return CheckTIFFExportFilter (param);
   case BFT_XBitmap:  return CheckXBitmapExportFilter (param);
   case BFT_YUV12C:   return CheckYUV12CExportFilter (param);
   default:           return BFE_OK;
   }
}

PSZ GetImportFilterString (XBitmapFilter* param) {
   switch (param->cFilterID) {
   case BFT_Bitmap:   return GetBitmapImportFilterString (param);
   case BFT_GEM:      return GetGEMImportFilterString (param);
   case BFT_GIF:      return GetGIFImportFilterString (param);
   case BFT_Greymap:  return GetGreymapImportFilterString (param);
   case BFT_IAX:      return GetIAXImportFilterString (param);
   case BFT_ILBM:     return GetILBMImportFilterString (param);
   case BFT_JPEG:     return GetJPEGImportFilterString (param);
   case BFT_KIPS:     return GetKIPSImportFilterString (param);
   case BFT_PCX:      return GetPCXImportFilterString (param);
   case BFT_Pixmap:   return GetPixmapImportFilterString (param);
   case BFT_Portrait: return GetPortraitImportFilterString (param);
   case BFT_PSeg:     return GetPSegImportFilterString (param);
   case BFT_Sprite:   return GetSpriteImportFilterString (param);
   case BFT_Targa:    return GetTargaImportFilterString (param);
   case BFT_TIFF:     return GetTIFFImportFilterString (param);
   case BFT_XBitmap:  return GetXBitmapImportFilterString (param);
   case BFT_YUV12C:   return GetYUV12CImportFilterString (param);
   default:           return "";
   }
}

PSZ GetExportFilterString (XBitmapFilter* param) {
   switch (param->cFilterID) {
   case BFT_Bitmap:   return GetBitmapExportFilterString (param);
   case BFT_GEM:      return GetGEMExportFilterString (param);
   case BFT_GIF:      return GetGIFExportFilterString (param);
   case BFT_Greymap:  return GetGreymapExportFilterString (param);
   case BFT_IAX:      return GetIAXExportFilterString (param);
   case BFT_ILBM:     return GetILBMExportFilterString (param);
   case BFT_JPEG:     return GetJPEGExportFilterString (param);
   case BFT_KIPS:     return GetKIPSExportFilterString (param);
   case BFT_PCX:      return GetPCXExportFilterString (param);
   case BFT_Pixmap:   return GetPixmapExportFilterString (param);
   case BFT_Portrait: return GetPortraitExportFilterString (param);
   case BFT_PSeg:     return GetPSegExportFilterString (param);
   case BFT_Sprite:   return GetSpriteExportFilterString (param);
   case BFT_Targa:    return GetTargaExportFilterString (param);
   case BFT_TIFF:     return GetTIFFExportFilterString (param);
   case BFT_XBitmap:  return GetXBitmapExportFilterString (param);
   case BFT_YUV12C:   return GetYUV12CExportFilterString (param);
   default:           return "";
   }
}

ULONG GetImportFilterSize (UCHAR cFilterID) {
   switch (cFilterID) {
   case BFT_Bitmap:   return sizeof (XBitmapImportFilter);
   case BFT_GEM:      return sizeof (XGEMImportFilter);
   case BFT_GIF:      return sizeof (XGIFImportFilter);
   case BFT_Greymap:  return sizeof (XGreymapImportFilter);
   case BFT_IAX:      return sizeof (XIAXImportFilter);
   case BFT_ILBM:     return sizeof (XILBMImportFilter);
   case BFT_JPEG:     return sizeof (XJPEGImportFilter);
   case BFT_KIPS:     return sizeof (XKIPSImportFilter);
   case BFT_PCX:      return sizeof (XPCXImportFilter);
   case BFT_Pixmap:   return sizeof (XPixmapImportFilter);
   case BFT_Portrait: return sizeof (XPortraitImportFilter);
   case BFT_PSeg:     return sizeof (XPSegImportFilter);
   case BFT_Sprite:   return sizeof (XSpriteImportFilter);
   case BFT_Targa:    return sizeof (XTargaImportFilter);
   case BFT_TIFF:     return sizeof (XTIFFImportFilter);
   case BFT_XBitmap:  return sizeof (XXBitmapImportFilter);
   case BFT_YUV12C:   return sizeof (XYUV12CImportFilter);
   default:           return sizeof (XBitmapFilter);
   }
}

ULONG GetExportFilterSize (UCHAR cFilterID) {
   switch (cFilterID) {
   case BFT_Bitmap:   return sizeof (XBitmapExportFilter);
   case BFT_GEM:      return sizeof (XGEMExportFilter);
   case BFT_GIF:      return sizeof (XGIFExportFilter);
   case BFT_Greymap:  return sizeof (XGreymapExportFilter);
   case BFT_IAX:      return sizeof (XIAXExportFilter);
   case BFT_ILBM:     return sizeof (XILBMExportFilter);
   case BFT_JPEG:     return sizeof (XJPEGExportFilter);
   case BFT_KIPS:     return sizeof (XKIPSExportFilter);
   case BFT_PCX:      return sizeof (XPCXExportFilter);
   case BFT_Pixmap:   return sizeof (XPixmapExportFilter);
   case BFT_Portrait: return sizeof (XPortraitExportFilter);
   case BFT_PSeg:     return sizeof (XPSegExportFilter);
   case BFT_Sprite:   return sizeof (XSpriteExportFilter);
   case BFT_Targa:    return sizeof (XTargaExportFilter);
   case BFT_TIFF:     return sizeof (XTIFFExportFilter);
   case BFT_XBitmap:  return sizeof (XXBitmapExportFilter);
   case BFT_YUV12C:   return sizeof (XYUV12CExportFilter);
   default:           return sizeof (XBitmapFilter);
   }
}

ULONG InitBitmapImportFilter (XBitmapFilter* generic, char* filename) {
   XBitmapImportFilter* param = (XBitmapImportFilter*) generic;
   param->uIndex          = 0;
   param->cInversionMode  = BFP_BITMAP_INVERSE_NOTHING;
   return BFE_OK;
}

ULONG InitBitmapExportFilter (XBitmapFilter* generic, char* filename) {
   XBitmapExportFilter* param = (XBitmapExportFilter*) generic;
   param->cVersion        = BFP_BITMAP_WINDOWS;
   param->cInversionMode  = BFP_BITMAP_INVERSE_NOTHING;
   return BFE_OK;
}

ULONG InitGEMImportFilter (XBitmapFilter* generic, char* filename) {
   XGEMImportFilter* param = (XGEMImportFilter*) generic;
   return BFE_OK;
}

ULONG InitGEMExportFilter (XBitmapFilter* generic, char* filename) {
   XGEMExportFilter* param = (XGEMExportFilter*) generic;
   param->fGreyscaled     = FALSE;
   param->fWritePalette   = FALSE;
   param->uPixelWidth     = 85;
   param->uPixelHeight    = 85;
   return BFE_OK;
}

ULONG InitGIFImportFilter (XBitmapFilter* generic, char* filename) {
   XGIFImportFilter* param = (XGIFImportFilter*) generic;
   param->uIndex          = 0;
   param->fIgnoreErrors   = TRUE;
   return BFE_OK;
}

ULONG InitGIFExportFilter (XBitmapFilter* generic, char* filename) {
   XGIFExportFilter* param = (XGIFExportFilter*) generic;
   param->fInterlaced     = FALSE;
   param->cTransColor     = BFP_GIF_TRANSCOLOR_NONE;
   param->cBackColor      = 0;
   param->ulPosX          = 0;
   param->ulPosY          = 0;
   param->ulScreenWidth   = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
   param->ulScreenHeight  = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
   return BFE_OK;
}

ULONG InitGreymapImportFilter (XBitmapFilter* generic, char* filename) {
   XGreymapImportFilter* param = (XGreymapImportFilter*) generic;
   param->cShadingType    = BFP_GREYMAP_GREY_SHADE;
   return BFE_OK;
}

ULONG InitGreymapExportFilter (XBitmapFilter* generic, char* filename) {
   XGreymapExportFilter* param = (XGreymapExportFilter*) generic;
   return BFE_OK;
}

ULONG InitIAXImportFilter (XBitmapFilter* generic, char* filename) {
   XIAXImportFilter* param = (XIAXImportFilter*) generic;
   param->cShadingType    = BFP_GREYMAP_GREY_SHADE;
   param->ulWidth         = 512;
   return BFE_OK;
}

ULONG InitIAXExportFilter (XBitmapFilter* generic, char* filename) {
   XIAXExportFilter* param = (XIAXExportFilter*) generic;
   return BFE_OK;
}

ULONG InitILBMImportFilter (XBitmapFilter* generic, char* filename) {
   XILBMImportFilter* param = (XILBMImportFilter*) generic;
   return BFE_OK;
}

ULONG InitILBMExportFilter (XBitmapFilter* generic, char* filename) {
   XILBMExportFilter* param = (XILBMExportFilter*) generic;
   param->cTransColor     = BFP_ILBM_TRANSCOLOR_NONE;
   param->fHAM6           = FALSE;
   param->ulPosX          = 0;
   param->ulPosY          = 0;
   param->ulScreenWidth   = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
   param->ulScreenHeight  = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);
   param->uXAspect        = 1.0;
   param->uYAspect        = 1.0;
   return BFE_OK;
}

ULONG InitJPEGImportFilter (XBitmapFilter* generic, char* filename) {
   XJPEGImportFilter* param = (XJPEGImportFilter*) generic;
   return BFE_OK;
}

ULONG InitJPEGExportFilter (XBitmapFilter* generic, char* filename) {
   XJPEGExportFilter* param = (XJPEGExportFilter*) generic;
   param->cQuality        = 75;
   param->fProgressive    = FALSE;
   return BFE_OK;
}

ULONG InitKIPSImportFilter (XBitmapFilter* generic, char* filename) {
   XKIPSImportFilter* param = (XKIPSImportFilter*) generic;
   char tmpFN [strlen (filename)]; 
   FILE* tmpFile;

   strcpy (tmpFN, filename);
   strcpy (strrchr (tmpFN, '.'), ".kpl");
   if (NULL != (tmpFile = fopen (tmpFN, "r"))) {
      param->cPaletteFileExtension = BFP_KIPS_KPL_EXTENSION;
      fclose (tmpFile);
   } else
      param->cPaletteFileExtension = BFP_KIPS_PAL_EXTENSION;

   return BFE_OK;
}

ULONG InitKIPSExportFilter (XBitmapFilter* generic, char* filename) {
   XKIPSExportFilter* param = (XKIPSExportFilter*) generic;
   param->cPaletteFileExtension = BFP_KIPS_PAL_EXTENSION;
   return BFE_OK;
}

ULONG InitPCXImportFilter (XBitmapFilter* generic, char* filename) {
   XPCXImportFilter* param = (XPCXImportFilter*) generic;
   param->fFixLines       = TRUE;
   return BFE_OK;
}

ULONG InitPCXExportFilter (XBitmapFilter* generic, char* filename) {
   XPCXExportFilter* param = (XPCXExportFilter*) generic;
   return BFE_OK;
}

ULONG InitPixmapImportFilter (XBitmapFilter* generic, char* filename) {
   XPixmapImportFilter* param = (XPixmapImportFilter*) generic;
   return BFE_OK;
}

ULONG InitPixmapExportFilter (XBitmapFilter* generic, char* filename) {
   XPixmapExportFilter* param = (XPixmapExportFilter*) generic;
   return BFE_OK;
}

ULONG InitPortraitImportFilter (XBitmapFilter* generic, char* filename) {
   XPortraitImportFilter* param = (XPortraitImportFilter*) generic;
   return BFE_OK;
}

ULONG InitPortraitExportFilter (XBitmapFilter* generic, char* filename) {
   XPortraitExportFilter* param = (XPortraitExportFilter*) generic;
   return BFE_OK;
}

ULONG InitPSegImportFilter (XBitmapFilter* generic, char* filename) {
   XPSegImportFilter* param = (XPSegImportFilter*) generic;
   param->fFixRecords     = TRUE;
   return BFE_OK;
}

ULONG InitPSegExportFilter (XBitmapFilter* generic, char* filename) {
   XPSegExportFilter* param = (XPSegExportFilter*) generic;
   param->fDoubleSized    = FALSE;
   return BFE_OK;
}

ULONG InitSpriteImportFilter (XBitmapFilter* generic, char* filename) {
   XSpriteImportFilter* param = (XSpriteImportFilter*) generic;
   param->uIndex          = 0;
   return BFE_OK;
}

ULONG InitSpriteExportFilter (XBitmapFilter* generic, char* filename) {
   XSpriteExportFilter* param = (XSpriteExportFilter*) generic;
   return BFE_OK;
}

ULONG InitTargaImportFilter (XBitmapFilter* generic, char* filename) {
   XTargaImportFilter* param = (XTargaImportFilter*) generic;
   return BFE_OK;
}

ULONG InitTargaExportFilter (XBitmapFilter* generic, char* filename) {
   XTargaExportFilter* param = (XTargaExportFilter*) generic;
   param->cDirection      = BFP_TARGA_START_AT_BOTTOM;
   return BFE_OK;
}

ULONG InitTIFFImportFilter (XBitmapFilter* generic, char* filename) {
   XTIFFImportFilter* param = (XTIFFImportFilter*) generic;
   param->uIndex          = 0;
   return BFE_OK;
}

ULONG InitTIFFExportFilter (XBitmapFilter* generic, char* filename) {
   XTIFFExportFilter* param = (XTIFFExportFilter*) generic;
   int n;

   param->fLZW            = FALSE;
   param->fMonoPal        = FALSE;

   for (n = 0; n < BFP_TIFF_TAGCOUNT; n++) param->pszTagValue [n] = NULL;
   param->pszTagValue [BFP_TIFF_SOFTWARE] = strdup ("OOLGLIB 1.0 for GBM");

   return BFE_OK;
}

ULONG InitXBitmapImportFilter (XBitmapFilter* generic, char* filename) {
   XXBitmapImportFilter* param = (XXBitmapImportFilter*) generic;
   return BFE_OK;
}

ULONG InitXBitmapExportFilter (XBitmapFilter* generic, char* filename) {
   XXBitmapExportFilter* param = (XXBitmapExportFilter*) generic;
   return BFE_OK;
}

ULONG InitYUV12CImportFilter (XBitmapFilter* generic, char* filename) {
   XYUV12CImportFilter* param = (XYUV12CImportFilter*) generic;
   return BFE_OK;
}

ULONG InitYUV12CExportFilter (XBitmapFilter* generic, char* filename) {
   XYUV12CExportFilter* param = (XYUV12CExportFilter*) generic;
   param->ulPosX          = 0;
   param->ulPosY          = 0;
   return BFE_OK;
}

ULONG CheckBitmapImportFilter (XBitmapFilter* generic) {
   XBitmapImportFilter* param = (XBitmapImportFilter*) generic;
   if (param->cInversionMode > BFP_BITMAP_INVERSE_BITMAP) returnUL (BFE_INVALID_INVERSION);
   return BFE_OK;
}

ULONG CheckBitmapExportFilter (XBitmapFilter* generic) {
   XBitmapExportFilter* param = (XBitmapExportFilter*) generic;
   if (param->cVersion != BFP_BITMAP_OS2_11 &&
       param->cVersion != BFP_BITMAP_OS2_20 &&
       param->cVersion != BFP_BITMAP_WINDOWS) returnUL (BFE_INVALID_BITMAP_VERSION);
   if (param->cInversionMode > BFP_BITMAP_INVERSE_BITMAP) returnUL (BFE_INVALID_INVERSION);
   return BFE_OK;
}

ULONG CheckGEMImportFilter (XBitmapFilter* generic) {
   XGEMImportFilter* param = (XGEMImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckGEMExportFilter (XBitmapFilter* generic) {
   XGEMExportFilter* param = (XGEMExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckGIFImportFilter (XBitmapFilter* generic) {
   XGIFImportFilter* param = (XGIFImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckGIFExportFilter (XBitmapFilter* generic) {
   XGIFExportFilter* param = (XGIFExportFilter*) generic;
   if (param->cTransColor < BFP_GIF_TRANSCOLOR_NONE ||
       param->cTransColor > (1 << param->cBitCount) - 1) 
      returnUL (BFE_INVALID_COLOR_INDEX);
   return BFE_OK;
}

ULONG CheckGreymapImportFilter (XBitmapFilter* generic) {
   XGreymapImportFilter* param = (XGreymapImportFilter*) generic;
   if (param->cShadingType != BFP_GREYMAP_GREY_SHADE &&
       param->cShadingType != BFP_GREYMAP_RED_SHADE &&
       param->cShadingType != BFP_GREYMAP_GREEN_SHADE &&
       param->cShadingType != BFP_GREYMAP_BLUE_SHADE)  returnUL (BFE_INVALID_SHADE);
   return BFE_OK;
}

ULONG CheckGreymapExportFilter (XBitmapFilter* generic) {
   XGreymapExportFilter* param = (XGreymapExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckIAXImportFilter (XBitmapFilter* generic) {
   XIAXImportFilter* param = (XIAXImportFilter*) generic;
   if (param->cShadingType != BFP_IAX_GREY_SHADE &&
       param->cShadingType != BFP_IAX_RED_SHADE &&
       param->cShadingType != BFP_IAX_GREEN_SHADE &&
       param->cShadingType != BFP_IAX_BLUE_SHADE)  returnUL (BFE_INVALID_SHADE);
   return BFE_OK;
}

ULONG CheckIAXExportFilter (XBitmapFilter* generic) {
   XIAXExportFilter* param = (XIAXExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckILBMImportFilter (XBitmapFilter* generic) {
   XILBMImportFilter* param = (XILBMImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckILBMExportFilter (XBitmapFilter* generic) {
   XILBMExportFilter* param = (XILBMExportFilter*) generic;
   if (param->cTransColor < BFP_ILBM_TRANSCOLOR_NONE ||
       param->cTransColor > (1 << param->cBitCount) - 1) 
      returnUL (BFE_INVALID_COLOR_INDEX);
   return BFE_OK;
}

ULONG CheckJPEGImportFilter (XBitmapFilter* generic) {
   XJPEGImportFilter* param = (XJPEGImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckJPEGExportFilter (XBitmapFilter* generic) {
   XJPEGExportFilter* param = (XJPEGExportFilter*) generic;
   if (param->cQuality > 100) returnUL (BFE_INVALID_QUALITY);
   return BFE_OK;
}

ULONG CheckKIPSImportFilter (XBitmapFilter* generic) {
   XKIPSImportFilter* param = (XKIPSImportFilter*) generic;
   if (param->cPaletteFileExtension > BFP_KIPS_KPL_EXTENSION) returnUL (BFE_INVALID_EXTENSION);
   return BFE_OK;
}

ULONG CheckKIPSExportFilter (XBitmapFilter* generic) {
   XKIPSExportFilter* param = (XKIPSExportFilter*) generic;
   if (param->cPaletteFileExtension > BFP_KIPS_KPL_EXTENSION) returnUL (BFE_INVALID_EXTENSION);
   return BFE_OK;
}

ULONG CheckPCXImportFilter (XBitmapFilter* generic) {
   XPCXImportFilter* param = (XPCXImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPCXExportFilter (XBitmapFilter* generic) {
   XPCXExportFilter* param = (XPCXExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPixmapImportFilter (XBitmapFilter* generic) {
   XPixmapImportFilter* param = (XPixmapImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPixmapExportFilter (XBitmapFilter* generic) {
   XPixmapExportFilter* param = (XPixmapExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPortraitImportFilter (XBitmapFilter* generic) {
   XPortraitImportFilter* param = (XPortraitImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPortraitExportFilter (XBitmapFilter* generic) {
   XPortraitExportFilter* param = (XPortraitExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPSegImportFilter (XBitmapFilter* generic) {
   XPSegImportFilter* param = (XPSegImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckPSegExportFilter (XBitmapFilter* generic) {
   XPSegExportFilter* param = (XPSegExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckSpriteImportFilter (XBitmapFilter* generic) {
   XSpriteImportFilter* param = (XSpriteImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckSpriteExportFilter (XBitmapFilter* generic) {
   XSpriteExportFilter* param = (XSpriteExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckTargaImportFilter (XBitmapFilter* generic) {
   XTargaImportFilter* param = (XTargaImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckTargaExportFilter (XBitmapFilter* generic) {
   XTargaExportFilter* param = (XTargaExportFilter*) generic;
   if (param->cDirection != BFP_TARGA_START_AT_BOTTOM &&
       param->cDirection != BFP_TARGA_START_AT_TOP) returnUL (BFE_INVALID_DIRECTION);
   return BFE_OK;
}

ULONG CheckTIFFImportFilter (XBitmapFilter* generic) {
   XTIFFImportFilter* param = (XTIFFImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckTIFFExportFilter (XBitmapFilter* generic) {
   XTIFFExportFilter* param = (XTIFFExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckXBitmapImportFilter (XBitmapFilter* generic) {
   XXBitmapImportFilter* param = (XXBitmapImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckXBitmapExportFilter (XBitmapFilter* generic) {
   XXBitmapExportFilter* param = (XXBitmapExportFilter*) generic;
   return BFE_OK;
}

ULONG CheckYUV12CImportFilter (XBitmapFilter* generic) {
   XYUV12CImportFilter* param = (XYUV12CImportFilter*) generic;
   return BFE_OK;
}

ULONG CheckYUV12CExportFilter (XBitmapFilter* generic) {
   XYUV12CExportFilter* param = (XYUV12CExportFilter*) generic;
   return BFE_OK;
}

PSZ GetBitmapImportFilterString (XBitmapFilter* generic) {
   XBitmapImportFilter* param = (XBitmapImportFilter*) generic;
	sprintf (__XGLIBpstr__, "index=%i", param->uIndex);
	switch (param->cInversionMode) {
  	case BFP_BITMAP_INVERSE_PALETTE: strcat (__XGLIBpstr__, " inv"); break;
  	case BFP_BITMAP_INVERSE_BITMAP:  strcat (__XGLIBpstr__, " invb"); break;
  	}
  	return __XGLIBpstr__;
}

PSZ GetBitmapExportFilterString (XBitmapFilter* generic) {
   XBitmapExportFilter* param = (XBitmapExportFilter*) generic;
	sprintf (__XGLIBpstr__, "%s", param->cVersion == BFP_BITMAP_OS2_11 ? "1.1" : "2.0");
	switch (param->cInversionMode) {
  	case BFP_BITMAP_INVERSE_PALETTE:	strcat (__XGLIBpstr__, " inv"); break;
  	case BFP_BITMAP_INVERSE_BITMAP:	strcat (__XGLIBpstr__, " invb"); break;
  	case BFP_BITMAP_FORCE_DARKFG:		strcat (__XGLIBpstr__, " darkfg"); break;
  	case BFP_BITMAP_FORCE_LIGHTFG:	strcat (__XGLIBpstr__, " lightfg"); break;
  	}
  	return __XGLIBpstr__;
}

PSZ GetGEMImportFilterString (XBitmapFilter* generic) {
   XGEMImportFilter* param = (XGEMImportFilter*) generic;
   return "";
}

PSZ GetGEMExportFilterString (XBitmapFilter* generic) {
   XGEMExportFilter* param = (XGEMExportFilter*) generic;
   sprintf (__XGLIBpstr__, "pixw=%u pixh=%u%s%s", param->uPixelWidth, param->uPixelHeight,
      param->fGreyscaled ? "grey" : "", param->fWritePalette ? "pal" : "");
	return __XGLIBpstr__;
}

PSZ GetGIFImportFilterString (XBitmapFilter* generic) {
   XGIFImportFilter* param = (XGIFImportFilter*) generic;
	sprintf (__XGLIBpstr__, "index=%i%s", param->uIndex, 
 		param->fIgnoreErrors ? " errok" : "");
	return __XGLIBpstr__;
}

PSZ GetGIFExportFilterString (XBitmapFilter* generic) {
   XGIFExportFilter* param = (XGIFExportFilter*) generic;
   char transStr [15];
	sprintf (transStr, BFP_GIF_TRANSCOLOR_NONE == param->cTransColor ? "" :
      (BFP_GIF_TRANSCOLOR_EDGE == param->cTransColor ? " transcol=edge" : " transcol=%u"), 
      param->cTransColor);

	sprintf (__XGLIBpstr__, "xscreen=%u yscreen=%u background=%u xpos=%u ypos=%u%s%s",
 		param->ulScreenWidth, param->ulScreenHeight, param->cBackColor,
      param->ulPosX, param->ulPosY, param->fInterlaced ? " ilace" : "", transStr);
	return __XGLIBpstr__;
}

PSZ GetGreymapImportFilterString (XBitmapFilter* generic) {
   XGreymapImportFilter* param = (XGreymapImportFilter*) generic;
	sprintf (__XGLIBpstr__, "%c", param->cShadingType);
	return __XGLIBpstr__;
}

PSZ GetGreymapExportFilterString (XBitmapFilter* generic) {
   XGreymapExportFilter* param = (XGreymapExportFilter*) generic;
   switch (param->cPalette) {
   case BFP_PALETTE_GREYSCALE:  strcpy (__XGLIBpstr__, "k"); break;
   case BFP_PALETTE_REDSCALE:   strcpy (__XGLIBpstr__, "r"); break;
   case BFP_PALETTE_GREENSCALE: strcpy (__XGLIBpstr__, "g"); break;
   case BFP_PALETTE_BLUESCALE:  strcpy (__XGLIBpstr__, "b"); break;
   }
	return __XGLIBpstr__;
}

PSZ GetIAXImportFilterString (XBitmapFilter* generic) {
   XIAXImportFilter* param = (XIAXImportFilter*) generic;
	sprintf (__XGLIBpstr__, "WIDTH=%u %c", param->ulWidth, param->cShadingType);
	return __XGLIBpstr__;
}

PSZ GetIAXExportFilterString (XBitmapFilter* generic) {
   XIAXExportFilter* param = (XIAXExportFilter*) generic;
   switch (param->cPalette) {
   case BFP_PALETTE_GREYSCALE:  strcpy (__XGLIBpstr__, "k"); break;
   case BFP_PALETTE_REDSCALE:   strcpy (__XGLIBpstr__, "r"); break;
   case BFP_PALETTE_GREENSCALE: strcpy (__XGLIBpstr__, "g"); break;
   case BFP_PALETTE_BLUESCALE:  strcpy (__XGLIBpstr__, "b"); break;
   }
	return __XGLIBpstr__;
}

PSZ GetILBMImportFilterString (XBitmapFilter* generic) {
   XILBMImportFilter* param = (XILBMImportFilter*) generic;
   return "";
}

PSZ GetILBMExportFilterString (XBitmapFilter* generic) {
   XILBMExportFilter* param = (XILBMExportFilter*) generic;
	sprintf (__XGLIBpstr__, "xpos=%u ypos=%u transcol=%u xaspect=%f yaspect=%f"
	   "xscreen=%u yscreen=%u %s",
 		param->ulScreenWidth, param->ulScreenHeight, param->cTransColor,
 	   param->uXAspect, param->uYAspect, param->ulPosX, param->ulPosY, param->fHAM6 ? " ham6" : "");
	return __XGLIBpstr__;
}

PSZ GetJPEGImportFilterString (XBitmapFilter* generic) {
   XJPEGImportFilter* param = (XJPEGImportFilter*) generic;
   return "";
}

PSZ GetJPEGExportFilterString (XBitmapFilter* generic) {
   XJPEGExportFilter* param = (XJPEGExportFilter*) generic;
	sprintf (__XGLIBpstr__, "quality=%d%s", param->cQuality, 
		param->fProgressive ? " prog" : "");
	return __XGLIBpstr__;
}

PSZ GetKIPSImportFilterString (XBitmapFilter* generic) {
   XKIPSImportFilter* param = (XKIPSImportFilter*) generic;
   return (BFP_KIPS_KPL_EXTENSION == param->cPaletteFileExtension ? "kpl" : "pal");
}

PSZ GetKIPSExportFilterString (XBitmapFilter* generic) {
   XKIPSExportFilter* param = (XKIPSExportFilter*) generic;
   return (BFP_KIPS_KPL_EXTENSION == param->cPaletteFileExtension ? "kpl" : "pal");
}

PSZ GetPCXImportFilterString (XBitmapFilter* generic) {
   XPCXImportFilter* param = (XPCXImportFilter*) generic;
	return (param->fFixLines ? "trunc" : "");
}

PSZ GetPCXExportFilterString (XBitmapFilter* generic) {
   XPCXExportFilter* param = (XPCXExportFilter*) generic;
   return "";
}

PSZ GetPixmapImportFilterString (XBitmapFilter* generic) {
   XPixmapImportFilter* param = (XPixmapImportFilter*) generic;
   return "";
}

PSZ GetPixmapExportFilterString (XBitmapFilter* generic) {
   XPixmapExportFilter* param = (XPixmapExportFilter*) generic;
   return "";
}

PSZ GetPortraitImportFilterString (XBitmapFilter* generic) {
   XPortraitImportFilter* param = (XPortraitImportFilter*) generic;
   return "";
}

PSZ GetPortraitExportFilterString (XBitmapFilter* generic) {
   XPortraitExportFilter* param = (XPortraitExportFilter*) generic;
   return "";
}

PSZ GetPSegImportFilterString (XBitmapFilter* generic) {
   XPSegImportFilter* param = (XPSegImportFilter*) generic;
	return (param->fFixRecords ? "fixrec" : "");
}

PSZ GetPSegExportFilterString (XBitmapFilter* generic) {
   XPSegExportFilter* param = (XPSegExportFilter*) generic;
	return (param->fDoubleSized ? "double" : "");
   return "";
}

PSZ GetSpriteImportFilterString (XBitmapFilter* generic) {
   XSpriteImportFilter* param = (XSpriteImportFilter*) generic;
	sprintf (__XGLIBpstr__, "index=%i", param->uIndex);
	return __XGLIBpstr__;
}

PSZ GetSpriteExportFilterString (XBitmapFilter* generic) {
   XSpriteExportFilter* param = (XSpriteExportFilter*) generic;
   return "";
}

PSZ GetTargaImportFilterString (XBitmapFilter* generic) {
   XTargaImportFilter* param = (XTargaImportFilter*) generic;
   return "";
}

PSZ GetTargaExportFilterString (XBitmapFilter* generic) {
   XTargaExportFilter* param = (XTargaExportFilter*) generic;
	sprintf (__XGLIBpstr__, "%d%s", 
		16 >= param->cBitCount ? 16 : (24 >= param->cBitCount ? 24 : 32),
		BFP_TARGA_START_AT_BOTTOM == param->cDirection ? " up" : " down");
	return __XGLIBpstr__;
}

PSZ GetTIFFImportFilterString (XBitmapFilter* generic) {
   XTIFFImportFilter* param = (XTIFFImportFilter*) generic;
	sprintf (__XGLIBpstr__, "index=%i", param->uIndex);
	return __XGLIBpstr__;
}

#define AddTag(tag, name) if (NULL != param->pszTagValue [tag] && \
   strlen (param->pszTagValue [tag])) { \
   if (strlen (__XGLIBpstr__) + strlen (name) + \
      strlen (param->pszTagValue [tag]) >= __XGLIBpstrLen__) \
      return __XGLIBpstr__; \
   strcat (__XGLIBpstr__, name); \
   strcat (__XGLIBpstr__, param->pszTagValue [tag]); \
}

PSZ GetTIFFExportFilterString (XBitmapFilter* generic) {
   XTIFFExportFilter* param = (XTIFFExportFilter*) generic;
	sprintf (__XGLIBpstr__, "%s%s", param->fLZW ? "lzw" : "", param->fMonoPal ? " pal1bpp" : "");
   AddTag (BFP_TIFF_ARTIST,      " artist=");
   AddTag (BFP_TIFF_SOFTWARE,    " software=");
/*   AddTag (BFP_TIFF_MAKE,        " make="); */
   AddTag (BFP_TIFF_MODEL,       " model=");
   AddTag (BFP_TIFF_HOST,        " hostcomputer=");
   AddTag (BFP_TIFF_DOCNAME,     " documentname=");
   AddTag (BFP_TIFF_PAGENAME,    " pagename=");
   AddTag (BFP_TIFF_DESCRIPTION, " imagedescription=");
   return __XGLIBpstr__;
}

#undef AddTag

PSZ GetXBitmapImportFilterString (XBitmapFilter* generic) {
   XXBitmapImportFilter* param = (XXBitmapImportFilter*) generic;
   return "";
}

PSZ GetXBitmapExportFilterString (XBitmapFilter* generic) {
   XXBitmapExportFilter* param = (XXBitmapExportFilter*) generic;
   return "";
}

PSZ GetYUV12CImportFilterString (XBitmapFilter* generic) {
   XYUV12CImportFilter* param = (XYUV12CImportFilter*) generic;
   return "";
}

PSZ GetYUV12CExportFilterString (XBitmapFilter* generic) {
   XYUV12CExportFilter* param = (XYUV12CExportFilter*) generic;
	sprintf (__XGLIBpstr__, "xpos=%u ypos=%u", param->ulPosX, param->ulPosY);
	return __XGLIBpstr__;
}


