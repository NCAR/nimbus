Attribute VB_Name = "Calibration"
Function fATPc(dZp!, Init As Boolean) As Single
' dZp is distance in km above (+) and below (-) flight level
' Init if TRUE initializes fit coefficients which are stored as Static variables
' mjm ... 20101213
Static c0!, c1!, c2!, c3!, c4!, c5!, c6!
' Remove Accuracy data from ATPs
  If dZp <= -8 Then
  c0 = c0
  End If
  Select Case Mission$
  Case "TORERO"
    If Init Then
    ' y = 2.5435E-05x6 - 1.0415E-05x5 - 2.3928E-03x4 + 1.8518E-03x3 + 4.7314E-02x2 + 5.3600E-03x - 2.6890E-01
      c6 = 0.000025435
      c5 = -0.000010415
      c4 = -0.0023928
      c3 = 0.0018518
      c2 = 0.047314
      c1 = 0.00536
      c0 = -0.2689
    End If
  Case "HIPPO-5"
    If Init Then
    ' y = 4.3729E-06x6 - 1.7917E-05x5 - 4.0388E-04x4 + 1.6670E-03x3 + 1.3740E-02x2 + 1.8029E-03x - 1.6935E-01
      c6 = 0.0000043729
      c5 = -0.000017917
      c4 = -0.00040388
      c3 = 0.001667
      c2 = 0.01374
      c1 = 0.0018029
      c0 = -0.16935
    End If

  Case "ATTREX"
    If Init Then
    ' y = 7.0504E-06x6 + 1.8312E-04x5 - 2.5215E-03x4 - 1.1498E-02x3 + 1.2689E-01x2 + 5.0847E-02x - 5.9681E-01
      c6 = 0.0000070504
      c5 = 0.00018312
      c4 = -0.0025215
      c3 = -0.011498
      c2 = 0.12689
      c1 = 0.050847
      c0 = -0.59681
    End If
    
  Case "HIPPO-4"
    If Init Then
    ' y = 1.3556E-05x6 - 8.1944E-05x5 - 1.0307E-03x4 + 5.3685E-03x3 + 4.5125E-03x2 + 3.9842E-02x - 2.0700E-01
      c6 = 0.000013556
      c5 = -0.000081944
      c4 = -0.0010307
      c3 = 0.0053685
      c2 = 0.0045125
      c1 = 0.039842
      c0 = -0.207
    End If
    
  Case "GloPac"
    If Init Then  'mjm 20110926
    ' y = -1.6248E-05x6 - 2.6354E-05x5 + 1.7977E-03x4 + 6.3575E-04x3 - 4.1697E-02x2 + 8.9640E-02x - 1.8556E-03
      c6 = -0.000016248
      c5 = -0.000026354
      c4 = 0.0017977
      c3 = 0.00063575
      c2 = -0.041697
      c1 = 0.08964
      c0 = -0.0018556
    End If
  
  Case "HIPPO-2"
    If Init Then  'mjm 20110406
   ' y = -2.5643E-05x6 + 3.0783E-05x5 + 3.0902E-03x4 - 8.3132E-03x3 - 5.7883E-02x2 + 2.6627E-02x + 9.0707E-02
      c6 = -0.000025643
      c5 = 0.000030783
      c4 = 0.0030902
      c3 = -0.0083132
      c2 = -0.057883
      c1 = 0.026627
      c0 = 0.090707
    End If
  
  Case "HIPPO-3"
    If Init Then
    ' y = -4.2588E-06x6 + 3.7811E-05x5 - 1.3640E-04x4 + 1.9086E-03x3 + 3.5238E-03x2 - 5.0725E-02x - 2.7073E-01
      c6 = -0.0000042588
      c5 = 0.000037811
      c4 = -0.0001364
      c3 = 0.0019086
      c2 = 0.0035238
      c1 = -0.050725
      c0 = -0.27073
    End If

  Case PREDICT
    If Init Then
    ' y = 5.5912E-06x6 - 1.0463E-04x5 - 4.0802E-04x4 + 9.5406E-03x3 + 1.7384E-02x2 - 1.0526E-01x - 5.5634E-03
      c6 = 0.0000055912
      c5 = -0.00010463
      c4 = -0.00040802
      c3 = 0.0095406
      c2 = 0.017384
      c1 = -0.10526
      c0 = -0.0055634
    End If
    
  End Select
  fATPc = (((((c6 * dZp + c5) * dZp + c4) * dZp + c3) * dZp + c2) * dZp + c1) * dZp + c0
