#ifndef __OOL_XFILEINFO_H__
#define __OOL_XFILEINFO_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XFileInfo                             */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xstring.h"
#include "xobject.h"
#ifdef OOL_FOR_WIN32_X86
#include <io.h>
#endif

#ifdef OOL_FOR_OS2_X86
   #define XFILE_IS_NORMAL      0x0000
   #define XFILE_IS_HIDDEN      0x0002
   #define XFILE_IS_SYSTEM      0x0004
   #define XFILE_IS_DIRECTORY      0x0010
   #define XFILE_IS_ARCHIVED      0x0020
   #define XFILE_IS_READONLY   FILE_READONLY
#endif

class XDate;
class XTime;

class _export_ XFileInfo: public XObject
{
      friend class XFile;
      friend class XFileFind;
   private:
      FILEFINDBUF3 buffer;
   public:
      XFileInfo( ) { ;}
      ~XFileInfo() { ;}
      void GetCreationDate( XDate *) const ;
      void GetCreationTime( XTime * time) const ;
      ULONG GetFileSize(void) const ;
      LONG GetFileAttributes(void) const ;
      void GetLastWriteDate( XDate * date) const ;
      void GetLastWriteTime( XTime * time) const ;
      void SetCreationDate( const XDate *);
      void SetCreationTime( const XTime * time);
      void SetLastWriteDate( const XDate * date);
      void SetLastWriteTime( const XTime * time);
      void SetFileAttributes( const LONG);
      BOOL IsReadonly() const { return GetFileAttributes() & XFILE_IS_READONLY; }
      BOOL IsHidden() const { return GetFileAttributes() & XFILE_IS_HIDDEN; }
      BOOL IsDirectory() const { return GetFileAttributes() & XFILE_IS_DIRECTORY; }
      BOOL IsSystem() const { return GetFileAttributes() & XFILE_IS_SYSTEM; }
      BOOL IsArchived() const { return GetFileAttributes() & XFILE_IS_ARCHIVED; }
};

#endif
