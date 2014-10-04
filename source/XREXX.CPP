#include "xrexx.h"
#include "xrexxex.h"
#include <stdlib.h>
#include <string.h>
#include "xfile.h"
#include "xfileinf.h"
XRexxScript::XRexxScript( const char* filename )
{
        XString tmp( "\0" );

/********************************
XMessageBox((char*) "1");
        // initialise
//        XStringToRXString( tmp, arxsScript[0] );
      void * p;
      DosAllocMem( &p, tmp.GetLength() + 1, PAG_COMMIT|PAG_READ|PAG_WRITE);
      memcpy( p, (char*) st, st.GetLength() + 1);
      arxsScript[0].strptr = (char*) p;
XMessageBox((char*) "2");
        XStringToRXString( tmp, arxsScript[1] );
XMessageBox((char*) "3");
*************************************/
        if( !Load( filename ) )
                OOLRexxThrow("Unable to read Rexx script from file", -1);
}


XRexxScript::~XRexxScript()
{
/***************
        free( arxsScript[0].strptr );
        free( arxsScript[1].strptr );
*****************/
}


/*@
@class XRexxScript
@parent XObject
@type overview
@symbol _
*/

/*@ XRexxScript :: Load( const char* filename )
@group misc
@remarks Loads a rexx script from a file
@parameters const char* filename        Name of the file with the Rexx script
*/
BOOL XRexxScript::Load( const char* filename )
{
        XFile f;
        if( 0 != f.Open( filename ) )
                return FALSE;

        XFileInfo fi;
        f.GetFileInfo( &fi );

        XString s, tmp( "\0" );
        if( fi.GetFileSize() != f.Read( s, fi.GetFileSize() ) )
                return FALSE;

//        XStringToRXString( s, arxsScript[0] );
      void * p;
      DosAllocMem( &p, s.GetLength() + 1, PAG_COMMIT|PAG_READ|PAG_WRITE);
      memcpy( p, (char*) s, s.GetLength() + 1);
      arxsScript[0].strptr = (char*) p;
      arxsScript[0].strlength = s.GetLength();

//        XStringToRXString( tmp, arxsScript[1] );
      DosAllocMem( &p, tmp.GetLength() + 1, PAG_COMMIT|PAG_READ|PAG_WRITE);
      memcpy( p, (char*) tmp, tmp.GetLength() + 1);
      arxsScript[1].strptr = (char*) p;
      arxsScript[1].strlength = 0;
        return TRUE;
}


/*@ XRexxScript :: operator = Load( XString& astring )
@group misc
@remarks assigns a rexx script from a XString
*/

XRexxScript XRexxScript::operator = ( XString &aString )
{
        XString tmp( "\0" );
        XRexxScript rs;


        XStringToRXString( (XString&)aString, rs.arxsScript[0] );
        XStringToRXString( tmp, rs.arxsScript[1] );

        return rs;
}


/*@
@class XRexxInterface
@parent XObject
@type overview
@symbol _
*/

/*@ XRexxInterface :: XRexxInterface(char *n, PFN proc)
@group constructors/destructors
@remarks Creates an interface to REXX. Therefore a C-function of your application (EXE) will be registered.
The function must have following format:<P>
REXXINTERFACE Handler(PRXSTRING commandString, PUSHORT flags, PRXSTRING returnString)<P>
Commands which are not known in a rexx-script running in this interface
will be posted to the handler.
@parameters
<t 'ø' c=2>
øchar * øthe symbolic name of the interface
øPFN øthe adress of your function
</t>
@exceptions   If the method fails an exception of the type XRexxException is thrown.
*/
XRexxInterface :: XRexxInterface(char *n, PFN proc)
{
   ULONG user[2];

   user[0] = (ULONG) this;
   name = n;
   ULONG rc = RexxRegisterSubcomExe( (PSZ) (char*) name, proc, (PUCHAR) & user);

   if (rc)
      OOLRexxThrow("cannot install XRexxInterfaceer", rc);
}


