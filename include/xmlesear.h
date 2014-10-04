#ifndef __OOL_XMLESEARCH_H__
#define __OOL_XMLESEARCH_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XMLESearch                            */
/*                       derived from: XObject                               */
/*                        last update: 12/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xobject.h"

class _export_ XMLESearch: public XObject
{
      friend class XMultiLineEdit;
   private:
      MLE_SEARCHDATA sStruct;
      LONG mode;
   public:
      XMLESearch();
      ~XMLESearch();
      LONG GetStartPoint(void);
      void GetReplaceItem( XString * s);
      void GetSearchItem( XString * s);
      void SetReplaceAll( BOOL replace = TRUE);
      void SetReplaceItem( char * );
      void SetCaseSensitive( BOOL sensitive = TRUE);
      void SetSearchEndPoint( LONG );
      void SetSearchItem( char * );
      void SetStartPoint( LONG );
};
#endif
