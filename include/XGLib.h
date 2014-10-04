/*****************************************************************************/
/* The Open Objects Library 1.0: Graphics Library (Bitmap Filter)              */
/* (C)1998 by Mathias Hasselmann <mhasselm@arco.met.fu-berlin.de>              */
/*****************************************************************************/
#ifndef _XGLIB_H_
#define _XGLIB_H_
class ABitmap;

#include "XPoint.h"
#include "XSize.h"

#define  BFE_OK                        0
#define  BFE_UNKNOWN_FILE              1
#define  BFE_IO_ERROR                  2
#define  BFE_INVALID_HEADER            3
#define  BFE_INVALID_PALETTE           4
#define  BFE_INVALID_PARAMETERS        5
#define  BFE_OUT_OF_MEMORY             6
#define  BFE_GPI_ERROR                 7

#define  BFP_BPP_DONT_TOUCH            0
#define  BFP_BPP_MONOCHROME            1
#define  BFP_BPP_EGA                   4
#define  BFP_BPP_VGA                   8
#define  BFP_BPP_HIGHCOLOR            16
#define  BFP_BPP_TRUECOLOR            24

#define  BFP_DITHERING_NONE            0
#define  BFP_DITHERING_ERRORDILBMUSION 1
#define  BFP_DITHERING_SMALL_HALFTONE  2
#define  BFP_DITHERING_LARGE_HALFTONE  3
#define  BFP_DITHERING_OPTIMIZED       4
#define  BFP_DITHERING_MEDIANCUT       5

#define  BFP_PALETTE_STANDARD          0
#define  BFP_PALETTE_6R6G6B            1
#define  BFP_PALETTE_7R8G4B            2
#define  BFP_PALETTE_2R2G2B            3
#define  BFP_PALETTE_4BIT_GRAYSCALE    4
#define  BFP_PALETTE_8BIT_GRAYSCALE    5

#define  BFP_GIF_TRANSCOLOR_NONE        -2
#define  BFP_GIF_TRANSCOLOR_EDGE        -1

#define  BFP_BITMAP_OS2_11            11
#define  BFP_BITMAP_OS2_20            20

#define  BFP_BITMAP_INVERSE_NOTHING    0
#define  BFP_BITMAP_INVERSE_PALETTE    1
#define  BFP_BITMAP_INVERSE_BITMAP     2
#define  BFP_BITMAP_FORCE_DARKFG       3
#define  BFP_BITMAP_FORCE_LIGHTFG      4

#define  BFP_TIFF_KEEP_SPACES         ' '

#define  BFP_TARGA_START_AT_BOTTOM      0
#define  BFP_TARGA_START_AT_TOP         0

#define  BFP_GREYMAP_GREY_SCALE       'k'
#define  BFP_GREYMAP_RED_SCALE        'r'
#define  BFP_GREYMAP_GREEN_SCALE      'g'
#define  BFP_GREYMAP_BLUE_SCALE       'b'

#define  BFP_IAX_GREY_SCALE           'k'
#define  BFP_IAX_RED_SCALE            'r'
#define  BFP_IAX_GREEN_SCALE          'g'
#define  BFP_IAX_BLUE_SCALE           'b'

#define  BFP_KIPS_PAL_EXTENSION        0
#define  BFP_KIPS_KPL_EXTENSION        1

class XBitmapFilterParameter: public XObject {
   friend class XBitmap;
//   friend ULONG _System _export_ OOL_GLIB_IMP (const char*, XBitmapFilterParameter*, ULONG, ULONG);
   friend MRESULT _System _export_ OOL_GLIB_IMP ( HWND h, ULONG m, MPARAM m1, MPARAM m2);
   friend MRESULT _System _export_ OOL_GLIB_EXP( HWND h, ULONG m, MPARAM m1, MPARAM mp2);

   protected:
      XBitmapFilterParameter (HPS iHps, HBITMAP iHbm) {
         Set (iHps, iHbm);
      }

      XBitmapFilterParameter* Set (HPS iHps, HBITMAP iHbm) {
         hps = iHps;
         hbm = iHbm;
         return this;
      }

      virtual char*  GetString () { return ""; }
      virtual BOOL   ImportCheck () { return TRUE; }
      virtual BOOL   ExportCheck () { return TRUE; }

      HPS      hps;
      HBITMAP   hbm;

   public:
      XBitmapFilterParameter (UCHAR iBPP       = BFP_BPP_DONT_TOUCH,
                              UCHAR iDithering = BFP_DITHERING_NONE,
                              UCHAR iPalette   = BFP_PALETTE_STANDARD) {
         hps = NULLHANDLE;
         hbm = NULLHANDLE;

         ColorBits        = iBPP;
         DitheringMethod = iDithering;
         Palette          = iPalette;
      }

      static char** GetFilterNames ();
      static char** GetFilterExtensions ();

      UCHAR ColorBits;
      UCHAR DitheringMethod;
      UCHAR Palette;
};
/*
#include "XGLibBitmap.h"
#include "XGLibGEM.h"
#include "XGLibGIF.h"
#include "XGLibGreymap.h"
#include "XGLibIAX.h"
#include "XGLibILBM.h"
#include "XGLibJPEG.h"
#include "XGLibKIPS.h"
#include "XGLibPCX.h"
#include "XGLibPixmap.h"
#include "XGLibPortrait.h"
#include "XGLibPSeg.h"
#include "XGLibSprite.h"
#include "XGLibTarga.h"
#include "XGLibTIFF.h"
#include "XGLibXBitmap.h"
#include "XGLibYUV12C.h"
*/
#endif