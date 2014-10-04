/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                            library: OOLGLIB                               */
/*                        last update: 3/98                                  */
/*                      programmed by: Mathias Hasselmann                    */
/*                                     mhasselm@arco.met.fu-berlin.de        */
/*===========================================================================*/

USHORT  GetLanguageBase ();

ULONG   BitmapFilterError (ULONG, char*, ULONG, XBitmapFilter*);
ULONG   BitmapFilterExtError (ULONG, char*, ULONG, XBitmapFilter*, ULONG, ULONG);

ULONG   CreateBitmapInfo (PBITMAPINFO2*, ULONG, ULONG, UCHAR);
ULONG   TrueColorExpansion (PBITMAPINFO2*, PBITMAPINFO2);
ULONG   Remap (PBITMAPINFO2, PBITMAPINFO2*, XBitmapFilter*);

UCHAR   GetFilterID (PSZ, XBitmapFilterProfile*);
UCHAR   AdjustColorBits (UCHAR, UCHAR);
ULONG   GetPaletteMask (UCHAR, UCHAR, UCHAR);
ULONG   GetDefaultPalette (UCHAR, UCHAR);
ULONG   GetDitheringMask (UCHAR, UCHAR);

void    InitFileTypeMapping (XBitmapFilterProfile* profile);
ULONG   GetProfileSize ();

MRESULT FilterSetupWinProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT ColorSetupWinProc (HWND, ULONG, MPARAM, MPARAM);
MRESULT BitmapFilterDialog (HWND, XBitmapFilterPageParameter*, USHORT, PFNWP);

ULONG          CheckImportFilter (XBitmapFilter*);
ULONG            GetImportFilterSize (UCHAR);
PSZ              GetImportFilterString (XBitmapFilter*);
PFNWP            GetImportWinProc (UCHAR cFilter);
ULONG            GetExportFilterSize (UCHAR);
ULONG          CheckExportFilter  (XBitmapFilter*);
PSZ              GetExportFilterString (XBitmapFilter*);
PFNWP            GetExportWinProc (UCHAR cFilter);

ULONG     InitBitmapImportFilter (XBitmapFilter*, char*);
PSZ        GetBitmapImportFilterString (XBitmapFilter*);
ULONG    CheckBitmapImportFilter (XBitmapFilter*);
ULONG     InitBitmapExportFilter (XBitmapFilter*, char*);
PSZ        GetBitmapExportFilterString (XBitmapFilter*);
ULONG    CheckBitmapExportFilter (XBitmapFilter*);

ULONG        InitGEMImportFilter (XBitmapFilter*, char*);
PSZ           GetGEMImportFilterString (XBitmapFilter*);
ULONG       CheckGEMImportFilter (XBitmapFilter*);
ULONG        InitGEMExportFilter (XBitmapFilter*, char*);
PSZ           GetGEMExportFilterString (XBitmapFilter*);
ULONG       CheckGEMExportFilter (XBitmapFilter*);

ULONG        InitGIFImportFilter (XBitmapFilter*, char*);
PSZ           GetGIFImportFilterString (XBitmapFilter*);
ULONG       CheckGIFImportFilter (XBitmapFilter*);
ULONG        InitGIFExportFilter (XBitmapFilter*, char*);
PSZ           GetGIFExportFilterString (XBitmapFilter*);
ULONG       CheckGIFExportFilter (XBitmapFilter*);

ULONG    InitGreymapImportFilter (XBitmapFilter*, char*);
PSZ       GetGreymapImportFilterString (XBitmapFilter*);
ULONG   CheckGreymapImportFilter (XBitmapFilter*);
ULONG    InitGreymapExportFilter (XBitmapFilter*, char*);
PSZ       GetGreymapExportFilterString (XBitmapFilter*);
ULONG   CheckGreymapExportFilter (XBitmapFilter*);

ULONG        InitIAXImportFilter (XBitmapFilter*, char*);
PSZ           GetIAXImportFilterString (XBitmapFilter*);
ULONG       CheckIAXImportFilter (XBitmapFilter*);
ULONG        InitIAXExportFilter (XBitmapFilter*, char*);
PSZ           GetIAXExportFilterString (XBitmapFilter*);
ULONG       CheckIAXExportFilter (XBitmapFilter*);

ULONG       InitILBMImportFilter (XBitmapFilter*, char*);
PSZ          GetILBMImportFilterString (XBitmapFilter*);
ULONG      CheckILBMImportFilter (XBitmapFilter*);
ULONG       InitILBMExportFilter (XBitmapFilter*, char*);
PSZ          GetILBMExportFilterString (XBitmapFilter*);
ULONG      CheckILBMExportFilter (XBitmapFilter*);

