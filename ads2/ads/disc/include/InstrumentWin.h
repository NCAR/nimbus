
#ifndef INSTWIN_H
#define INSTWIN_H


#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>


class InstrumentWin {
  public:

    InstrumentWin(Widget parent);
    void addDismiss(void (dissMissWrap)(Widget, XtPointer, XmPushButtonCallbackStruct*));
    void popUp(Widget, XtPointer, XmPushButtonCallbackStruct *);
    void popDown(Widget, XtPointer, XmPushButtonCallbackStruct *);

    Widget RowColumn()	{return instrumentRC;}


  private:
   Widget	instrumentForm, instrumentFrame, instrumentRC;

};

#endif
