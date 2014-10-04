#pragma pack (1)

#ifndef __OOLGLIB__
#define __OOLGLIB__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                            library: OOLGLIB                               */
/*                        last update: 3/98                                  */
/*                      programmed by: Mathias Hasselmann                    */
/*                                     mhasselm@arco.met.fu-berlin.de        */
/*===========================================================================*/

#ifndef _export_
#ifdef __IBMCPP__                        //required export definition _Export
  #define _export_
#elif defined( __WATCOMC__ )
  #define _export_ __export
#else
  #define _export_
#endif
#endif

#define  SCANLINEBYTES(w, bpp) (((w * bpp + 31) >> 5) << 2)
#define  PALETTESIZE(bpp) (24 == bpp ? 0 : (1 << bpp))
#define  DATAOFFSET(bpp) (sizeof (BITMAPINFOHEADER2) + \
            PALETTESIZE (bpp) * sizeof (RGB2))

#define  returnUL(v) return (ULONG)   BitmapFilterError ((ULONG) (v), __FILE__, __LINE__, (XBitmapFilter*) param)
#define  returnMR(v) return (MRESULT) BitmapFilterError ((ULONG) (v), __FILE__, __LINE__, (XBitmapFilter*) param)
#define  returnExtUL(v, v1, v2) return (ULONG)   BitmapFilterExtError ((ULONG) (v), __FILE__, __LINE__, (XBitmapFilter*) param, (ULONG) v1, (ULONG) v2)
#define  returnExtMR(v, v1, v2) return (MRESULT) BitmapFilterExtError ((ULONG) (v), __FILE__, __LINE__, (XBitmapFilter*) param, (ULONG) v1, (ULONG) v2)

#define  BFE_OK                      0x00000000
#define  BFE_OOLGLIB                 0x10000000
#define  BFE_ZERO_FILENAME           BFE_OOLGLIB | 0x00000001
#define  BFE_ZERO_PROFILE            BFE_OOLGLIB | 0x00000002
#define  BFE_ZERO_PARARAMETER        BFE_OOLGLIB | 0x00000003
#define  BFE_INVALID_CBFIX           BFE_OOLGLIB | 0x00000004
#define  BFE_INVALID_PALETTE         BFE_OOLGLIB | 0x00000005
#define  BFE_INVALID_DITHERING       BFE_OOLGLIB | 0x00000006
#define  BFE_INVALID_INVERSION       BFE_OOLGLIB | 0x00000007
#define  BFE_INVALID_BITMAP_VERSION  BFE_OOLGLIB | 0x00000008
#define  BFE_INVALID_COLOR_INDEX     BFE_OOLGLIB | 0x00000009
#define  BFE_INVALID_SHADE           BFE_OOLGLIB | 0x0000000a
#define  BFE_INVALID_QUALITY         BFE_OOLGLIB | 0x0000000b
#define  BFE_INVALID_EXTENSION       BFE_OOLGLIB | 0x0000000c
#define  BFE_INVALID_DIRECTION       BFE_OOLGLIB | 0x0000000d
#define  BFE_MAPPING_ERROR           BFE_OOLGLIB | 0x0000000e
#define  BFE_GBM                     0x20000000
#define  BFE_OS2                     0x30000000
#define  BFE_MEMORY                  0x40000000

#define  BFT_Bitmap                    0
#define  BFT_GEM                       1
#define  BFT_GIF                       2
#define  BFT_Greymap                   3
#define  BFT_IAX                       4
#define  BFT_ILBM                      5
#define  BFT_JPEG                      6
#define  BFT_KIPS                      7
#define  BFT_PCX                       8
#define  BFT_Pixmap                    9
#define  BFT_Portrait                 10
#define  BFT_PSeg                     11
#define  BFT_Sprite                   12
#define  BFT_Targa                    13
#define  BFT_TIFF                     14
#define  BFT_XBitmap                  15
#define  BFT_YUV12C                   16
#define  BFT_COUNT                    17

#define  BFF_PROCESS_1BPP              1
#define  BFF_PROCESS_4BPP              2
#define  BFF_PROCESS_8BPP              4
#define  BFF_PROCESS_24BPP             8
#define  BFF_PROCESS_16BPP            16
#define  BFF_PROCESS_32BPP            32
#define  BFF_PROCESS_ALL              63

#define  BFP_BPP_DONT_TOUCH            0
#define  BFP_BPP_MONOCHROME            1
#define  BFP_BPP_EGA                   4
#define  BFP_BPP_VGA                   8
#define  BFP_BPP_HIGHCOLOR            16
#define  BFP_BPP_TRUECOLOR            24
#define  BFP_BPP_DIRECTCOLOR          32

#define  BFP_DITHERING_NONE            1
#define  BFP_DITHERING_ERRORDIFFUSION  2
#define  BFP_DITHERING_SMALL_HALFTONE  4
#define  BFP_DITHERING_LARGE_HALFTONE  8
#define  BFP_DITHERING_OPTIMIZED      16
#define  BFP_DITHERING_MEDIANCUT      32
#define  BFP_DITHERING_COUNT           6