/*@ XRexxInterface :: XRexxInterface(char *n, char *dllName, char *proc)
@group constructors/destructors
@remarks Creates an interface to REXX. Therefore a C-function of you application (DLL) will be registered.
The function must have following format:<P>
REXXINTERFACE Handler(PRXSTRING commandString, PUSHORT flags, PRXSTRING returnString)<P>
Commands which are not known in a rexx-script running in this interface
will be posted to the handler.
@parameters <t 'ø' c=2>
            øchar * name      øthe symbolic name of the interface
           øchar * dllName   øthe name of the DLL
           øchar * proc      øthe name of your function
            </t>
@exceptions   If the method fails an exception of the type XRexxException is thrown.
*/
XRexxInterface :: XRexxInterface(char *n, char *dllName, char *proc)
{
   ULONG user[2];

   user[0] = (ULONG) this;
   name = n;
   dll = dllName;
   ULONG rc = RexxRegisterSubcomDll( (PSZ) (char*) name, (PSZ) dllName, (PSZ) proc, (PUCHAR) & user, RXSUBCOM_NONDROP);

   if (rc)
      OOLRexxThrow("cannot install XRexxInterface", rc);
}


XRexxInterface :: ~XRexxInterface()
{
   ULONG rc = RexxDeregisterSubcom( (PSZ) (char*) name, (PSZ) (char*) dll);

   if (rc)
      OOLRexxThrow("cannot remove XRexxInterface", rc);
}


/*@ XRexxInterface::GetHandler(char *handlerName, char *dllName)
@group misc
@remarks Returns a pointer to a XRexxInterface
@parameters char * handlerName    the name of the rexx interface used in the constructor<BR>
           char *   dllName         name of the DLL if used (default is NULL)
@returns     XRexxInterface * pointer      pointer to an interface (NULL if no interface found)
*/
XRexxInterface * XRexxInterface::GetHandler(char *handlerName, char *dllName)
{
   ULONG user[2];
   unsigned short flag;

   if (RexxQuerySubcom( (PSZ) handlerName, (PSZ) dllName, &flag, (PUCHAR) & user) == 0)
      return (XRexxInterface *) user[0];
   else
      return NULL;
}


/*@ XRexxInterface::Execute(char *commandFile, LONG type, XString * resultBuffer, SHORT * returnCode, XString * argList)
@group misc
@remarks Run a REXX-script
@parameters <t 'ø' c=2>
            øchar * commandFile      øname of a file which contains the script
           øLONG type               øtype of command (see rexx program reference)
           øXString * result      øbuffer for result
           øSHORT returnCode      øbuffer for returncode
           øXString * args         østring which contains the arguments for the script
            </t>
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::Execute(char *commandFile, XRexxScript*script, LONG type, XString * resultBuffer, SHORT * returnCode, XString * argList)
{
   ULONG fi = 0, old = 0;

   argList->Strip();

   RXSTRING *args = NULL;
   RXSTRING res;
   SHORT argCount = 0;
   char retBuffer[250];

   MAKERXSTRING(res, retBuffer, 250);

   if (type != RXCOMMAND)
   {
      *argList += " ";
      while (argList->Find(fi, " ", fi))
      {
         XString buffer = argList->Mid(old, fi - old);

         argCount += 1;
         args = (RXSTRING *) realloc(args, argCount * sizeof(RXSTRING));
         MAKERXSTRING(args[argCount - 1], (char *) buffer, buffer.GetLength());
         fi++;
         old = fi;
      }
   }
   else
   {
      argCount = 1;
      args = (RXSTRING *) malloc(sizeof(RXSTRING));
      MAKERXSTRING(args[argCount - 1], (char *) *argList, argList->GetLength());
   }
//   LONG rc = RexxStart(argCount, args, (PSZ) commandFile, NULL, (PSZ) (char*) name, type, NULL, returnCode, &res);
   LONG rc = RexxStart(argCount, args, (PSZ) commandFile, script ? script->arxsScript : NULL, (PSZ) (char*) name, type, NULL, returnCode, &res);
   free(args);

   if (res.strlength)
   {
      memcpy(resultBuffer->GetBuffer(res.strlength + 1), res.strptr, res.strlength);
      resultBuffer->ReleaseBuffer(res.strlength);
   }

   return rc;
}


/*@ XRexxInterface::SetVar(char *name, char *value)
@group variables
@remarks Set a variable for a running rexx-script
@parameters char * name      name of the variable<BR>
           char *   value      value of the variable
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::SetVar(char *name, char *value)
{
   SHVBLOCK block;

   memset(&block, 0, sizeof(block));
   block.shvcode = RXSHV_SET;

   MAKERXSTRING(block.shvname, name, strlen(name));
   MAKERXSTRING(block.shvvalue, value, strlen(value));
   block.shvvaluelen = strlen(value);
   RexxVariablePool(&block);
   return block.shvret;
}


/*@ XRexxInterface::GetVar(char *name, XString * value)
@group variables
@remarks Query a variable for a running rexx-script
@parameters char * name      name of the variable<BR>
           XString *   value      buffer for the value of the variable
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::GetVar(char *name, XString * value)
{
   SHVBLOCK block;
   char buffer[250];

   memset(&block, 0, sizeof(block));
   block.shvcode = RXSHV_FETCH;
   MAKERXSTRING(block.shvname, name, strlen(name));
   MAKERXSTRING(block.shvvalue, buffer, 250);
   block.shvvaluelen = 250;
   RexxVariablePool(&block);
/*
   if( block.shvret & RXSHV_OK)
   {
*/
   memcpy(value->GetBuffer(block.shvvalue.strlength + 1), block.shvvalue.strptr, block.shvvalue.strlength);
   value->ReleaseBuffer(block.shvvalue.strlength);
