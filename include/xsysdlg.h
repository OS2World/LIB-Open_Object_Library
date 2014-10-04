#ifndef __XSYSTEMDLG_H__
#define __XSYSTEMDLG_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSystemDialog                         */
/*                       derived from: XObject                               */
/*                        last update: 4/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"


class _export_ XSystemDialog: public XObject
{
   protected:
      SHORT command;
      XSystemDialog() { command = 0; }
   public:
      SHORT GetCommand() const { return command; }
};

#endif
