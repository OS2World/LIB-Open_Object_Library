#ifndef __OOL_XCDPlayer_H__
#define __OOL_XCDPlayer_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCDPlayer                                   */
/*                       derived from: XMediaWindow                          */
/*                        last update: 8/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xmedia.h"

class XFrameWindow;

typedef struct cdRecord
{
      ULONG startAddr;
      ULONG endAddr;
      USHORT trackNumber;
} cdRecord;


class _export_ XCDPlayer: public XMediaWindow{
   private:
      SHORT count;
      cdRecord * records;
   public:
      XCDPlayer( const XFrameWindow * w);
      ~XCDPlayer();
      LONG GetRecordCount() const { return count; }
      ULONG GetRecordLength( const USHORT index);
      ULONG PlayRecord( const USHORT record );
      void ReadRecordList();

};


#endif
