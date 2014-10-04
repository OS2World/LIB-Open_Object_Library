#ifndef __OOL_XCOUNTRY_H__
#define __OOL_XCOUNTRY_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XCountryInfo                          */
/*                       derived from: XObject                               */
/*                        last update: 1/97                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/



#include "xobject.h"
#include "xstring.h"

class _export_ XCountryInfo: public XObject
{
   private:
      COUNTRYINFO cInfo;
   public:
      XCountryInfo();
      ULONG GetCountry() const { return cInfo.country; }
      ULONG GetCodePage() const { return cInfo.codepage; }
      void GetCurrency( XString * s) const { *s = cInfo.szCurrency; }
      ULONG GetDateFormat() const { return cInfo.fsDateFmt; }
      CHAR GetDateSeparator() const { return cInfo.szDateSeparator[0]; }
      CHAR GetDecimalSeparator( ) const { return cInfo.szDecimal[0]; }
      CHAR GetThousandsSeparator( ) const { return cInfo.szThousandsSeparator[0]; }
      CHAR GetTimeSeparator( ) const { return cInfo.szTimeSeparator[0]; }
      BOOL IsTimeAmPm() const { return (cInfo.fsTimeFmt == 0 ? TRUE: FALSE); }
};



#endif
