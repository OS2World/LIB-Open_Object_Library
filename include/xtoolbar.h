#ifndef __OOL_XTOOLBAR_H__
#define __OOL_XTOOLBAR_H__

/*===========================================================================*/
/* OOL ------------------- the Open Object Library ------------------- r 1.0 */
/*===========================================================================*/
/*                              class: XToolBar                              */
/*                       derived from: XFrameControl                         */
/*                        last update: 11/96                                 */
/*                      programmed by: Stefan von Brauk (sbrauk@gwdg.de)     */
/*===========================================================================*/

#include "xfrmcnt.h"
#include "xfrmwnd.h"

#define TB_RIPABLE 16
#define TB_NOCLOSE  32


class XColor;
//class XGraphicDevice;
//class XLine;
class XControlEvent;

typedef struct clientWin
{
   LONG xOffset, yOffset;
   BOOL group;
   XWindow * clientWin;
} ___clientWin___;

class XSize;
class ToolBarHandler;

class _export_ XToolBar: public XFrameControl
{
      friend class XFrame;
      friend class ToolBarParent;
      friend class ToolBarHandler;
   private:
      clientWin ** clientArray;
      USHORT clients;
      USHORT borderSize;
      XFrameWindow * frame, * cutWindow;
      char spacing;
      ToolBarHandler * hdl;
      BOOL border;
      ULONG style;
      XString title;
   protected:
      virtual void Attach( void );
      virtual void Cut( void );
   public:
      XToolBar( const XFrameWindow *, const ULONG style = TB_TOP, const ULONG id=0, const USHORT ySize = 20, const char groupSpacing = 15);
      virtual ~XToolBar();
      void AddWindow( const XWindow * clientWindow, const BOOL newGroup = FALSE, const BOOL adjustSize = FALSE, const XWindow * insertBehind = NULL, const LONG x_offset = 0, LONG y_Offset = 0);
      BOOL DoCommand( LONG);
      void DoControl( XControlEvent*);
      void DoSize( XSize* );
      void Draw(void);
      void GetBackgroundColor( XColor * rgb) const;
      XFrameWindow * GetCutWindow() const { return cutWindow; }
      virtual void GetText( XString * buffer ) const { *buffer = title; }
      BOOL RemoveWindow( const XWindow *, const BOOL redraw = FALSE);
      void ReSize( void );
      void SetBorderSize( const USHORT siz);
      BOOL SetHeight( const USHORT cy);
      virtual void SetText(const char * text) { title = text; if(cutWindow) cutWindow->SetText(title); }
      void Show( const BOOL show = TRUE);
};

#endif