#define  BFP_PALETTE_DEFAULT           1
#define  BFP_PALETTE_GREYSCALE         2
#define  BFP_PALETTE_1BIT_2_COLORS     4
#define  BFP_PALETTE_4BIT_8_COLORS     8
#define  BFP_PALETTE_4BIT_16_COLORS   16
#define  BFP_PALETTE_8BIT_666         32
#define  BFP_PALETTE_8BIT_784         64
#define  BFP_PALETTE_16BIT_555       128
#define  BFP_PALETTE_16BIT_565       256
#define  BFP_PALETTE_TRUECOLORS      512
#define  BFP_PALETTE_REDSCALE       1024
#define  BFP_PALETTE_GREENSCALE     2048
#define  BFP_PALETTE_BLUESCALE      4096
#define  BFP_PALETTE_COUNT            12

#define  BFP_BITMAP_OS2_11            11
#define  BFP_BITMAP_OS2_20            20
#define  BFP_BITMAP_WINDOWS           BFP_BITMAP_OS2_20

#define  BFP_BITMAP_INVERSE_NOTHING    0
#define  BFP_BITMAP_INVERSE_PALETTE    1
#define  BFP_BITMAP_INVERSE_BITMAP     2
#define  BFP_BITMAP_FORCE_DARKFG       3
#define  BFP_BITMAP_FORCE_LIGHTFG      4

#define  BFP_GIF_TRANSCOLOR_NONE	     -2
#define  BFP_GIF_TRANSCOLOR_EDGE	     -1

#define  BFP_GREYMAP_GREY_SHADE       'k'
#define  BFP_GREYMAP_RED_SHADE        'r'
#define  BFP_GREYMAP_GREEN_SHADE      'g'
#define  BFP_GREYMAP_BLUE_SHADE       'b'

#define  BFP_IAX_GREY_SHADE           'k'
#define  BFP_IAX_RED_SHADE            'r'
#define  BFP_IAX_GREEN_SHADE          'g'
#define  BFP_IAX_BLUE_SHADE           'b'

#define  BFP_KIPS_PAL_EXTENSION        0
#define  BFP_KIPS_KPL_EXTENSION        1

#define  BFP_TARGA_START_AT_BOTTOM		1
#define  BFP_TARGA_START_AT_TOP		   2

#define  BFP_TIFF_KEEP_SPACES         ' '
#define  BFP_TIFF_ARTIST               0
#define  BFP_TIFF_SOFTWARE             1
/* #define  BFP_TIFF_MAKE                 2 */
#define  BFP_TIFF_MODEL                2
#define  BFP_TIFF_HOST                 3
#define  BFP_TIFF_DOCNAME              4
#define  BFP_TIFF_PAGENAME             5
#define  BFP_TIFF_DESCRIPTION          6
#define  BFP_TIFF_TAGCOUNT             7

#define  BFP_ILBM_TRANSCOLOR_NONE     -2
#define  BFP_ILBM_TRANSCOLOR_EDGE	  -1

typedef struct _XBitmapFilterProfile {
   ULONG   cbFix;
   
   int     gbmFileType  [BFT_COUNT];
   PSZ     pszExtension [BFT_COUNT + 1];
   PSZ     pszName      [BFT_COUNT + 1];
   UCHAR   readingModes [BFT_COUNT];
   UCHAR   writingModes [BFT_COUNT];
   USHORT  languageBase;
   ULONG   maxFilterSize;
} XBitmapFilterProfile;

#define XBitmapFilterPrefix \
   ULONG   cbFix; \
   HPS     hps; \
   HBITMAP hbm; \
   HAB     hab; \
   UCHAR   cFilterID; \
   UCHAR   cBitCount; \
   UCHAR   cDithering; \
   USHORT  cPalette; \
   USHORT  cClrEntries; \
   BYTE    cRedBits; \
   BYTE    cGreenBits; \
   BYTE    cBlueBits; \
   SHORT   command; \
   ULONG   errorCode; \
   char*   errorFile; \
   ULONG   errorLine; \
   BOOL    errorExtInfo; \
   ULONG   errorVal1; \
   ULONG   errorVal2;


typedef struct _XBitmapFilter {
   XBitmapFilterPrefix
} XBitmapFilter;

typedef struct _XBitmapFilterPageParameter {
   XBitmapFilter*        fltParam;
   XBitmapFilterProfile* profile;
   HMODULE               hmod;
   HWND                  hwndDialog;
   HWND                  hwndNoteBook;
   ULONG                 pageID [2];
   PSZ                   filename;
   BOOL                  fImport;

   BOOL                  modifyColors;
   UCHAR                 bppID;
   USHORT                paletteID [6];
   UCHAR                 ditheringID [6];
   LONG                  entryCount [6];
} XBitmapFilterPageParameter;

typedef struct _XBitmapImportFilter {
   XBitmapFilterPrefix

   USHORT  uIndex;
   UCHAR   cInversionMode;
} XBitmapImportFilter;

typedef struct _XBitmapExportFilter {
   XBitmapFilterPrefix

   UCHAR   cVersion;
   UCHAR   cInversionMode;
} XBitmapExportFilter;

