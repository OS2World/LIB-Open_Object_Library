#ifndef __XGLIBFILEDIALOG_H__
#define __XGLIBFILEDIALOG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XGLibFileDialog                       */
/*                       derived from: XFileDialog                           */
/*                        last update: 3/98                                  */
/*                      programmed by: Mathias Hasselmann                    */
/*                                     mhasselm@arco.met.fu-berlin.de        */
/*===========================================================================*/

#include "xfiledlg.h"

class XResourceLibrary;

#define FD_OPTIONSBUTTON FDS_APPLYBUTTON

class _export_ XGLibFileDialog: public XFileDialog {
   private:
      XResourceLibrary* pResLib;
      FNWP* pDlgProc;
      struct _XBitmapFilter* pFilter;
   public:
      XGLibFileDialog( const XWindow * owner, const char * filename ="*", const char * title = NULL, const char * fileType = NULL, const LONG options = FD_OPEN | FD_CENTER);
      ~XGLibFileDialog ();
      XBitmapFilter* GetFilter () { return pFilter; }
};

#endif
