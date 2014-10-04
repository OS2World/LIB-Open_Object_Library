/* sample1 i a simple text-editor which uses the sytem-multiline-entryfield */
/* to edit the text.                                                        */
/* sample1 shows you how to                                                 */
/* - create multithraeded programms                                         */
/* - use menubars                                                           */
/* - use the xfile-class                                                    */
/* - accept drag-events                                                     */
/* - use the system defined filedialog                                      */

#include "resource.h"      //include definitionf for resources

#define MY_MLE     300     //id for the mle-control


#define INCL_OOL_WIN
#define INCL_OOL_FILE
#include "ool.h"

class SearchDialog;

/* the derived framewindow */
class MyAppWindow: public XFrameWindow
{
      friend class MyMLE;
      friend class SearchDialog;
   public:
      MyAppWindow( ULONG );
      ~MyAppWindow();
      BOOL DoCommand( LONG );                //overridden
      BOOL QueryForClose ( void );           //overriden
      BOOL LoadFile( char*);
      BOOL SaveFile(void);
   private:
      XMenuBar * menu;
      XMultiLineEdit * mle;
      BOOL saved;
      BOOL loaded;
      XString path;
      BOOL loading;
      SearchDialog * dlg;
};

