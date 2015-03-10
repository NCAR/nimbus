Attribute VB_Name = "Pointing"
Public MAM!(1 To 3, 1 To 3), gYo!, gPo!, gRo!, EmaxFlag As Boolean
Public PointingVersionDate As Date
Private Yi!, Pi!, Ri!

Function fEcAtEmax(Pd!, Rd!, Elevation!)
Dim P!, r!, cP!, sP!, cR!, sR!, E!, sE!, A!, b!, C!, E1!, E2!, E3!, E4!
Dim y!, alpha!, beta!, Arg!, Ec_at_Emax!, X!, fEcc(1 To 4), Ep90!, Em90!
Dim fEc1!, fEc2!, rpd!

' Calculate commanded elevation angle needed to be at a specified
' elevation angle (Elevation) with respect to the horizon
'
' MJ Mahoney, July 02, 2002
' Enter with MTP Attitude Matrix MAM, aircraft Pitch and Roll in degrees, and
' desired elevation angle wrt to horizon.
'
' Return with commanded elevation angle
' If Elevation = 180 degrees, the returned value will be the target position
' In this case the pitch and roll values are irrelevant

  
' rpd = Atn(1) / 45#       'Radians per degree 3.14159265358979

  P = Pd * rpd
  r = Rd * rpd
  E = Elevation * rpd

  cP = Cos(P): sP = Sin(P)
  cR = Cos(r): sR = Sin(r)
  sE = Sin(E)
  alpha = -cR * sP * MAM(1, 1) + sR * MAM(2, 1) + cR * cP * MAM(3, 1)
  beta = -1 * (-cR * sP * MAM(1, 3) + sR * MAM(2, 3) + cR * cP * MAM(3, 3))
  
  A = alpha ^ 2 + beta ^ 2
  b = 2 * sE * beta
  C = sE ^ 2 - alpha ^ 2
  Arg = b ^ 2 - 4 * A * C
  
  fEcAtEmax = Atn(beta / alpha) / rpd
'  E_max = -ASN(alpha * Cos(Ec_at_Emax * rpd) + beta * Sin(Ec_at_Emax * rpd))
'  Emax = E_max / rpd 'Always + since it is maximum elevation angle
'  'Debug.Print alpha; beta; Ec_at_Emax; alpha * Cos(Ec_at_Emax) + beta * Sin(Ec_at_Emax)
'  Ep90 = Abs(-ASN(beta) / rpd)
'  Em90 = -Ep90
'  E_Ec_0 = -ASN(alpha) / rpd  'Elevation at which Ec=0
  
End Function
Function fEmax(Pd!, Rd!, Elevation!)
Dim P!, r!, cP!, sP!, cR!, sR!, E!, sE!, A!, b!, C!, E1!, E2!, E3!, E4!
Dim y!, alpha!, beta!, Arg!, Ec_at_Emax!, X!, fEcc(1 To 4), Ep90!, Em90!
Dim fEc1!, fEc2!

' Calculate commanded elevation angle needed to be at a specified
' elevation angle (Elevation) with respect to the horizon
'
' MJ Mahoney, July 02, 2002
' Enter with MTP Attitude Matrix MAM, aircraft Pitch and Roll in degrees, and
' desired elevation angle wrt to horizon.
'
' Return with commanded elevation angle
' If Elevation = 180 degrees, the returned value will be the target position
' In this case the pitch and roll values are irrelevant

 
' rpd = Atn(1) / 45#       'Radians per degree 3.14159265358979

  P = Pd * rpd
  r = Rd * rpd
  E = Elevation * rpd

  cP = Cos(P): sP = Sin(P)
  cR = Cos(r): sR = Sin(r)
  sE = Sin(E)
  alpha = -cR * sP * MAM(1, 1) + sR * MAM(2, 1) + cR * cP * MAM(3, 1)
  beta = -1 * (-cR * sP * MAM(1, 3) + sR * MAM(2, 3) + cR * cP * MAM(3, 3))
  
  A = alpha ^ 2 + beta ^ 2
  b = 2 * sE * beta
  C = sE ^ 2 - alpha ^ 2
  Arg = b ^ 2 - 4 * A * C
  
  If alpha = 0 Then Ec_at_Emax = 0 Else Ec_at_Emax = Atn(beta / alpha) / rpd
  E_Max = ASN(alpha * Cos(Ec_at_Emax * rpd) + beta * Sin(Ec_at_Emax * rpd))
  fEmax = Abs(E_Max / rpd) 'Always + since it is maximum elevation angle
  'Debug.Print alpha; beta; Ec_at_Emax; alpha * Cos(Ec_at_Emax) + beta * Sin(Ec_at_Emax)
