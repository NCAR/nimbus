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

  /* Setup Window resources (2nd window to appear).
   */
  "*setupShell.x:				250",
  "*setupShell.y:				125",
  "*setupForm.resizePolicy:			RESIZE_GROW",
  "*menuBar*setupPD_CB.mnemonic:		S",
  "*menuBar*editPD_CB.mnemonic:			E",
  "*menuBar*synthButton.labelString:		Synthetic Data File",
  "*menuBar*loadSetupButton.labelString:	Load setup",
  "*menuBar*loadSetupButton.mnemonic:		L",
  "*menuBar*loadSetupButton.accelerator:	Ctrl<Key>L",
  "*menuBar*loadSetupButton.acceleratorText:	Ctrl-L",
  "*menuBar*saveSetupButton.labelString:	Save setup",
  "*menuBar*saveSetupButton.mnemonic:		S",
  "*menuBar*saveSetupButton.accelerator:	Ctrl<Key>S",
  "*menuBar*saveSetupButton.acceleratorText:	Ctrl-S",
  "*menuBar*printButton.labelString:		Printsetup",
  "*menuBar*printButton.mnemonic:		P",
  "*menuBar*printButton.accelerator:		Ctrl<Key>P",
  "*menuBar*printButton.acceleratorText:	Ctrl-P",
  "*menuBar*cancelSetupButton.labelString:	Quit",
  "*menuBar*cancelSetupButton.mnemonic:		Q",
  "*menuBar*cancelSetupButton.accelerator:	Ctrl<Key>Q",
  "*menuBar*cancelSetupButton.acceleratorText:	Ctrl-Q",
  "*menuBar*editDefaultsButton.labelString:	Edit Defaults",
  "*menuBar*editDefaultsButton.mnemonic:	D",
  "*menuBar*editDefaultsButton.accelerator:	Ctrl<Key>D",
  "*menuBar*editDefaultsButton.acceleratorText:	Ctrl-D",
  "*menuBar*editConfiguration.labelString:	Edit Configuration",
  "*menuBar*editConfiguration.mnemonic:		C",
  "*menuBar*editConfiguration.accelerator:	Ctrl<Key>C",
  "*menuBar*editConfiguration.acceleratorText:	Ctrl-C",
  "*menuBar*toggleOutput.labelString:		Toggle output",
  "*menuBar*toggleOutput.mnemonic:		O",
  "*menuBar*toggleOutput.accelerator:		Ctrl<Key>O",
  "*menuBar*toggleOutput.acceleratorText:	Ctrl-O",
  "*menuBar*toggleRate.labelString:		Toggle rate",
  "*menuBar*toggleRate.mnemonic:		R",
  "*menuBar*toggleRate.accelerator:		Ctrl<Key>R",
  "*menuBar*toggleRate.acceleratorText:		Ctrl-R",
  "*menuBar*toggleProbe.mnemonic:		P",
  "*menuBar*toggleProbe.accelerator:		Ctrl<Key>P",
  "*menuBar*toggleProbe.acceleratorText:	Ctrl-P",
  "*goButton.height:				37",
  "*goButton.labelString:			Start Processing",
  "*goButton.recomputeSize:			False",
  "*goButton.traversalOn:			False",
  "*goButton.width:				200",
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
  "*listRC.width:				750",
  "*listTitle.alignment:			ALIGNMENT_BEGINNING",
  "*varList.background:				black",
  "*varList.foreground:				green",
  "*varList.listSizePolicy:			CONSTANT",
  "*varList.selectionPolicy:			EXTENDED_SELECT",
  "*varList.scrollBarDisplayPolicy:		STATIC",
  "*varList.visibleItemCount:			16",
  "*logText.background:				Gray",
  "*logText.editable:				FALSE",
  "*logText.editMode:				MULTI_LINE_EDIT",
  "*logText.rows:				20",
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
  "*lagLabel.labelString:			Static Lag (ms)",
  "*lagText.columns:				4",
  "*lagText.maxLength:				4",
  "*spikeForm.marginWidth:			15",
  "*spikeForm.horizontalSpacing:		15",
  "*spikeForm.verticalSpacing:			3",
  "*spikeLabel.labelString:			Spike Slope",
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

  "*synthConstText.columns:			10",
  "*synthConstText.maxLength:			10",

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

  /* Edit Configuration
   */
  "*editFlightShell.x:				600",
  "*editFlightShell.y:				150",
  "*editFlightShell.configWindow.spacing:	10",
  "*configWindow.XmFrame.XmLabel.childHorizontalAlignment: ALIGNMENT_CENTER",
  "*configWindow.XmFrame.XmLabel.childVerticalAlignment:   ALIGNMENT_BASELINE_TOP",
  "*configWindow.XmFrame.XmLabel.childHorizontalSpacing:   5",
  "*configWindow.XmFrame.XmLabel.childType:        FRAME_TITLE_CHILD",

  "*flightEntryRC.orientation:			HORIZONTAL",
  "*flightInfoTitle.labelString:		Flight Information",
  "*timeSliceTitle.labelString:			Time Period to Process",
  "*rateTitle.labelString:			Processing Rate",
  "*interpTitle.labelString:			Interpolation Type",
  "*twoDpmTitle.labelString:			PMS-2D Particle Acceptance Method",
  "*twoDarrTitle.labelString:			PMS-2D Area Ratio Rejection",
  "*createFilesTitle.labelString:		Auxilary File Creation",
  "*optionsTitle.labelString:			Processing Options",
  "*efiRC.entryAlignment:			ALIGNMENT_CENTER",
  "*efiRC.spacing:				12",
  "*efiRC.XmPushButton.recomputeSize:		False",
  "*efiRC.XmPushButton.width:			130",
  "*efiRC.orientation:				HORIZONTAL",
  "*rateRadioBox.orientation:			HORIZONTAL",
  "*lowRateButton.labelString:			Low Rate",
  "*lowRateButton.traversalOn:			False",
  "*sampleRateButton.labelString:		Sample Rate",
  "*sampleRateButton.traversalOn:		False",
  "*highRate25Button.labelString:		High Rate (25)",
  "*highRate25Button.traversalOn:		False",
  "*highRate50Button.labelString:		High Rate (50)",
  "*highRate50Button.traversalOn:		False",
  "*highRate100Button.labelString:		High Rate (100)",
  "*highRate100Button.traversalOn:		False",
  "*radioFrame.shadowType:			SHADOW_IN",
  "*radioFrame.entryAlignment:			ALIGNMENT_CENTER",
  "*interpRadioBox.orientation:			HORIZONTAL",
  "*linearButton.labelString:			Linear",
  "*linearButton.traversalOn:			False",
  "*csplineButton.labelString:			Cubic Spline",
  "*csplineButton.traversalOn:			False",
  "*akimaButton.labelString:			Akima Spline",
  "*akimaButton.traversalOn:			False",
  "*centerInButton.labelString:			Center In",
  "*centerInButton.traversalOn:			False",
  "*reconstructionButton.labelString:		Reconstruction",
  "*reconstructionButton.traversalOn:		False",
  "*kmlButton.labelString:			Google Earth KML File",
  "*kmlButton.traversalOn:			False",
  "*navButton.labelString:			IWGADTS ASCII File (real-time broadcast)",
  "*navButton.traversalOn:			False",

  "*optFrame.shadowType:			SHADOW_IN",
  "*optFrame.entryAlignment:			ALIGNMENT_CENTER",
  "*despikeButton.labelString:			Despiking",
  "*despikeButton.traversalOn:			False",
  "*lagButton.labelString:			Time shifting",
  "*lagButton.traversalOn:			False",
  "*blankButton.labelString:			Blankout Variables",
  "*blankButton.traversalOn:			False",
  "*hwCleanButton.labelString:			Honeywell IRS cleanup (ADS2 only)",
  "*hwCleanButton.traversalOn:			False",
  "*inertialShift.labelString:			1 second GPS/CMIGITS shift (ADS2 only)",
  "*inertialShift.traversalOn:			False",

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
  "*defWin.height:			510",
  "*defWin*XmLabel.height:		35",
  "*defWin*XmLabel.width:		150",
  "*defWin*XmLabel.alignment:		ALIGNMENT_END",

  /* Dialog Boxes
   */
  "*fileBox.defaultPosition:		FALSE",
  "*fileBox.x:				350",
  "*fileBox.y:				200",
  "*fileBox*textColumns:		50",
  "*fileBox*dialogTitle:		Select a File",
  "*errorBox.defaultPosition:		FALSE",
  "*errorBox.x:				450",
  "*errorBox.y:				300",
  "*errorBox*background:		DarkSlateGray",
  "*errorBox*dialogTitle:		Error Message",
  "*errorBox*foreground:		Red",
  "*errorBox*XmLabel.alignment:		ALIGNMENT_CENTER",
  "*queryBox*dialogTitle:		Need input",
  "*queryBox*alignment:			ALIGNMENT_CENTER",
  "*warnBox.defaultPosition:		FALSE",
  "*warnBox.x:				450",
  "*warnBox.y:				300",
  "*warnBox*background:			DarkSlateGray",
  "*warnBox*dialogTitle:		Warning Message",
  "*warnBox*foreground:			Yellow",
  "*warnBox*XmLabel.alignment:		ALIGNMENT_CENTER",
  NULL
  };
