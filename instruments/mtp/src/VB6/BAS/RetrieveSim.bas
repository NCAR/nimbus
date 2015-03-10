Attribute VB_Name = "RetrievalSubs"
Option Explicit
' RET
  Public ShowZp As Boolean, RAWscans%, Tbase!
  Public T1#(1 To 33), T2#(1 To 33)
  Public PALTseq!(1 To 15), pALTn$(1 To 15)
  Public Trop1!, Trop2!, SE1!
  Public ByteCount&, LOFraw&, dZg!, LOFbin&, RecordCount&, BINrecords&
  Public UseSmatrix As Boolean           'Apply TB Sensitivity Matrix to AA Observables to correct for bias
  Public RemoveOBbias As Boolean         'Remove Observable Bias from AA Observables
  Public UseOriginalMRI As Boolean       'Use population SD
  Public TAavg!(1 To 33), ZAavg!(1 To 33)
  Public ATPbias!
  Public OBbias!(0 To 64)                'OB bias, one for each set of RCs
  Public OBbiasUp!(0 To 64)              'OB bias for Up RCs, one for each set of RCs
  Public OBbiasDn!(0 To 64)              'OB bias for Dn RCs, one for each set of RCs
  Public Tshift!(0 To 64)
  Public BiasMode%                       '1 = bias of all observables, 2 = bias at FL only
  
' Retrieval arrays
  Public ob!(0 To 32)                    'Raw observables ob, either 0-19, or 0-28
  Public OBup!(1 To 32)                  'Observables for UP retrieval
  Public OBdn!(1 To 32)                  'Observables for DN retrieval
  Public OBavgWt!(0 To 64, 1 To 30)      '65 sets of RCs for 30 observables
  Public OBrmsWt!(1 To 30)
  Public TAA!(0 To 64, 1 To 3, 1 To 30)  'AA observables for each RC set and frequency
  Public TAAi!(1 To 33)                  'AA T-profile interpolated to FL
  Public TAAi2!(1 To 33)                 'AA T-profile interpolated to FL
  Public Stats!(1 To 3, 1 To 2)          'Avg, RMS, and Total Error in ATP compared to RAOBs and AA ATP
  Public OBcorr!(0 To 64, 1 To 30)

  Public lnP!(0 To 64)                   'log of TB probability
  Public lnPup!(0 To 64)                 'log of TB probability for Up observables
  Public lnPdn!(0 To 64)                 'log of TB probability for Dn observables
  Public TBavg!(0 To 64)                 'ob()-obAA bias
  Public TBrms!(0 To 64)                 'ob()-obAA rms
  Public TBavgUp!(0 To 64)               'ob()-obAA bias up observables only
  Public TBrmsUp!(0 To 64)               'ob()-obAA rms up observables only
  Public TBavgDn!(0 To 64)               'ob()-obAA bias down observables only
  Public TBrmsDn!(0 To 64)               'ob()-obAA rms down observables only

  Public ShowArcAvgRet As Boolean, ArcAvgRet As Boolean
  Public Speed%, pid$, fid$
  Public Remote%
  Public CAC%

  Public NeedReset As Boolean
  Public RollFlag As Boolean
  Public Risky As Boolean
  Public useMMS As Boolean
  Public FirstPass As Boolean
  Public DoNAT As Boolean                'If TRUE then use Tnat instead of Tice
  Public TtgtFix As Boolean              'Set TRUE when target temperature has been editted for spikes
  Public RollThreshold!                  'Threshold at which to set RollThresholdFlag
  Public RollThresholdFlag As Boolean    'Set TRUE whenever Roll > RollThreshold
  Public CycleTime!, Err53count%, Err57count%, Errxxcount%
  Public FooterTitle$, FooterFormat$
  Public Quit As Boolean
  
  Public PrintDiagnostics As Boolean, TSTlu%
  Public IlnPmin1%, lnPmin1!             'Index of best RCs amongst those used
  Public IlnPmin2%, lnPmin2!             'Index of second best RCs amongst those used
  Public IlnPmin3%, lnPmin3!             'Index of third best RCs amongst those used
  Public IlnPminUp%, lnPminUp!           'Index of best Up RC set amongst those used
  Public IlnPminDn%, lnPminDn!           'Index of best Dn RC set amongst those used

Sub CalcBiasRmsWrtAA(ByRef ob!(), ByVal iRC%, ByVal OBbias!, UpDn As Boolean)
Dim X!, Wt!, i%, j%, k%, Wi!, SWi!, Sum1!, Sum2!, Xw!, rVar!, NobsP!
' This code assumes that RCformat=3 (30 observables)
' Calculate ob() bias and rms wrt AA observables using all observables
  
  Sum1 = 0#
  Sum2 = 0#
  SWi = 0#
  NobsP = 0
  For j = 1 To Nobs
    Wi = 1 / OBrmsWt(j) ^ 2                    'Weight Wi
    SWi = SWi + Wi                             'Sum of Weights
    X = (ob(j) - OBavgWt(iRC, j) - OBbias)     'Compare measured TBs to OBavgWt
    If Wi > 0# Then
      NobsP = NobsP + 1
      Sum1 = Sum1 + Wi * X                     'Weighted average
      Sum2 = Sum2 + Wi * X ^ 2                 'Sum of squares
    End If
  Next j
  Xw = Sum1 / SWi                         'Calculate Weighted Mean
  TBavg(iRC) = Xw
' Correct expression for weighted mean
' See NIST site: http://www.itl.nist.gov/div898/software/dataplot/refman2/ch2/weightsd.pdf
' SDw = SQRT(Sum(1->N)wi*(xi - xw)^2 / (Np-1)* Sum(1->N)wi / Np)
' Xw= weighted avg= Sum(1->N)wi xi / Sum(1->N)wi = Sum(1->N)wi xi / Swi
' where  wi= weights, Swi= sum of weights. Np= number of non-zero weights
' This expands to:
' SDw = SQRT((Sum(1->N)wi*xi^2 - 2*wi*xi + wi*xw^2) / (Np-1)* Swi / Np)
'     = SQRT((Sum(1->N)wi*xi^2 - 2*wi*xi*xw + wi*xw^2) / (Np-1)* Swi / Np)
'     = SQRT((Sum2 - 2*Swi*xw^2 + Swi*xw^2)) / (Np-1)* Swi / Np)
'     = SQRT((Sum2 - Swi*xw^2)) / (Np-1)* Swi / Np)
  X = (Sum2 - SWi * Xw ^ 2) / ((NobsP - 1) * SWi / NobsP)
  If X >= 0 Then
    TBrms(iRC) = Sqr(X)     'Calculate Std Dev about the mean
  Else
    TBrms(iRC) = TBavg(iRC) 'xxxmjm 2009/02/03
  End If
  If Not UpDn Then Exit Sub
  If (Nobs Mod 10) <> 0 Then Exit Sub
' Calculate ob() bias and rms wrt AA observables using only upward observables
  Sum1 = 0#
  Sum2 = 0#
  SWi = 0#
  NobsP = 0#
  For i = 1 To Channels
    For j = 1 To LocHor
      k = j + (i - 1) * 10
      Wi = 1 / OBrmsWt(k) ^ 2
      SWi = SWi + Wi
      X = (ob(k) - OBavgWt(iRC, k) - OBbias)  'Compare measured TBs to OBavgWt
      If Wi > 0 Then
        NobsP = NobsP + 1
        Sum1 = Sum1 + Wi * X                       'Weighted average
        Sum2 = Sum2 + Wi * X ^ 2                   'Sum of squares
      End If
      'Debug.Print k; ob(k); OBavgWt(iRC, k); OBbias; X
    Next j
  Next i
  Xw = Sum1 / SWi
  TBavgUp(iRC) = Xw                  'Calculate mean
  TBrmsUp(iRC) = Sqr((Sum2 - SWi * Xw ^ 2) / ((NobsP - 1) * SWi / NobsP))  'Calculate Std Dev about the mean
  
' Calculate ob() bias and rms wrt AA observables using only downward observables
  Sum1 = 0#
  Sum2 = 0#
  SWi = 0#
  NobsP = 0#
  NobsP = Channels * (10 - LocHor + 1)
  For i = 1 To Channels
    For j = LocHor To 10
      k = j + (i - 1) * 10
      Wi = 1 / OBrmsWt(k) ^ 2
      SWi = SWi + Wi
      X = (ob(k) - OBavgWt(iRC, k) - OBbias)  'Compare measured TBs to OBavgWt
      If Wi > 0 Then
        NobsP = NobsP + 1
        Sum1 = Sum1 + Wi * X                       'Weighted average
        Sum2 = Sum2 + Wi * X ^ 2                   'Sum of squares
      End If
    Next j
  Next i
  Xw = Sum1 / SWi
  TBavgDn(iRC) = Xw                  'Calculate mean
  TBrmsDn(iRC) = Sqr((Sum2 - SWi * Xw ^ 2) / ((NobsP - 1) * SWi / NobsP))  'Calculate Std Dev about the mean
   
End Sub

Function fCalcBiasWrtAA(ByRef ob!(), ByVal iRC%, ByVal Mode%)
Dim Sum1!, SWi!, Wi!, i%, j%, X!
' Mode 1 = Bias using all observables
' Mode 2 = Bias with respect to FL temperature
  
  Select Case Mode
  Case 1
    Sum1 = 0#
    SWi = 0#
    For j = 1 To Nobs
      Wi = 1 / OBrmsWt(j) ^ 2               'Weight Wi
      SWi = SWi + Wi                        'Sum of Weights
      X = (ob(j) - OBavgWt(iRC, j)) * Wi    'Compare measured TBs to OBavgWt
      Sum1 = Sum1 + X                       'Weighted average
    Next j
    fCalcBiasWrtAA = Sum1 / SWi             'Calculate Weighted Mean
  Case 2
    X = 0
    For i = 1 To Channels
      X = X + (ob(LocHor + (i - 1) * 10) - OBavgWt(iRC, LocHor + (i - 1) * 10))
    Next i
    fCalcBiasWrtAA = X / Channels
  End Select
End Function

Function fGetiRC(i%, f As Form)
Dim j%
' Use cboReg text entry to get RC index iRC

  j = -1
  Do
    j = j + 1
  Loop Until f.cboReg(j).List(j) = f.cboReg(i).Text Or j = NRCmax - 1
  fGetiRC = j
  
End Function

