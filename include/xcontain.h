#ifndef __OOL_XCONTAINER_H__
#define __OOL_XCONTAINER_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XContainerControl                     */
/*                       derived from: XControl                              */
/*                        last update: 9/96                                  */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/


#include "xcontrol.h"

#ifdef OOL_FOR_OS2_X86
   #ifndef CRA_SOURCE
      #define CRA_SOURCE  0x00004000L
   #endif

   //container creation styles
   #define CON_AUTOPOSITION CCS_AUTOPOSITION
   #define CON_EXTENDSEL    CCS_EXTENDSEL
   #define CON_MULTIPLESEL  CCS_MULTIPLESEL
   #define CON_READONLY     CCS_READONLY
   #define CON_SINGLESEL    CCS_SINGLESEL

   //object-emphasis, can be or-ed
   #define CON_CURSORED         CRA_CURSORED
   #define CON_DISABLED         CRA_DISABLED
   #define CON_INUSE            CRA_INUSE
   #define CON_PICKED           CRA_PICKED
   #define CON_SELECTED         CRA_SELECTED
   #define CON_SOURCE           CRA_SOURCE
   #define CON_RECORDREADONLY   CRA_RECORDREADONLY
   #define CON_EXPANDED         CRA_EXPANDED

   //enumeration styles (FindObject)
   #define CON_FIRST      CMA_FIRST
   #define CON_FIRSTCHILD CMA_FIRSTCHILD
   #define CON_LAST       CMA_LAST
   #define CON_LASTCHILD  CMA_LASTCHILD
   #define CON_NEXT       CMA_NEXT
   #define CON_PARENT     CMA_PARENT
   #define CON_PREV       CMA_PREV
   #define CON_END        CMA_END

   //other
   #define CON_REPOSITION    CMA_REPOSITION
   #define CON_NOREPOSITION  CMA_NOREPOSITION
   #define CON_TEXTCHANGED   CMA_TEXTCHANGED
   #define CON_ERASE         CMA_ERASE

   #define CON_FREE       CMA_FREE
   #define CON_INVALIDATE CMA_INVALIDATE

#endif

class XContainerInfo;
class XContainerObject;
class XContainerColumn;

class _export_ XContainerControl: public XControl{
      friend void BuildChilds( HWND );
      friend MRESULT HandleDefault(XWindow * w, ULONG msg, MPARAM mp1, MPARAM mp2, BOOL & handled);
   private:
      XContainerControl( const HWND hwnd): XControl(hwnd) {;}
      void Clean( PRECORDCORE first );
      XString title;
   public:
      XContainerControl( const XWindow * owner, const XRect& rec = XRect(), const USHORT id = 0, const ULONG style=WIN_VISIBLE|WIN_BORDER, const char * font = NULL);
      virtual ~XContainerControl() { RemoveAll(TRUE); }
      BOOL AddObject( XContainerObject * object, XContainerObject * parentObject=NULL, XContainerObject * siblingObject = (XContainerObject*) CON_END, BOOL draw = TRUE);
      BOOL AddObjectList( XContainerObject ** objectList, const USHORT count, XContainerObject * parentObject=NULL, XContainerObject * siblingObject = (XContainerObject*) CON_END, BOOL draw = TRUE);
      void Arrange(void) const ;
      BOOL BeginEdit(const XContainerObject * obj, const XContainerColumn * col = NULL, const ULONG winID = 0) const;
      virtual void DoHScroll( LONG pix) {;}
      virtual void DoVScroll( LONG pix) {;}
      void EndEdit() const;
      BOOL ExpandTreeObject( const XContainerObject *, const BOOL=TRUE) const ;
      XContainerObject * FindObject( const XContainerObject * startObject = NULL, const SHORT select=CON_FIRST);
      BOOL FreeObject(XContainerObject*);
      XContainerColumn * GetColumn( const XContainerColumn * col, const SHORT option);
      void GetInfo( XContainerInfo*);
      XContainerObject * GetObject( const XContainerObject * startObject = NULL, const SHORT filter= CON_SELECTED);
      BOOL GetObjectRect(XRect * r, const XContainerObject * o, const BOOL rightWin = FALSE, const SHORT extension = 0) const ;
      void HScroll( const LONG pix ) const ;
      BOOL InsertColumn( const XContainerColumn * column, const XContainerColumn * insertBehind = NULL, const BOOL redraw = FALSE);
      void InvalidateObject( const XContainerObject * obj = NULL, const SHORT option = CMA_REPOSITION) const ;
      void RemoveAll( BOOL destroyObjects = TRUE );
      BOOL RemoveColumn( XContainerColumn * column, const SHORT option);
      BOOL RemoveObject( XContainerObject *, const BOOL destroyObject = TRUE, const BOOL redraw = FALSE) const ;
      BOOL RemoveObjectList( XContainerObject **, const SHORT count, const BOOL destroyObject = TRUE, const BOOL redraw = FALSE) const ;
      void SortObjects( void ) const ;
      void SetInfo( XContainerInfo*);
      void SetObjectEmphasis( const XContainerObject * obj, const SHORT emph, const BOOL enableEmph=TRUE) const ;
      virtual void TitleEdited( const char *) { ;}
      void UpdateColumns( void ) const;
      void VScroll( const LONG pix ) const ;
};

#endif