'  Ep90 = Abs(-ASN(beta) / rpd)
'  Em90 = -Ep90
'  E_Ec_0 = -ASN(alpha) / rpd  'Elevation at which Ec=0
  

  

End Function

Function fEp90(Pd!, Rd!, Elevation!)
Dim P!, r!, cP!, sP!, cR!, sR!, E!, sE!, A!, b!, C!, E1!, E2!, E3!, E4!
Dim y!, alpha!, beta!, Arg!, Ec_at_Emax!, X!, fEcc(1 To 4), Ep90!, Em90!
Dim fEc1!, fEc2!

' Calculate commanded elevation angle needed to be at a specified
' elevation angle (Elevation) with respect to the horizon
'
' MJ Mahoney, July 02, 2002
' Enter with MTP Attitude Matrix MAM, aircraft Pitch and Roll in degrees, and
' desired elevation angle wrt to horizon.
'
' Return with commanded elevation angle
' If Elevation = 180 degrees, the returned value will be the target position
' In this case the pitch and roll values are irrelevant

  
' rpd = Atn(1) / 45#       'Radians per degree 3.14159265358979

  P = Pd * rpd
  r = Rd * rpd
  E = Elevation * rpd

  cP = Cos(P): sP = Sin(P)
  cR = Cos(r): sR = Sin(r)
  sE = Sin(E)
  alpha = -cR * sP * MAM(1, 1) + sR * MAM(2, 1) + cR * cP * MAM(3, 1)
  beta = -1 * (-cR * sP * MAM(1, 3) + sR * MAM(2, 3) + cR * cP * MAM(3, 3))
  
  A = alpha ^ 2 + beta ^ 2
  b = 2 * sE * beta
  C = sE ^ 2 - alpha ^ 2
  Arg = b ^ 2 - 4 * A * C
  
  Ec_at_Emax = Atn(beta / alpha) / rpd
  E_Max = -ASN(alpha * Cos(Ec_at_Emax * rpd) + beta * Sin(Ec_at_Emax * rpd))
  Emax = E_Max / rpd 'Always + since it is maximum elevation angle
'  'Debug.Print alpha; beta; Ec_at_Emax; alpha * Cos(Ec_at_Emax) + beta * Sin(Ec_at_Emax)
  fEp90 = Abs(-ASN(beta) / rpd)
'  Em90 = -Ep90
'  E_Ec_0 = -ASN(alpha) / rpd  'Elevation at which Ec=0

End Function



Function fEc(MAM!(), ByVal Pd!, ByVal Rd!, Elevation!, EmaxFlag As Boolean)
Dim P!, r!, cP!, sP!, cR!, sR!, E!, sE!, A!, b!, C!, E1!, E2!, E3!, E4!
Dim y!, alpha!, beta!, Arg!, Ec_at_Emax!, X!, fEcc(1 To 4), Ep90!, Em90!
Dim fEc1!, fEc2!

' Calculate commanded elevation angle needed to be at a specified
' elevation angle (Elevation) with respect to the horizon
'
' MJ Mahoney, November 19, 2002
' Enter with MTP Attitude Matrix MAM, aircraft Pitch and Roll in degrees, and
' desired elevation angle wrt to horizon.
'
' Return with commanded elevation angle
' If Elevation = 180 degrees, the returned value will be the target position
' In this case the pitch and roll values are irrelevant
' Excel spreadsheet shows that LHS and RHS are the only difference in solutions
  
  If Elevation = 180# Then fEc = 180#: Return
  