Function fLevelBelowFLwt(pALT!, ib%)
' Calculate weights for above and below flight levels
  If FLA(ib - 1) <> FLA(ib) Then
    fLevelBelowFLwt = 1 - (pALT * 100# - FLA(ib)) / (FLA(ib - 1) - FLA(ib))
  Else
    fLevelBelowFLwt = 1
  End If
End Function

Function fSet_Apriori_OBrms(f As Form) As Boolean
Dim i%, j%, k%, RMS!(1 To 30), RMSlu%, OBrmsfile$
' This subroutine sets the apriori observable errors
' These are a combination of the WCT noise and radiometric noise
' Both are dependent on the Sensor Unit used and the campaign
' And as of 2004.05.12 are stored in Mission.RMS files in the mission folder
' These files are generated from the mission WCT.xls spreadsheet by copying
' the OB Noise tab to the text conversion utility on the Tools form.

  OBrmsfile$ = SUpath$ + Mission$ + "_RCS.txt"
  If Len(Dir$(OBrmsfile)) = 0 Then
    OBrmsfile$ = SUpath$ + Mission$ + "_RMS.txt"
    If Len(Dir$(OBrmsfile)) = 0 Then
      OBrmsfile$ = Drive$ + "\MTP\Setup\" + fAC(AC$) + "default.RMS"
      If Len(Dir$(OBrmsfile)) = 0 Then
        MsgBox "You must create a file named: " & OBrmsfile$ & vbCrLf & "Containing the default observable errors.", vbOKOnly
        fSet_Apriori_OBrms = False
        Exit Function
      End If
    End If
    RMSlu% = FreeFile
    Open OBrmsfile$ For Input As RMSlu
    Nobs = 0
    Do
      Nobs = Nobs + 1
      Input #RMSlu, RMS(Nobs)
    Loop Until EOF(RMSlu)
    Close RMSlu
  Else
    Call ReadMissionRCS(Mission$, "OBSERVABLE_ERRORS", False)
  End If
  
  Select Case Nobs
  Case 19, 28: RCformatUsed = 2
  Case 20, 30: RCformatUsed = 3
  End Select
  
  For j = 1 To NFL
    For i = 1 To Nobs
      OBrms(j, i) = sOBrms(i)    'RMS(i)
    Next i
  Next j

' Check to see if any channels are disabled and downweight if so
  If f.chkDisabledChannels(0).Value = 1 Then   'Channel 1 not used
    If RCformatUsed = 2 Then k = 9 Else k = 10
    For j = 1 To NFL
      For i = 1 To k:  OBrms(j, i) = 9999#: Next i
    Next j
  End If

  If f.chkDisabledChannels(1).Value = 1 Then   'Channel 2 not used
    If RCformatUsed = 2 Then k = 18 Else k = 20
    For j = 1 To NFL
      For i = 10 To 18:  OBrms(j, i) = 9999#: Next i
    Next j
  End If

  If f.chkDisabledChannels(2).Value = 1 Then   'Channel 3 not used
    If RCformatUsed = 2 Then k = 27 Else k = 30
    For j = 1 To NFL
      For i = 19 To 27:  OBrms(j, i) = 9999#: Next i
    Next j
  End If

  fSet_Apriori_OBrms = True

End Function

Sub Do_RetrievalSim(Algorithm%, Parameter!, ob!(), RT!(), RA!(), frm As Form)
Static Aavg!, Arms!, LATold!
Dim w1!, w2!, X!, T1017!, Fregion!, MRIsum!, RemoveBias As Boolean

' Retrieval Algorithm Index Definition:
' 0 ... Use specified RCs
' 1 ... MRI-based algorithm
' 2 ... Latitude Blend algorithm
' 3 ... Longitude Blend
' 4 ... UT Blend
' 5 ... Retrievable Index
' 6 ... Observable Index
' 7 ... Separate Up Dn OBs

' In with following parameters:
' Algorithm
' Latitude
' ob() .... observable array
' frm ..... Calling form
'
' Out with the following
' RT() ... Temperature profile
' RA() ... Altitudes
' T1017
'
' Common parameters used:
' LAT1, LAT2, LAT3, LAT4, MRI, MRIavg, MRIrms
' ALTkm, RetAltMin
'
' Local variables
'  absLAT!, Index%,f!, fr!, i%,j%,L%
' RT1!(), RT2!(), MRI1sum, MRI2sum, LATT!, LATold!
' T1!(),T2!(), jt1%, jt2%, T10km!, T17km!
Dim absLAT!, Index%, f!, fr!, i%, j%, L%
Dim RT1!(1 To 33), RT2!(1 To 33), LATT!
Dim jt1%, jt2%, T10km!, T17km!, dZ!, dT!

RCindex1 = -1
RCindex2 = -1
  For i = 0 To NRCbin - 1
    frm.txtWavg(i).Text = "": frm.txtWrms(i).Text = ""
  Next i

Select Case Algorithm
Case 0  'Just use User Selected RCs
  Call RetrieveATP(UserRCindex, ob(), RT(), RA(), Aavg, Arms)
  Stats(1, UserRCindex) = Aavg
  Stats(2, UserRCindex) = Arms
  Stats(3, UserRCindex) = Sqr(Aavg ^ 2 + Arms ^ 2)
  If frm.chkShowTotalError.Value = 0 Then
    frm.txtWavg(UserRCindex).Text = Format(Aavg, "#0.0")
    frm.txtWrms(UserRCindex).Text = Format(Arms, "#0.0")
  Else
    frm.txtWavg(UserRCindex).Text = Format(Stats(3, UserRCindex), "#0.0")
  End If

Case 1 'MRI Retrieval
' First, use latitude-based algorithm to decide initial RC region
' Set MRI to correspond to latitude (between 0 and 2)
' Default synoptic transition values are: LAT1=18, LAT2=28, LAT3=45, LAT4=53
' These can be over-ridden if better information is available, eg XS info
'
'  LAT1 = 18   'xxx mjm 19991130 These need to be set whenever MRI retrieval is used!!
'  LAT2 = 28
'  LAT3 = 45
'  LAT4 = 53
  Call Do_Blend(Parameter!, ob!(), RT!(), RA!(), frm)
' Now use new algorithm to determine retrieval coefficients based on
' temperatures at 10 and 17 km from first retrieval ...  BLG 980522
' However, skip if lat is <18 or >53 and use lat-based results only
' Needed to remark out the following code to avoid MRI discontinuity xxx 19981017 mjm
' xxx unremarked mjm 19991201 as Region2 was seeing midlat retrievals! Not sure
' why MRI discontinuity was an issue.
' xxx unremarked mjm 20000215 as was getting MRI discontinuity!
'  Select Case MRI
'  Case 0, 1, 2  ' Integer MRI, nothing to do!
'  Case Else
'   First, figure out number of non-zero retrieved levels
    jt1 = 0: Do: jt1 = jt1 + 1: Loop Until RA(jt1) > 0 Or jt1 = 33
    jt2 = 34: Do: jt2 = jt2 - 1: Loop Until RA(jt2) > 0 Or jt2 = 1
    If jt1 = 33 Then Exit Sub 'GoTo EndOfSub 'GoTo ReadLine      ' No retrieved level, start all over
' Do the SPLINE fit to retrieved ATP
    Call SPLINE(RA(), RT(), T1(), T2(), jt1, jt2)
' And calculate the Temperatures at 10 km and 17 km
    T10km = SPLINT(RA(), RT(), T2(), jt1, jt2, 10#, True)
    T17km = SPLINT(RA(), RT(), T2(), jt1, jt2, 17#, True)
    T1017 = T10km - T17km
    Select Case T1017             ' Calculate Region Number based on Delta T
    Case Is >= 38: MRI = 0
    Case Is >= 24: MRI = (38 - T1017) / 14
    Case Is >= 5
      MRI = 1
    Case Is >= 1
      MRI = 1 + (5 - T1017) / 4
    Case Else: MRI = 2
    End Select
'Debug.Print "MRI2="; MRI
' Use the new T-based rather than Latitude-based region
    Index = Int(MRI): f = MRI - Index
    If f = 0 Then            'Pure Tropical, Mid-latitude, or Arctic
      Call RetrieveATP(Index, ob(), RT(), RA(), Aavg, Arms)
      For i = 0 To 2: frm.txtWavg(i).Text = "": frm.txtWrms(i).Text = "": Next i
      frm.txtWavg(Index).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index).Text = Format(Arms, "#0.0")
                             'RT() & RA() are set, Retrieve T()
    Else                     'non-integer value, requiring blend of regions
      If Index = 0 Then
        frm.txtWavg(2).Text = "": frm.txtWrms(2).Text = ""
      Else
        frm.txtWavg(0).Text = "": frm.txtWrms(0).Text = ""
      End If
      Call RetrieveATP(Index, ob(), RT1(), RA(), Aavg, Arms)
      frm.txtWavg(Index).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index).Text = Format(Arms, "#0.0")
     
      Call RetrieveATP(Index + 1, ob(), RT2(), RA(), Aavg, Arms)
      frm.txtWavg(Index + 1).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index + 1).Text = Format(Arms, "#0.0")
'     Now interpolate between the two solutions
      For j = 1 To 33: RT(j) = RT1(j) + f * (RT2(j) - RT1(j)): Next j
    End If
'  End Select
' Keep track of avg and rms MRI (MJM 980831)
  If pALT > RetAltMin Then
    MRI1sum = MRI1sum + MRI         'Sum MRI values
    MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
    INC MRIrec
    If MRIrec > 10 Then
      MRIavg = MRI1sum / MRIrec                           'Calculate average
      MRIrms = Sqr((MRI2sum - MRIrec * MRIavg ^ 2) / (MRIrec - 1)) 'Calculate RMS
    End If
    'Debug.Print MRI; MRIavg; MRIrms
  End If

Case 2, 3, 4        'Old WB57 Algorithm
' Start of blending code, Correct LAT to Latitude 981006 mjm
  Select Case Algorithm
  Case 2: Parameter = Latitude
  Case 3: Parameter = Longitude
  Case 4: Parameter = UTsec / 1000#
  End Select
  Call Do_Blend(Parameter!, ob!(), RT!(), RA!(), frm)

  Fregion = (Int(100 * MRI)) / 100  'Put in footer, different defn from DC8
  MRIavg = 0#: MRIsum = 0#

Case 5  'Retrievable Index


Case 6  'Observable Index
  RCindex1 = aRC(IlnPmin1)
  Call RetrieveATP(RCindex1, ob(), RT1(), RA(), Aavg, Arms)
  Stats(1, IlnPmin1) = Aavg
  Stats(2, IlnPmin1) = Arms
  Stats(3, IlnPmin1) = Sqr(Aavg ^ 2 + Arms ^ 2)
  If frm.chkShowTotalError.Value = 0 Then
    frm.txtWavg(IlnPmin1).Text = Format(Aavg, "#0.0")
    frm.txtWrms(IlnPmin1).Text = Format(Arms, "#0.0")
  Else
    frm.txtWavg(IlnPmin1).Text = Format(Stats(3, IlnPmin1), "#0.0")
  End If
  If NRC > 1 Then
    lnPmin2 = 9999#
    For i = 0 To NRC - 1 'Find next closest RC set
      If i <> IlnPmin1 Then
        If lnP(i) < lnPmin2 Then lnPmin2 = lnP(i): IlnPmin2 = i
      End If
    Next i
    ' Blend only if RC sets give opposite signs wrt to observables
    RCindex2 = aRC(IlnPmin2)
    If Sgn(TBavg(RCindex2)) <> Sgn(TBavg(RCindex1)) And frm.chkDisableBlending.Value = False Then
      Call RetrieveATP(RCindex2, ob(), RT2(), RA(), Aavg, Arms)
      Stats(1, IlnPmin2) = Aavg
      Stats(2, IlnPmin2) = Arms
      Stats(3, IlnPmin2) = Sqr(Aavg ^ 2 + Arms ^ 2)
      If frm.chkShowTotalError.Value = 0 Then
        frm.txtWavg(IlnPmin2).Text = Format(Aavg, "#0.0")
        frm.txtWrms(IlnPmin2).Text = Format(Arms, "#0.0")
      Else
        frm.txtWavg(IlnPmin2).Text = Format(Stats(3, IlnPmin2), "#0.0")
      End If
'     Now take weighted average between the two solutions with weighting by inverse variance
      If TBavg(RCindex1) = 0 And TBrms(RCindex1) = 0 Then w1 = 1 Else w1 = 1# / (TBavg(RCindex1) ^ 2 + TBrms(RCindex1) ^ 2)
      If TBavg(RCindex2) = 0 And TBrms(RCindex2) = 0 Then w2 = 1 Else w2 = 1# / (TBavg(RCindex2) ^ 2 + TBrms(RCindex2) ^ 2)
      For j = 1 To 33: RT(j) = (w1 * RT1(j) + w2 * RT2(j)) / (w1 + w2): Next j
      RCwt = w1 / (w1 + w2)
    Else
      'frm.txtWavg(IlnPmin2).Text = Format(0#, "#0.0")
      'frm.txtWrms(IlnPmin2).Text = Format(0#, "#0.0")
      For j = 1 To 33
        RT(j) = RT1(j)
        'Debug.Print j, RA(j); RT(j)
      Next j
      RCwt = 1#
    End If
  Else
    For j = 1 To 33: RT(j) = RT1(j): Next j
  End If
