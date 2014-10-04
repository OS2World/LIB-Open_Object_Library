#ifndef __OOL_XCOMHDL_H__
#define __OOL_XCOMHDL_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XRexxInterface                        */
/*                       derived from: XObject                               */
/*                        last update: 1/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xobject.h"
#include "XString.h"

#define REXXINTERFACE ULONG APIENTRY

inline void RXStringToXString( RXSTRING& rx, XString& st)
{
   memcpy( st.GetBuffer(rx.strlength+1), rx.strptr, rx.strlength);
   st.ReleaseBuffer( rx.strlength);
}


inline void PRXStringToXString( RXSTRING * rx, XString& st)
{
   memcpy( st.GetBuffer(rx->strlength+1), rx->strptr, rx->strlength);
   st.ReleaseBuffer( rx->strlength);
}


inline void XStringToRXString( XString& st, RXSTRING& rx)
{
   if( st.GetLength() < 256 )
      memcpy( rx.strptr, (char*) st, st.GetLength() + 1);
   else
   {
      void * p;
      DosAllocMem( &p, st.GetLength() + 1, PAG_COMMIT|PAG_READ|PAG_WRITE);
      memcpy( p, (char*) st, st.GetLength() + 1);
      #if !defined(__IBMCPP__) && !defined(__WATCOMC__)
         rx.strptr = (unsigned char*) p;
      #else
         rx.strptr = (char*) p;
      #endif
   }
   rx.strlength = st.GetLength();
}


inline void XStringToPRXString( XString st, RXSTRING * rx)
{
   if( st.GetLength() < 256)
      memcpy( rx->strptr, (char*) st, st.GetLength() + 1);
//      strcpy(rx->strptr, st);
   else
   {
      void * p;
      DosAllocMem( &p, st.GetLength() + 1, PAG_COMMIT|PAG_READ|PAG_WRITE);
      memcpy( p, (char*) st, st.GetLength() + 1);
      #if !defined(__IBMCPP__) && !defined(__WATCOMC__)
         rx->strptr = (unsigned char*) p;
      #else
         rx->strptr = (char*) p;
      #endif
   }
   rx->strlength = st.GetLength();
/*
   memcpy( rx->strptr, (char*) st, st.GetLength() + 1);
   rx->strlength = st.GetLength();
*/
}


class _export_ XRexxScript : public XObject
{
        friend class XRexxInterface;

        private:
                RXSTRING arxsScript[2];

        public:
                 XRexxScript() {}
                 XRexxScript( const char* filename );
                ~XRexxScript();

                BOOL Load( const char* filename );

                XRexxScript operator = ( XString &aString );
};


class _export_ XRexxInterface: public XObject
{
      XString name, dll;
   public:
      XRexxInterface( char * handlerName, PFN routineName );
      XRexxInterface( char * handlerName, char * dllName, char * routineName);
      ~XRexxInterface();
      static LONG DeregisterFunction( char * name);
      LONG Execute( char * commandFile, XRexxScript*, LONG type, XString * resultBuffer, SHORT * returnCode, XString * argList);
      static XRexxInterface * GetHandler( char * handlerName, char * dllName = NULL);
      void GetName( XString * buffer) { *buffer = name; }
      LONG GetVar( char* name, XString * value);
      static BOOL IsFunctionRegistered(char*name);
      static LONG RegisterFunction( char * name, PFN entry);
      static LONG RegisterFunction( char * name, char * dllName, char * routine);
      LONG SetVar( char * name, char* value);
};

#endif
