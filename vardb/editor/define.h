

Widget	CreateMainWindow(Widget Shell000);


void	Initialize(int argc, char *argv[]),

	OpenNewFile(Widget w, XtPointer client, XtPointer call),
	OpenNewFile_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	SaveFileAs(Widget w, XtPointer client, XtPointer call),
	SaveFileAs_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	SaveFile(Widget w, XtPointer client, XtPointer call),
	EditVariable(Widget w, XtPointer client, XmListCallbackStruct *call),
	Accept(Widget w, XtPointer client, XtPointer call),
	Clear(Widget w, XtPointer client, XtPointer call),
	Delete(Widget w, XtPointer client, XtPointer call),
	SetCategory(Widget w, XtPointer client, XtPointer call),
	Quit(Widget w, XtPointer client, XtPointer call);

