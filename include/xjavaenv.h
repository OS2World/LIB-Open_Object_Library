#ifndef __OOL_XJAVAENV_H__
#define __OOL_XJAVAENV_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XJavaEnvironment                      */
/*                       derived from: XObject                               */
/*                        last update: 02/97                                 */
/*                      programmed by: Thomas Bonk (thomas@ghecko.saar.de)   */
/*===========================================================================*/

#include <xstring.h>
#include <xobject.H>
#include <xwindow.h>
#include <xjvcmsgh.h>
#include <xjvsurlh.h>
#include <xjvsmsgh.h>
#include <jemperr.h>
#include <jempower.h>


#ifdef OOL_FOR_OS2_X86
        // error codes
        #define JAVA_OK                         JE_OK
        #define JAVA_INVALID_APPLET             JE_INVALID_APPLET
        #define JAVA_INVALID_CONTEXT            JE_INVALID_CONTEXT
        #define JAVA_INVALID_PARAMETER          JE_INVALID_PARAMETER
        #define JAVA_ALREADY_INITIALIZED        JE_ALREADY_INITIALIZED
        #define JAVA_OUT_OF_MEMORY              JE_OUT_OF_MEMORY
        #define JAVA_EXCEPTION_OCCURRED         JE_EXCEPTION_OCCURRED
        #define JAVA_NOT_INITIALIZED            JE_NOT_INITIALIZED
        #define JAVA_INTERNAL_ERROR             JE_INTERNAL_ERROR
#endif


class _export_ XJavaEnvironment : public XObject
{
        private:
                XJavaConsoleMessageHandler* pMsgHandler;

        public:
                // ctor/dtor
                         XJavaEnvironment( XJavaConsoleMessageHandler* = NULL,
                                           int argc = 0, char** argv = 0       );
                virtual ~XJavaEnvironment();

                static void              DisplayError( char* msg, int rc );
                static XJavaEnvironment* Get( void );
                static int               Iconify( XWindow* );
                static int               Uniconify( XWindow* );

                int SetMessageHandler( XJavaConsoleMessageHandler* );
                XJavaConsoleMessageHandler* GetMessageHandler( void ) { return pMsgHandler; }

                int SetShowURLHandler( XWindow*, XJavaShowURLHandler* );
                XJavaShowURLHandler* GetShowURLHandler( XWindow* );

                int SetStatusMsgHandler( XWindow*, XJavaStatusMsgHandler* );
                XJavaStatusMsgHandler* GetStatusMsgHandler( XWindow* );

                void GetVersion( int* piMajor, int* piMinor );
                //int  GetProperty( const char* key, char* value, int buflen, int* pvallen );
                int GetProperty( const char* key, XString* value );
                int  SetProperty( const char* key, const char* value );
};

#endif

