#ifndef __OOL_XPROFILE_H__
#define __OOL_XPROFILE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XProfile                              */
/*                       derived from: XObject                               */
/*                        last update: 6/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"


class XString;

class _export_ XProfile: public XObject
{
   private:
      HINI hini;
      XString appName;
   public:
      XProfile() { hini=0;}
      ~XProfile( ) { if(hini) Close(); }
      BOOL Open( const char * appTitle, const char * fileName = "os2.ini" );
      BOOL Close( ) { ULONG res = PrfCloseProfile( hini); hini = 0; return res;}
      BOOL Read( const char * itemName, void * buffer, ULONG& size);
      BOOL Read( const char * itemName, XString * string);
      BOOL Read( const char * itemName, LONG& value) { ULONG size = sizeof(value); return Read(itemName, &value, size); }
      BOOL Read( const char * itemName, SHORT& value) {ULONG size = sizeof(value); return Read(itemName, &value, size); }
      BOOL Read( const char * itemName, BOOL& value) {ULONG size = sizeof(value); return Read(itemName, &value, size); }
      BOOL Write( const char * itemName, const void * buffer, const ULONG size);
      BOOL Write( const char * itemName, XString * string) { return Write(itemName, (char*) *string, string->GetLength()); }
      BOOL Write( const char * itemName, LONG value) { return Write(itemName, &value, sizeof(value)); }
      BOOL Write( const char * itemName, SHORT value) { return Write(itemName, &value, sizeof(value)); }
      BOOL Write( const char * itemName, BOOL value) { return Write(itemName, &value, sizeof(value)); }
};


#endif
