#ifndef __OOL_XCONTINFO_H__
#define __OOL_XCONTINFO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerInfo                        */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"

#define CO_TITLESEPARATOR CA_TITLESEPARATOR
#define CO_TITLEREADONLY  CA_TITLEREADONLY
#define CO_DETAILTITLES   CA_DETAILSVIEWTITLES
#define CO_TITLELEFT      CA_TITLELEFT
#define CO_TITLERIGHT     CA_TITLERIGHT
#define CO_TITLECENTER    CA_TITLECENTER
#define CO_TEXT    0x00000001
#define CO_NAME    0x00000002
#define CO_ICON    0x00000004
#define CO_DETAIL  0x00000008
#define CO_FLOW    0x00000010
#define CO_MINI    CV_MINI
#define CO_TREE    0x00000040
#define CO_OWNERPAINTBACKGROUND      0x00100000
#define CO_TREELINE         0x00400000
#define CO_DRAWBITMAP CA_DRAWBITMAP
#define CO_DRAWICON   CA_DRAWICON
#define CO_TITLE CA_CONTAINERTITLE

class XString;
class XContainerColumn;
class XBitmap;
class XIcon;
class XSize;

class _export_ XContainerInfo: public XObject
{
      friend class XContainerControl;
   private:
      CNRINFO cnrinfo;
      LONG changes;
   public:
      XContainerInfo( char * title = "", LONG attr = CO_ICON);
      virtual ~XContainerInfo() {;}
      void EnableBackgroundPainting( const BOOL enable = TRUE);
      void EnableSorting( const BOOL enable = TRUE);
      BOOL IsBackgroundPaintingEnabled() const ;
      LONG GetAttributes( void ) const ;
      void GetBitmapSize( XSize*);
      LONG GetSplitbarPos( void ) const { return cnrinfo.xVertSplitbar; }
      LONG GetObjectCount ( void ) const { return cnrinfo.cRecords; }
      void GetTitle( XString * s );
      void SetAttributes( const LONG attribute );
      void SetBitmapSize( const XSize*);
      void SetSplitbarPos( const LONG pos );
      void SetSplitbarColumn( const XContainerColumn * col);
      void SetTitle( const char * t );
      void SetTreeLineWidth( const SHORT w);
      void SetTreeLineSpacing( const SHORT s);
      void SetTreeBitmapSize( const XSize*);
      void SetExpandedBitmap( const XBitmap * );
      void SetExpandedBitmap( const XIcon * );
      void SetCollapsedBitmap( const XBitmap *);
      void SetCollapsedBitmap( const XIcon *);
      void SetSpacing( const SHORT w);
};

#endif
