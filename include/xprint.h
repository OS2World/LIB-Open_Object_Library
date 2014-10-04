#ifndef __XPRINT_H__
#define __XPRINT_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XPrinterDevice                        */
/*                       derived from: XGraphicDevice                        */
/*                        last update: 1/97                                  */
/*       based on the code of the MAVERIK CLASS LIBRARY by Fabrizio AVERSA     */
/*                          ported by: Stefan von Brauk                      */
/*===========================================================================*/

#include "xobject.h"
#include "xgraphdv.h"
#include "xsize.h"
#include "xfont.h"

typedef struct _PRINTERSETUP
{
   LONG           lWorldCoordinates;
   HAB            hab;
   CHAR           szPreferredQueue[ 64 ];
   PDRIVDATA      pDriverData;
   HDC            hdcPrinterInfo;
   HPS            hpsPrinterInfo;
   LONG           lDCType;
   PDEVOPENDATA   pDevOpenData;
   DEVOPENSTRUC   devopenstruc;
   LONG           cQueues;
   PPRQINFO3      pQueueInfo;
   BOOL           fToFile;
   CHAR           szFileName[ CCHMAXPATH ];
} PRINTERSETUP;
typedef PRINTERSETUP *PPRINTERSETUP;


class _export_ XPrinterDevice: public XGraphicDevice
{
   private:
      PPRINTERSETUP pSetup;
      //SIZEL sizPage;
      PPRQINFO3 FindQueue( PPRINTERSETUP );
   public:
      XPrinterDevice(const XFrameWindow*, const LONG res = PU_LOMETRIC);
      ~XPrinterDevice();
//      virtual void Draw(void);
      void GetPageSize( XSize * s) const { s->SetWidth(width); s->SetHeight(height);}
      //PDEVOPENDATA GetDevOpenData(void) {return pSetup->pDevOpenData;}
      //PDRIVDATA GetDrivData(void) {return pSetup->pDriverData;}
      void CleanupPrinter(void);
      void ClosePrinterJob(void);
      void KillPrinterJob(void);
      BOOL NewPage(void) const;
      BOOL OpenPrinterJob(const char *);
      virtual void SetColor( const LONG c) { return; }
      virtual void SetFont( XFont * font) { EnableFont(font, font->fontSize); }
      BOOL SetupPrinter(const char *, const XFrameWindow *, XString*, XString*);
};

#endif
