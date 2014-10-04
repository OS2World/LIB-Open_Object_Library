/* memory observation routines, (c)opyright Stefan von Brauk, 1998 */
/* STL required */
#define __CREATE_DLL__

#include "xmemchk.h"
#include "stdio.h"
#include <map.h>
#include <stdlib.h>
#include <string.h>


class MemoryObject
{
      char * fileName;
      int lineNumber;
      int length;
      void * pointer;
   public:
      int GetLength() { return length; }
      MemoryObject( void * ptr, size_t len, const char * file, const int line )
         {
            pointer = ptr;
            length = len;
            int nameLength = strlen(file) + 1;
            fileName = (char*) malloc( nameLength);
            memcpy( fileName, file, nameLength);
            lineNumber = line;
         }
      MemoryObject operator =(const MemoryObject &obj)
         {
            pointer = obj.pointer;
            length = obj.length;
            int nameLength = strlen(obj.fileName) + 1;
            fileName = (char*) malloc( nameLength);
            memcpy( fileName, obj.fileName, nameLength);
            lineNumber = obj.lineNumber;
            return *this;
         }
      MemoryObject() {}
      void Dump()
         {
            char * s = (char*) pointer;
            fprintf( stderr, "%s(%i): size: %i, content: \"", fileName, lineNumber, length);
            int i = 0;
            while (i < length)
            {
               switch(s[i])
               {
                  case 0:
                     fprintf( stderr, " ");
                     break;
                  case 13:
                     fprintf( stderr, "\\n");
                     break;
                  case 10:
                     fprintf( stderr, "\\r");
                     break;
                  default:
                     fprintf( stderr, "%c", s[i]);
               }
               if(i == 50 )
               {
                  fprintf( stderr, "\"(first 50 bytes)");
                  break;
               }
               else if(i == length-1)
                  fprintf( stderr, "\"");
               i++;
            }
            fprintf( stderr, "\n");
         }
};

map<void*, MemoryObject, less<void*> > memoryObjects;

#if !defined( __WATCOMC__ )
void dump_memcheck()
#else
void __export dump_memcheck()
#endif
{
   fprintf( stderr, "\n\ndumping not freed memory objects:\n" );
   fprintf( stderr,   "-------------------------------------------------------------------------------------\n" );
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.begin();
   while( it != memoryObjects.end())
   {
      (*it).second.Dump();
      it++;
   }
   fprintf( stderr,   "-------------------------------------------------------------------------------------\n" );
   fprintf( stderr, "all objects dumped\n" );
   fflush( stderr);
}


#if !defined( __WATCOMC__ )
void * memcheck_malloc( size_t length, const char * file, const int line)
#else
void __export * memcheck_malloc( size_t length, const char * file, const int line)
#endif
{
   if(length == 0 && file)
   {
      fprintf( stderr, "\n%s(%i): zero length allocation", file, line );
      fflush(stderr);
   }
   void * ptr = malloc( length );
   MemoryObject obj(ptr, length, (file != NULL ? file : ""), line);
   memoryObjects[ptr] = obj;
   return ptr;
}


#if !defined( __WATCOMC__ )
void * memcheck_calloc( size_t num, size_t length, const char * file, const int line)
#else
void __export * memcheck_calloc( size_t num, size_t length, const char * file, const int line)
#endif
{
   if(num == 0 && file)
   {
      fprintf( stderr, "\n%s(%i): zero elements allocation", file, line );
      fflush(stderr);
   }
   void * ptr = calloc( num, length );
   MemoryObject obj(ptr, length*num, (file != NULL ? file : ""), line);
   memoryObjects[ptr] = obj;
   return ptr;
}


#if !defined( __WATCOMC__ )
void * memcheck_realloc( void * dest, size_t length, const char * file, const int line)
#else
void __export * memcheck_realloc( void * dest, size_t length, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );
   if( it != memoryObjects.end())
      memoryObjects.erase(it);
   void * ptr = realloc( dest, length);
   MemoryObject obj(ptr, length, (file != NULL ? file : ""), line);
   memoryObjects[ptr] = obj;
   return ptr;
}



#if !defined( __WATCOMC__ )
void * memcheck_memcpy( void * dest, void * src, size_t count, const char * file, const int line)
#else
void __export * memcheck_memcpy( void * dest, void * src, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for memcpy is not large enough (target size: %i, requested for copy: %i), memcpy aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }

   it = memoryObjects.find( src );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): source for memcpy is not large enough (source size: %i, requested for copy: %i), memcpy aborted", file, line, (*it).second.GetLength(), count);
         fflush(stderr);
         return NULL;
      }
   }
   return memcpy( dest, src, count);
}


#if !defined( __WATCOMC__ )
void memcheck_free( void * dest, const char * file, const int line )
#else
void __export memcheck_free( void * dest, const char * file, const int line )
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );

   if( it != memoryObjects.end())
      memoryObjects.erase(it);
   free(dest);
}