End Function

Function fTptC(ByVal k1!, ByVal k2!, ByVal V!) As Single
' Calculate temperature of platinum thermistor which has linear fit vs counts (v)
' R(v) = k1 + k2 * v
' For platinum thermistor, calibration curve is
' T(R) = -293.23 + 0.4527 * R +0.00005 * R^2
' T(R) = a + b*R + c*R^2 + d*R^3
' mjm ... 2004.03.01
Dim A#, b#, C#, D#, c0#, c1#, c2#, c3#, X#
  X = V
' Calculate temperature of Platinum resistor given resistance
' See T_vs_R.xls
  A = -239.529603289315
  b = 0.450385127204133
  C = 6.71883132179858E-05
  D = -1.96781369379622E-08
'  a = -239.2295089: b = 0.4527259: c = 0.000051
' Substitute R(v) for R in expression for T(R) and collect terms
  c0 = (A + b * k1 + C * k1 * k1 + D * k1 * k1 * k1)
  c1 = (b * k2 + 2 * C * k1 * k2 + 3 * D * k1 * k1 * k2)
  c2 = (C * k2 ^ 2 + 3 * D * k1 * k2 * k2)
  c3 = (D * k2 * k2 * k2)
  'Debug.Print c0, c1, c2
' c0, c1, and c2 are terms needed in Thermistor_ER_1 and _2 routines for Thi, Tlo and Twin
  fTptC = c0 + c1 * X + c2 * X ^ 2 + c3 * X ^ 3

End Function

Function fTptR(Resistance!)
Dim A#(0 To 3)
' Calculate temperature of Platinum resistor given resistance
' T_vs_R.xls dated 20041207
' Valid for -100 C to +45 C
  A(0) = -239.529603289315
  A(1) = 0.450385127204133
  A(2) = 6.71883132179858E-05
  A(3) = -1.96781369379622E-08
  
  fTptR = A(0) + A(1) * Resistance + A(2) * Resistance ^ 2 + A(3) * Resistance ^ 3
  
End Function
Function fTc(T, Zp!, Mach2!) As Single
Dim Tc!, C!
   
  Select Case Mission
  Case "TORERO"
    Tc = T - 1.263 * Mach2 + 0.4293
  Case "HIPPO-5"
    Tc = T - 2.1997 * Mach2 + 0.9019         'mjm 20120227
  Case "ATTREX"
    Tc = T * (1 - 0.002383 * Mach2) - 0.0992 'mjm 20111128
  Case "HIPPO-4"
    Tc = T * (1 - 0.00922 * Mach2) + 0.885   'mjm 201100928
  Case "DC3-Test"
    Tc = T * (1 - 0.0111 * Mach2) + 0.41     'mjm 201100927
  Case "GloPac"
    Tc = T * (1 + 0.0271 * Mach2) - 1.3      'mjm 201100912
  Case "START-08"
    Tc = T
  Case "HIPPO"
    C = 1# + 0.0096 * (0.02089 + 0.06555 * Zp - 0.00122 * Zp ^ 2)
    Tc = T * C - 0.06
  Case "HIPPO-2"
    Tc = T * (1 - 0.002 * Mach2) - 0.42
  Case "HIPPO-3"
    '=Q13*(1+AF$11*AA13)+AF$10,        Param(2)=AT_A, Param(20)=Mach Squared
    'Coefficients are from: HIPPO-3_RAOBcomparison­_FLonlynonredundantM.xlsx
    'Which uses Fit with M^2 instead Zp function for fit
    'Tc = T * (1 - 0.007 * Mach2) + 0.48
    Tc = T * (1 - 0.0079 * Mach2) + 0.51   'mjm 20101229
  Case "PREDICT"
    Tc = T * (1 - 0.0074 * Mach2) + 0.56   'mjm 20110509
  Case "PREDICT2"
    Tc = T * (1 - 0.0097 * Mach2) + 0.852   'mjm 20110607
  Case Else
    MsgBox "fTc() not found for this mission", vbOK
  End Select
  fTc = Tc

