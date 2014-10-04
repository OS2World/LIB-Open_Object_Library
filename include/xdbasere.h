#ifndef __XDBASERECORD_H__
#define __XDBASERECORD_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XDBaseRecord                          */
/*                       derived from: XObject                               */
/*                        last update: 5/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xdbasefi.h"

class _export_ XDBaseRecord: public XObject
{
   friend class XDBaseFile;
   private:
      ULONG index;
      char * buffer;
      BOOL isNew, changed, deleted;
   protected:
      XDBaseFile * owner;
   public:
      XDBaseRecord( XDBaseFile * f, ULONG index = 0);
      ~XDBaseRecord();
      BOOL GetFieldData( USHORT index, XDate& );
      BOOL GetFieldData( USHORT index, LONG& );
      BOOL GetFieldData( USHORT index, double& );
      BOOL GetFieldData( USHORT index, XString& );
      BOOL GetFieldData( USHORT i, BOOL& data);
      BOOL IsChanged() { return changed; }
      BOOL IsDeleted() { return deleted; }
      BOOL IsNew() { return isNew; }
      BOOL Remove();
      BOOL Save();
      BOOL SetFieldData( USHORT index, XString& data);
      BOOL SetFieldData( USHORT index, XDate& date);
      BOOL SetFieldData( USHORT index, LONG data);
      BOOL SetFieldData( USHORT index, double data);
      BOOL SetFieldData( USHORT i, BOOL data);
};

#endif