' rpd = Atn(1) / 45#       'Radians per degree 3.14159265358979

  P = Pd * rpd
  r = Rd * rpd
  E = Elevation * rpd

  cP = Cos(P): sP = Sin(P)
  cR = Cos(r): sR = Sin(r)
  sE = Sin(E)
  alpha = -cR * sP * MAM(1, 1) + sR * MAM(2, 1) + cR * cP * MAM(3, 1)
  beta = -1 * (-cR * sP * MAM(1, 3) + sR * MAM(2, 3) + cR * cP * MAM(3, 3))
  
  A = alpha ^ 2 + beta ^ 2
  b = 2 * sE * beta
  C = sE ^ 2 - alpha ^ 2
  Arg = b ^ 2 - 4 * A * C
Debug.Print MAM(1, 1); MAM(2, 1); MAM(3, 1); MAM(1, 3); MAM(2, 3); MAM(3, 3); A; b; C; Arg
If alpha = 0 Then
    If beta > 0 Then
      Ec_at_Emax = 90
    Else
      Ec_at_Emax = -90
    End If
  Else
    Ec_at_Emax = Atn(beta / alpha) / rpd
  End If
  E_Max = -ASN(alpha * Cos(Ec_at_Emax * rpd) + beta * Sin(Ec_at_Emax * rpd))
  Emax = E_Max / rpd 'Always + since it is maximum elevation angle
  'Debug.Print alpha; beta; Ec_at_Emax; alpha * Cos(Ec_at_Emax) + beta * Sin(Ec_at_Emax)
  Ep90 = Abs(-ASN(beta) / rpd)
  Em90 = -Ep90
  E_Ec_0 = -ASN(alpha) / rpd  'Elevation at which Ec=0
  
  EmaxFlag = False
  
  Select Case AC$
  Case "WB"
    If Abs(Elevation) > Abs(Emax) Then  ' Go to highest or lowest elevation angle possible
      EmaxFlag = True
      If E_Ec_0 >= 0 Then
        If Elevation >= 0 Then fEc = Ec_at_Emax Else fEc = Ec_at_Emax + 180
      Else
        If Elevation >= 0 Then fEc = -180 + Ec_at_Emax Else fEc = Ec_at_Emax
      End If
    Else
      If Arg < 0 Then Arg = 0
      fEc1 = ASN((-b - Sqr(Arg)) / (2 * A)) / rpd
      fEc2 = ASN((-b + Sqr(Arg)) / (2 * A)) / rpd
      If E_Ec_0 >= 0 Then
        If Elevation < E_Ec_0 Then
          Select Case Elevation
          Case Is >= Em90: fEc = fEc2
          Case Else: fEc = 180 - fEc2
          End Select
        Else
          Select Case Elevation
          Case Is >= Em90: fEc = fEc2
          Case Is >= -Emax: fEc = 180 + fEc2
          Case Else
          End Select
        End If
      Else
        If Elevation < E_Ec_0 Then
          Select Case Elevation
          Case Is >= Emax: fEc = fEc1
          Case Else:  fEc = 180 - Ec_at_Emax
          End Select
        Else
          Select Case Elevation
          Case Is <= Ep90: fEc = fEc1
          Case Is <= -Emax: fEc = -180 - fEc1
          Case Else: fEc = Ec_at_Emax - 180
          End Select
        End If
      End If
    End If

  Case Else
    If Abs(Elevation) > Abs(Emax) Then  ' Go to highest or lowest elevation angle possible
      EmaxFlag = True
      If E_Ec_0 >= 0 Then
        If Elevation >= 0 Then
          fEc = Ec_at_Emax
        Else
          fEc = Ec_at_Emax - 180
        End If
      Else
        If Elevation >= 0 Then
          fEc = 180 + Ec_at_Emax
        Else
          fEc = Ec_at_Emax
        End If
      End If
    Else
      If Arg < 0 Then Arg = 0
      fEc1 = ASN((-b - Sqr(Arg)) / (2 * A)) / rpd
      fEc2 = ASN((-b + Sqr(Arg)) / (2 * A)) / rpd
      If E_Ec_0 < 0 Then
        If Elevation < E_Ec_0 Then
          fEc = fEc2       '180 - fEc1
        Else
          Select Case Elevation
          Case Is >= Ep90
            fEc = 180 - fEc2
          Case Is >= -Emax
            fEc = fEc2
          Case Else
            fEc = fEc2
          End Select
        End If
      Else
        If Elevation >= E_Ec_0 Then
          Select Case Elevation
          Case Is >= Emax: fEc = fEc1
          Case Else:  fEc = fEc1
          End Select
        Else
          Select Case Elevation
          Case Is >= Em90: fEc = fEc1
          Case Is > -Emax: fEc = -180 - fEc1
          Case Else: fEc = Ec_at_Emax - 180
          End Select
        End If
      End If
    End If
  End Select
  
