#ifndef __OOL_XTOM_H__
#define __OOL_XTOM_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XAtom                                 */
/*                       derived from: XObject                               */
/*                        last update: 12/97                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"
#include "xexcept.h"

class _export_ XAtom: public XObject
{
      OOL_ATOMHANDLE atom;
   public:
      XAtom( const char * atomName, const BOOL failIfExist = FALSE)
      {
         HATOMTBL table = WinQuerySystemAtomTable();
         atom = WinFindAtom(table, (PSZ) atomName);
         if(atom)
         {
            if(failIfExist)
               throw(XException("atom exists", -20));
         } /* end if */
         else
            atom = WinAddAtom(table, (PSZ) atomName);
         if(failIfExist)
            throw(XException("couldn't create atom", -21));
      }
      OOL_ATOMHANDLE GetAtom() const { return atom; }
};


#endif