#if !defined( __WATCOMC__ )
void *  memcheck_memset( void * dest, int c, size_t count, const char * file, const int line)
#else
void __export *  memcheck_memset( void * dest, int c, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for memset is not large enough (target size: %i, requested for set: %i), memset aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr);
         return NULL;
      }
   }
   return memset( dest, c, count);
}


#if !defined( __WATCOMC__ )
void *  memcheck_memmove( void * dest, void * src, size_t count, const char * file, const int line)
#else
void __export *  memcheck_memmove( void * dest, void * src, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for memmove is not large enough (target size: %i, requested for copy: %i), memmove aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }

   it = memoryObjects.find( src );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): source for memmove is not large enough (source size: %i, requested for copy: %i), memmove aborted", file, line, (*it).second.GetLength(), count);
         fflush(stderr);
         return NULL;
      }
   }
   return memcpy( dest, src, count);
}


#if !defined( __WATCOMC__ )
void *  memcheck_memcmp( void * dest, void * src, size_t count, const char * file, const int line)
#else
void __export *  memcheck_memcmp( void * dest, void * src, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( dest );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for memcmp is not large enough (target size: %i, requested for compare: %i), memcmp aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }

   it = memoryObjects.find( src );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): source for memcmp is not large enough (source size: %i, requested for compare: %i), memcmp aborted", file, line, (*it).second.GetLength(), count);
         fflush(stderr);
         return NULL;
      }
   }
   return memcpy( dest, src, count);
}


#if !defined( __WATCOMC__ )
char *  memcheck_strcpy( char * string1, const char * string2, const char * file, const int line)
#else
char __export *  memcheck_strcpy( char * string1, const char * string2, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( string1 );
   if( it != memoryObjects.end())
   {
      int count = strlen( string2 ) + 1;
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for strcpy is not large enough (target size: %i, requested for copy (including NULL): %i), strcpy aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }
   return strcpy( string1, string2);
}


#if !defined( __WATCOMC__ )
char *  memcheck_strcat( char * string1, const char * string2, const char * file, const int line)
#else
char __export *  memcheck_strcat( char * string1, const char * string2, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( string1 );
   if( it != memoryObjects.end())
   {
      int count = strlen( string2 ) + 1 + strlen(string1);
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for strcat is not large enough (target size: %i, requested for concat (including NULL): %i), strcat aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }
   return strcpy( string1, string2);
}


#if !defined( __WATCOMC__ )
char *  memcheck_strncpy( char * string1, const char * string2, size_t count, const char * file, const int line)
#else
char __export *  memcheck_strncpy( char * string1, const char * string2, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( string1 );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): target for strncpy is not large enough (target size: %i, requested for copy: %i), strncpy aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }
   int c = strlen(string2) + 1;
   if( c < count)
   {
      fprintf( stderr, "\n%s(%i): string2 for strncpy is not large enough (size: %i, requested for copy: %i), strncpy aborted", file, line, c, count);
      fflush( stderr );
      return NULL;
   } /* end if */
   return strncpy( string1, string2, count);
}


#if !defined( __WATCOMC__ )
int memcheck_strncmp( char * string1, const char * string2, size_t count, const char * file, const int line)
#else
int __export memcheck_strncmp( char * string1, const char * string2, size_t count, const char * file, const int line)
#endif
{
   map<void*, MemoryObject, less<void*> > ::iterator it = memoryObjects.find( string1 );
   if( it != memoryObjects.end())
   {
      if( (*it).second.GetLength() < count )
      {
         fprintf( stderr, "\n%s(%i): string1 for strncmp is not large enough (size: %i, requested for compare: %i), strncmp aborted", file, line, (*it).second.GetLength(), count);
         fflush( stderr );
         return NULL;
      }
   }
   int c = strlen(string2) + 1;
   if( c < count)
   {
      fprintf( stderr, "\n%s(%i): string2 for strncmp is not large enough (size: %i, requested for compare: %i), strncmp aborted", file, line, c, count);
      fflush( stderr );
      return NULL;
   } /* end if */
   return strncmp( string1, string2, count);
}


/*@
@class XMemoryCheck
@type overview
@symbol _
@remarks XMemoryCheck is a class to observe memory-manipulating routines, eg.
alloc/free memory, copy memory-blocks etc. It also cheks string routines like strcpy().
To enable this feature create an object of this class when your program starts,
detected errors are printed to stderr. <B>XMemoryCheck does not observe
the new() and delete() operators!</B>
@example
void main ( int argc, void ** argv)
{
XMemoryCheck memObj;
XConsole::CheckConsole(argc, argv);
MyAppWindow * window = new MyAppWindow();   //create new window
XApplication::GetApplication()->Start();  //let the application work
}
*/