End Function
Function ASN(X!)
' Take arcsine of a number
' Valid only for +- 90 degrees
 
 If Abs(X) > 0.99999 Then ASN = X * 3.14159 / 2 Else ASN = Atn(X / Sqr(1 - X ^ 2))
  
End Function
Sub GetMTPattitude(AC$, Yi!, Pi!, Ri!)
' Not currently used
  Select Case AC$    'Pi = 1.513 * rpd: Ri = 27.277 * rpd
'  Case "ER": yi = 20# * rpd: Pi = 1.862 * rpd: Ri = 26.332 * rpd
  Case "ER": Yi = 20# * rpd: Pi = -2.2 * rpd: Ri = -30.72 * rpd
  Case "DC": Yi = 15# * rpd: Pi = 0 * rpd: Ri = -8 * rpd
'  Case "WB": yi = 4# * rpd:   Pi = -2.993 * rpd: Ri = -0.209 * rpd
  Case "WB": Yi = 4# * rpd:   Pi = 2.4 * rpd: Ri = 180 * rpd
  Case "EL": Yi = 15# * rpd: Pi = 0# * rpd: Ri = -11.585 * rpd
  Case Else: MsgBox AC$ + " does not have defined values for yaw, pitch and roll!", vbOKOnly
  End Select

End Sub


Sub Get_MTP_Attitude_Matrix(MAM!(), Yi!, Pi!, Ri!)
Dim y!, P!, r!, cY!, sY!, cP!, sP!, cR!, sR!, b!, T!, z!
' MJ Mahoney, June 17, 2002
' Note that the values of yaw, pitch and roll given below were based
' on field measurements of instrument yaw A at zero elevation angle and
' pitch-like and roll-like parameters B and T along the MTP Sensor Unit
' edges: The measured angles in degrees were:
'
' ER2:     A=20.0   B=10.3  T=25.0
' DC8:     A=15.0   B=00.0  T=08.3
' WB57:    A=04.0   B=-3.0  T=00.0
' Electra: A=15.0   B=00.0  T=12.0
'
' These were converted to the Y P and R values below by numerically
' solving equations given at URL: http://mtp.jpl.nasa.gov/notes/pointing/pointing.html

' rpd = Atn(1) / 45#      'Radians per degree
  
  cY = Cos(Yi * rpd): sY = Sin(Yi * rpd)
  cP = Cos(Pi * rpd): sP = Sin(Pi * rpd)
  cR = Cos(Ri * rpd): sR = Sin(Ri * rpd)

  MAM(1, 1) = cP * cY
  MAM(1, 2) = -cP * sY
  MAM(1, 3) = sP
  MAM(2, 1) = sR * sP * cY + cR * sY
  MAM(2, 2) = -sR * sP * sY + cR * cY
  MAM(2, 3) = -sR * cP
  MAM(3, 1) = -cR * sP * cY + sR * sY
  MAM(3, 2) = cR * sP * sY + sR * cY
  MAM(3, 3) = cR * cP
End Sub

Sub InitializePointing()
' One time calculation of misalignment matrix to correct for Sensor Unit
' Heading Pitch and Roll offsets
Dim Elevation!, E0!, E1!, lu%, Emax!, Elev!(1 To 10)
'Dim Pd!, Rd!, Elevation!, E0!, E1!, lu%, Emax!, Elev!(1 To 10), Nel%
Dim BAD%, Inaccessable%, sElev!
Dim sElev0!, BadFlag As Boolean
' Note: Inst Tilt is in roll axis
'       Inst Azimuth is in Yaw axis
'       Inst Pitch is in pitch axis but included in pointing corrn
  