' Calculate pseudo MRI to monitor how good fit is (RMS<10 - MRI=0, RMS< 1000 - MRI=1-2)
  If lnP(IlnPmin1) = 0 Then X = 0 Else X = Log(lnP(IlnPmin1)) / Log(10)
  Select Case X
  Case Is <= 0: MRI = 0
  Case Is < 2: MRI = X
  Case Else: MRI = 2
  End Select
  
  If pALT > RetAltMin Then
    MRI1sum = MRI1sum + MRI         'Sum MRI values
    MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
    INC MRIrec
    If MRIrec > 10 Then
      MRIavg = MRI1sum / MRIrec                            'Calculate average
      If MRI2sum - MRIrec * MRIavg ^ 2 > 0 Then
        MRIrms = Sqr((MRI2sum - MRIrec * MRIavg ^ 2) / (MRIrec - 1)) 'Calculate RMS
      End If
    End If
    'Debug.Print MRI; MRIavg; MRIrms
  End If

Case 7  'Separate Up Dn OBs
    If frm.chkRemoveAAbias.Value = 1 Then RemoveBias = True Else RemoveBias = False
'    RemoveBias = False
    RCindex1 = aRC(IlnPminUp)
    Call RetrieveATPup(RCindex1, OBup(), RT1(), RA(), Aavg, Arms, RemoveBias)
    If IlnPminUp < NRCbin Then
      frm.txtWavg(IlnPminUp).Text = Format(Aavg, "#0.0")
      frm.txtWrms(IlnPminUp).Text = Format(Arms, "#0.0")
    End If
    RCindex2 = aRC(IlnPminDn)
    Call RetrieveATPdn(RCindex2, OBdn(), RT2(), RA(), Aavg, Arms, RemoveBias)
    If IlnPminDn < NRCbin Then
      frm.txtWavg(IlnPminDn).Text = Format(Aavg, "#0.0")
      frm.txtWrms(IlnPminDn).Text = Format(Arms, "#0.0")
    End If
    For j = 16 To 33
      RT(j) = RT1(j)
    Next j
    For j = 1 To 16
      RT(j) = RT2(j)
    Next j
    RCwt = 1#
    OATmtpCOR = OAT - RT(16)

    If pALT > RetAltMin And GoodScan Then
      MRI1sum = MRI1sum + MRI         'Sum MRI values
      MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
      INC MRIrec
      If MRIrec > 10 Then
        MRIavg = MRI1sum / MRIrec                           'Calculate average
        X = (MRI2sum - MRIrec * MRIavg ^ 2)
        If X > 0 Then
          MRIrms = Sqr(X / (MRIrec - 1)) 'Calculate RMS
        End If
      End If
    ' Debug.Print MRI; MRIavg; MRIrms
    End If

End Select
End Sub

Function fZnavCorr(ByVal z!, ByVal dUT!) As Single
' z is NAV pressure altitude (km)
' dUT are UT ks since takeoff
' Return with corrected NAV pressure altitude (km)

  Select Case AC$
  Case "ER"
    Select Case Mission$
    Case "CAMEX4"
      'ZPnavCOR [meters] = 0 + 85 * (utsec - utsec_takeoff)/14400 + 162 * (ZPnav/20 km)2 + 105 * (ZPnav/20 km)10
      fZnavCorr = z + (85 * dUT / 14.4 + 162 * (z / 20) ^ 2 + 105 * (z / 20) ^ 10) / 1000
    Case Else
      fZnavCorr = z
    End Select

  Case Else
    fZnavCorr = z
  End Select

End Function

Function fTTO(ByVal pALT!) As Single
' Calculate Time Tag Offset (TTO) to location of MTP measurement (includes scan time, flight time)
' Note: TTO should be different for each frequency to avoid optical depth smearing of T
' The TB fit procedure could be used to determine the TTO frequency dependence
' For now there is only the average TTO, eventually this code should use separate TTO's for
' each frequency (TTO1, TTO2, TTO3). This means that there should be separate OATnav values
' for each frequency, which is currently not implemented.
Dim TTO!
' Check for where Sensor Unit is to correct time tag offset for MTP measurement
' On Starboard (RHS) of A/C after 19960325
  Select Case AC$
  Case "DC"
    If RHS Then                              'Time Tag offset (time to scan to
'      TTO = 15 + 0.28 * (pALTft / 1000 - 35) 'horizon (posn 6 on RHS and posn 5
      TTO = 15 + 0.28 * (pALT * kft_km - 35) 'horizon (posn 6 on RHS and posn 5
    Else                                     'on LHS+ time to reach measurement
'      TTO = 14 + 0.28 * (pALTft / 1000 - 35) 'location ahead of A/C) Equation
      TTO = 14 + 0.28 * (pALT * kft_km - 35) 'location ahead of A/C) Equation
    End If                                   'corrects for altitude dependence of absorption coefficient)
  
  Case "ER"
    TTO = NAVoffset * ScanTime   'TTO from TB fit (neglects Zp dependence)
  
  Case Else                                  '
    TTO = NAVoffset * ScanTime   'TTO from TB fit
  End Select
  
' Flight to MTP time assumes channel wts of 0.9, 1, 1 and sideband distances
' of 3.85 & 2.17 (Ch1), 1.33 & 1.06 (Ch2), and 0.57 & 0.60 (Ch3), and an A/C
' speed of 230 m/s. Based on regression against DADS where time variable OATs
' were present, MTP ahead by 0.8 cycle. 0.8*16.7 + 6 s = 19.6 +- 4 s when a
' scan.t.to.horizon time of 6 s was used. Our prediction is: 7.9+6.7 = 14.6 s
' In excellent agreement with 19.6 +- 4 from regression analysis.
  
'  UTsec = UTsecMTP + utMTPcorf                 'Set MTP clock to NAV clock
'  UTsec = Int(UTsec + TTO + 0.5)               'Time Tag Offset
'  If UTsec > 86400 Then UTsec = UTsec - 86400
  
' Now, UTsec times in exchange file correspond to horizon data corrected for
' MTP clock drift, scan time to horizon, and flight time to MTP measurement

  fTTO = TTO

End Function


Sub DetermineRCregion(ob!(), f As Form, f2 As Form, f3 As Form)
' Normally                   frmMTPbin  frmStatus   frmFLTINFO
Dim Tret!(1 To 2, 1 To 33), rVar!, WtB!, WtT!, UpDn As Boolean
Dim iTop%, iBot%, i%, j%, k%, L%, pALT!, iNRC%, iRC%, iRCup%, iRCdn%
Dim z!, Tsum!, Rsum!, X!, Wt!, dT!, Range!(1 To 65), D2!, D3!, RangeMin!, iRangeMin
Dim Coeff1!(0 To 64, 1 To 30), Coeff2!(0 To 64, 1 To 30), obSAVE!(0 To 30)

  RCsiteUsed = False

' Calculate weighted average values of archive average observables
' NRC is number of RC sets
  For iNRC = 0 To NRC - 1
    iRC = aRC(iNRC)
    Call Get_iBot_iTop(iRC, ob(0), iBot, iTop, WtB, WtT)
    If iBot = 1 Then
      iBot = 2: iTop = 1
    End If
    
    For j = 1 To Nobs
      OBavgWt(iRC, j) = OBav(iRC, iBot, j) * WtB + OBav(iRC, iTop, j) * WtT
      OBrmsWt(j) = OBrms(iBot, j) * WtB + OBrms(iTop, j) * WtT
      Coeff1(iRC, j) = SmatrixOB(iRC, iBot, j, 1) * WtB + SmatrixOB(iRC, iTop, j, 1) * WtT
      Coeff2(iRC, j) = SmatrixOB(iRC, iBot, j, 2) * WtB + SmatrixOB(iRC, iTop, j, 2) * WtT
    Next j
  Next iNRC
  
  If f.cboAlgorithm.ListIndex = 7 Then                  'Separate Up and Dn retrievals
    BiasMode = 2
    For iNRC = 0 To NRC - 1
      iRC = aRC(iNRC)
      OBbias(iRC) = fCalcBiasWrtAA(ob(), iRC, BiasMode)  'Calculate weighted OB bias. BiasMode=2 means only at FL
      CalcBiasRmsWrtAA ob(), iRC, OBbias(iRC), True           'Remove bias and calculate TBavg(), TBrms(), TBavgUp, etc
'     The weighted average rms TBrms(iRC), TBrmsUp(iRC) and TBrmsDn(iRC0 are calculated in CalcBiasRmsWrtAA with
      lnPup(iNRC) = 10 * Sqr(TBavgUp(iRC) ^ 2 + TBrmsUp(iRC) ^ 2) / Nobs       'Sum of ln of Probabilities Up
      lnPdn(iNRC) = 10 * Sqr(TBavgDn(iRC) ^ 2 + TBrmsDn(iRC) ^ 2) / Nobs       'Sum of ln of Probabilities Dn
      SetArcAvgObs iRC                                   'Map AA Avg Observable to TAA array for UpdateTBwindow
    Next iNRC

'   Order the lowest values of lnPup(i) and lnPdn(i)
    lnPminUp = 9999#: lnPminDn = 9999#
    IlnPminUp = 99: IlnPminDn = 99
    For iNRC = 0 To NRC - 1
      If lnPup(iNRC) < lnPminUp Then
        IlnPminUp = iNRC: lnPminUp = lnPup(iNRC)
      End If
      If lnPdn(iNRC) < lnPminDn Then
        IlnPminDn = iNRC: lnPminDn = lnPdn(iNRC)
      End If
    Next iNRC
    iRCup = IlnPminUp
    iRCdn = IlnPminDn
    IlnPmin1 = IlnPminUp
    IlnPmin2 = IlnPminDn
    lnP(iRCup) = lnPup(iRCup)
    lnP(iRCdn) = lnPdn(iRCdn)
    
'   RC set is determined with FL OBbias(iRC) removed
'   Now remove bias and Smatrix
    iRCup = IlnPminUp
    iRCdn = IlnPminDn
'    For i = 1 To Channels
'      For j = 1 To LocHor
'        k = j + (i - 1) * 10
     For k = 1 To Nobs
        'OBbias() is ob()-obAA(); therefore negative if obAA()>ob(), positive if obAA()<=ob()
        'Subtracting OBbias makes ob = obAA
        'Then subtract from ATP if RemoveOBbias = TRUE
        OBup(k) = ob(k) - OBbias(iRCup)
        ' If observables are biased wrt AA observables, the sensitivity matrix can be applied
        ' to remove the effect of this bias so that the observables match the RC conditions.
        ' The Sensitivity Matrix accounts for the temperature sensitivity of the observables.
        ' It is calculated in RCmain by fitting linear and quadratic terms for the change
        ' in the observable values for a 1 K temperature shift. There is no bias as the S matrix
        ' involves separate fits for above and below the horizon.
        If UseSmatrix Then
          OBcorr(iRCup, k) = (Coeff1(iRCup, k) * OBbias(iRCup) + Coeff2(iRCup, k) * OBbias(iRCup) ^ 2)
          OBup(k) = OBup(k) - OBcorr(iRCup, k)
        End If
    Next k
'      Next j
'    Next i
    
'    For i = 1 To Channels
'      For j = LocHor To 10
'        k = j + (i - 1) * 10
     For k = 1 To Nobs
        OBdn(k) = ob(k) - OBbias(iRCdn)
        If UseSmatrix Then
          OBcorr(iRCdn, k) = (Coeff1(iRCdn, k) * OBbias(iRCdn) + Coeff2(iRCdn, k) * OBbias(iRCdn) ^ 2)
          OBdn(k) = OBdn(k) - OBcorr(iRCdn, k)
        End If
     Next k
'      Next j
'    Next i
    For iNRC = 0 To NRC - 1
      If f.chkDown.Value = 1 Then
        If iNRC < NRCbin Then  'MTPbin form does not have space for >NRCbin sets of statistics
          f.txtTBavg(iNRC).Text = Format(TBavgDn(aRC(iNRC)), "#0.00")
          f.txtTBrms(iNRC).Text = Format(TBrmsDn(aRC(iNRC)), "#0.00")
          f.txtlnP(iNRC).Text = Format(lnPdn(iNRC), "#0.00")
        End If
      Else
        If iNRC < NRCbin Then  'MTPbin form does not have space for >NRCbin sets of statistics
          f.txtTBavg(iNRC).Text = Format(TBavgUp(aRC(iNRC)), "#0.00")
          f.txtTBrms(iNRC).Text = Format(TBrmsUp(aRC(iNRC)), "#0.00")
          f.txtlnP(iNRC).Text = Format(lnPup(iNRC), "#0.00")
        End If
      End If
    Next iNRC

