/*
 * Resource file for nimbus.
 */

static String fallback_resources[] =
  {
  "*defaultFontList:		-adobe-courier-bold-r-*-*-14-140-75-75-*-*-*-*",
  "*XmPushButton.height:			32",
  "*XmTextField.background:			Burlywood",
  "*XmTextField.foreground:			black",
  "*XmForm.horizontalSpacing:			8",
  "*XmForm.verticalSpacing:			8",

  "*buttonFrame.shadowType:			SHADOW_IN",
  "*buttonRC.XmPushButton.recomputeSize:	False",
  "*buttonRC.XmPushButton.width:		130",
  "*buttonRC.entryAlignment:			ALIGNMENT_CENTER",
  "*buttonRC.orientation:			HORIZONTAL",

  /*	Dismiss/Reset buttons	*/
  "*resetButton.labelString:			Reset",
  "*dismissButton.labelString:			Dismiss",

  /* Defaults for First window to appear.
   */
  "*topLevelShell.x:				75",
  "*topLevelShell.y:				75",
  "*topLevelShell.title:			Nimbus 3.5.0",
  "*topLevelShell.iconName:			Nimbus",
  "*topLevelForm.resizePolicy:			RESIZE_GROW",
  "*topLevelForm*buttonRC.spacing:		244",
  "*quitButton.labelString:			Quit",
  "*quitButton.traversalOn:			False",
  "*proceedButton.labelString:			Proceed",
  "*proceedButton.traversalOn:			False",
  "*fileNameRC*XmTextField.columns:		40",
  "*fileNameRC*XmTextField.maxLength:		256",
  "*fnRC.orientation:				HORIZONTAL",
  "*fnRC.XmLabel.alignment:			ALIGNMENT_END",
  "*fnRC.XmLabel.recomputeSize:			False",
  "*fnRC.XmLabel.width:				115",

  /* Setup Window resources (2nd window to appear).
   */
  "*setupShell.x:				100",
  "*setupShell.y:				150",
  "*setupForm.resizePolicy:			RESIZE_GROW",
  "*menuBar*setupPD_CB.labelString:		Setup",
  "*menuBar*setupPD_CB.mnemonic:		S",
  "*menuBar*editPD_CB.labelString:		Edit",
  "*menuBar*editPD_CB.mnemonic:			E",
  "*menuBar*loadSetupButton.labelString:	Load",
  "*menuBar*loadSetupButton.mnemonic:		L",
  "*menuBar*loadSetupButton.accelerator:	Ctrl<Key>L",
  "*menuBar*loadSetupButton.acceleratorText:	Ctrl-L",
  "*menuBar*saveSetupButton.labelString:	Save",
  "*menuBar*saveSetupButton.mnemonic:		S",
  "*menuBar*saveSetupButton.accelerator:	Ctrl<Key>S",
  "*menuBar*saveSetupButton.acceleratorText:	Ctrl-S",
  "*menuBar*printButton.labelString:		Print",
  "*menuBar*printButton.mnemonic:		P",
  "*menuBar*printButton.accelerator:		Ctrl<Key>P",
  "*menuBar*printButton.acceleratorText:	Ctrl-P",
  "*menuBar*cancelSetupButton.labelString:	Quit",
  "*menuBar*cancelSetupButton.mnemonic:		Q",
  "*menuBar*cancelSetupButton.accelerator:	Ctrl<Key>Q",
  "*menuBar*cancelSetupButton.acceleratorText:	Ctrl-Q",
  "*menuBar*editDefaultsButton.labelString:	Defaults",
  "*menuBar*editDefaultsButton.mnemonic:	D",
  "*menuBar*editDefaultsButton.accelerator:	Ctrl<Key>D",
  "*menuBar*editDefaultsButton.acceleratorText:	Ctrl-D",
  "*menuBar*editTimeSlices.labelString:		Time Slices",
  "*menuBar*editTimeSlices.mnemonic:		T",
  "*menuBar*editTimeSlices.accelerator:		Ctrl<Key>T",
  "*menuBar*editTimeSlices.acceleratorText:	Ctrl-T",
  "*menuBar*editFlightInfo.labelString:		Flight Info",
  "*menuBar*editFlightInfo.mnemonic:		F",
  "*menuBar*editFlightInfo.accelerator:		Ctrl<Key>F",
  "*menuBar*editFlightInfo.acceleratorText:	Ctrl-F",
  "*menuBar*toggleOutput.labelString:		Toggle Output",
  "*menuBar*toggleOutput.mnemonic:		O",
  "*menuBar*toggleOutput.accelerator:		Ctrl<Key>O",
  "*menuBar*toggleOutput.acceleratorText:	Ctrl-O",
  "*menuBar*toggleRate.labelString:		Toggle Rate",
  "*menuBar*toggleRate.mnemonic:		R",
  "*menuBar*toggleRate.accelerator:		Ctrl<Key>R",
  "*menuBar*toggleRate.acceleratorText:		Ctrl-R",
  "*menuBar*toggleProbe.labelString:		Toggle Groups",
  "*menuBar*toggleProbe.mnemonic:		P",
  "*menuBar*toggleProbe.accelerator:		Ctrl<Key>P",
  "*menuBar*toggleProbe.acceleratorText:	Ctrl-P",
  "*rateFrame.shadowType:			SHADOW_IN",
  "*rateRadioBox.orientation:			HORIZONTAL",
  "*lowRateButton.labelString:			Low Rate",
  "*lowRateButton.set:				True",
  "*lowRateButton.traversalOn:			False",
  "*highRateButton.labelString:			High Rate",
  "*highRateButton.set:				False",
  "*highRateButton.traversalOn:			False",
  "*goButton.height:				37",
  "*goButton.labelString:			Go",
  "*goButton.recomputeSize:			False",
  "*goButton.traversalOn:			False",
  "*goButton.width:				120",
  "*timeDisplayText.background:			black",
  "*timeDisplayText.columns:			9",
  "*timeDisplayText.cursorPositionVisible:	False",
  "*timeDisplayText.editable:			False",
  "*timeDisplayText.foreground:			red",
  "*timeDisplayText.maxLength:			10",
  "*timeDisplayText.traversalOn:		False",
  "*timeDisplayText.value:			00:00:00",
  "*timeDisplayText.x:				550",
  "*timeDisplayText.y:				10",
  "*listRC.resizeWidth:				False",
  "*listRC.width:				590",
  "*listTitle.alignment:			ALIGNMENT_BEGINNING",
  "*varList.background:				black",
  "*varList.foreground:				green",
  "*varList.listSizePolicy:			CONSTANT",
  "*varList.selectionPolicy:			EXTENDED_SELECT",
  "*varList.scrollBarDisplayPolicy:		STATIC",
  "*varList.visibleItemCount:			20",
  "*logText.background:				Gray",
  "*logText.editable:				FALSE",
  "*logText.editMode:				MULTI_LINE_EDIT",
  "*logText.rows:				5",
  "*logText.scrollHorizontal:			TRUE",
  "*logText.scrollVertical:			TRUE",
  "*logText.traversalOn:			False",

  /* Edit Window Resources.
   */
  "*editVariableShell.x:			500",
  "*editVariableShell.y:			275",
  "*editForm*buttonRC.spacing:			39",
  "*varNameFrame.shadowType:			SHADOW_ETCHED_OUT",
  "*outputRC.entryAlignment:			ALIGNMENT_CENTER",
  "*outputLabel.labelString:			Output",
  "*outputRB.marginWidth:			20",
  "*outputRB.orientation:			HORIZONTAL",
  "*outputRB.spacing:				29",
  "*outputVarYes.labelString:			Yes",
  "*outputVarNo.labelString:			No",
  "*outRateRC.entryAlignment:			ALIGNMENT_CENTER",
  "*outRateRC.resizeWidth:			False",
  "*outRateRC.width:				183",
  "*outRateLabel.labelString:			Output Rate",
  "*outRateRB.marginWidth:			25",
  "*outputLR.labelString:			Low Rate",
  "*outputSR.labelString:			Sample Rate",
  "*outputHR.labelString:			High Rate",
  "*lagForm.marginWidth:			15",
  "*lagForm.horizontalSpacing:			15",
  "*lagForm.verticalSpacing:			3",
  "*lagLabel.labelString:			Static\nLag (ms)",
  "*lagText.columns:				5",
  "*lagText.maxLength:				5",
  "*spikeForm.marginWidth:			15",
  "*spikeForm.horizontalSpacing:		15",
  "*spikeForm.verticalSpacing:			3",
  "*spikeLabel.labelString:			Spike\nSlope",
  "*spikeText.columns:				8",
  "*spikeText.maxLength:			8",
  "*applyButton.labelString:			Apply",
  "*evTextTitle.childHorizontalAlignment:	ALIGNMENT_CENTER",
  "*evTextTitle.childVerticalAlignment:		ALIGNMENT_BASELINE_BOTTOM",
  "*evTextTitle.childHorizontalSpacing:		5",
  "*evTextTitle.childType:			FRAME_TITLE_CHILD",
  "*evTextTitle.labelString:			Cal Coes/Dependencies",
  "*evTextRC1.numColumns:			2",
  "*evTextRC1.packing:				PACK_COLUMN",
  "*evTextRC1.XmTextField.columns:		13",
  "*evTextRC1.XmTextField.maxLength:		20",
  "*evTextRC1.XmTextField.translations:		#override <Key>Tab: activate()",

  /* Time Slice Window
   */
  "*timeSliceShell.x:				75",
  "*timeSliceShell.y:				300",
  "*timeSliceRC.numColumns:			2",
  "*timeSliceRC.packing:			PACK_COLUMN",
  "*timeSliceRC.XmTextField.columns:		9",
  "*timeSliceRC.XmTextField.maxLength:		8",
  "*timeSliceRC.XmTextField.traversalOn:	True",
  "*tsRC.entryAlignment:			ALIGNMENT_CENTER",
  "*tsRC.XmPushButton.recomputeSize:		False",
  "*tsRC.XmPushButton.width:			106",
  "*tsRC.orientation:				HORIZONTAL",

  /* Pause Window
   */
  "*pauseForm.defaultPosition:			FALSE",
  "*pauseForm.x:				525",
  "*pauseForm.y:				225",
  "*pauseForm.resizePolicy:			RESIZE_GROW",
  "*pauseForm.XmPushButton.width:		130",
  "*pauseForm*pauseStop.labelString:		Stop",
  "*pauseForm*pauseCont.labelString:		Continue",
  "*pauseForm*pauseXTV.labelString:		View ADS",

  /* Edit Flight Info
   */
  "*editFlightShell.x:			300",
  "*editFlightShell.y:			300",
  "!*flightEntryRC.entryAlignment:	ALIGNMENT_CENTER",
  "*flightEntryRC.orientation:		HORIZONTAL",
  "*flightEntryRC.txt.columns		10",
  "*flightEntryRC.txt.maxLength		10",
  "*efiRC.entryAlignment:		ALIGNMENT_CENTER",
  "*efiRC.spacing:			12",
  "*efiRC.XmPushButton.recomputeSize:	False",
  "*efiRC.XmPushButton.width:		130",
  "*efiRC.orientation:			HORIZONTAL",

  /* Edit Defaults Window
   */
  "*editDefaultsShell.x:		300",
  "*editDefaultsShell.y:		250",
  "*edRC.entryAlignment:		ALIGNMENT_CENTER",
  "*edRC.spacing:			250",
  "*edRC.XmPushButton.recomputeSize:	False",
  "*edRC.XmPushButton.width:		120",
  "*edRC.orientation:			HORIZONTAL",
  "*defWin.width:			500",
  "*defWin.height:			310",
  "*defWin*XmLabel.height:		35",
  "*defWin*XmLabel.width:		150",
  "*defWin*XmLabel.alignment:		ALIGNMENT_END",

  /* Dialog Boxes
   */
  "*fileBox.defaultPosition:		FALSE",
  "*fileBox.x:				130",
  "*fileBox.y:				200",
  "*fileBox*textColumns:		50",
  "*fileBox*dialogTitle:		Select a File",
  "*errorBox.defaultPosition:		FALSE",
  "*errorBox.x:				300",
  "*errorBox.y:				400",
  "*errorBox*background:		DarkSlateGray",
  "*errorBox*dialogTitle:		Error Message",
  "*errorBox*foreground:		Red",
  "*errorBox*XmLabel.alignment:		ALIGNMENT_CENTER",
  "*queryBox*dialogTitle:		Need input",
  "*queryBox*alignment:			ALIGNMENT_CENTER",
  "*warnBox.defaultPosition:		FALSE",
  "*warnBox.x:				450",
  "*warnBox.y:				100",
  "*warnBox*background:			DarkSlateGray",
  "*warnBox*dialogTitle:		Warning Message",
  "*warnBox*foreground:			Yellow",
  "*warnBox*XmLabel.alignment:		ALIGNMENT_CENTER",
  NULL
  };