//   }
   return block.shvret;
}



/*@ XRexxInterface::DeregisterFunction(char *name)
@group register/deregister functions
@remarks Deregister a function
@parameters char * name      symbolic name of the function
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::DeregisterFunction(char *name)
{
   return RexxDeregisterFunction((PSZ) name);
}


/*@ XRexxInterface::IsFunctionRegistered(char *name)
@group register/deregister functions
@remarks Query if a function is registered
@parameters char * name      symbolic name of the function
@returns     BOOL result
*/
BOOL XRexxInterface::IsFunctionRegistered(char *name)
{
   return (BOOL) RexxQueryFunction((PSZ) name);
}


/*@ XRexxInterface::RegisterFunction(char *name, PFN entry)
@remarks Register a C-function of your application (EXE) so that it can be called from a rexx-script. The
function must have the following format:<P>
REXXINTERFACE MyFunc( PUCHAR name, ULONG argc, PRXSTRING argv, PSZ queue, PRXSTRING ret)
@parameters char * name      symbolic name of the function which must be used in the rexx-script<BR>
           PFN proc         adress of the function
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::RegisterFunction(char *name, PFN entry)
{
#if !defined( __IBMCPP__ ) & !defined( __WATCOMC__ )
   return RexxRegisterFunctionExe((PSZ) name, (RexxFunctionHandler *) entry);
#else
   return RexxRegisterFunctionExe((PSZ) name, entry);
#endif
}


/*@ XRexxInterface::RegisterFunction(char *name, char *dllName, char *routine)
@group register/deregister functions
@remarks Register a C-function of your application (DLL) so that it can be called from a rexx-script. The
function must have the following format:<P>
REXXINTERFACE MyFunc( PUCHAR name, ULONG argc, PRXSTRING argv, PSZ queue, PRXSTRING ret)
@parameters <t 'ø' c=2>
            øchar * name      øsymbolic name of the function which must be used in the rexx-script
           øchar * dllName   øname of the dll
           øchar * proc      øreal name of the function
            </t>
@returns     LONG result            return-code from the operating system
*/
LONG XRexxInterface::RegisterFunction(char *name, char *dllName, char *routine)
{
   return RexxRegisterFunctionDll((PSZ) name, (PSZ) dllName, (PSZ) routine);
}
