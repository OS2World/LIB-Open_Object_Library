#ifndef __OOL_XHELP_H__
#define __OOL_XHELP_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XHelpInstance                         */
/*                       derived from: XObject                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

class XResourceLibrary;
class XResource;
class XFrameWindow;
class XProcess;
class XDialog;


class _export_ XHelpInstance: public XObject
{
   private:
      HWND helpInstance;
   public:
      XHelpInstance( const char * path, const char * title = NULL, const XResource * helpTable = NULL, const XResourceLibrary * actionBarLib = NULL );
      virtual ~XHelpInstance();
      void AssociateWindow ( const XFrameWindow * ) const ;
      void AssociateWindow(const XDialog * ) const ;
      void ShowHelpContents( void ) const ;
      void ShowHelpForHelp( void ) const ;
      void ShowHelpForId( const LONG id ) const ;
      void ShowHelpIndex( void ) const ;
};


#endif
