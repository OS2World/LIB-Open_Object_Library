#include <stdlib.h>
#include "xcrypt.h"

extern "C" {
#include "md5.h"
char *crypt( char *key, char *salt);
char *crypt16( char *key, char *salt);
void encrypt( char *block, int edflag);
}

XString XCrypt :: BuildMD5Key( const XString& userData)
{
   XString buffer;
   MD5_CTX mdContext;

   MD5Init (&mdContext);
   MD5Update (&mdContext, (unsigned char*) (char*) userData, userData.GetLength());
   MD5Final (&mdContext);

   buffer.GetBuffer(18);

   for (int i = 0; i < 16; i++)
      buffer[i] = mdContext.digest[i]; //printf ("%02x", mdContext->digest[i]);

   buffer.ReleaseBuffer();
   return buffer;
}


XString XCrypt :: Crypt16( const XString& data, const XString& key)
{
   XString result;
   result = crypt16( data, key );
   return result;
}