typedef struct _XGEMImportFilter {
   XBitmapFilterPrefix
} XGEMImportFilter;

typedef struct _XGEMExportFilter {
   XBitmapFilterPrefix

   ULONG   fGreyscaled;
   ULONG   fWritePalette;
   USHORT  uPixelWidth;
   USHORT  uPixelHeight;
} XGEMExportFilter;

typedef struct _XGIFImportFilter {
   XBitmapFilterPrefix
                        
   USHORT  uIndex;
   ULONG   fIgnoreErrors;
} XGIFImportFilter;

typedef struct _XGIFExportFilter {
   XBitmapFilterPrefix

   ULONG   fInterlaced;
   SHORT   cTransColor;
   UCHAR   cBackColor;
   ULONG   ulPosX;
   ULONG   ulPosY;
   ULONG   ulScreenWidth;
   ULONG   ulScreenHeight;
} XGIFExportFilter;

typedef struct _XGreymapImportFilter {
   XBitmapFilterPrefix
      
   UCHAR   cShadingType;
} XGreymapImportFilter;

typedef struct _XGreymapExportFilter {
   XBitmapFilterPrefix
} XGreymapExportFilter;

typedef struct _XIAXImportFilter {
   XBitmapFilterPrefix
      
   UCHAR   cShadingType;
   ULONG   ulWidth;
} XIAXImportFilter;

typedef struct _XIAXExportFilter {
   XBitmapFilterPrefix
} XIAXExportFilter;

typedef struct _XILBMImportFilter {
   XBitmapFilterPrefix
} XILBMImportFilter;

typedef struct _XILBMExportFilter {
   XBitmapFilterPrefix

   SHORT   cTransColor;
   ULONG   fHAM6;
   ULONG   ulPosX;
   ULONG   ulPosY;
   ULONG   ulScreenWidth;
   ULONG   ulScreenHeight;
   USHORT  uXAspect;
   USHORT  uYAspect;
} XILBMExportFilter;

typedef struct _XJPEGImportFilter {
   XBitmapFilterPrefix
} XJPEGImportFilter;

typedef struct _XJPEGExportFilter {
   XBitmapFilterPrefix

   UCHAR   cQuality;
   ULONG   fProgressive;
} XJPEGExportFilter;

typedef struct _XKIPSImportFilter {
   XBitmapFilterPrefix
      
   UCHAR   cPaletteFileExtension;
} XKIPSImportFilter;

typedef struct _XKIPSExportFilter {
   XBitmapFilterPrefix
      
   UCHAR   cPaletteFileExtension;
} XKIPSExportFilter;

typedef struct _XPCXImportFilter {
   XBitmapFilterPrefix
                        
   ULONG   fFixLines;
} XPCXImportFilter;

typedef struct _XPCXExportFilter {
   XBitmapFilterPrefix
} XPCXExportFilter;

typedef struct _XPixmapImportFilter {
   XBitmapFilterPrefix
} XPixmapImportFilter;

typedef struct _XPixmapExportFilter {
   XBitmapFilterPrefix
} XPixmapExportFilter;

typedef struct _XPortraitImportFilter {
   XBitmapFilterPrefix
} XPortraitImportFilter;

typedef struct _XPortraitExportFilter {
   XBitmapFilterPrefix
} XPortraitExportFilter;

typedef struct _XPSegImportFilter {
   XBitmapFilterPrefix
         
   ULONG   fFixRecords;
} XPSegImportFilter;

typedef struct _XPSegExportFilter {
   XBitmapFilterPrefix
        
   ULONG   fDoubleSized;
} XPSegExportFilter;

typedef struct _XSpriteImportFilter {
   XBitmapFilterPrefix
                                       
   USHORT  uIndex;
} XSpriteImportFilter;

typedef struct _XSpriteExportFilter {
   XBitmapFilterPrefix
} XSpriteExportFilter;

typedef struct _XTargaImportFilter {
   XBitmapFilterPrefix
} XTargaImportFilter;

typedef struct _XTargaExportFilter {
   XBitmapFilterPrefix

   UCHAR   cDirection;
} XTargaExportFilter;

typedef struct _XTIFFImportFilter {
   XBitmapFilterPrefix
                                       
   USHORT  uIndex;
} XTIFFImportFilter;

typedef struct _XTIFFExportFilter {
   XBitmapFilterPrefix
         
   ULONG   fLZW;
   ULONG   fMonoPal;
/* Use functions based on malloc/free to allocate the tags */
   PSZ     pszTagValue [BFP_TIFF_TAGCOUNT];
} XTIFFExportFilter;

typedef struct _XXBitmapImportFilter {
   XBitmapFilterPrefix
} XXBitmapImportFilter;

typedef struct _XXBitmapExportFilter {
   XBitmapFilterPrefix
} XXBitmapExportFilter;

typedef struct _XYUV12CImportFilter {
   XBitmapFilterPrefix
} XYUV12CImportFilter;

typedef struct _XYUV12CExportFilter {
   XBitmapFilterPrefix

   ULONG   ulPosX;
   ULONG   ulPosY;
} XYUV12CExportFilter;

#endif

#pragma pack (4)