'  rpd = Atn(1) / 45#                        'Radians per degree
'  AC$ = "DC"                                'Specify platform
  PointingVersionDate = "11-25-2002 10:00A"
' Get platform elevation angles (optional); not needed in anything calculated here
'   Call GetEls(AC$, Elev())                  'Put in CFG file
                              
' Initialize aircraft instrument attitude parameters (optional)
'   Call GetMTPattitude(AC$, Yi!, Pi!, Ri!)   'Put in CFG file
  
' Calculate the MTP Attitude Matrix (MAM) MAM should be declared globally
  Call Get_MTP_Attitude_Matrix(MAM(), Yi, Pi, Ri)
    
' The following shows how commanded elevation angles are calculated and verified
' fEc is function used to calculate the commanded elevation for given Pitch, Roll and desired Elevation
' EmaxFlag is set if desired Elevation cannot be reached
'
'    E0 = fEc(MAM(), Pitch, Roll, Elevation, EmaxFlag)
'
' Given the commanded elevation, the following function should return the desired Elevation
' It is a useful check on the validity of the commanded elevation angle since the solution
' space is multi-valued
'
'    E1 = fSe(MAM!(), Pitch, Roll, E0)
'
  DoEvents
End Sub

Sub GetEls(AC$, Elev!())
' Not currently used
Select Case AC$
Case "DC"
  Elev(1) = 80#
  Elev(2) = 55#
  Elev(3) = 42#
  Elev(4) = 25#
  Elev(5) = 12#
  Elev(6) = 0#
  Elev(7) = -12#
  Elev(8) = -25#
  Elev(9) = -42#
  Elev(10) = -80#

Case "ER"
  Elev(1) = 60#
  Elev(2) = 44.4
  Elev(3) = 30#
  Elev(4) = 17.5
  Elev(5) = 8.6
  Elev(6) = 0#
  Elev(7) = -8.6
  Elev(8) = -20.5
  Elev(9) = -36.9
  Elev(10) = -58.2

Case "WB"
  Elev(1) = 70#
  Elev(2) = 47#
  Elev(3) = 32#
  Elev(4) = 20#
  Elev(5) = 9.5
  Elev(6) = 0#
  Elev(7) = -10#
  Elev(8) = -22#
  Elev(9) = -39#
  Elev(10) = -70#

Case "M5"
'+55, +38, +24, +12, 0, -10, -21,-34, -49 and -70
  Elev(1) = 55#
  Elev(2) = 38#
  Elev(3) = 24#
  Elev(4) = 12#
  Elev(5) = 0#
  Elev(6) = -10
  Elev(7) = -21#
  Elev(8) = -34#
  Elev(9) = -49#
  Elev(10) = -70#
End Select

End Sub
Function fSe(MAM!(), ByVal Pd!, ByVal Rd!, Ecommanded!)
Dim P!, r!, cP!, sP!, cR!, sR!, alpha!, beta!, Ec!

' Given the commanded elevation angle (Ec) this routine should return the desired
' elevation angle (Elevation) with respect to the horizon.
' It is a check that the correct Ec value was calculated by fEc
'
' MJ Mahoney, January 10, 2002
' Enter with MTP Attitude Matrix MAM, aircraft Pitch and Roll in degrees, and
' commanded elevation angle wrt to horizon.
'
' Return with desired elevation angle

' rpd = Atn(1) / 45#      'Radians per degree
  
' Convert angle from degrees to radians
  P = Pd * rpd
  r = Rd * rpd
  Ec = Ecommanded * rpd

  cP = Cos(P): sP = Sin(P)
  cR = Cos(r): sR = Sin(r)

  alpha = -cR * sP * MAM(1, 1) + sR * MAM(2, 1) + cR * cP * MAM(3, 1)
  beta = -1 * (-cR * sP * MAM(1, 3) + sR * MAM(2, 3) + cR * cP * MAM(3, 3))
  fSe = -ASN(alpha * Cos(Ec) + beta * Sin(Ec)) / rpd

End Function