'   Hilite the RC sets being used
    For iNRC = 0 To NRC - 1
      If iNRC < NRCbin Then
        If iNRC = IlnPmin1 Or iNRC = IlnPmin2 Then
          f.txtlnP(iNRC).BackColor = &H80FF80             'make green
          f2.txtRC1.Text = f.lblRCname(iNRC).Caption
        Else:
          f.txtlnP(iNRC).BackColor = &HFFFFFF             'make gray
        End If
      End If
    Next iNRC

    RCuse(IlnPmin1) = RCuse(IlnPmin1) + 1
    If IlnPmin1 < NRCbin Then f.txtRCuse(IlnPmin1).Text = Str(RCuse(IlnPmin1))
    f3.txtRCuse(IlnPmin1).Text = Str(RCuse(IlnPmin1))   'FLTinfo form

  Else
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'   Need to modify code to treat up and down observables separately, instead of
'   treating them in one group, as this could affect the bias and rms.
'   However, need to work with TBs or redo RC calculation so that 30 observables are used
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'   Calculate sum of probability exponents for deviations weighted by a priori errors
'   First calculate the observable bias and rms wrt the AA observables
'    BiasMode = 2
    For iNRC = 0 To NRC - 1
      iRC = aRC(iNRC)
      OBbias(iRC) = fCalcBiasWrtAA(ob(), iRC, BiasMode)  'Calculate weighted OB bias. BiasMode=2 means only at FL
      If RemoveOBbias Then
        CalcBiasRmsWrtAA ob(), iRC, OBbias(iRC), False  'Remove bias and calculate TBavg(), TBrms(), TBavgUp, etc
      Else
        CalcBiasRmsWrtAA ob(), iRC, 0#, False            'Don't remove bias and calculate TBavg(), TBrms(), TBavgUp, etc
      End If
      'Debug.Print iRC; OBbias(iRC)
'     The weighted average rms TBrms(iRC), TBrmsUp(iRC) and TBrmsDn(iRC0 are calculated in CalcBiasRmsWrtAA with
'     OBbias removed if RemoveOBbias is TRUE.
      If f.chkOriginalMRI.Value = 1 Then
        lnP(iNRC) = 10 * TBrms(iRC) / Nobs   'Sum of ln of Probabilities (original MRI uses population Std Dev)
      Else
        If f.chkTotalError.Value = 1 Then
          lnP(iNRC) = 8 * Sqr(TBrms(iRC) ^ 2 + TBavg(iRC) ^ 2) / Nobs 'Sum of ln of Probabilities
        Else
          lnP(iNRC) = 80 * TBrms(iRC) / Nobs  'Sum of ln of Probabilities
        End If
      End If
'     Original MRI had scale of 0.5, this was changed to 0.05 when the apriori wts were reduced
'     This version uses the population std dev, which includes the bias, so that even
'     if the std dev about the mean is small (ie the shapes of the archive average
'     observables and measured observables are the same) the MRI could still be large.
'     The version below use only the std dev about the mean, but some technique should be
'     developed to take into account the bias, all other things being equal.
'     Factor of 4 above makes MRI old and new agree on a good fit
      SetArcAvgObs iRC 'Map AA Avg Observable to TAA array for UpdateTBwindow
                       'Note that bias has not been removed for TB display
    Next iNRC

'   Order three lowest values of lnP(i)
    lnPmin1 = 9999#: lnPmin2 = 9999#: lnPmin3 = 9999#
    IlnPmin1 = 99: IlnPmin2 = 99: IlnPmin3 = 99
    For iNRC = 0 To NRC - 1
      If lnP(iNRC) < lnPmin1 Then
        IlnPmin3 = IlnPmin2: lnPmin3 = lnPmin2
        IlnPmin2 = IlnPmin1: lnPmin2 = lnPmin1
        IlnPmin1 = iNRC: lnPmin1 = lnP(iNRC)
      Else
        If lnP(iNRC) < lnPmin2 Then
          IlnPmin3 = IlnPmin2: lnPmin3 = lnPmin2
          IlnPmin2 = iNRC: lnPmin2 = lnP(iNRC)
        Else
          If lnP(iNRC) < lnPmin3 Then IlnPmin3 = iNRC: lnPmin3 = lnP(iNRC)
        End If
      End If
    Next iNRC
    iRC = IlnPmin1

'   Check to see if close to RAOB site used to calculate RCs
    If nRAOBtemplates > 0 Then
    ' Get closest RAOB site
      RangeMin = 99999#
      For i = 1 To nRAOBtemplates
        Range(i) = fGCD(Latitude, Longitude, rLatitude(i), rLongitude(i))
        If Range(i) < RangeMin Then RangeMin = Range(i): iRangeMin = i
      Next i
    
      For i = 1 To nRAOBtemplates
        Select Case Range(i)
        Case Is <= 222   'Use if it's one of the three best
          If (RCset(i) = IlnPmin1 Or RCset(i) = IlnPmin2 Or RCset(i) = IlnPmin3) Then
            Select Case RCset(i)
            Case IlnPmin1: IlnPmin2 = IlnPmin1
            Case IlnPmin2: IlnPmin1 = IlnPmin2: IlnPmin2 = IlnPmin1
            Case IlnPmin3: IlnPmin1 = IlnPmin3: IlnPmin2 = IlnPmin1
            End Select
            RCsiteUsed = True
          End If
        Case Is <= 444   'Use if one of the best 2 and closest RAOB site
          If iRangeMin = i And (RCset(i) = IlnPmin1 Or RCset(i) = IlnPmin2) Then
            If RCset(i) = IlnPmin2 Then IlnPmin1 = IlnPmin2
            IlnPmin2 = IlnPmin1
            RCsiteUsed = True
          End If
        Case Else        'Use if best one and closest RAOB site
          If iRangeMin = i And RCset(i) = IlnPmin1 Then IlnPmin2 = IlnPmin1: RCsiteUsed = True
        End Select
    '   Debug.Print i; IlnPmin1; IlnPmin2; IlnPmin3; UTsec; Range(i); RangeMin
      Next i
    End If
  
'   RC set is determined with OBbias(iRC) removed (if necessary)
'   Now remove bias and Smatrix if these are enabled.
    iRC = IlnPmin1
    For j = 1 To Nobs
      'OBbias() is ob()-obAA(); therefore negative if obAA>ob, positive if obAA<=ob
      'Subtracting OBbias makes ob = obAA
      'Then subtract from ATP if RemoveOBbias = TRUE
      If RemoveOBbias Then ob(j) = ob(j) - OBbias(iRC)
      ' If observables are biased wrt AA observables, the sensitivity matrix can be applied
      ' to remove the effect of this bias so that the observables match the RC conditions.
      ' The Sensitivity Matrix accounts for the temperature sensitivity of the observables.
      ' It is calculated in RCmain by fitting linear and quadratic terms for the change
      ' in the observable values for a 1 K temperature shift. There is no bias as the S matrix
      ' involves separate fits for above and below the horizon.
      If UseSmatrix Then
        OBcorr(iRC, j) = (Coeff1(iRC, j) * OBbias(iRC) + Coeff2(iRC, j) * OBbias(iRC) ^ 2)
        ob(j) = ob(j) - OBcorr(iRC, j)
      End If
    Next j
  
'   See if another set of RCs with opposite sense of bias is to be used.
'   If NRC > 1 And Not RCsiteUsed And Not RemoveOBbias Then
'     lnPmin2 = 9999#
'     For iNRC = 0 To NRC - 1  'Find next closest RC set
'       iRC = aRC(iNRC)
'       If iNRC <> IlnPmin1 Then
'         If lnP(iNRC) < lnPmin2 Then lnPmin2 = lnP(iNRC): IlnPmin2 = iNRC
'       End If
'     Next iNRC
'   Else
'     IlnPmin2 = IlnPmin1
'   End If
'   xxxmjm 20090219
'   Fill in the TB display form with calculated statistics
    lnPmin1 = 9999#
    For iNRC = 0 To NRC - 1
      If iNRC < NRCbin Then  'MTPbin form does not have space for >NRCbin sets of statistics
        f.txtTBavg(iNRC).Text = Format(TBavg(aRC(iNRC)), "#0.00")
        f.txtTBrms(iNRC).Text = Format(TBrms(aRC(iNRC)), "#0.00")
        f.txtlnP(iNRC).Text = Format(lnP(iNRC), "#0.00")
      End If
    Next iNRC
  
'   Hilite the RC sets being used
    For iNRC = 0 To NRC - 1
      If iNRC < NRCbin Then
        If iNRC = IlnPmin1 Then
          f.txtlnP(iNRC).BackColor = &H80FF80             'make green
          f2.txtRC1.Text = f.lblRCname(iNRC).Caption
        Else:
          f.txtlnP(iNRC).BackColor = &HFFFFFF             'make gray
        End If
      End If
    Next iNRC
  
    If Sgn(TBavg(aRC(IlnPmin2))) <> Sgn(TBavg(aRC(IlnPmin1))) Then
      If IlnPmin2 < NRCbin Then f.txtlnP(IlnPmin2).BackColor = &H80FF80
      If IlnPmin2 < NRCbin Then f2.txtRC2.Text = f.lblRCname(IlnPmin2).Caption
    Else
      f2.txtRC2.Text = ""                      'Status form
    End If
  
    RCuse(IlnPmin1) = RCuse(IlnPmin1) + 1
    If IlnPmin1 < NRCbin Then f.txtRCuse(IlnPmin1).Text = Str(RCuse(IlnPmin1))
    f3.txtRCuse(IlnPmin1).Text = Str(RCuse(IlnPmin1))   'FLTinfo form

  End If
'Exit Sub

  X = lnP(IlnPmin1)
  Select Case X
  Case Is <= 0: MRI = 0
  Case Is < 2: MRI = X
  Case Else: MRI = 2
  End Select

  X = lnP(IlnPmin2)
  Select Case X
  Case Is <= 0: MRI2 = 0
  Case Is < 2: MRI2 = X
  Case Else: MRI2 = 2
  End Select

End Sub

Sub Get_iBot_iTop(iRC%, pALT!, iBot%, iTop%, WtB!, WtT!)
Dim den!, AltTop!, AltBot!
' Get pair of levels which border pALT [km]
  
  iBot = fLevelBelowFL(pALT)         'Index of level below FL
  iTop = iBot - 1                    'Index of level above FL
  WtB = fLevelBelowFLwt(pALT, iBot)  'Wt of level below FL
  WtT = 1 - WtB                      'Wt of level above FL
 
End Sub




Sub SetOBtoArcAvg(ob!(), OBavgWt!())
Dim iTop%, iBot%, i%, j%, altFR!, WtB!, WtT!
  
' Calculate average value of archive average observables
  For j = 0 To NRC - 1
    Call Get_iBot_iTop(j, ob(0), iBot, iTop, WtB, WtT)
    For i = 1 To Nobs
      OBavgWt(j, i) = OBav(j, iBot, i) * WtB + OBav(j, iTop, i) * WtT
    Next i
  Next j
End Sub

Sub Do_Blend(Parameter!, ob!(), RT!(), RA!(), frm As Form)
Dim Aavg!, Arms!, i%, j%
Dim RT1!(1 To 33), RT2!(1 To 33), LATT!, absLAT!
  
  absLAT = Abs(Parameter)   'Parameter is Latitude, Longitude, or UTks
  For i = 0 To NRC - 1
    frm.txtWavg(i).Text = "": frm.txtWrms(i).Text = ""
  Next i
  
  Select Case absLAT        'Outer SELECT forces Lat<18 and Lat>53 to do
  Case Is <= LAT1
    MRI = 0   'a single retrieval (save time)
    Call RetrieveATP(0, ob(), RT(), RA(), Aavg, Arms)
    frm.txtWavg(0).Text = Format(Aavg, "#0.0")
    frm.txtWrms(0).Text = Format(Arms, "#0.0")
  
  Case Is <= LAT2
    MRI = (absLAT - LAT1) / (LAT2 - LAT1)
    Call RetrieveATP(0, ob(), RT1(), RA(), Aavg, Arms)
    frm.txtWavg(0).Text = Format(Aavg, "#0.0")
    frm.txtWrms(0).Text = Format(Arms, "#0.0")
    Call RetrieveATP(1, ob(), RT2(), RA(), Aavg, Arms)
    frm.txtWavg(1).Text = Format(Aavg, "#0.0")
    frm.txtWrms(1).Text = Format(Arms, "#0.0")
