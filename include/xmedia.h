#ifndef __OOL_XMEDIA_H__
#define __OOL_XMEDIA_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMediaWindow                          */
/*                       derived from: XObjectWindow                         */
/*                        last update: 7/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobjwnd.h"


#define MEDIA_NOTIFY   0x0500

#define XVOLUME_BOTH 1

class XFrameWindow;

class _export_ XMediaWindow: public XObjectWindow{
   protected:
      USHORT mediaId;
      BOOL errorEnabled;
      void CloseDevice();
   public:
      XMediaWindow ( const XFrameWindow * );
      virtual ~XMediaWindow();
      LONG Acquire( void );
      LONG Pause( void );
      LONG Play( ULONG from = 0, ULONG to = 0 );
      LONG Rewind( void );
      virtual LONG SetDataFile( const char * p);
      LONG SetVolume( const USHORT vol, const USHORT chanel = XVOLUME_BOTH);
      LONG Stop(void);
};

#endif
