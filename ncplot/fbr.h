/*
 * Resource file for ncplot.
 */

static String fallback_resources[] =
  {
  "*defaultFontList:	-adobe-courier-bold-r-*-*-14-140-75-75-m-90-*-*",

  "*font24:		-adobe-courier-bold-r-*-*-24-240-75-75-*-*-*-*",
  "*font18:		-adobe-courier-bold-r-*-*-18-180-75-75-*-*-*-*",
  "*font14:		-adobe-courier-bold-r-*-*-14-140-75-75-*-90-*-*",
  "*font12:		-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*font10:		-adobe-courier-bold-r-*-*-10-100-75-75-*-*-*-*",

  "*plRC*fontList:	-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*controlRC*fontList:	-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*printParmsRC*fontList: -adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*varList*fontList:	-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*specOptForm*fontList: -adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*editSpecShell*fontList: -adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",

  "*asciiText.fontList:	-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*headerText.fontList: -adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",
  "*statsText.fontList:	-adobe-courier-bold-r-*-*-12-120-75-75-*-70-*-*",

  "*plotRC.spacing:			0",
  "*axisRC.spacing:			0",
  "*scaleRC.spacing:			0",
  "*trackRC.spacing:			0",
  "*fileRC.spacing:			0",
  "*miscRC.spacing:			0",

  "*XmPushButton.height:		32",
  "*XmTextField.background:		Burlywood",
  "*XmTextField.foreground:		black",
  "*XmForm.horizontalSpacing:		4",
  "*XmForm.verticalSpacing:		4",
  "*topLevelShell.title:		ncplot 4.7.3",
  "*topLevelShell.iconName:		ncplot",
  "*topLevelShell.allowShellResize:	True",
  "*topLevelShell.x:			0",
  "*topLevelShell.y:			50",
  "*controlShell.title:			Variable Selection",
  "*controlShell.x:			825",
  "*controlShell.y:			50",
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
  "*canvas.height:			625",
  "*canvas.width:			800",

  "*specCanvas.background:		grey",
  "*specCanvas.height:			600",
  "*specCanvas.width:			600",

  "*diffCanvas.background:              grey",
  "*diffCanvas.height:                  300",
  "*diffCanvas.width:                   800",

  "*varList.background:			black",
  "*varList.foreground:			green",
  "*varList.scrollBarDisplayPolicy:	STATIC",
  "*varList.width:			85",

  "*pgButtRC.entryAlignment:		ALIGNMENT_CENTER",
  "*pageFwd.labelString:		Pg Fwd",
  "*pageBkd.labelString:		Pg Bkd",

  "*asciiShell.title:			ASCII",
  "*diffShell.title:                    Difference",
  "*expShell.title:			Expression Parser",
  "*headerShell.title:			netCDF Header",
  "*titleShell.title:			Variable Titles",
  "*statsShell.title:			Data Set Statistics",
  "*specShell.title:			Spectrum",

  "*asciiText.background:		Gray",
  "*asciiText.columns:			40",
  "*asciiText.editable:			False",
  "*asciiText.editMode:			MULTI_LINE_EDIT",
  "*asciiText.rows:			45",

  "*catOpMenu.labelString:		Category",
  "*headerText.background:		Gray",
  "*headerText.columns:			85",
  "*headerText.editable:		False",
  "*headerText.editMode:		MULTI_LINE_EDIT",
  "*headerText.rows:			42",

  "*statsText.background:		Gray",
  "*statsText.columns:			95",
  "*statsText.editable:			False",
  "*statsText.editMode:			MULTI_LINE_EDIT",
  "*statsText.rows:			10",

  "*menuBar*newData.labelString:	New Data File",
  "*menuBar*newData.mnemonic:		N",
  "*menuBar*newData.accelerator:	Ctrl<Key>N",
  "*menuBar*newData.acceleratorText:	Ctrl-N",
  "*menuBar*addData.labelString:	Add Data File",
  "*menuBar*addData.mnemonic:		A",
  "*menuBar*addData.accelerator:	Ctrl<Key>A",
  "*menuBar*addData.acceleratorText:	Ctrl-A",
  "*menuBar*loadTemplate.labelString:	Load Template",
  "*menuBar*saveTemplate.labelString:	Save Template",
  "*menuBar*printSetup.labelString:	Printer Setup",
  "*menuBar*savePS.labelString:		Save PostScript",
  "*menuBar*savePS.mnemonic:		S",
  "*menuBar*savePS.accelerator:		Ctrl<Key>S",
  "*menuBar*savePS.acceleratorText:	Ctrl-S",
  "*menuBar*print.labelString:		Print",
  "*menuBar*print.mnemonic:		P",
  "*menuBar*print.accelerator:		Ctrl<Key>P",
  "*menuBar*print.acceleratorText:	Ctrl-P",
  "*menuBar*savePNG.labelString:	Save PNG Image",
  "*menuBar*quit.labelString:		Quit",
  "*menuBar*quit.mnemonic:		Q",
  "*menuBar*quit.accelerator:		Ctrl<Key>Q",
  "*menuBar*quit.acceleratorText:	Ctrl-Q",

  "*menuBar*editParms.labelString:	Time Series Parameters",
  "*menuBar*editParms.mnemonic:		T",
  "*menuBar*editParms.accelerator:	Ctrl<Key>E",
  "*menuBar*editParms.acceleratorText:	Ctrl-E",
  "*menuBar*editSpec.labelString:	Spectral Parameters",
  "*menuBar*editSpec.mnemonic:		S",
  "*menuBar*editStats.labelString:	Statistics Parameters",
  "*menuBar*editStats.mnemonic:		S",
  "*menuBar*editXY.labelString:		XY Parameters",
  "*menuBar*editXY.mnemonic:		Y",
  "*menuBar*editXYZ.labelString:	XYZ Parameters",
  "*menuBar*editXYZ.mnemonic:		Z",
  "*menuBar*editDiff.labelString:       Difference Parameters",
  "*menuBar*editDiff.mnemonic:          D",
  "*menuBar*editPrefer.labelString:	Edit Preferences",
  "*menuBar*editPrefer.mnemonic:	P",

  "*menuBar*viewASCII.labelString:	ASCII",
  "*menuBar*viewASCII.mnemonic:		A",
  "*menuBar*viewDiff.labelString:       Difference",
  "*menuBar*viewDiff.mnemonic:          D",
  "*menuBar*viewTitles.labelString:	Variable Titles",
  "*menuBar*viewTitles.mnemonic:	T",
  "*menuBar*viewHeader.labelString:	netCDF Header",
  "*menuBar*viewHeader.mnemonic:	H",
  "*menuBar*viewStats.labelString:	Statistics",
  "*menuBar*viewStats.mnemonic:		S",
  "*menuBar*viewSpectra.labelString:	Spectrum",
  "*menuBar*viewCoSpectra.labelString:	Co-Spectrum",
  "*menuBar*viewQuadrature.labelString:	Quadrature",
  "*menuBar*viewCoherence.labelString:	Coherence",
  "*menuBar*viewPhase.labelString:	Phase",
  "*menuBar*viewRatio.labelString:	Ratio",

  "*menuBar*getExp.labelString:		User Calculations",
  "*menuBar*getExp.mnemonic:		N",
  "*menuBar*clearPlot.labelString:	Clear current plot",
  "*menuBar*clearPlot.mnemonic:		C",
  "*menuBar*clearPlot.accelerator:	Ctrl<Key>C",
  "*menuBar*clearPlot.acceleratorText:	Ctrl-C",
  "*menuBar*unZoom.labelString:		Un-Zoom",
  "*menuBar*unZoom.mnemonic:		U",
  "*menuBar*unZoom.accelerator:		Ctrl<Key>U",
  "*menuBar*unZoom.acceleratorText:	Ctrl-U",

  "*menuBar*clearRegret.labelString:	Clear Regression",
  "*menuBar*linearRegret.labelString:	Linear Regression",
  "*menuBar*polyRegret.labelString:	Poly Regression",
  /*
   * Edit Parms Window
   */
  "*plotParmsTitle.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*asciiParmsTitle.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*panelParmsTitle.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*plotParmsTitle.childVerticalAlignment:	ALIGNMENT_BASELINE_BOTTOM",
  "*asciiParmsTitle.childVerticalAlignment:	ALIGNMENT_BASELINE_BOTTOM",
  "*panelParmsTitle.childVerticalAlignment:	ALIGNMENT_BASELINE_BOTTOM",
  "*plotParmsTitle.childHorizontalSpacing:	5",
  "*asciiParmsTitle.childHorizontalSpacing:	5",
  "*panelParmsTitle.childHorizontalSpacing:	5",
  "*plotParmsTitle.childType:		FRAME_TITLE_CHILD",
  "*asciiParmsTitle.childType:		FRAME_TITLE_CHILD",
  "*panelParmsTitle.childType:		FRAME_TITLE_CHILD",
  "*plotParmsTitle.labelString:		Plot Parameters",
  "*asciiParmsTitle.labelString:	ASCII Parameters",
  "*panelParmsTitle.labelString:	Per Panel Parameters",
  /*
   * Control window
   */
  "*controlRC.XmFrame.XmLabel.childHorizontalAlignment: ALIGNMENT_CENTER",
  "*controlRC.XmFrame.XmLabel.childVerticalAlignment:   ALIGNMENT_BASELINE_BOTTOM",
  "*controlRC.XmFrame.XmLabel.childHorizontalSpacing:   5",
  "*controlRC.XmFrame.XmLabel.childType:	FRAME_TITLE_CHILD",

  "*controlRC.XmFrame.shadowType:	SHADOW_IN",
  "*controlRC*panelRC*XmPushButton.width:	60",
  "*controlRC*panelRC*XmPushButton.height:	25",
  "*controlRC*panelRC*XmPushButton.recomputeSize:	False",
  "*controlRC*panelRC*XmPushButton.alignment:	ALIGNMENT_CENTER",

  "*controlRC*lineThick.columns:	2",
  "*controlRC*lineThick.maxLength:	2",

  "*timeTitle.labelString:		Time Period",
  "*fileTitle.labelString:		Current File",
  "*plotTitle.labelString:		Plot Type",
  "*axisTitle.labelString:		Select Axis",
  "*scaleTitle.labelString:		Scale Location",
  "*panelTitle.labelString:		Current Panel",
  "*trackTitle.labelString:		Options",

  "*plRC*XmLabel.alignment:		ALIGNMENT_END",
  "*plRC*XmLabel.recomputeSize:		False",
  "*plRC*XmLabel.width:			95",
  "*plRC.orientation:			HORIZONTAL",
  "*pnRC.orientation:			HORIZONTAL",
  "*plRC.marginHeight:			1",
  "*controlRC*plRC*XmLabel.recomputeSize:	True",
  "*trackOptRC*plRC*XmLabel.recomputeSize:	True",
  /*
   * Track Option Window
   */
  "*trackOptShell.title:		Track Plot Options",
  "*trackOptShell.x:			525",
  "*trackOptShell.y:			715",
  "*trackOptRC.orientation:		HORIZONTAL",
  "*trackOptRC.XmFrame.shadowType:	SHADOW_IN",

  "*trackOptRC*tiText.columns:		5",
  "*trackOptRC*tiText.maxLength:	5",
  "*trackOptRC*uiText.columns:		9",
  "*trackOptRC*uiText.maxLength:	12",

  "*trackOptRC*dirText.columns:		2",
  "*trackOptRC*dirText.maxLength:	2",

  "*trackOptRC*tsText.columns:		2",
  "*trackOptRC*tsText.maxLength:	2",
  /*
   * Edit Preferences
   */
  "*prefParmsShell.title:		Edit Preferences",
  "*prefParmsShell.x:			125",
  "*prefParmsShell.y:			280",
  "*prefRC*XmLabel.width:		150",
  "*prefRC*prefer0.labelString:		Colors",
  "*prefRC*prefer1.labelString:		Line Width",
  "*prefRC*prefer2.labelString:		Template Directory",
  "*prefRC*prefer3.labelString:		Print Color",
  "*prefRC*prefer4.labelString:		Print Command",
  "*prefRC*prefer5.labelString:		True Airspeed Variable",
  "*prefRC*prefer6.labelString:		GPS Latitude Variable",
  "*prefRC*prefer7.labelString:		GPS Longitude Variable",
  "*prefRC*prefer8.labelString:		GPS Altitude Variable",
  "*prefRC*prefer9.labelString:		INS Latitude Variable",
  "*prefRC*prefer10.labelString:	INS Longitude Variable",
  "*prefRC*prefer11.labelString:	Pressure Altitude",
  "*prefRC*prefer12.labelString:	Wind U Variable",
  "*prefRC*prefer13.labelString:	Wind V Variable",
  "*prefRC*prefer14.labelString:	Wind Interval",
  "*prefRC*prefText.columns:		50",
  "*prefRC*prefText.maxLength:		50",
  /*
   * Expression Parser (expar.c) Window
   */
  "*expRC*expText.columns:		80",
  "*expRC*expText.maxLength:		80",
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
  /*
   * Edit Parameter Window
   */
  "*editParmsShell.title:		Edit Time Series Parameters",
  "*editParmsShell.x:			125",
  "*editParmsShell.y:			280",
  "*editDiffShell.title:                Edit Difference Parameters",
  "*editSpecShell.title:		Edit Spectral Parameters",
  "*editStatsShell.title:		Edit Statistics Parameters",
  "*editXYShell.title:			Edit XY Parameters",
  "*editXYZShell.title:			Edit XYZ Parameters",

  "*plRC*titleTxt.columns:		40",
  "*plRC*titleTxt.maxLength:		80",
  "*plRC*subTitleTxt.columns:		40",
  "*plRC*subTitleTxt.maxLength:		80",
  "*plRC*rtText.columns:		6",
  "*plRC*rtText.maxLength:		6",
  "*plRC*timeText.columns:		8",
  "*plRC*timeText.maxLength:		10",
  "*plRC*xLabel.labelString:		X label",
  "*plRC*yLabel0.labelString:		Y label (left)",
  "*plRC*yLabel1.labelString:		Y label (right)",
  "*plRC*zLabel.labelString:		Z label",
  "*plRC*labelTxt.columns:		50",
  "*plRC*labelTxt.maxLength:		50",
  "*plRC*xMin.labelString:		X min",
  "*plRC*xMax.labelString:		X max",
  "*plRC*yMin.labelString:		Y min",
  "*plRC*yMax.labelString:		Y max",
  "*plRC*zMin.labelString:		Z min",
  "*plRC*zMax.labelString:		Z max",
  "*plRC*xMax.width:			75",
  "*plRC*yMax.width:			75",
  "*plRC*zMax.width:			75",
  "*plRC*minMaxTxt.columns:		13",
  "*plRC*minMaxTxt.maxLength:		15",
  "*plRC*xTicMaj.labelString:		# X tics",
  "*plRC*yTicMaj.labelString:		# Y tics",
  "*plRC*zTicMaj.labelString:		# Z tics",
  "*plRC*xTicMin.labelString:		# minor",
  "*plRC*yTicMin.labelString:		# minor",
  "*plRC*zTicMin.labelString:		# minor",
  "*plRC*ticTxt.columns:		10",
  "*plRC*ticTxt.maxLength:		10",

  "*ASCIIparms.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*ASCIIparms.childVerticalAlignment:	ALIGNMENT_WIDGET_BOTTOM",
  "*ASCIIparms.childHorizontalSpacing:	5",
  "*ASCIIparms.childType:		FRAME_TITLE_CHILD",
  "*ASCIIparms.labelString:		ASCII Window Options",

  "*asciiNPlbl*labelString:		# data points",
  "*asciiNPtxt*columns:			10",
  "*asciiNPtxt*maxLength:		10",
  "*asciiFlbl*labelString:		Format",
  "*asciiFtxt*columns:			10",
  "*asciiFtxt*maxLength:		10",
  /*
   * Stats edit window
   */
  "*olVarList.background:		black",
  "*olVarList.foreground:		green",
  "*olVarList.scrollBarDisplayPolicy:	STATIC",
  "*olVarList.selectionPolicy:		SINGLE_SELECT",
  "*olVarList.width:			85",

  "*statsRC.XmFrame.XmLabel.childHorizontalAlignment: ALIGNMENT_CENTER",
  "*statsRC.XmFrame.XmLabel.childVerticalAlignment:  ALIGNMENT_BASELINE_BOTTOM",
  "*statsRC.XmFrame.XmLabel.childHorizontalSpacing:   5",
  "*statsRC.XmFrame.XmLabel.childType:                FRAME_TITLE_CHILD",

  "*statsRC.XmFrame.shadowType:			SHADOW_IN",
  "*statsRC*panelRC*XmPushButton.width:		60",
  "*statsRC*panelRC*XmPushButton.height:	25",
  "*statsRC*panelRC*XmPushButton.recomputeSize:	False",
  "*statsRC*panelRC*XmPushButton.alignment:	ALIGNMENT_CENTER",

  "*outlierTitle.labelString:		Outlier Bounds",
  "*averageTitle.labelString:		Data Reduction",
  "*regretTitle.labelString:		Regression",
  /*
   * Spectral Options stuff.
   */
  "*specDefsFrame.shadowType:		SHADOW_IN",
  "*specDefsFrame*XmRowColumn.spacing:	0",
  "*slOpMenu.labelString:		Segment Length",
  "*winOpMenu.labelString:		Window",
  "*dtOpMenu.labelString:		Detrend",
  "*specDefsFrame*plRC*XmLabel.recomputeSize:	True",
  "*fluxLabel.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*fluxLabel.childVerticalAlignment:	ALIGNMENT_WIDGET_BOTTOM",
  "*fluxLabel.childHorizontalSpacing:	5",
  "*fluxLabel.childType:		FRAME_TITLE_CHILD",
  "*fluxLabel.labelString:		Band Limited Variance",
  "*fluxFreq.columns:			10",
  "*fluxFreq.maxLength:			10",
  "*eliaFrame.shadowType:		SHADOW_IN",
  "*eliaFrame*XmRowColumn.spacing:	0",
  "*eliaFrame*plRC*XmLabel.width:	145",
  "*eliaText.columns:			4",
  "*eliaText.maxLength:			4",
  "*timeShift.columns:			4",
  "*timeShift.maxLength:		4",
  "*plotMethodLabel.childHorizontalAlignment: ALIGNMENT_CENTER",
  "*plotMethodLabel.childVerticalAlignment: ALIGNMENT_WIDGET_BOTTOM",
  "*plotMethodLabel.childHorizontalSpacing: 5",
  "*plotMethodLabel.childType:		FRAME_TITLE_CHILD",
  "*plotMethodLabel.labelString:	Plotting Variations",

  "*dismissButton.leftAttachment:	ATTACH_POSITION",
  "*dismissButton.leftPosition:		35",

  "*applyButton.labelString:		Apply",
  "*dismissButton.labelString:		Dismiss",
  "*printButton.labelString:		Print",
  "*resetButton.labelString:		Reset",
  "*saveButton.labelString:		Save",
  "*parmsButton.labelString:		Edit Parms",
  "*savepngButton.labelString:		Save PNG",
  /*
   *       Dialog Boxes
   */
  "*fileBox.defaultPosition:            FALSE",
  "*fileBox.x:                          130",
  "*fileBox.y:                          200",
  "*fileBox*textColumns:                50",
  "*fileBox*dialogTitle:                Select a File",
  "*errorBox.defaultPosition:           FALSE",
  "*errorBox.x:                         350",
  "*errorBox.y:                         300",
  "*errorBox*background:                DarkSlateGray",
  "*errorBox*dialogStyle:               DIALOG_FULL_APPLICATION_MODAL",
  "*errorBox*dialogTitle:               Error Message",
  "*errorBox*foreground:                Red",
  "*errorBox*XmLabel.alignment:         ALIGNMENT_CENTER",
  "*queryBox*dialogStyle:               DIALOG_FULL_APPLICATION_MODAL",
  "*queryBox*dialogTitle:               Need input",
  "*queryBox*alignment:                 ALIGNMENT_CENTER",
  "*warnBox.defaultPosition:            FALSE",
  "*warnBox.x:                          350",
  "*warnBox.y:                          300",
  "*warnBox*background:                 DarkSlateGray",
  "*warnBox*dialogStyle:		DIALOG_FULL_APPLICATION_MODAL",
  "*warnBox*dialogTitle:                Warning Message",
  "*warnBox*foreground:                 Yellow",
  "*warnBox*XmLabel.alignment:          ALIGNMENT_CENTER",
  NULL
  };

