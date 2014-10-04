#ifndef __OOL_INCLUDES_H__
#define __OOL_INCLUDES_H__


#define OOL_FOR_OS2_X86  //make the library for OS/2 intel-version


#ifdef OOL_FOR_OS2_X86
   #define INCL_PM
   #define INCL_GPI
   #define INCL_WINSTDDRAG
   #define INCL_DOSMODULEMGR
   #define INCL_WINCLIPBOARD
   #define INCL_WINWORKPLACE
   #define INCL_DOSQUEUES
   #define INCL_DOSPROCESS
   #define DOS_INCLDATETIME
   #define INCL_DEV
   #define INCL_DOS
   #define INCL_DOSERRORS
   #define INCL_RXSUBCOM
   #define INCL_RXSHV
   #define INCL_RXFUNC
   #define INCL_SPL
   #define INCL_SPLDOSPRINT
   #define INCL_SPLERRORS
   #define INCL_DOSNLS

   #include <os2.h>

   #if !defined(__IBMCPP__) && !defined(__WATCOMC__)
      #include <cpp/std/typeinfo.h>
   #endif

   #include "xmemchk.h"
   #ifndef DB_RAISED
      #define DB_RAISED 0x400
   #endif
   #ifndef DB_DEPRESSED
      #define DB_DEPRESSED 0x800
   #endif

   //disable this include if you work with EMX *WITH* IBM header files
   #include <rexxsaa.h>

   //this structure is missing in EMX 0.9c, if the compiler
   //tells you that this stucture is already defined, delete
   //this definiton including <typedef PRINTDEST *PPRINTDEST;>

   #if !defined( __IBMCPP__ ) & !defined( __WATCOMC__ )
      #include <typeinfo>
      //disable this definition if you use EMX *WITH* IBM header files
      typedef struct _PRINTDEST
      {
         ULONG        cb;
         LONG         lType;
         PSZ          pszToken;
         LONG         lCount;
         PDEVOPENDATA pdopData;
         ULONG        fl;
         PSZ          pszPrinter;
      } PRINTDEST;
      typedef PRINTDEST *PPRINTDEST;

   typedef  PSZ       APSZ[1];
   typedef  APSZ *PAPSZ;          /* papsz */

   #endif

   #define OOL_ATOMHANDLE    ATOM
   #define OOL_BITMAPHANDLE  HBITMAP
   #define OOL_POINTERHANDLE HPOINTER
   #define OOL_ICONHANDLE    HPOINTER
   #define OOL_WINDOWHANDLE  HWND
   #define USER_OK           DID_OK
   #define USER_CANCEL       DID_CANCEL
#endif


#define X_ERR -1
//#define X_OK  0

#ifdef _DEBUG_
  #include "assert.h"
  #define X_ASSERT(x) assert(x)
#else
  #define X_ASSERT(x)
#endif

#ifndef ABSTRACT
  #define ABSTRACT = 0
#endif

#ifndef XSIZE_T
  #define XSIZE_T unsigned long int
#endif



#endif


#ifndef _export_
#ifdef __IBMCPP__                        //required export definition _Export
  #define _export_ _Export
#elif defined( __WATCOMC__ )
  #define _export_ __export
#else
  #define _export_
#endif

#if (defined (__IBMCPP__) && (__IBMCPP__ == 300))
  #pragma hdrstop
#endif
#endif