'   Now interpolate between the two solutions
    For j = 1 To 33: RT(j) = RT1(j) + MRI * (RT2(j) - RT1(j)): Next j
  
  Case Is <= LAT3
    MRI = 1   'a single retrieval (save time)
    Call RetrieveATP(1, ob(), RT(), RA(), Aavg, Arms)
    frm.txtWavg(1).Text = Format(Aavg, "#0.0")
    frm.txtWrms(1).Text = Format(Arms, "#0.0")
  
  Case Is <= LAT4
    MRI = 1 + (absLAT - LAT3) / (LAT4 - LAT3)
    Call RetrieveATP(1, ob(), RT1(), RA(), Aavg, Arms)
    frm.txtWavg(1).Text = Format(Aavg, "#0.0")
    frm.txtWrms(1).Text = Format(Arms, "#0.0")
    Call RetrieveATP(2, ob(), RT2(), RA(), Aavg, Arms)
    frm.txtWavg(2).Text = Format(Aavg, "#0.0")
    frm.txtWrms(2).Text = Format(Arms, "#0.0")
'   Now interpolate between the two solutions
    For j = 1 To 33: RT(j) = RT1(j) + (MRI - 1) * (RT2(j) - RT1(j)): Next j
  
  Case Else
    MRI = 2#
    Call RetrieveATP(2, ob(), RT(), RA(), Aavg, Arms)
    frm.txtWavg(2).Text = Format(Aavg, "#0.0")
    frm.txtWrms(2).Text = Format(Arms, "#0.0")
  End Select
  
End Sub

Sub SetArcAvgObs(Index%)
' Put Archive Average Observables in the same order as Calculated TBs
' for display in graphics window
' 20001011 mjm
' Index is RC index, 0 to NRC-1
Dim i%, j%

  If (Nobs Mod 10) <> 0 Then
    Select Case Channels
  ' Works for any LocHor but only 10 elevation angles
    Case 3 ' This is final ob order: CH1:1-5, 7-10, CH2:1-5, 7-10, CH3:1-5, 7-10, OAT
      For j = 1 To LocHor - 1: TAA(Index, 1, j) = OBavgWt(Index, j): Next j
      For j = LocHor + 1 To 10: TAA(Index, 1, j) = OBavgWt(Index, j - 1): Next j
      For j = 11 To 10 + LocHor - 1: TAA(Index, 2, j - 10) = OBavgWt(Index, j - 1): Next j
      For j = 10 + LocHor + 1 To 20: TAA(Index, 2, j - 10) = OBavgWt(Index, j - 2): Next j
      For j = 21 To 20 + LocHor - 1: TAA(Index, 3, j - 20) = OBavgWt(Index, j - 2): Next j
      For j = 20 + LocHor + 1 To 30: TAA(Index, 3, j - 20) = OBavgWt(Index, j - 3): Next j
      For i = 1 To 3: TAA(Index, i, LocHor) = OBavgWt(Index, 28): Next i
    Case 2
      For j = 1 To LocHor - 1: TAA(Index, 1, j) = OBavgWt(Index, j): Next j
      For j = LocHor + 1 To 10: TAA(Index, 1, j) = OBavgWt(Index, j - 1): Next j
      For j = 11 To 10 + LocHor - 1: TAA(Index, 2, j - 10) = OBavgWt(Index, j - 1): Next j
      For j = 10 + LocHor + 1 To 20: TAA(Index, 2, j - 10) = OBavgWt(Index, j - 2): Next j
      For j = 21 To 20 + LocHor - 1: TAA(Index, 3, j - 20) = OBavgWt(Index, j - 2): Next j
      For i = 1 To 2: TAA(Index, i, LocHor) = OBavgWt(Index, 19): Next i
    End Select

  Else   'Natural order with 3 OAT temperatures
    For i = 1 To Channels
      For j = 1 To 10: TAA(Index, i, j) = OBavgWt(Index, j + (i - 1) * 10): Next j
    Next i
    
  End If

End Sub




