#ifndef __OOL_XIO_H__
#define __OOL_XIO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XIO                                   */
/*                       derived from: XObject                               */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"
#include "xstring.h"
#include "xdate.h"
#include "xtime.h"
#include "xfilefi.h"

#ifdef OOL_FOR_OS2_X86
   //seek relative positions
   #define XFILE_BEGIN                   0
   #define XFILE_CURRENT                 1
   #define XFILE_END                     2
#endif


#ifdef OOL_FOR_OS2_X86
   //modeForOpen, can be or-ed
   #define XFILE_FAIL_IF_NEW             OPEN_ACTION_FAIL_IF_NEW
   #define XFILE_CREATE_IF_NEW           OPEN_ACTION_CREATE_IF_NEW
   #define XFILE_REPLACE_EXISTING        OPEN_ACTION_REPLACE_IF_EXISTS
   #define XFILE_OPEN_EXISTING           OPEN_ACTION_OPEN_IF_EXISTS
   #define XFILE_FAIL_EXISTING           OPEN_ACTION_FAIL_IF_EXISTS

   //modeForAccess, can be or-ed
   #define XFILE_SHARE_DENYREAD          OPEN_SHARE_DENYREAD
   #define XFILE_SHARE_DENYWRITE         OPEN_SHARE_DENYWRITE
   #define XFILE_SHARE_DENYREADWRITE     OPEN_SHARE_DENYREADWRITE
   #define XFILE_SHARE_DENYNONE          OPEN_SHARE_DENYNONE
   #define XFILE_READONLY                OPEN_ACCESS_READONLY
   #define XFILE_WRITEONLY               OPEN_ACCESS_WRITEONLY
   #define XFILE_READWRITE               OPEN_ACCESS_READWRITE
#endif

#ifdef OOL_FOR_WIN32_X86
   //modeForOpen, can be or-ed
   #define XFILE_FAIL_IF_NEW             OPEN_EXISTING
   #define XFILE_CREATE_IF_NEW           CREATE_NEW
   #define XFILE_REPLACE_EXISTING        TRUNCATE_EXISTING
   #define XFILE_OPEN_EXISTING           OPEN_EXISTING
   #define XFILE_FAIL_EXISTING           0

   //modeForAccess, can be or-ed
   #define XFILE_SHARE_DENYREAD          FILE_SHARE_READ
   #define XFILE_SHARE_DENYWRITE         FILE_SHARE_WRITE
   #define XFILE_SHARE_DENYREADWRITE     FILE_SHARE_READ | FILE_SHARE_WRITE
   #define XFILE_SHARE_DENYNONE          0
   #define XFILE_READONLY                GENERIC_READ
   #define XFILE_WRITEONLY               GENERIC_WRITE
   #define XFILE_READWRITE               GENERIC_READ|GENERIC_WRITE
#endif


class _export_ XIO: public XObject
{
   protected:
      HFILE handle;
      ULONG pointerPos;
      XIO() { handle = 0; pointerPos = 0; isOpen=FALSE; }
      ~XIO() { Close();}
      BOOL isOpen;
   public:
      virtual void Close()
         {
            DosClose(handle);
            if( isOpen )
            {
               isOpen = FALSE;
            }
         }
      BOOL IsOpen() const { return isOpen; }
      ULONG GetPointerPos( void ) const { return pointerPos; }
      virtual ULONG _Read( void * buffer, const ULONG sizeofbuffer)
            {
               ULONG r;
               DosRead(handle, (PVOID) buffer, sizeofbuffer, &r);
               return r;
            }
      ULONG Read( void * buffer, const ULONG sizeofbuffer) { return _Read(buffer, sizeofbuffer); }
      ULONG Read( char& value ) { return _Read( &value, sizeof(char) ); }
      ULONG Read( SHORT& value ) { return _Read( &value, sizeof(SHORT) ); }
      ULONG Read( LONG& value ) { return _Read( &value, sizeof(LONG) ); }
      ULONG Read( XDate& date ) { return _Read( &date.d, sizeof(date.d) ); }
      ULONG Read( XTime& time ) { return _Read( &time.t, sizeof(time.t) ); }
      ULONG Read( XString& string, const ULONG length)
            {
               ULONG r = _Read( string.GetBuffer( (ULONG) length + 1), length);
               string.ReleaseBuffer(r);
               return r;
            }
      ULONG Seek( const LONG pos, const ULONG relativePos = XFILE_BEGIN){ return DosSetFilePtr(handle, pos, relativePos, &pointerPos); }
      virtual ULONG _Write( const void * buffer, const ULONG sizeofbuffer) const
            {
               ULONG r;
               DosWrite(handle, (PVOID) buffer, sizeofbuffer, &r);
               return r;
            }
      ULONG Write( const void * buffer, const ULONG sizeofbuffer) const { return _Write(buffer, sizeofbuffer); }
      ULONG Write( const char value ) const { return _Write( &value, sizeof(char) ); }
      ULONG Write( const SHORT value ) const { return _Write( &value, sizeof(SHORT) ); }
      ULONG Write( const LONG value ) const { return _Write( &value, sizeof(LONG) ); }
      ULONG Write( const XDate& date ) const { return _Write( &date.d, sizeof(date.d) ); }
      ULONG Write( const XTime& time ) const { return _Write( &time.t, sizeof(time.t) ); }
      ULONG Write( const char * value ) const { return _Write( value, strlen(value) ); }
      ULONG Write( const XString& string ) const { return _Write( (const void*) (char*) string, string.GetLength()); }
};

#endif
