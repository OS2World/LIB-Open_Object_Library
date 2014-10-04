#ifndef __OOL_XPINBTN_H__
#define __OOL_XPINBTN_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XSpinButton                           */
/*                       derived from: XControl                              */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xcontrol.h"

#define SP_LEFT     SPBS_JUSTLEFT
#define SP_RIGHT    SPBS_JUSTRIGHT
#define SP_CENTER   SPBS_JUSTCENTER
#define SP_NOBORDER SPBS_NOBORDER
#define SP_FAST     SPBS_FASTDSPIN
#define SP_MASTER   SPBS_MASTER
#define SP_SERVANT  SPBS_SERVANT
#define SP_READONLY SPBS_READONLY
#define SP_NUMERIC  SPBS_NUMERICONLY
#define SP_CHAR     SPBS_ALLCHARACTERS
#define SP_FILLZERO SPBS_PADWITHZEROS


class _export_ XSpinButton: public XControl{
      friend void BuildChilds( HWND );
   private:
      XSpinButton( const OOL_WINDOWHANDLE hwnd): XControl(hwnd) {;}
   public:
      XSpinButton( const XWindow * owner, const XRect& rec=XRect(), const USHORT id = 0, const ULONG style=SP_NUMERIC|SP_MASTER|SP_RIGHT|WIN_VISIBLE, const char * string="", const char * font = NULL);
      virtual ~XSpinButton() {;}
      LONG GetValue( void) const;
      void SetLimits( const LONG lowerLimit, const LONG upperLimit) const ;
      BOOL SetMaster( const XWindow * master) const ;
      void SetTextLimit( const SHORT limit) const ;
      void SetValue( const LONG val) const ;
      void SpinDown( const LONG units) const ;
      void SpinUp( const LONG units) const ;
};

#endif