Sub WMOTropLoc(z0!(), T0!(), T1#(), T2#(), M%, n%, Zt1!, Tzt1!, Zt2!, Tzt2!)
' z0(), T0() contain retrieved altitude and temperature data
' T1()       contains 1st derivatives of T wrt z (from SPLINE)
' T2()       contains 2nd derivatives of T wrt z (from SPLINE)
' n%         contrains number of retrieved altitudes=dim of above arrays
' zt1, zt2   return with two trop altitudes, 99.9 if none found
' Tzt1, Tzt2 return with two trop temperatures, 999.9 if none found
'
' Revision History:
' 960323 ... mjm ... initial version
Dim z!, LRt!, j%, Thickness!, Zlo!, Tlo!, Zhi!, Thi!, pass%, jat1%, LRz!, jbm3%, TZ!

  Zt1 = 99.9: Zt2 = 99.9                        'No Trop Flags
  Tzt1 = 999.9: Tzt2 = 999.9

  j% = -2 + M
  Do: j = j + 1: Loop Until (z0(j + 1) > 5.6 Or j > 31) 'assume Trop >500 mb = 5.6 km
  If j > 32 Then Exit Sub                                            'avoid inversions on ground
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  pass% = 0
BeginM2Trop:                         'Find first trop if any
  Thickness = 2!                       'Required Tropopause thickness
  LRt = -2!                            'LRt has req'd lapse rate of -2 K/km
  Do: j = j + 1
    If j + 1 > n Then Exit Sub
  Loop Until T1(j + 1) >= LRt         'j% is for level BELOW LRt on exit
                                       'LRt is now bound from below
  z = Calc_LR_Level(z0(), T0(), T2(), j, LRt) 'Calc Trop Height at LR=LRt
  If z = 99.9 Then Exit Sub            'Failed to find a solution
  Tlo = SPLINT(z0(), T0(), T2(), M%, n%, z!, True) 'Save z and T at potential trop
  Zlo = z: Zhi = z + Thickness         'Save z and apply thickness criterion
  jat1% = j%                           'And save index ABOVE trop #1 in case
                                       'j% does not increment to force a save
                                       'in LR test below
  Do                                   'Now make sure trop last for 2 km
    z = z + 0.1                        'in 100 m steps
    While z > z0(j%)                   'Bump index j% to correspond to
      j = j + 1                       'level ABOVE current z value
      If j > n Then Exit Sub
    Wend
    TZ = SPLINT(z0(), T0(), T2(), M%, n%, z, True)
    LRz = LapseRate(z, TZ, Zlo, Tlo)
    If LRz < LRt Then                  'If current LR is less than -2 K/km
      Zlo = z: Tlo = TZ                'move up profile and try again
      Zhi = z + Thickness              'Apply thickness criterion
      jat1% = j%                       'Save j% index ABOVE zlo
    End If
  Loop Until z >= Zhi                  'Was it thick enough?

  pass = pass + 1                           'YES!
  Select Case pass%
  Case 1: Zt1 = Zlo: Tzt1 = Tlo           'Only get here if valid Trop #1
  Case 2
    If Zt1 < 11 And Zlo > 15 Then     'avoid structure near aircraft
      Zt2 = Zlo: Tzt2 = Tlo: Exit Sub 'And if Trop #2 exists
    End If
  Case 3
      Zt2 = Zlo: Tzt2 = Tlo: Exit Sub  'And if Trop #2 exists
  Case Else: Return                       'Just in case!
  End Select

'Now look for a second Trop if LR not zero
  LRt = -3: Thickness = 1!             'Make WMO happy with 1 km @ -3 K/km
  j% = jat1% - 1                       'Start off at level BELOW zlo Trop1
                                       'to find -3 K/km lapse rate
  Do
    j = j + 1
    If j% + 1 > n% Then Exit Sub
  Loop Until T1(j% + 1) < LRt          'Lapse rate <-3 K/km
  jbm3% = j%                           'j% is for level BELOW this condition

' LRt is now bound from below
  z = Calc_LR_Level(z0(), T0(), T2(), j%, LRt)  'Calc Trop Height at LR=LRt
  If z = 99.9 Then Exit Sub
  Tlo = SPLINT(z0(), T0(), T2(), M%, n%, z, True)  'And save z and T
  Zlo = z: Zhi = z + Thickness             'for >1 km, if second Trop

  Do
    z = z + 0.1                        'Do 100 m steps for Thickness
    While z > z0(j%)
      j = j + 1                       'Bump index j% if necessary
      If j > n Then Exit Sub         'to correspond to level ABOVE
    Wend
    TZ = SPLINT(z0(), T0(), T2(), M%, n%, z, True)
    LRz = LapseRate(z, TZ, Zlo, Tlo)
    If LRz > LRt Then                  'Must go more negative than -3 K/km
      Zlo = z: Tlo = TZ                'And save z and T
      Zhi = z + Thickness              'Then check thickness criterion
      jbm3% = j% - 1                   'Save index to level ABOVE zlo
    End If
  Loop Until z >= Zhi

  j% = jbm3%                           'Set index to level BELOW zlo and
  GoTo BeginM2Trop                   'look for another -2 K/km trop

End Sub

Sub Do_Retrieval(Algorithm%, Parameter!, ob!(), RT!(), RA!(), frm As Form)
Static Aavg!, Arms!, LATold!
Dim w1!, w2!, X!, T1017!, Fregion!, MRIsum!, RemoveBias As Boolean
' Retrieval Algorithm Index Definition:
' 0 ... Use specified RCs
' 1 ... MRI-based algorithm
' 2 ... Latitude Blend algorithm
' 3 ... Longitude Blend
' 4 ... UT Blend
' 5 ... Retrievable Index
' 6 ... Observable Index
' 7 ... Separate Up Dn OBs

' In with following parameters:
' Algorithm
' Latitude
' ob() .... observable array
' frm ..... Calling form
'
' Out with the following
' RT() ... Temperature profile
' RA() ... Altitudes
' T1017
'
' Common parameters used:
' LAT1, LAT2, LAT3, LAT4, MRI, MRIavg, MRIrms
' ALTkm, RetAltMin
'
' Local variables
'  absLAT!, Index%,f!, fr!, i%,j%,L%
' RT1!(), RT2!(), MRI1sum, MRI2sum, LATT!, LATold!
' T1!(),T2!(), jt1%, jt2%, T10km!, T17km!
Dim absLAT!, Index%, f!, fr!, i%, j%, L%
Dim RT1!(1 To 33), RT2!(1 To 33), LATT!
Dim jt1%, jt2%, T10km!, T17km!, dZ!, dT!

  RCindex1 = -1
  RCindex2 = -1

  Select Case Algorithm
  Case 0  'Just use User Selected RCs
    Call RetrieveATP(UserRCindex, ob(), RT(), RA(), Aavg, Arms)
    For i = 0 To NRCmax - 1: frm.txtWavg(i).Text = "": frm.txtWrms(i).Text = "": Next i
    frm.txtWavg(Index).Text = Format(Aavg, "#0.0")
    frm.txtWrms(Index).Text = Format(Arms, "#0.0")

  Case 1 'MRI Retrieval
'   First, use latitude-based algorithm to decide initial RC region
'   Set MRI to correspond to latitude (between 0 and 2)
'   Default synoptic transition values are: LAT1=18, LAT2=28, LAT3=45, LAT4=53
'   These can be over-ridden if better information is available, eg XS info
'
'   LAT1 = 18   'xxx mjm 19991130 These need to be set whenever MRI retrieval is used!!
'   LAT2 = 28
'   LAT3 = 45
'   LAT4 = 53
    Call Do_Blend(Parameter!, ob!(), RT!(), RA!(), frm)
'   Now use new algorithm to determine retrieval coefficients based on
'   temperatures at 10 and 17 km from first retrieval ...  BLG 980522
'   However, skip if lat is <18 or >53 and use lat-based results only
'   Needed to remark out the following code to avoid MRI discontinuity xxx 19981017 mjm
'   xxx unremarked mjm 19991201 as Region2 was seeing midlat retrievals! Not sure
'   why MRI discontinuity was an issue.
'   xxx unremarked mjm 20000215 as was getting MRI discontinuity!
'   Select Case MRI
'   Case 0, 1, 2  ' Integer MRI, nothing to do!
'   Case Else
'   First, figure out number of non-zero retrieved levels
    jt1 = 0: Do: jt1 = jt1 + 1: Loop Until RA(jt1) > 0 Or jt1 = 33
    jt2 = 34: Do: jt2 = jt2 - 1: Loop Until RA(jt2) > 0 Or jt2 = 1
    If jt1 = 33 Then Exit Sub 'GoTo EndOfSub 'GoTo ReadLine      ' No retrieved level, start all over
'   Do the SPLINE fit to retrieved ATP
    Call SPLINE(RA(), RT(), T1(), T2(), jt1, jt2)
'   And calculate the Temperatures at 10 km and 17 km
    T10km = SPLINT(RA(), RT(), T2(), jt1, jt2, 10#, True)
    T17km = SPLINT(RA(), RT(), T2(), jt1, jt2, 17#, True)
    T1017 = T10km - T17km
    Select Case T1017             ' Calculate Region Number based on Delta T
    Case Is >= 38: MRI = 0
    Case Is >= 24: MRI = (38 - T1017) / 14
    Case Is >= 5
      MRI = 1
    Case Is >= 1
      MRI = 1 + (5 - T1017) / 4
    Case Else: MRI = 2
    End Select
'Debug.Print "MRI2="; MRI
'   Use the new T-based rather than Latitude-based region
    Index = Int(MRI): f = MRI - Index
    If f = 0 Then            'Pure Tropical, Mid-latitude, or Arctic
      Call RetrieveATP(Index, ob(), RT(), RA(), Aavg, Arms)
      For i = 0 To 2: frm.txtWavg(i).Text = "": frm.txtWrms(i).Text = "": Next i
      frm.txtWavg(Index).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index).Text = Format(Arms, "#0.0")
                             'RT() & RA() are set, Retrieve T()
    Else                     'non-integer value, requiring blend of regions
      If Index = 0 Then
        frm.txtWavg(2).Text = "": frm.txtWrms(2).Text = ""
      Else
        frm.txtWavg(0).Text = "": frm.txtWrms(0).Text = ""
      End If
      Call RetrieveATP(Index, ob(), RT1(), RA(), Aavg, Arms)
      frm.txtWavg(Index).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index).Text = Format(Arms, "#0.0")
     
      Call RetrieveATP(Index + 1, ob(), RT2(), RA(), Aavg, Arms)
      frm.txtWavg(Index + 1).Text = Format(Aavg, "#0.0")
      frm.txtWrms(Index + 1).Text = Format(Arms, "#0.0")
'     Now interpolate between the two solutions
      For j = 1 To 33: RT(j) = RT1(j) + f * (RT2(j) - RT1(j)): Next j
    End If
'   End Select
'   Keep track of avg and rms MRI (MJM 980831)
    If pALT > RetAltMin Then
      MRI1sum = MRI1sum + MRI         'Sum MRI values
      MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
      INC MRIrec
      If MRIrec > 10 Then
        MRIavg = MRI1sum / MRIrec                            'Calculate average
        MRIrms = Sqr((MRI2sum - MRIrec * MRIavg ^ 2) / (MRIrec - 1)) 'Calculate RMS
      End If
    ' Debug.Print MRI; MRIavg; MRIrms
    End If

  Case 2, 3, 4        'Old WB57 Algorithm
' Start of blending code, Correct LAT to Latitude 981006 mjm
    Select Case Algorithm
    Case 2: Parameter = Latitude
    Case 3: Parameter = Longitude
    Case 4: Parameter = UTsec / 1000#
    End Select
    Call Do_Blend(Parameter!, ob!(), RT!(), RA!(), frm)

    Fregion = (Int(100 * MRI)) / 100  'Put in footer, different defn from DC8
    MRIavg = 0#: MRIsum = 0#

  Case 5  'Retrievable Index


  Case 6  'Observable Index
    RCindex1 = aRC(IlnPmin1)
    Call RetrieveATP(RCindex1, ob(), RT1(), RA(), Aavg, Arms)
    If IlnPmin1 < NRCbin - 2 Then
      frm.txtWavg(IlnPmin1).Text = Format(Aavg, "#0.0")
      frm.txtWrms(IlnPmin1).Text = Format(Arms, "#0.0")
    End If
    frm.txtWavg(NRCbin - 2).Text = Format(Aavg, "#0.0")
    frm.txtWrms(NRCbin - 2).Text = Format(Arms, "#0.0")
    Stats(1, 1) = Aavg
    Stats(2, 1) = Arms
    Stats(3, 1) = Sqr(Aavg ^ 2 + Arms ^ 2)
    
    If NRC > 1 And Not RCsiteUsed And Not RemoveOBbias Then
      lnPmin2 = 9999#
      For i = 0 To NRC - 1 'Find next closest RC set
        If i <> IlnPmin1 Then
          If lnP(i) < lnPmin2 Then lnPmin2 = lnP(i): IlnPmin2 = i
        End If
      Next i
    ' Blend only if RC sets give opposite signs wrt to observables
      RCindex2 = aRC(IlnPmin2)
      If Sgn(TBavg(RCindex2)) <> Sgn(TBavg(RCindex1)) And frm.chkDisableBlending.Value = False Then
        Call RetrieveATP2(RCindex2, ob(), RT2(), RA(), Aavg, Arms)
        If IlnPmin2 < NRCbin - 2 Then
          frm.txtWavg(IlnPmin2).Text = Format(Aavg, "#0.0")
          frm.txtWrms(IlnPmin2).Text = Format(Arms, "#0.0")
        End If
        frm.txtWavg(NRCbin - 1).Text = Format(Aavg, "#0.0")
        frm.txtWrms(NRCbin - 1).Text = Format(Arms, "#0.0")
        Stats(1, 2) = Aavg
        Stats(2, 2) = Arms
        Stats(3, 2) = Sqr(Aavg ^ 2 + Arms ^ 2)

'       Now take weighted average between the two solutions with weighting by inverse variance
        w1 = 1# / (TBavg(RCindex1) ^ 2 + TBrms(RCindex1) ^ 2)
        w2 = 1# / (TBavg(RCindex2) ^ 2 + TBrms(RCindex2) ^ 2)
        For j = 1 To 33: RT(j) = (w1 * RT1(j) + w2 * RT2(j)) / (w1 + w2): Next j
        RCwt = w1 / (w1 + w2)
      Else
        If IlnPmin2 < NRCbin Then
          frm.txtWavg(IlnPmin2).Text = Format(0#, "#0.0")
          frm.txtWrms(IlnPmin2).Text = Format(0#, "#0.0")
        End If
        frm.txtWavg(NRCbin - 1).Text = Format(0#, "#0.0")
        frm.txtWrms(NRCbin - 1).Text = Format(0#, "#0.0")
        Stats(1, 2) = 0
        Stats(2, 2) = 0
        Stats(3, 2) = 0
        
        For j = 1 To 33
          RT(j) = RT1(j)
'         Debug.Print j, RA(j); RT(j)
        Next j
        RCwt = 1#
      End If
    Else
      For j = 1 To 33: RT(j) = RT1(j): Next j
    End If
    OATmtpCOR = OAT - RT(16)

    If pALT > RetAltMin And GoodScan Then
      MRI1sum = MRI1sum + MRI         'Sum MRI values
      MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
      INC MRIrec
      If MRIrec > 10 Then
        MRIavg = MRI1sum / MRIrec                           'Calculate average
        X = (MRI2sum - MRIrec * MRIavg ^ 2)
        If X > 0 Then
          MRIrms = Sqr(X / (MRIrec - 1)) 'Calculate RMS
        End If
      End If
    ' Debug.Print MRI; MRIavg; MRIrms
    End If

Case 7  'Separate Up Dn OBs
    If frm.chkRemoveAAbias.Value = 1 Then RemoveBias = True Else RemoveBias = False
'    RemoveBias = False
    RCindex1 = aRC(IlnPminUp)
    Call RetrieveATPup(RCindex1, OBup(), RT1(), RA(), Aavg, Arms, RemoveBias)
    If IlnPminUp < NRCbin - 2 Then
      frm.txtWavg(IlnPminUp).Text = Format(Aavg, "#0.0")
      frm.txtWrms(IlnPminUp).Text = Format(Arms, "#0.0")
    End If
    frm.txtWavg(NRCbin - 2).Text = Format(Aavg, "#0.0")
    frm.txtWrms(NRCbin - 2).Text = Format(Arms, "#0.0")
    Stats(1, 1) = Aavg
    Stats(2, 1) = Arms
    Stats(3, 1) = Sqr(Aavg ^ 2 + Arms ^ 2)
    RCindex2 = aRC(IlnPminDn)
    Call RetrieveATPdn(RCindex2, OBdn(), RT2(), RA(), Aavg, Arms, RemoveBias)
    If IlnPminDn < NRCbin - 2 Then
      frm.txtWavg(IlnPminDn).Text = Format(Aavg, "#0.0")
      frm.txtWrms(IlnPminDn).Text = Format(Arms, "#0.0")
    End If
    frm.txtWavg(NRCbin - 1).Text = Format(Aavg, "#0.0")
    frm.txtWrms(NRCbin - 1).Text = Format(Arms, "#0.0")
    Stats(1, 2) = Aavg
    Stats(2, 2) = Arms
    Stats(3, 2) = Sqr(Aavg ^ 2 + Arms ^ 2)
    For j = 16 To 33
      RT(j) = RT1(j)
    Next j
    For j = 1 To 16
      RT(j) = RT2(j)
    Next j
    RCwt = 1#
    OATmtpCOR = OAT - RT(16)

    If pALT > RetAltMin And GoodScan Then
      MRI1sum = MRI1sum + MRI         'Sum MRI values
      MRI2sum = MRI2sum + MRI ^ 2     'Sum square of MRI values
      INC MRIrec
      If MRIrec > 10 Then
        MRIavg = MRI1sum / MRIrec                           'Calculate average
        X = (MRI2sum - MRIrec * MRIavg ^ 2)
        If X > 0 Then
          MRIrms = Sqr(X / (MRIrec - 1)) 'Calculate RMS
        End If
      End If
    ' Debug.Print MRI; MRIavg; MRIrms
    End If

End Select


End Sub



Function fTtgtFix(EditTtgt As Boolean, TtgtFix As Boolean, Ttgt!, Cycle%) As Single
Dim Tavg!, i%, Tmin!, Tmax!, T!, n%, n2%, Diff1!
Static T10!(0 To 20), T10last!, NN%
' Fix Target Temperature glitches by checking trend. Examination
' and data shows that even on descent, delta T scan to scan is
' <0.7 K therefore set threshold to 0.75
' Glitches are always positive therefore only consider Diff1> (+) threshold
' Diff1 is greater on descent

  If EditTtgt Then
    n = 20   'N must be even
    n2 = n / 2
    TtgtFix = False

    If Cycle < n Then
      If Cycle > 1 And Ttgt - T10(n - Cycle + 1) > 0.8 Then 'Substitute previous in case bad cycle in first N cycles
        Ttgt = T10(n - Cycle + 1)
      End If
      T10(n - Cycle) = Ttgt
      Muxs(0) = Ttgt
      T10(0) = Ttgt
      fTtgtFix = Ttgt
      
    Else
      For i% = n To 1 Step -1:  T10(i%) = T10(i% - 1):  Next i%
      Tmin = 0   'Most recent data
      For i = 1 To n2: Tmin = Tmin + T10(i): Next i
      Tmin = Tmin / n2
      Tmax = 0   'First N/2 data points
      For i = n2 + 1 To n: Tmax = Tmax + T10(i): Next i
      Tmax = Tmax / n2
      Tavg = (Tmax + Tmin) / 2
      If T10(0) = 0 Then
        T10(0) = Ttgt
        Muxs(0) = Tavg
        fTtgtFix = Ttgt
      Else
        T = Tavg + Tmin - Tmax
        Diff1 = Ttgt - T                   'Current cycle difference
      'Debug.Print Diff1
        If Abs(Diff1) > 0.8 Then  'ACCENT 19990422 died here Cycle 34 when .7 was used, use 0.8
          T10(0) = T          'Tmax-Tmin accounts for trend over 5 cycles
          fTtgtFix = T
          Muxs(0) = T
          TtgtFix = True
        Else
          T10(0) = Ttgt
          Muxs(0) = Tavg
          fTtgtFix = Ttgt
        End If
      End If
    End If
  
  'If UTsec > 38400 Then Debug.Print y; y0; y10; fTtgtFix; Diff1; Diff2
  Else
    
    fTtgtFix = Ttgt
  End If

End Function





Function GetSerialInQB$(lu%)
'Save serial port data one line at a time.
'Return only when CR detected; ignore LF

Dim A$, IO$, T1
T1 = Timer

GetSerialInQB$ = ""
If Loc(lu%) = 0 Then Exit Function
IO$ = ""

Do
  If Loc(lu%) = 0 Then     'Out here if data stops B4 CR
    If Timer > T1 + 20 Then: GetSerialInQB$ = "": Exit Function
  End If
 
  A$ = Input$(1, lu%)
  
  Select Case A$
  Case Chr$(10)  'LineFeed -- ignore
  Case Chr$(13)  'Carriage Return -- ignore
  Case Else
    IO$ = IO$ + A$
  End Select
Loop Until A$ = Chr$(13)  'Return only if CR found
 
  Print #lu%, IO$
  GetSerialInQB$ = IO$

End Function

Function GetSerialIn$(INPlu%, f As Form)
Static MTPin$
Dim COM1$, Char$, i%, L%

Do
  DoEvents
  If f.MSComm1.InBufferCount > 0 Then
    COM1$ = f.MSComm1.Input
    L% = Len(COM1$)
    Select Case f.MSComm1.InputLen
    Case 0                                              'Entire buffer read into COM1$
        For i% = 1 To L%                                'Check for CR in buffer
          Char$ = Mid$(COM1$, i%, 1)
          Select Case Char$
          Case vbCr
            GetSerialIn$ = MTPin$
            MTPin$ = Right$(COM1$, L% - i% - 1) 'Save leftovers
            Exit Function                                  'And exit
          Case vbLf                                         'Ignore LF
          Case Else
            MTPin$ = MTPin$ + Char$               'No CR, save everything
          End Select
        Next
    
    Case 1
      Select Case COM1$
      Case vbCr
        GetSerialIn$ = MTPin$
        'lstSerial.Additem = MTPin$
        MTPin$ = ""
        Exit Function
      Case vbLf
        ' Ignore linefeeds
      Case Else
        MTPin$ = MTPin$ + COM1$
      End Select
    End Select
  End If
Loop

End Function




Function LapseRate(z, TZ, z0, T0)
'Lapse rate from (z0,T0) to (z,T)
    LapseRate = (TZ - T0) / (z - z0)
End Function

Sub LOG_line(X$)
'SHARED LOGlu%
'  PRINT TIME$ + " " + x$
  Print #LOGlu%, Time$ + " " + X$

End Sub


Function Read_a_Line$(INPlu%, OUTlu%, Source$, Risky As Boolean, Drive$, Cycle%)
' Get one line of input data from either file ("F") or data unit ("D")
' Use next line only to simulate "D" mode; null if real-time COM1 connection
Dim A$, D%, TYM1!, TYM2!, Lt$

  Quit = False
  Select Case Source$
  Case "F"                                 'Get data from a file
    If Not RealTime Then
      If EOF(INPlu%) Then
        Read_a_Line$ = "EOF": Exit Function
      End If
    End If
    Input #INPlu%, A$
    ByteCount = Loc(INPlu%) * 128
    If ByteCount < LOFraw Then
'    frmMTPret.ProgressBar1.value = Int(ByteCount * 100 / LOFraw)
    End If
    DoEvents
  
  Case "T"
    If EOF(INPlu%) Then Read_a_Line$ = "EOF": Exit Function
    Input #INPlu%, A$
  Case "D"
    Do                                     'Get input string from COM1
      If Risky Then                        'String input on COM1
        D% = 0
        Do
         If Loc(INPlu%) > 0 Then Input #INPlu%, A$: D% = 1
         'Call CheckKeyboard(Quit)
         If Quit Then Read_a_Line$ = "QUIT": Exit Function
        Loop Until D% = 1

      Else                                 'Character input on COM1
        TYM1 = Timer
        TYM2 = TYM1 + 30
        Do
'          A$ = GetSerialIn$(INPlu%, frmMTPbin)
          If Timer - TYM1 > 30 Then
            Debug.Print "WARNING: NOT GETTING ANY SERIAL DATA!"
            Debug.Print "         RESET SYSTEM AND PRAY!      "
            Exit Do
          End If
        Loop Until A$ <> ""
        'Call CheckKeyboard(Quit)
        If Quit Then Read_a_Line$ = "QUIT": Exit Function
      End If
    Loop While Len(A$) = 0
'
    Lt$ = Left$(A$, 1)
    Select Case Lt$                         'Write raw data to file
    Case "A", "B", "C", "D", "E", "I"       'Don't write partial data lines
      If Drive$ <> "A:" Then                'Don't write to Floppy Drive
        Print #OUTlu%, A$
        If Lt$ = "E" Then Print #OUTlu%, "" 'Put blank line after "E" line
      End If
    End Select
  End Select

  Read_a_Line$ = A$

End Function

Sub RetrieveATP(Index%, ob!(), RT!(), RA!(), Tavg!, Trms!)
Dim iTop%, iBot%, i%, j%, k%, L%, WtT!, WtB!, OBavi!(1 To 30), Tcorr!
Dim z!, Tsum!, Rsum!, X!, Wt!, RCi!(1 To 33, 1 To 30)
  
' Get pair of levels which border pALT [km]
  Call Get_iBot_iTop(Index, pALT!, iBot%, iTop%, WtB!, WtT!)
  If iBot = 1 Then iBot = 2: iTop = 1
  
  For L = 1 To 33
    RA(L) = RAav(Index, iBot, L) * WtB + RAav(Index, iTop, L) * WtT             'Retrieval level
    TAAi(L) = RTav(Index, iBot, L) * WtB + RTav(Index, iTop, L) * WtT           'Average T interpolated to FL
    TSEi(Index, L) = RMSe(Index, iBot, L) * WtB + RMSe(Index, iTop, L) * WtT    'Interpolated SE on T
    sTSE(L) = TSEi(Index, L)
'   Blend RCs for iTop and iBot: rc indices, RC set, FL, Ret Level, Observable
    For j = 1 To Nobs
      RCi(L, j) = rc(Index, iBot, L, j) * WtB + rc(Index, iTop, L, j) * WtT
    Next j
  Next L
' Debug.Print "RetrieveATP: pALT="; pALT; " RA(16)="; RA(16)

' Do Retrieval using interpolated RCs
  For L = 1 To 33
    RT(L) = TAAi(L)
    For j = 1 To Nobs
      RT(L) = RT(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))     'OBavgWt from DetermineRCregion
      'Rt(L) = TAAi(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))  'xxxmjm 20100605
    'Debug.Print L; RA(L); Rt(L); TAAi(L); RCi(L, j); j
    Next j
  Next L
  
' Remove observable bias if this option is selected
  If RemoveOBbias Then
    If OBbias(aRC(IlnPmin1)) = OBbias(aRC(IlnPmin2)) Then
      Tcorr = OBbias(aRC(IlnPmin1))
    Else
      Tcorr = (OBbias(aRC(IlnPmin1)))  ' + OBbias(aRC(IlnPmin2))) / 2#
    End If
'    Tcorr = RT(16) - OATmtp          'Original implementation
    For L = 1 To 33: RT(L) = RT(L) + Tcorr: Next L
  End If

' Calculate Average and RMS of retrieved profile wrt interpolated AA T-profile
  k = 0: Tsum = 0#: Rsum = 0#
  For L = 1 To 33
    If RA(L) > 0 And Abs(RA(L) - RA(16)) < ATPrange Then
      k = k + 1
      'wt = 1# + 0.2 * Abs(RA(L) - pALT)          'Adhoc weighting
      If TSEi(Index, L) = 0 Then Wt = 1 Else Wt = TSEi(Index, L)
      X = (RT(L) - TAAi(L)) / Wt
      Tsum = Tsum + X
      Rsum = Rsum + X ^ 2
    End If
  Next L
  
  If k > 1 Then
    Tavg = Tsum / k                              'Calculate average
    Trms = Sqr((Rsum - Tavg ^ 2) / (k - 1))      'Calculate RMS
  Else
    Tavg = 99.9
    Trms = 99.9
  End If

End Sub

Sub RetrieveATPup(Index%, ob!(), RT!(), RA!(), Tavg!, Trms!, RemoveBias As Boolean)
Dim iTop%, iBot%, i%, j%, k%, L%, WtT!, WtB!, OBavi!(1 To 30), Tcorr!
Dim z!, Tsum!, Rsum!, X!, Wt!, RCi!(1 To 33, 1 To 30)
  
' Get pair of levels which border pALT [km]
  Call Get_iBot_iTop(Index, pALT!, iBot%, iTop%, WtB!, WtT!)
  If iBot = 1 Then iBot = 2: iTop = 1
  
  For L = 1 To 33  '16 To 33
    RA(L) = RAav(Index, iBot, L) * WtB + RAav(Index, iTop, L) * WtT             'Retrieval level
    TAAi(L) = RTav(Index, iBot, L) * WtB + RTav(Index, iTop, L) * WtT           'Average T interpolated to FL
    TSEi(Index, L) = RMSe(Index, iBot, L) * WtB + RMSe(Index, iTop, L) * WtT    'Interpolated SE on T
    sTSE(L) = TSEi(Index, L)
'   Blend RCs for iTop and iBot: rc indices, RC set, FL, Ret Level, Observable
    For j = 1 To Nobs
      RCi(L, j) = rc(Index, iBot, L, j) * WtB + rc(Index, iTop, L, j) * WtT
    Next j
  Next L
' Debug.Print "RetrieveATP: pALT="; pALT; " RA(16)="; RA(16)

' Do Retrieval using interpolated RCs
  For L = 16 To 33
    RT(L) = TAAi(L)
    For j = 1 To Nobs
      RT(L) = RT(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))     'OBavgWt from DetermineRCregion
    ' Debug.Print L; RA(L); Rt(L); TAAi(L); RCi(L, j); j
    Next j
  Next L
  If RemoveBias Then
    For L = 1 To 33: TAAi(L) = TAAi(L) + OBbias(Index): Next L
  End If
' Remove observable bias if this option is selected
  Tcorr = OBbias(aRC(IlnPminUp))
  For L = 16 To 33: RT(L) = RT(L) + Tcorr: Next L

' Calculate Average and RMS of retrieved profile wrt interpolated AA T-profile
  k = 0: Tsum = 0#: Rsum = 0#
  For L = 16 To 33
    If RA(L) > 0 And Abs(RA(L) - RA(16)) < ATPrange Then
      k = k + 1
      'wt = 1# + 0.2 * Abs(RA(L) - pALT)          'Adhoc weighting
      If TSEi(Index, L) = 0 Then Wt = 1 Else Wt = TSEi(Index, L)
      X = (RT(L) - TAAi(L)) / Wt
      Tsum = Tsum + X
      Rsum = Rsum + X ^ 2
    End If
  Next L
  
  If k > 1 Then
    Tavg = Tsum / k                              'Calculate average
    Trms = Sqr((Rsum - Tavg ^ 2) / (k - 1))      'Calculate RMS
  Else
    Tavg = 99.9
    Trms = 99.9
  End If

End Sub
Sub RetrieveATPdn(Index%, ob!(), RT!(), RA!(), Tavg!, Trms!, RemoveBias As Boolean)
Dim iTop%, iBot%, i%, j%, k%, L%, WtT!, WtB!, OBavi!(1 To 30), Tcorr!
Dim z!, Tsum!, Rsum!, X!, Wt!, RCi!(1 To 33, 1 To 30)
' This is identical to RetrieveATP except that TAAi2 is introduced for plotting purposes
' Get pair of levels which border pALT [km]
  Call Get_iBot_iTop(Index, pALT!, iBot%, iTop%, WtB!, WtT!)
  If iBot = 1 Then
      iBot = 2: iTop = 1
  End If
  For L = 1 To 33
    RA(L) = RAav(Index, iBot, L) * WtB + RAav(Index, iTop, L) * WtT             'Retrieval level
    TAAi2(L) = RTav(Index, iBot, L) * WtB + RTav(Index, iTop, L) * WtT          'Average T interpolated to FL
    TSEi(Index, L) = RMSe(Index, iBot, L) * WtB + RMSe(Index, iTop, L) * WtT    'Interpolated SE on T
    sTSE(L) = TSEi(Index, L)
'   Blend RCs for iTop and iBot: rc indices, RC set, FL, Ret Level, Observables
    For j = 1 To Nobs
      RCi(L, j) = rc(Index, iBot, L, j) * WtB + rc(Index, iTop, L, j) * WtT
    Next j
  Next L
  'Debug.Print "RetrieveATP: pALT="; pALT; " RA(16)="; RA(16)
' Do Retrieval using interpolated RCs
  For L = 1 To 16
    RT(L) = TAAi2(L)
    For j = 1 To Nobs                       'OBavgWt from DetermineRCregion
      RT(L) = RT(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))
    'Debug.Print L; RA(L); Rt(L); TAAi(L); RCi(L, j); j
    Next j
  Next L
  If RemoveBias Then
    For L = 1 To 33: TAAi2(L) = TAAi2(L) + OBbias(Index): Next L
  End If
' Remove observable bias if this option is selected
  Tcorr = OBbias(aRC(IlnPminDn))
  For L = 1 To 16: RT(L) = RT(L) + Tcorr: Next L

' Calculate Average and RMS of retrieved profile wrt interpolated AA T-profile
  k = 0: Tsum = 0#: Rsum = 0#
  For L = 1 To 16
    If RA(L) > 0 And Abs(RA(L) - RA(16)) < ATPrange Then
      k = k + 1
      'wt = 1# + 0.2 * Abs(RA(L) - pALT)          'Adhoc weighting
      If TSEi(Index, L) = 0 Then Wt = 1 Else Wt = TSEi(Index, L)
      X = (RT(L) - TAAi(L)) / Wt
      Tsum = Tsum + X
      Rsum = Rsum + X ^ 2
    End If
  Next L
  
  If k > 1 Then
    Tavg = Tsum / k                              'Calculate average
    Trms = Sqr((Rsum - Tavg ^ 2) / (k - 1))      'Calculate RMS
  Else
    Tavg = 99.9
    Trms = 99.9
  End If

End Sub

Sub RetrieveATP2(Index%, ob!(), RT!(), RA!(), Tavg!, Trms!)
Dim iTop%, iBot%, i%, j%, k%, L%, WtT!, WtB!, OBavi!(1 To 30), Tcorr!
Dim z!, Tsum!, Rsum!, X!, Wt!, RCi!(1 To 33, 1 To 30)
' This is identical to RetrieveATP except that TAAi2 is introduced for plotting purposes
' Get pair of levels which border pALT [km]
  Call Get_iBot_iTop(Index, pALT!, iBot%, iTop%, WtB!, WtT!)
  If iBot = 1 Then
      iBot = 2: iTop = 1
  End If
  For L = 1 To 33
    RA(L) = RAav(Index, iBot, L) * WtB + RAav(Index, iTop, L) * WtT             'Retrieval level
    TAAi2(L) = RTav(Index, iBot, L) * WtB + RTav(Index, iTop, L) * WtT          'Average T interpolated to FL
    TSEi(Index, L) = RMSe(Index, iBot, L) * WtB + RMSe(Index, iTop, L) * WtT    'Interpolated SE on T
    sTSE(L) = TSEi(Index, L)
'   Blend RCs for iTop and iBot: rc indices, RC set, FL, Ret Level, Observable
    For j = 1 To Nobs
      RCi(L, j) = rc(Index, iBot, L, j) * WtB + rc(Index, iTop, L, j) * WtT
    Next j
  Next L
  'Debug.Print "RetrieveATP: pALT="; pALT; " RA(16)="; RA(16)
' Do Retrieval using interpolated RCs
  For L = 1 To 33
    RT(L) = TAAi2(L)
    For j = 1 To Nobs                       'OBavgWt from DetermineRCregion
      RT(L) = RT(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))
      'Rt(L) = TAAi(L) + RCi(L, j) * (ob(j) - OBavgWt(Index, j))  'xxxmjm 20100605
    'Debug.Print L; RA(L); Rt(L); TAAi(L); RCi(L, j); j
    Next j
  Next L
  
' Remove observable bias if this option is selected
  If RemoveOBbias Then
    If OBbias(aRC(IlnPmin1)) = OBbias(aRC(IlnPmin2)) Then
      Tcorr = OBbias(aRC(IlnPmin1))
    Else
      Tcorr = (OBbias(aRC(IlnPmin1)))  ' + OBbias(aRC(IlnPmin2))) / 2#
    End If
'    Tcorr = RT(16) - OATmtp          'Original implementation
    For L = 1 To 33: RT(L) = RT(L) + Tcorr: Next L
  End If

' Calculate Average and RMS of retrieved profile wrt interpolated AA T-profile
  k = 0: Tsum = 0#: Rsum = 0#
  For L = 1 To 33
    If RA(L) > 0 And Abs(RA(L) - RA(16)) < ATPrange Then
      k = k + 1
      'wt = 1# + 0.2 * Abs(RA(L) - pALT)          'Adhoc weighting
      If TSEi(Index, L) = 0 Then Wt = 1 Else Wt = TSEi(Index, L)
      X = (RT(L) - TAAi(L)) / Wt
      Tsum = Tsum + X
      Rsum = Rsum + X ^ 2
    End If
  Next L
  
  If k > 1 Then
    Tavg = Tsum / k                              'Calculate average
    Trms = Sqr((Rsum - Tavg ^ 2) / (k - 1))      'Calculate RMS
  Else
    Tavg = 99.9
    Trms = 99.9
  End If

End Sub

Public Sub RetrievalError(pALT!, RCset%, re!())
Dim i%, ia%, ib%, k%, WtA!, WtB!
' Calculate estimated retrieval errors for a temperature profile
' Find first RC set above flight level
  ib = 0
  Do
    ib = ib + 1
  Loop Until pALT > FLA(ib) / 100
  ia = ib - 1
' Calculate weights for above and below flight levels
  WtA = (pALT * 100 - FLA(ib)) / (FLA(ia) - FLA(ib))
  WtB = 1 - WtA
' Calculate weighted retrieval error estimates
  For i = 1 To Nret
    re(i) = RMSe(RCset, ib, i) * WtB + RMSe(RCset, ia, i) * WtA
  Next i

End Sub

Function fLevelBelowFL(pALT!)
Dim ib%

  ib = 0
  Do
    ib = ib + 1
  Loop Until pALT > FLA(ib) / 100# Or ib >= NFL
  fLevelBelowFL = ib
  
End Function
Sub T_minimum(pALT!, Cycle%, z0!(), T0!(), T1#(), T2#(), M%, n%, Zmin5!, Tmin5!)
' Find minimum temperature on Temperature profile
' z0(), T0() contain retrieved altitude and temperature data
' T2() ..... contains 2nd derivatives of T wrt z (from SPLINE)
' n% ....... contrains number of retrieved altitudes=dim of above arrays
' zmin ..... altitude of minimum temperature
' Tzmin .... minimum temperature
' Revision History:
' 960323 ... mjm ... initial version
' 960505 ... blg ... valid only after climbing above 5 km
Dim z!, t1_Old!, j%

  Tmin5 = 999!: Zmin5 = 99.9: t1_Old = -999!
  If pALT < 5 And Cycle% < 200 Then Exit Sub
  For j% = M% To n%
    If T0(j%) <= Tmin5 Then Tmin5 = T0(j%): Zmin5 = z0(j%)
    If T1(j%) > 0 And t1_Old < 0 Then            'LR increased thru 0
      If j% + 1 <= n% Then
        z = Calc_LR_Level(z0(), T0(), T2(), j%, 0) 'Calc Height at LR=0
        If z <> 99.9 Then
          Tlo = SPLINT(z0(), T0(), T2(), M%, n%, z, True)
          If Tlo < Tmin5 Then Tmin5 = Tlo: Zmin5 = z
        End If
      End If
    End If
    t1_Old = T1(j%)
    'PRINT USING "###.#  "; n%; j%; T0(j%); z0(j%); Tmin; zmin; T1.old; z
  Next j%
End Sub


Sub TropLoc(altkmdc!(), tretF!(), Trop!)
Dim Zlo!, Zhi!, z!, altACkm!, Ztop!, ThicknessCrit!, T!
Dim Tt!, TB!, Zt!, Zb!, LRi!, TropTemp!
Dim j%

'  92.04.05 (DC-8), 931022
  j = 0: Ztop = altkmdc(33): altACkm = altkmdc(16)
  Do
    j = j + 1: Zlo = altkmdc(j) - 0.2
  Loop While (Zlo < 5.6 And j < 33) '  trop must be above 500 mb
  If j = 33 Then Exit Sub
TryAnotherZ: '  chk for LR > -2 [K/km] for 1.5 km layer
  Zlo = Zlo + 0.2: ThicknessCrit = 1.5
' Roe uses 1.0 km, but my retrievals still have artifact inflections at low end
  If altACkm < 10 Or Zlo < 8 And (altACkm - Zlo) > 1 Then ThicknessCrit = 2
' above line can be relaxed when I have complete set of RC alt's
  Zhi = Zlo + ThicknessCrit: If Zhi > Ztop Then Trop = 999.9: Exit Sub

' Let's find temp at both z's
  z = Zlo: GoSub getTemp: Tlo = T
  z = Zhi: GoSub getTemp: Thi = T
  If T = 999.9 Then Trop = 999.9: Exit Sub
  LRi = (Thi - Tlo) / (Zhi - Zlo)
  If LRi < -2 Then GoTo TryAnotherZ Else Trop = Zlo '  92.04.05

Chk2: 'Now chk for LR over 0.5 km layer being > -2 [K/km]
  Zhi = Zlo + 0.5: z = Zhi: GoSub getTemp: Thi = T
  LRi = (Thi - Tlo) / (Zhi - Zlo)
  If LRi > -2 Then GoTo Chk3 Else GoTo TryAnotherZ

Chk3: 'Now chk for LR over 0.25 km layer being > -2 [K/km]
  Zhi = Zlo + 0.25: z = Zhi: GoSub getTemp: Thi = T
  LRi = (Thi - Tlo) / (Zhi - Zlo)
  If LRi > -2 Then GoTo DoTropTemp Else GoTo TryAnotherZ 'OK, we've found trop  92.05.03

getTemp:
' Enter with z and exit with T from interpolation in altkmdc() & tretF()
  j = 1: If z <= altkmdc(j) + 0.01 Then T = tretF(j): Return
  Do
    j = j + 1: Zt = altkmdc(j)
  Loop While z > Zt And j < 34
  If j = 34 Then T = 999.9
  Zb = altkmdc(j - 1): Tt = tretF(j): TB = tretF(j - 1)
  T = TB + (Tt - TB) * (z - Zb) / (Zt - Zb)
Return

DoTropTemp:
  Trop = Zlo: GoSub getTemp: TropTemp = T
  
End Sub
