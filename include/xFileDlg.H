#ifndef __AFILEDLG_H__
#define __AFILEDLG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFileDialog                           */
/*                       derived from: XObject                               */
/*                        last update: 3/98                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*                      Win32-port by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xsysdlg.h"
#include "xstring.h"

#ifdef OOL_FOR_OS2_X86
   #define FD_OPEN           FDS_OPEN_DIALOG
   #define FD_CENTER         FDS_CENTER
   #define FD_SAVEAS         FDS_SAVEAS_DIALOG
   #define FD_MULTIPLESEL    FDS_MULTIPLESEL
   #define FD_HELP           FDS_HELPBUTTON
   #define FD_MODELESS       FDS_MODELESS
   #define FD_APPLYBUTTON    FDS_APPLYBUTTON
   //#define FD_EAS            FDS_INCLUDE_EAS
   #define FD_FILTERUNION    FDS_FILTERUNION
   #define FD_HELPBUTTON   FDS_HELPBUTTON
#endif


#ifdef OOL_FOR_WIN32_X86
   #include <commdlg.h>
   #define FD_OPEN           0x00400000
   #define FD_CENTER         0x00800000
   #define FD_SAVEAS         2
   #define FD_MULTIPLESEL    OFN_ALLOWMULTISELECT
   //#define FD_HELP           FDS_HELPBUTTON
   //#define FD_MODELESS       FDS_MODELESS
   //#define FD_APPLYBUTTON    FDS_APPLYBUTTON
   //#define FD_EAS            FDS_INCLUDE_EAS
   //#define FD_FILTERUNION    FDS_FILTERUNION
   //#define FD_HELPBUTTON   FDS_HELPBUTTON
#endif

class XWindow;


class _export_ XFileDialog: public XSystemDialog
{
friend class XGLibFileDialog;
   private:
      XString fileName;
#ifdef OOL_FOR_OS2_X86
      PAPSZ pap;
#endif
      USHORT numbers;
   protected:
      ULONG Start( const XWindow * owner, const char *f, const char *title, const char *fileType, const LONG options, char ** typeList, FNWP* dlgProc, ULONG ulUser );
   public:
      XFileDialog() {}
      XFileDialog( const XWindow * owner, const char * filename ="*", const char * title = NULL, const char * fileType = NULL, const LONG options = FD_OPEN | FD_CENTER, char ** typeList = NULL, FNWP* dlgProc = NULL, ULONG ulUser = 0L);
      virtual ~XFileDialog();
#ifdef OOL_FOR_OS2_X86
      void GetFileName( XString * st, const USHORT index = 0) { *st = (index == 0 ? (char*) fileName : (char*) *pap[index]); }
#endif
#ifdef OOL_FOR_WIN32_X86
      void GetFileName( XString * st, const USHORT index = 0);
#endif
      SHORT GetFileCount( void ) const { return numbers; }
};

#endif