ULONG       InitJPEGImportFilter (XBitmapFilter*, char*);
PSZ          GetJPEGImportFilterString (XBitmapFilter*);
ULONG      CheckJPEGImportFilter (XBitmapFilter*);
ULONG       InitJPEGExportFilter (XBitmapFilter*, char*);
PSZ          GetJPEGExportFilterString (XBitmapFilter*);
ULONG      CheckJPEGExportFilter (XBitmapFilter*);

ULONG       InitKIPSImportFilter (XBitmapFilter*, char*);
PSZ          GetKIPSImportFilterString (XBitmapFilter*);
ULONG      CheckKIPSImportFilter (XBitmapFilter*);
ULONG       InitKIPSExportFilter (XBitmapFilter*, char*);
PSZ          GetKIPSExportFilterString (XBitmapFilter*);
ULONG      CheckKIPSExportFilter (XBitmapFilter*);

ULONG        InitPCXImportFilter (XBitmapFilter*, char*);
PSZ           GetPCXImportFilterString (XBitmapFilter*);
ULONG       CheckPCXImportFilter (XBitmapFilter*);
ULONG        InitPCXExportFilter (XBitmapFilter*, char*);
PSZ           GetPCXExportFilterString (XBitmapFilter*);
ULONG       CheckPCXExportFilter (XBitmapFilter*);

ULONG     InitPixmapImportFilter (XBitmapFilter*, char*);
PSZ        GetPixmapImportFilterString (XBitmapFilter*);
ULONG    CheckPixmapImportFilter (XBitmapFilter*);
ULONG     InitPixmapExportFilter (XBitmapFilter*, char*);
PSZ        GetPixmapExportFilterString (XBitmapFilter*);
ULONG    CheckPixmapExportFilter (XBitmapFilter*);

ULONG   InitPortraitImportFilter (XBitmapFilter*, char*);
PSZ      GetPortraitImportFilterString (XBitmapFilter*);
ULONG  CheckPortraitImportFilter (XBitmapFilter*);
ULONG   InitPortraitExportFilter (XBitmapFilter*, char*);
PSZ      GetPortraitExportFilterString (XBitmapFilter*);
ULONG   CheckPortraitExportFilter (XBitmapFilter*);

ULONG       InitPSegImportFilter (XBitmapFilter*, char*);
PSZ          GetPSegImportFilterString (XBitmapFilter*);
ULONG      CheckPSegImportFilter (XBitmapFilter*);
ULONG       InitPSegExportFilter (XBitmapFilter*, char*);
PSZ          GetPSegExportFilterString (XBitmapFilter*);
ULONG      CheckPSegExportFilter (XBitmapFilter*);

ULONG     InitSpriteImportFilter (XBitmapFilter*, char*);
PSZ        GetSpriteImportFilterString (XBitmapFilter*);
ULONG    CheckSpriteImportFilter (XBitmapFilter*);
ULONG     InitSpriteExportFilter (XBitmapFilter*, char*);
PSZ        GetSpriteExportFilterString (XBitmapFilter*);
ULONG    CheckSpriteExportFilter (XBitmapFilter*);

ULONG      InitTargaImportFilter (XBitmapFilter*, char*);
PSZ         GetTargaImportFilterString (XBitmapFilter*);
ULONG     CheckTargaImportFilter (XBitmapFilter*);
ULONG      InitTargaExportFilter (XBitmapFilter*, char*);
PSZ         GetTargaExportFilterString (XBitmapFilter*);
ULONG     CheckTargaExportFilter (XBitmapFilter*);

ULONG       InitTIFFImportFilter (XBitmapFilter*, char*);
PSZ          GetTIFFImportFilterString (XBitmapFilter*);
ULONG      CheckTIFFImportFilter (XBitmapFilter*);
ULONG       InitTIFFExportFilter (XBitmapFilter*, char*);
PSZ          GetTIFFExportFilterString (XBitmapFilter*);
ULONG      CheckTIFFExportFilter (XBitmapFilter*);

ULONG    InitXBitmapImportFilter (XBitmapFilter*, char*);
PSZ       GetXBitmapImportFilterString (XBitmapFilter*);
ULONG   CheckXBitmapImportFilter (XBitmapFilter*);
ULONG    InitXBitmapExportFilter (XBitmapFilter*, char*);
PSZ       GetXBitmapExportFilterString (XBitmapFilter*);
ULONG   CheckXBitmapExportFilter (XBitmapFilter*);

ULONG     InitYUV12CImportFilter (XBitmapFilter*, char*);
PSZ        GetYUV12CImportFilterString (XBitmapFilter*);
ULONG    CheckYUV12CImportFilter (XBitmapFilter*);
ULONG     InitYUV12CExportFilter (XBitmapFilter*, char*);
PSZ        GetYUV12CExportFilterString (XBitmapFilter*);
ULONG    CheckYUV12CExportFilter (XBitmapFilter*);

