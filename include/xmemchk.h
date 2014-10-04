#ifndef __XMEMCHECK_H__
#define __XMEMCHECK_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMemoryCheck                          */
/*                       derived from: -                                     */
/*                        last update: 9/97                                  */
/*                      programmed by: Stefan von Brauk                      */
/*===========================================================================*/


#include <stdlib.h>
#include <string.h>
#include "xobject.h"

#if __cplusplus
extern "C" {
#endif

#if !defined( __WATCOMC__ )
   void dump_memcheck();
   void * memcheck_calloc( size_t, size_t, const char *, const int);
   void * memcheck_malloc( size_t, const char *, const int);
   void * memcheck_realloc( void*, size_t, const char *, const int);
   void memcheck_free( void*, const char *, const int);
   void * memcheck_memcmp( void * buf1, void * buf2, size_t count, const char * file, const int line);
   void * memcheck_memcpy( void * dest, void * src, size_t count, const char * file, const int line);
   void * memcheck_memmove( void * dest, void * src, size_t count, const char * file, const int line);
   void * memcheck_memset( void * dest, int c, size_t count, const char * file, const int line);
   char * memcheck_strcat( char * string1, const char * string2, const char * file, const int line);
   char * memcheck_strcpy( char * string1, const char * string2, const char * file, const int line);
   char * memcheck_strncpy( char * string1, const char * string2, size_t count, const char * file, const int line);
   int memcheck_strncmp( char * string1, const char * string2, size_t count, const char * file, const int line);
#else
   void __export dump_memcheck();
   void __export * memcheck_calloc( size_t, size_t, const char *, const int);
   void __export * memcheck_malloc( size_t, const char *, const int);
   void __export * memcheck_realloc( void*, size_t, const char *, const int);
   void __export memcheck_free( void*, const char *, const int);
   void __export * memcheck_memcmp( void * buf1, void * buf2, size_t count, const char * file, const int line);
   void __export * memcheck_memcpy( void * dest, void * src, size_t count, const char * file, const int line);
   void __export * memcheck_memmove( void * dest, void * src, size_t count, const char * file, const int line);
   void __export * memcheck_memset( void * dest, int c, size_t count, const char * file, const int line);
   char __export * memcheck_strcat( char * string1, const char * string2, const char * file, const int line);
   char __export * memcheck_strcpy( char * string1, const char * string2, const char * file, const int line);
   char __export * memcheck_strncpy( char * string1, const char * string2, size_t count, const char * file, const int line);
   int __export memcheck_strncmp( char * string1, const char * string2, size_t count, const char * file, const int line);
#endif

#if __cplusplus
};
#endif


#ifndef __CREATE_DLL__
#ifdef DEBUG
   #define calloc(num, length)                     memcheck_calloc( (int) num, (int) length, __FILE__, __LINE__)
   #define malloc(length)                          memcheck_malloc( length, __FILE__, __LINE__)
   #define realloc(pointer,length)                 memcheck_realloc( pointer, length, __FILE__, __LINE__)
   #define free(pointer)                           memcheck_free( pointer, __FILE__, __LINE__ )
   #define memcmp(dest, src, count)                memcheck_memcmp( dest, src, count, __FILE__, __LINE__ )
   #define memcpy(dest, src, count)                memcheck_memcpy( dest, (void*) src, (int) count, __FILE__, __LINE__ )
   #define memmove(dest, src, count)               memcheck_memmove( dest, src, count, __FILE__, __LINE__ )
   #define memset(dest, c, count)                  memcheck_memset( dest, c, count, __FILE__, __LINE__ )
   #define strcat(string1, string2)                memcheck_strcat( string1, string2, __FILE__, __LINE__ )
   #define strcpy(string1, string2)                memcheck_strcpy( string1, string2, __FILE__, __LINE__ )
   #define strncpy(string1, string2, count)        memcheck_strncpy( string1, string2, count, __FILE__, __LINE__ )
   #define strncmp(string1, string2, count)        memcheck_strncmp( string1, string2, count, __FILE__, __LINE__ )
#endif

#if __cplusplus

class XMemoryCheck
{
   public:
      XMemoryCheck() {;}
      ~XMemoryCheck() { dump_memcheck(); }
};
#endif
#endif
#endif