End Function

Function ThermistorDC!(ch%, iv%)
Dim a0!, a1!, a2!, a3!, RR!, LOGrr!, V!, xden!, Rlo!, Rhi!, C#(0 To 3), X#
' ch% is A/D channel number defined below
' iv% is decimal value of MUX channel (ie Hex value has already been converted
'     using the function: fHexToDec(HexNumber)
' Channel Numbers are:
' 1 tND, 2 tLO1, 3 tLO2, 4 tIFA, 5 tTGTl, 6 tTGTh, 7 ACCp
' 8 tWIN, 9 tMTR, 10 asterick, 11 ref, 12 tDC1, 13 tDC2
' 14 tPS5, 15 tPS12, 16 ACCm

  If iv = 0 Then V = 1# Else V = iv / 16#  '981008 mjm Make real to avoid loosing 4 bits of precision
  
  Select Case ch%
  Case 1, 2, 3, 4, 9, 10, 11, 12, 13 'Tnd, Tlo1,Tlo2 Tifa, Motor,spare,Vref, DC1, DC2
  ' following algorithm comes from RFD's eval'n of thermister tables 91.08.13 'f44032 (new data system)
    a1 = 0.0009336018:    a2 = 0.0002213609:    a3 = 0.000000126198
    RR = (1 / ((256 - V) / 256)) - 1
    If RR > 0 Then xden = (a1 + a2 * Log(34800 * RR) + a3 * Log(34800 * RR) ^ 3)
    ThermistorDC = 1 / xden - cTo
  ' Target thermistor was re-calibrated at JPL on 20000420 because there was
  ' a 0.9 K difference between them in the SOLVE data, with Thi>Tlo.
  ' Calibration removed this difference, with the Thi=Tlo 0.5 K < previous Tlo
  '
  Case 5    'Ttgt Lo
    Select Case YYYYMMDD$
    Case Is < "20041201"
      a0 = -241.0423: a1 = 0.4614729: a2 = 0.00004114943
      'v = 5 * (v / 255) ' start target low calc, commented out 20000420
      Rlo = 0.0404 * 16 * V + 287.49 'mjm 20000420, was ((v + 8.6308) / 28.727) / 0.0010464
      ThermistorDC = a0 + a1 * Rlo + a2 * Rlo ^ 2 '  tgt_lo is set
    Case Else
      'Based on JPL measurements and fit 20041206
      'c(0) = -103.8402
      'c(1) = 0.037535
      'c(2) = 0.0000003007
      'c(3) = -0.000000000009157
      'Based on Post mission measurements
      C(0) = -103.1573
      C(1) = 0.037426
      C(2) = 0.0000002983
      C(3) = -0.00000000000907
      ThermistorDC = C(0) + C(1) * iv + C(2) * iv ^ 2 + C(3) * iv ^ 3
    End Select
    
    
    
  Case 6    'TtgtHi
    Select Case YYYYMMDD$
    Case Is < "20041201"
      a0 = -241.0423: a1 = 0.4614729: a2 = 0.00004114943
      'v = 5 * (v / 255) ' start tgt hi calc, commented out 20000420
      Rhi = 0.0406 * 16 * V + 418.65 'mjm 20000420, was ((v + 12.66724) / 28.727) / 0.0010464
      ThermistorDC = a0 + a1 * Rhi + a2 * Rhi ^ 2 '  tgt_hi is set
    Case Else
    ' Based on JPL measurements Post PAVE
      C(0) = -105.3481
      C(1) = 0.038164
      C(2) = 0.0000003124
      C(3) = -0.000000000009644
      ThermistorDC = C(0) + C(1) * iv + C(2) * iv ^ 2 + C(3) * iv ^ 3
    End Select
    
  Case 8    'Twin
    Select Case YYYYMMDD$
    Case Is < "20041201"
    ' ER2: Thermistor = -132.81 + 0.689408 * v ' based on NZ lab cal 941012
    ' DC8 QB pgm: n = MUX(8) / 16: v = -122.9 + .61803 * n '
      V = -122.9 + 0.61803 * V
      If V < -80 Or V > 45 Then V = 99
      ThermistorDC = V
    Case Else
    ' Based on JPL measurements and fit 20041206
    '  c(0) = -103.314
    '  c(1) = 0.037633
    '  c(2) = 0.0000003018
    '  c(3) = -0.000000000009223
    'Post mission calibration
      C(0) = -103.5184
      C(1) = 0.038224
      C(2) = 0.0000003115
      C(3) = -0.000000000009666
      ThermistorDC = C(0) + C(1) * iv + C(2) * iv ^ 2 + C(3) * iv ^ 3
    End Select
  
  Case 14
    ThermistorDC = iv * 2 / 1000 'answer should be  5.00 (volts)
  
  Case 15
    ThermistorDC = iv * 0.0079114  'answer should be 15.00 (volts)
  
  Case 7 'ACC+
  ' DC8 QB Code for converting accelerometer measurements to gees.
  ' i = 16: j = 7: tBase = Ttgt
  ' acc1 = Mux(i): acc2 = Mux(j)
  ' dACC = acc2 - acc1: dACC = Int(dACC / 5.2)  'dACC is in units of 0.01 g
    ThermistorDC = (iv - 2048#) / 1024# + 1 'Gees 20041130 mjm/rfd
  ' Accelerometer has 2048 as 1g and goes +/- 2 gs wrt to this.
  Case 16
    Select Case YYYYMMDD$
    Case Is < "20041201"  'ACC+
      ThermistorDC = (iv - 2048#) / 1024# + 1 'Gees 20041130 mjm/rfd
    Case Else  'Tplate
      a1 = 0.0009336018:    a2 = 0.0002213609:    a3 = 0.000000126198
      RR = (1 / ((256 - V) / 256)) - 1
      If RR > 0 Then xden = (a1 + a2 * Log(34800 * RR) + a3 * Log(34800 * RR) ^ 3)
      ThermistorDC = 1 / xden - cTo
    End Select
  Case 17  'ACC-
    ThermistorDC = (iv - 2048#) / 1024# + 1 'Gees 20041130 mjm/rfd
  
  Case Else
    ThermistorDC = 0
  End Select

End Function
Function Thermistor!(ch%, iv%)
Dim x1!, x2!, x3!, RR!, LOGrr!, V!, xden!
' Gunn Diode receiver thermistor calibration
  V = iv / 16#             '981008 mjm Make real to avoid loosing 4 bits of precision
  If V = 0 Then V = 1
  ' Old%=1: XT(1) = 12100 * rr: XT(2) = 34800 * rr   ' Old data unit calibration prior to 931015
  Select Case ch%
  Case 1, 2, 3, 4, 9                    ' ND, LO1, LO2, IFA, MTR
    ' following algorithm comes from RFD's eval'n of thermister tables 91.08.13+-
    x1 = 0.001028792                    ' 44031 thermistors (sensor unit)
    x2 = 0.0002391957
    x3 = 0.000000156301
    RR = (1 / (1 - V / 256)) - 1        ' Resistance Ratio (rr) for thermistors only
    LOGrr = Log(RR * 34800)
    xden = (x1 + x2 * LOGrr + x3 * LOGrr ^ 3)
    Thermistor = 1 / xden - 273.16
  Case 5                                ' target low (Platinum)
    'Tlo = -108.54 + .306483 * v        ' based on re-calibration of 90.10.12
    'Tlo = -108.22 + 0.296 * v          ' based on re-calibration of 94.01.31
    Thermistor = -107.35 + 0.31264 * V  ' based on NZ lab cal 941012
  Case 6                                ' target hi (Platinum)
  ' Thi = -40.26 + .308206 * v          ' based on re-calibration of 90.10.12
  ' thi = -42.45 + .30835 * v           ' based on re-calibration of 94.01.31
    Thermistor = -40.5 + 0.31459 * V    ' based on NZ lab cal 941012
  Case 8                                ' Window (Platinum)
    ' tWIN = -122.9 + .61803# * v       ' used < 94.01.31
    ' tWIN = -122.9 + .61803# * v       ' used > 94.01.31
    Thermistor = -132.81 + 0.689408 * V ' based on NZ lab cal 941012
  Case 12, 13                           ' Data unit rovers   'PWR Supplies '931015, per RFD  (MTP Lite)
    ' following algorithm comes from RFD's eval'n of thermister tables 91.08.13+-
    x1 = 0.0009336018                   ' 44032 (new data system)
    x2 = 0.0002213609
    x3 = 0.000000126198
    RR = (1 / (1 - V / 256)) - 1        ' Resistance Ratio (rr) for thermistors only
    LOGrr = Log(RR * 34800)
    xden = (x1 + x2 * LOGrr + x3 * LOGrr ^ 3)
    Thermistor = 1 / xden - 273.16
  Case 14                               ' 5 volt PS
    Thermistor = 5 * V / 128
  Case 15                               ' 12 volt PS
    Thermistor = 12 * V / 128
  Case Else
    Thermistor = 0
  End Select
  
End Function

Function ThermistorER_1!(ch%, iv%)
'This routine was ThermistorERX, renamed 20020609 to reflect SU1 and SU2
' The three coefficients needed for platinum thermistors can be calculated using the
' function fTptC, and using Excel to do a linear fit for R vs counts from data taken in
' the lab by substituting precision thermistors for the target and window themistors
' noting the hexidecimal counts in the D-line for Mux data.
Dim x1!, x2!, x3!, RR!, LOGrr!, V!, xden!

  V = iv / 16#             '981008 mjm Make real to avoid loosing 4 bits of precision
  If V = 0 Then V = 1
  ' Old%=1: XT(1) = 12100 * rr: XT(2) = 34800 * rr   ' Old data unit calibration prior to 931015
  Select Case ch%
  Case 1, 2, 3, 4, 9, 12, 13            ' ND, AMP, PLO, IFA, MTR, DU1, DU2
    ' following algorithm comes from RFD's eval'n of thermister tables 91.08.13+-
    x1 = 0.0009336018                   ' 44032 (new data system)
    x2 = 0.0002213609
    x3 = 0.000000126198
    RR = (1 / (1 - V / 256)) - 1        ' Resistance Ratio (rr) for thermistors only
    LOGrr = Log(RR * 34800)
    xden = (x1 + x2 * LOGrr + x3 * LOGrr ^ 3)
    ThermistorER_1 = 1 / xden - 273.16
  
  Case 5                                ' target low (Platinum)
    ' Tlo = -108.54 + .306483 * v       ' based on re-calibration of 90.10.12
    ' Tlo = -108.22 + 0.296 * v         ' based on re-calibration of 94.01.31
    ' Tlo = -107.35 + 0.31264 * v       ' based on NZ lab cal 941012
    Select Case YYYYMMDD$
    Case Is < "20000602"
      ThermistorER_1 = -107.35 + 0.31264 * V
    Case Is < "20020610"
      ThermistorER_1 = -107.2710409 + 0.305581926 * V + 0.000016353 * V ^ 2 ' based of JPL test 20000602
    Case Is < "20040101" 'Before pre-AVE
      ThermistorER_1 = -104.103246875 + 0.0194094875 * iv + 8.12045 * 10 ^ -8 * iv ^ 2 ' based of JPL test 20000602
    Case Is < "20050513" 'C:\MTP\Pt\PtERlow.txt -104.306  1.936339E-02  8.083334E-08
      ThermistorER_1 = -104.305520555 + 0.0193596366 * iv + 8.0802 * 10 ^ -8 * iv ^ 2 ' based of JPL test 20000602
    Case Is < "20051021" 'C:\MTP\Data\WB57\HAVE2\Setup\Pt_ER_low_B.txt c(0) = -104.6552 c(1) = 0.019499 c(2) = 0.00000008138 c(3) = -0.000000000001285
      ThermistorER_1 = -104.6552 + 0.019499 * iv + 0.00000008138 * iv ^ 2 - 0.000000000001285 * iv ^ 3 ' based of JPL test 20050513
    Case Is < "20070614"
      ThermistorER_1 = -97.2043 + 0.020652 * iv + 8.907 * 10 ^ -8 * iv ^ 2 - 1.512 * 10 ^ -12 * iv ^ 3 ' based of JPL test 20051021
    Case Else
      ThermistorER_1 = -97.369 + 0.02062 * iv + 8.884 * 10 ^ -8 * iv ^ 2 - 1.506 * 10 ^ -12 * iv ^ 3   ' based of JPL test 20070614
    End Select

  Case 6                                ' target hi (Platinum)
    ' Thi = -40.26 + .308206 * v        ' based on re-calibration of 90.10.12
    ' thi = -42.45 + .30835 * v         ' based on re-calibration of 94.01.31
    ' Thi = -40.5 + 0.31459 * v         ' based on NZ lab cal 941012
    Select Case YYYYMMDD$
    Case Is < "20000602"
      ThermistorER_1 = -40.5 + 0.31459 * V         ' based on NZ lab cal 941012
    Case Is < "20020610"
      ThermistorER_1 = -42.62746225 + 0.321146517 * V + 0.0000172788 * V ^ 2 ' based of JPL test 20000602
    Case Is < "20040101"  'Before pre-AVE
      ThermistorER_1 = -38.35581128 + 0.0199520464 * iv + 8.12045 * 10 ^ -8 * iv ^ 2 ' based of JPL test 20020607
    Case Is < "20050513" 'C:\MTP\Pt\PtERhigh.txt -38.51244  1.991832E-02  8.094053E-08
      ThermistorER_1 = -38.51423432 + 0.0199012512 * iv + 8.0802 * 10 ^ -8 * iv ^ 2 ' based of JPL test 20020607
    Case Is < "20051021"    'C:\MTP\Data\WB57\HAVE2\Setup\Pt_ER_high_B.txt c(0) = -38.8199 c(1) = 0.020027 c(2) = 0.00000006873 c(3) = -0.000000000001291
      ThermistorER_1 = -38.8199 + 0.020027 * iv + 0.00000006873 * iv ^ 2 - 0.000000000001291 * iv ^ 3 ' based of JPL test 20050513
    Case Is < "20070614" 'C:\MTP\Data\WB57\CRAVE\Setup\Pt_ER_high_B.txt
      ThermistorER_1 = -27.5029 + 0.021172 * iv + 7.38 * 10 ^ -8 * iv ^ 2 - 1.508 * 10 ^ -12 * iv ^ 3 ' based of JPL test 20051021
    Case Else
      ThermistorER_1 = -27.8715 + 0.021256 * iv + 7.449 * 10 ^ -8 * iv ^ 2 - 1.526 * 10 ^ -12 * iv ^ 3 ' based of JPL test 20070614
    End Select
  
  Case 8                                ' Window (Platinum)
    ' tWIN = -122.9 + .61803# * v       ' used < 94.01.31
    ' tWIN = -122.9 + .61803# * v       ' used > 94.01.31
    Select Case YYYYMMDD$
    Case Is < "20020610"
      ThermistorER_1 = -132.81 + 0.689408 * V     ' based on NZ lab cal 941012
    Case Is < "20040101" 'Before Pre-AVE
      ThermistorER_1 = -128.15005382 + 0.0412740796 * iv + 3.74978 * 10 ^ -7 * iv ^ 2 ' based on JPL Lab Cal 20020607
    Case Is < "20050513" 'C:\MTP\Pt\PtERwin.txt -130.1722  4.117574E-02  3.738589E-07
      ThermistorER_1 = -130.1722 + 0.04117574 * iv + 0.0000003738589 * iv ^ 2 ' based on JPL Lab Cal 20040304
    Case Is < "20050513"   'C:\MTP\Data\WB57\HAVE2\Setup\Pt_ER_high_B.txt c(0) = -104.6552 c(1) = 0.019499 c(2) = 0.00000008138 c(3) = -0.000000000001285
      ThermistorER_1 = -104.6552 + 0.019499 * iv + 0.00000008138 * iv ^ 2 - 0.000000000001285 * iv ^ 3 ' based of JPL test 20050513
    Case Is < "20051021"   'C:\MTP\Data\WB57\HAVE2\Setup\Pt_ER_win_B.txt c(0) = -129.3676 c(1) = 0.041538 c(2) = 0.0000004003 c(3) = -0.00000000001284
      ThermistorER_1 = -129.3676 + 0.041538 * iv + 0.0000004003 * iv ^ 2 - 0.00000000001284 * iv ^ 3 ' based of JPL test 20050513
    Case Else
      ThermistorER_1 = -122.805 + 0.043982 * iv + 4.393 * 10 ^ -7 * iv ^ 2 - 1.511 * 10 ^ -11 * iv ^ 3  ' based of JPL test 20051021
    End Select
    
  Case 14                               ' 5 volt PS
    ThermistorER_1 = 5 * V / 128
  Case 15                               ' 12 volt PS
    ThermistorER_1 = 12 * V / 128
  Case 7, 16                            ' ACC+ and ACC-
'    ThermistorER_1 = iv
    ThermistorER_1 = (iv - 2048#) / 1024# + 1 'Gees 20041130 mjm/rfd
  Case 11                               ' Vref
    ThermistorER_1 = 2.5 * V / 128       'Voltage is scaled so 2.5 V = 128 *16 or midscale
  Case Else
    ThermistorER_1 = 0
  End Select

End Function

Function ThermistorER_2(ch%, iv%)
Dim x1!, x2!, x3!, RR!, LOGrr!, V!, xden!
' Thermistor calibration for new WB57 sensor unit (SU #2)
  V = iv / 16#             '981008 mjm Make real to avoid loosing 4 bits of precision
  If V = 0 Then V = 1
  ' Old%=1: XT(1) = 12100 * rr: XT(2) = 34800 * rr   ' Old data unit calibration prior to 931015
  Select Case ch%
  Case 1, 2, 3, 4, 9, 12, 13            ' ND, AMP, PLO, IFA, MTR, DU1, DU2
    ' following algorithm comes from RFD's eval'n of thermister tables 91.08.13+-
    x1 = 0.0009336018                   ' 44032 (new data system)
    x2 = 0.0002213609
    x3 = 0.000000126198
    RR = (1 / (1 - V / 256)) - 1        ' Resistance Ratio (rr) for thermistors only
    LOGrr = Log(RR * 34800)
    xden = (x1 + x2 * LOGrr + x3 * LOGrr ^ 3)
    ThermistorER_2 = 1 / xden - 273.16
' Thi, Tlo and Twin on MTP#2 use PT-539AW resistors
' Post AVE calibration for SU#2 on 2004.03.11
'C:\MTP\Pt\PtWBlow.txt   -105.10930  1.927487E-02  8.015152E-08
'C:\MTP\Pt\PtWBhigh.txt   -39.23671  1.988515E-02  8.071891E-08
'C:\MTP\Pt\PtWBwin.txt   -130.95630  4.085367E-02  3.682880E-07
  
  Case 5                                ' target low (Platinum) Lab Cal 2002.04.26
    Select Case YYYYMMDD$
    Case Is < "20040101"
      ThermistorER_2 = -104.44517542 + 0.0191176938 * iv + 7.88045 * 10 ^ -8 * iv ^ 2 ' based of JPL test 20000610
    Case Is < "20070702"
      ThermistorER_2 = -105.1093 + 0.01927487 * iv + 0.00000008071891 * iv ^ 2 ' based of JPL test 20040311
    Case Is < "20070810"    'TC4
      ThermistorER_2 = -77.5976 + 0.020895 * iv + 0.00000008546 * iv ^ 2 - 0.00000000000153 * iv ^ 3 ' based of JPL test 20070530
    Case Else
      ThermistorER_2 = -77.7449 + 0.020898 * iv + 0.00000008544 * iv ^ 2 - 0.00000000000153 * iv ^ 3 ' based of JPL test 20100308
    End Select
  
  Case 6                                ' target hi (Platinum) Lab Cal 2002.04.26
    Select Case YYYYMMDD$
    Case Is < "20040101"
      ThermistorER_2 = -39.06863432 + 1.98967488 * 10 ^ -2 * iv + 8.0802 * 10 ^ -8 * iv ^ 2                          ' based of JPL test 20000610
    Case Is < "20070702" 'Use this date so made up test data from CRAVE will process ok
      ThermistorER_2 = -39.23671 + 0.01988515 * iv + 7.88045 * 10 ^ -8 * iv ^ 2   ' based of JPL test 20040311
    Case Is < "20070810"   'TC4
      ThermistorER_2 = -4.686 + 0.021516 * iv + 7.019 * 10 ^ -8 * iv ^ 2 - 1.548 * 10 ^ (-12) * iv ^ 3 ' based of JPL test 20070810
    Case Else
      ThermistorER_2 = -4.8265 + 0.02153 * iv + 7.031 * 10 ^ -8 * iv ^ 2 - 1.552 * 10 ^ (-12) * iv ^ 3  ' based of JPL test 20100308
    End Select
  
  Case 8                                ' Window (Platinum)
    Select Case YYYYMMDD$
    Case Is < "20040101"
      ThermistorER_2 = -131.850263795 + 0.0406357966 * iv + 3.64658 * 10 ^ -7 * iv ^ 2 ' based of JPL test 20000610
    Case Is < "20070530"
      ThermistorER_2 = -130.9563 + 0.04085367 * iv + 0.000000368288 * iv ^ 2 ' based of JPL test 20040311
    Case Else
      ThermistorER_2 = -123.2966 + 0.044025 * iv + 0.0000004409 * iv ^ 2 - 1.516 * 10 ^ (-11) * iv ^ 3 ' based of JPL test 20100308
      If Cycle > 2392 Then
      'Debug.Print Cycle; iv; ThermistorER_2
      X = X
      End If
    End Select
  
  Case 14                               ' 5 volt PS
    ThermistorER_2 = 5 * V / 128
  Case 15                               ' 12 volt PS
    ThermistorER_2 = 12 * V / 128
  Case 7, 16                            ' ACC+ and ACC-
'    ThermistorER_2 = iv
    ThermistorER_2 = (iv - 2048#) / 1024# + 1 'Gees 20041130 mjm/rfd
  Case 11
    ThermistorER_2 = 2.5 * V / 128       'Voltage is scaled so 2.5 V = 128 *16 or midscale
  Case Else
    ThermistorER_2 = 0
  End Select

End Function


