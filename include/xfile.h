#ifndef __OOL_XFILE_H__
#define __OOL_XFILE_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFile                                 */
/*                       derived from: XIO                                   */
/*                        last update: 11/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xio.h"


class XFileInfo;


class XEA: public XObject
{
   public:
      XEA() { value = NULL; valueSize = 0;}
      ~XEA();
      XString name;
      void * value;
      USHORT valueSize;
      SHORT type;
      USHORT GetType()
      {
         if( valueSize == 0)
            return 0;
         USHORT *pusPtr = (USHORT *) value;
         return *pusPtr;
      }
};


// TBO
class _export_ XEAList: public XObject
{
   friend class XFile;
      XEA ** list;
      USHORT count;
      void ClearList();
   public:
      XEAList() {list = NULL; count = 0;}
      ~XEAList() { ClearList(); }
      XEA* GetEA( const USHORT index ) const { return index < count ? list[index] : NULL; }
      USHORT GetEACount() const { return count; }
};


class _export_ XFile: public XIO
{
#ifdef OOL_FOR_WIN32_X86
   XString path;
#endif
   public:
      XFile() { };
      static ULONG Copy( const char* from, const char * to) { return DosCopy( (PSZ) from, (PSZ) to, 0); }
      static ULONG CreateDirectory( const char * path);
      static ULONG DeleteDirectory( const char * path);
      static ULONG GetEA( const char * path, XEA * buffer, const char * entryName);
      static ULONG GetEAList( const char * path, XEAList * buffer);
      static ULONG SetEA( const char * path, const XEA * buffer);
      static ULONG SetEAList( const char * path, const XEAList * buffer);
      static BOOL IsDriveAvaible( const char );
      static ULONG Move( const char* from, const char * to) { return DosMove( (PSZ) from, (PSZ) to); }
      ULONG Open( const char * filename, const ULONG modeForOpen = XFILE_FAIL_IF_NEW|XFILE_OPEN_EXISTING, const ULONG modeForAcces = XFILE_READWRITE, const ULONG modeShare = XFILE_SHARE_DENYREADWRITE, const ULONG size=0, const EAOP2 * ealist=NULL);
      static void GetCurrentDirectory( XString * buffer);
      static void GetCurrentDisk( char& buffer);
      LONG GetFileInfo( XFileInfo *);
      static LONG GetPathInfo( const char * path, XFileInfo*);
      static BOOL Remove( const char* path) { return ( DosDelete((PSZ) path) == 0 ? TRUE : FALSE); }
      LONG ResetBuffer( void ) const { return DosResetBuffer(handle); }
      static BOOL SetCurrentDirectory( const char * path) { if( DosSetCurrentDir( (PSZ) path) != 0) return FALSE; else return TRUE; }
      static BOOL SetDefaultDisk( const char );
      LONG SetFileInfo( const XFileInfo*);
      static LONG SetPathInfo( const char * path, const XFileInfo*);
      static ULONG SetMaxFiles( ULONG handles );
};


#endif





