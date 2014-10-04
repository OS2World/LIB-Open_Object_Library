#include "xprofile.h"
#include "xapp.h"

/*@ 
@class XProfile
@type overview
@symbol _
@remarks XProfile is a class to handle INI-files.
*/


/*@ XProfile::Read(const char *itemName, const void *buffer, const ULONG size)
@group read/write
@remarks Read from a profile
@parameters 
<t 'ø' c=2>
øchar *itemName	øitem name to read
øvoid *buffer		øbuffer for the data to read
øULONG size			øsize of the buffer
</t>
*/
BOOL XProfile::Read(const char *itemName, void *buffer, ULONG & size)
{
	return PrfQueryProfileData(hini, (PSZ) (char *) appName, (PSZ) itemName, buffer, &size);
}


#include "xmsgbox.h"
/*@ XProfile::Read(const char *itemName, const XString*buffer)
@group read/write
@remarks Read from a profile
@parameters 
<t 'ø' c=2>
øchar *itemName		øitem name to read
øXString *buffer		øXString-buffer for the data to read (must not be longer than 2048 bytes)
</t>
*/
BOOL XProfile::Read(const char *itemName, XString * string)
{
	ULONG size = 2048;
	BOOL rc = Read(itemName, string->GetBuffer(2048), size);

	string->ReleaseBuffer();
	return rc;
}


/*@ XProfile::Write(const char *itemName, const void *buffer, const ULONG size)
@group read/write
@remarks Write to a profile
@parameters 
<t 'ø' c=2>
øchar *itemName	øitem name to write
øvoid *buffer		øbuffer with the data to write
øULONG size			øsize of the buffer
</t>
*/
BOOL XProfile::Write(const char *itemName, const void *buffer, const ULONG size)
{
	return PrfWriteProfileData(hini, (PSZ) (char *) appName, (PSZ) itemName, (PVOID) buffer, size);
}


/*@ XProfile::Close()
@group open/close
@remarks close a profile
*/


/*@ XProfile::Open( const char *appTitle, const char *fileName)
@group open/close
@remarks Open a profile/create non existing profile
@parameters <t 'ø' c=2>
            øchar *      øapplication title
            øchar *      øfilename of the profile (default is OS2.INI )
				</t>
*/
BOOL XProfile::Open( const char *appTitle, const char *fileName)
{
	hini = PrfOpenProfile( XApplication::GetApplication()->GetAnchorBlock(), (PSZ) fileName);
	appName = appTitle;
	return (hini != NULLHANDLE ? TRUE : FALSE);
}
