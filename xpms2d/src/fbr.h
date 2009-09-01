/*
 * Resource file for xpms2d.
 */
 
static String fallback_resources[] =
  {
  "*defaultFontList:	-adobe-courier-bold-r-*-*-14-*-*-*-m-90-*-*",
  "*foreground:         black",

  "*font24:		-adobe-courier-bold-r-*-*-24-*-*-*-*-*-*-*",
  "*font18:		-adobe-courier-bold-r-*-*-18-*-*-*-*-*-*-*",
  "*font14:		-adobe-courier-bold-r-*-*-14-*-*-*-*-*-*-*",
  "*font12:		-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*font10:		-adobe-courier-bold-r-*-*-10-*-*-*-*-*-*-*",

  "*plRC*fontList:	-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*controlRC*fontList:	-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*printParmsRC*fontList:  -adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*editSpecShell*fontList: -adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",

  "*XmPushButton.height:		32",
  "*XmTextField.background:		Burlywood",
  "*XmTextField.foreground:		black",
  "*XmForm.horizontalSpacing:		4",
  "*XmForm.verticalSpacing:		4",
  "*canvasShell.title:			xpms2d 2.5.2",
  "*canvasShell.iconName:		xpms2d",
  "*canvasShell.allowShellResize:	True",
  "*canvasShell.x:			25",
  "*canvasShell.y:			250",
  "*controlShell.title:			Probe Selection",
  "*controlShell.x:			100",
  "*controlShell.y:			25",

  "*movieRC*XmScale.orientation:	HORIZONTAL",
  "*movieRC*timeScale.titleString:	Start            Time warp           Finish",
  "*movieRC*delayScale.titleString:	0          Frame delay (seconds)          4",
  "*movieRC*delayScale.decimalPoints:   3",
  "*movieRC*delayScale.maximum:         2000",
  "*movieRC*delayScale.scaleMultiple:   250",
  "*movieRC*delayScale.showValue:       True",

  "*pgButtRC.entryAlignment:            ALIGNMENT_CENTER",
  "*pgButtRC.orientation:               HORIZONTAL",
  "*pgButtRC.XmPushButton.recomputeSize:        False",
  "*movieRC*pgButtRC.XmPushButton.width:        75",

  "*movieRC*XmFrame.shadowType:		SHADOW_IN",
  "*movieRC*XmRowColumn.spacing:	0",
  "*probeRC*XmRowColumn.spacing:	0",
  "*cncRC*XmRadioBox.spacing:		0",
  "*denRC*XmRadioBox.spacing:		0",

  "*buttonFrame.shadowType:		SHADOW_IN",
  "*buttonRC.XmPushButton.recomputeSize:	False",
  "*buttonRC.XmPushButton.width:	125",
  "*buttonRC.entryAlignment:		ALIGNMENT_CENTER",
  "*buttonRC.isAligned:			True",
  "*buttonRC.orientation:		HORIZONTAL",

  "*drawForm.XmPushButton.recomputeSize:	False",
  "*drawForm.XmPushButton.width:	32",
  "*drawForm.XmPushButton.height:	32",

  "*canvas.background:			grey",
  "*canvas.height:			675",
  "*canvas.width:			1050",

  "*magShell.title:			Magnify Window",
  "*magShell.iconName:			2d Mag",
  "*magShell.allowShellResize:		True",
  "*magShell.x:				600",
  "*magShell.y:				25",

  "*magCanvas.background:		grey",
  "*magCanvas.height:			200",
  "*magCanvas.width:			400",

  "*pgButtRC.entryAlignment:		ALIGNMENT_CENTER",
  "*pgButtRC.XmPushButton.recomputeSize:	False",
  "*pgButtRC.XmPushButton.width:	100",

  "*pageFwd.labelString:		Page Fwd",
  "*pageBkd.labelString:		Page Bkd",

  "*headerShell.title:			netCDF Header",
  "*statsShell.title:			Data Set Statistics",
  "*specShell.title:			Spectrum",

  "*enchiladaShell.title:		Particle details",
  "*enchiladaText.background:		Gray",
  "*enchiladaText.columns:		80",
  "*enchiladaText.editable:		False",
  "*enchiladaText.editMode:		MULTI_LINE_EDIT",
  "*enchiladaText.fontList:	-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*enchiladaText.rows:			42",

  "*histogramShell.title:		Accumulation Histogram",
  "*histogramText.background:		Gray",
  "*histogramText.columns:		200",
  "*histogramText.editable:		False",
  "*histogramText.editMode:		MULTI_LINE_EDIT",
  "*histogramText.fontList:	-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*histogramText.rows:			12",

  "*hexShell.title:			Hex Output",
  "*hexText.background:			Gray",
  "*hexText.columns:			96",
  "*hexText.editable:			False",
  "*hexText.editMode:			MULTI_LINE_EDIT",
  "*hexText.fontList:	-adobe-courier-bold-r-*-*-12-*-*-*-*-*-*-*",
  "*hexText.rows:			42",

  "*menuBar*newFile.labelString:	New Data File",
  "*menuBar*newFile.mnemonic:		N",
  "*menuBar*newFile.accelerator:	Ctrl<Key>N",
  "*menuBar*newFile.acceleratorText:	Ctrl-N",
  "*menuBar*addFile.labelString:	Add Data File",
  "*menuBar*addFile.mnemonic:		A",
  "*menuBar*readParms.labelString:	Read Parms File",
  "*menuBar*readParms.mnemonic:		R",
  "*menuBar*writeParms.labelString:	Write Parms File",
  "*menuBar*writeParms.mnemonic:	W",
  "*menuBar*printSetup.labelString:	Printer Setup",
  "*menuBar*savePS.labelString:		Save PostScript",
  "*menuBar*savePS.mnemonic:		S",
  "*menuBar*savePS.accelerator:		Ctrl<Key>S",
  "*menuBar*savePS.acceleratorText:	Ctrl-S",
  "*menuBar*print.labelString:		Print",
  "*menuBar*print.mnemonic:		P",
  "*menuBar*print.accelerator:		Ctrl<Key>P",
  "*menuBar*print.acceleratorText:	Ctrl-P",
  "*menuBar*savePNG.labelString:        Save PNG",
  "*menuBar*savePNG.mnemonic:           G",
  "*menuBar*quit.labelString:		Quit",
  "*menuBar*quit.mnemonic:		Q",
  "*menuBar*quit.accelerator:		Ctrl<Key>Q",
  "*menuBar*quit.acceleratorText:	Ctrl-Q",

  "*menuBar*viewHex.labelString:	View Hex",
  "*menuBar*viewHex.mnemonic:		X",
  "*menuBar*viewHex.accelerator:	Ctrl<Key>X",
  "*menuBar*viewHex.acceleratorText:	Ctrl-X",

  "*menuBar*viewData.labelString:	Normal",
  "*menuBar*viewDiag.labelString:	Diagnostic",
  "*menuBar*viewRaw.labelString:	Raw Data Records",
  "*menuBar*viewHisto.labelString:	View Histograms",
  "*menuBar*viewAll.labelString:	Whole Enchilada",
  "*menuBar*synthetic.labelString:	Toggle Synthetic Data",
  "*menuBar*wrapDisplay.labelString:	Toggle Display Wrap",
  "*menuBar*toggleTiming.labelString:	Toggle Timing Words",

  /*
   * Control window
   */
  "*controlRC*XmFrame.XmLabel.childHorizontalAlignment: ALIGNMENT_CENTER",
  "*controlRC*XmFrame.XmLabel.childVerticalAlignment:   ALIGNMENT_BASELINE_BOTTOM",
  "*controlRC*XmFrame.XmLabel.childHorizontalSpacing:   5",
  "*controlRC*XmFrame.XmLabel.childType:                FRAME_TITLE_CHILD",

  "*controlRC.orientation:		HORIZONTAL",
  "*controlRC*XmFrame.shadowType:	SHADOW_IN",
//  "*controlRC*plRC*XmLabel.recomputeSize:	True",

  "*timeTitle.labelString:		Time Period",
  "*fileTitle.labelString:		Current File",
  "*probeTitle.labelString:		Probes",
  "*cncTitle.labelString:		Method",
  "*denTitle.labelString:		Density",
  "*ratioTitle.labelString:		Reject Ratio",

  "*conc0.labelString:			None",
  "*conc1.labelString:			Entire-in",
  "*conc2.labelString:			Center-in",
  "*conc3.labelString:			Reconstruct",

  "*plRC*XmLabel.alignment:		ALIGNMENT_END",
  "*plRC*XmLabel.recomputeSize:		False",
  "*plRC*XmLabel.width:			95",
  "*plRC.orientation:			HORIZONTAL",
  "*pnRC.orientation:			HORIZONTAL",
  "*plRC.marginHeight:			1",
  /*
   * Printer Setup Window
   */
  "*printShell.title:			Printer Setup",
  "*prOpMenu.labelString:		Printer",
  "*printParmsRC*lbl0.labelString:	Print command",
  "*printParmsRC*txt0.columns:		40",
  "*printParmsRC*txt0.maxLength:	40",
  "*printParmsRC*lbl1.labelString:	Plot Width",
  "*printParmsRC*txt1.columns:		10",
  "*printParmsRC*txt1.maxLength:	10",
  "*printParmsRC*lbl2.labelString:	Plot Height",
  "*printParmsRC*txt2.columns:		10",
  "*printParmsRC*txt2.maxLength:	10",

  "*applyButton.labelString:		Apply",
  "*dismissButton.labelString:		Dismiss",
  "*printButton.labelString:		Print",
  "*resetButton.labelString:		Reset",
  "*saveButton.labelString:		Save",
  "*searchButton.labelString:		Search",
  "*searchTxt.columns:			12",
  "*searchTxt.maxLength:		32",
  "*parmsButton.labelString:		Edit Parms",
  /*
   *       Dialog Boxes
   */
  "*fileBox.defaultPosition:            FALSE",
  "*fileBox.x:                          300",
  "*fileBox.y:                          300",
  "*fileBox*textColumns:                50",
  "*fileBox*dialogTitle:                Select a File",
  "*errorBox.defaultPosition:           FALSE",
  "*errorBox.x:                         400",
  "*errorBox.y:                         400",
  "*errorBox*background:                DarkSlateGray",
  "*errorBox*dialogStyle:               DIALOG_FULL_APPLICATION_MODAL",
  "*errorBox*dialogTitle:               Error Message",
  "*errorBox*foreground:                Red",
  "*errorBox*XmLabel.alignment:         ALIGNMENT_CENTER",
  "*queryBox*dialogStyle:               DIALOG_FULL_APPLICATION_MODAL",
  "*queryBox*dialogTitle:               Need input",
  "*queryBox*alignment:                 ALIGNMENT_CENTER",
  "*warnBox.defaultPosition:            FALSE",
  "*warnBox.x:                          400",
  "*warnBox.y:                          400",
  "*warnBox*background:                 DarkSlateGray",
  "*warnBox*dialogStyle:		DIALOG_FULL_APPLICATION_MODAL",
  "*warnBox*dialogTitle:                Warning Message",
  "*warnBox*foreground:                 Yellow",
  "*warnBox*XmLabel.alignment:          ALIGNMENT_CENTER",
  NULL
  };

