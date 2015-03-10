Attribute VB_Name = "Math"
Option Explicit

Private Const Pi As Single = 3.14159265358979
Private Const rpd As Single = 1.74532925199433E-02


Sub BubbleSort(ByRef A!(), ByRef n%)
Dim T!, i%, j%

For i = 1 To n
  For j = i + 1 To n
    If A(j) < A(i) Then  'Swap
      T = A(i)
      A(i) = A(j)
      A(j) = T
    End If
  Next j
Next i

End Sub

Sub BubbleSortL(ByRef A&(), ByRef n&)
Dim T&, i&, j&

For i = 1 To n
  For j = i + 1 To n
    If A(j) < A(i) Then  'Swap
      T = A(i)
      A(i) = A(j)
      A(j) = T
    End If
  Next j
Next i

End Sub

Sub RollOver(iY%, iM%, iD%, UTks!)
' Bump day, month and year if UTks > 86.4
  If UTks < 86.4 Then Exit Sub
  UTks = UTks - 86.4
  iD = iD + 1
  Select Case iM
  Case 1
    If iD > 31 Then iM = 2: iD = 1
  Case 2
    If iY Mod 4 = 0 Then  'Leap Year
      If iD > 29 Then iM = 3: iD = 1
    Else
      If iD > 28 Then iM = 3: iD = 1
    End If
  Case 3
    If iD > 31 Then iM = 4: iD = 1
  Case 4
    If iD > 30 Then iM = 5: iD = 1
  Case 5
    If iD > 31 Then iM = 6: iD = 1
  Case 6
    If iD > 30 Then iM = 7: iD = 1
  Case 7
    If iD > 31 Then iM = 8: iD = 1
  Case 8
    If iD > 31 Then iM = 9: iD = 1
  Case 9
    If iD > 30 Then iM = 10: iD = 1
  Case 10
    If iD > 31 Then iM = 11: iD = 1
  Case 11
    If iD > 30 Then iM = 12: iD = 1
  Case 12
    If iD > 31 Then iY = iY + 1: iM = 1: iD = 1
  End Select

End Sub

Function fSDT(SDT!) As Single
' Normal noise with standard deviation SDT
  fSDT = fNormal() * SDT

End Function

Function ACN(ByVal x!) As Single
' Take arccosine of a number (x!)
' Valid only for +- 90 degrees
Dim y!

  If Abs(x) < 0.00001 Then
    Select Case Sgn(x)
    Case Is < 0: ACN = -Pi / 2
    Case Else: ACN = Pi / 2
    End Select
  Else
    ACN = Atn(Sqr(1 - x ^ 2) / x)
  End If

End Function

Function ASN(ByVal x!) As Single
' Take arcsine of a number
' Valid only for +- 90 degrees
 
 If Abs(x) > 0.99999 Then ASN = x * Pi / 2 Else ASN = Atn(x / Sqr(1 - x ^ 2))
  
End Function

Sub AvgRMSdiff(Z1!(), T1!(), n1%, Z2!(), T2!(), n2%, AVG!, RMS!)
Dim Sum1!, Sum2!, x!, i%, nA%, n%
' This code is not yet working. Need to interpolate in Z
'i = 0
'Do: i = i + 1
'Loop Until z1(1) < z2(i)
' Scan to Scan RMS
If n1 = n2 Then
  For i = 1 To n1
    x = T1(i) - T2(i)
    Sum1 = Sum1 + x   'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  AVG = Sum1 / n     'Calculate average
  If Sum2 - AVG ^ 2 * n > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * n) / (n - 1)) 'Calculate RMS

Else

End If
End Sub

Sub AvgRMSdiffT(T1!(), T2!(), n%, Threshold!, M%, AVG!, RMS!)
Dim i%, j%, x!, Sum1!, Sum2!

  j = 0                       'Number of samples found
  For i = 1 To n
    x = T1(i) - T2(i)
    If Abs(x) <= Threshold Then
      j = j + 1
      Sum1 = Sum1 + x         'Sum time difference
      Sum2 = Sum2 + x ^ 2     'Sum square of time differences
    End If
'    Debug.Print i; T1(i); T2(i); X; j
  Next i
  M = j
  If M > 1 Then AVG = Sum1 / M Else Exit Sub  'Calculate average
  If Sum2 - AVG ^ 2 * M > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * M) / (M - 1)) 'Calculate RMS
End Sub
Sub AvgRMSdiffTP(Zac!(), Ztp!(), n%, TPhreshold!, M%, AVG!, RMS!)
Dim i%, j%, x!, Sum1!, Sum2!

  j = 0                       'Number of samples found
  For i = 1 To n
    x = Zac(i) - Ztp(i)
    If Abs(x) >= TPhreshold Then
      j = j + 1
      Sum1 = Sum1 + x         'Sum time difference
      Sum2 = Sum2 + x ^ 2     'Sum square of time differences
    End If
  Next i
  M = j
  If M > 1 Then AVG = Sum1 / M Else Exit Sub  'Calculate average
  If Sum2 - AVG ^ 2 * M > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * M) / (M - 1)) 'Calculate RMS
End Sub

Sub AvgRMSdiffR(T1!(), T2!(), Range!(), n%, Threshold!, M%, AVG!, RMS!)
Dim i%, j%, x!, Sum1!, Sum2!

  j = 0 'Number of samples found
  For i = 1 To n
    If Range(i) <= Threshold Then
      j = j + 1
      x = T1(i) - T2(i)
      Sum1 = Sum1 + x         'Sum time difference
      Sum2 = Sum2 + x ^ 2     'Sum square of time differences
    End If
'    Debug.Print i; T1(i); T2(i); X; Range(i); j
  Next i
  M = j
  If M > 1 Then AVG = Sum1 / M Else Exit Sub 'Calculate average
  If Sum2 - AVG ^ 2 * M > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * M) / (M - 1)) 'Calculate RMS
  
End Sub

Sub AvgRmsMinMax(ByRef A!(), ByVal n%, ByRef AVG!, ByRef RMS!, ByRef Min!, ByRef Max!)
Dim Sum1!, Sum2!, x!, i%
' Calculate average,rms, min and max of n numbers in A()
  Min = 99999999#
  Max = -99999999#
  For i = 1 To n
    x = A(i)
    If x < Min Then Min = x
    If x > Max Then Max = x
    Sum1 = Sum1 + x   'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  AVG = Sum1 / n     'Calculate average
  If Sum2 - AVG ^ 2 * n > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * n) / (n - 1)) Else RMS = 0# 'Calculate RMS
  
End Sub

Function fVaverage(z!(), V!(), n%, Z1!, Z2!)
Dim V1!, V2!, i%, i1%, i2%, Wt!, Vsum!

  V1 = fLinterp(z!(), V!(), n%, Z1)
  V2 = fLinterp(z!(), V!(), n%, Z1)
' Do linear interpolation in z to find value of at zo
  i1 = 0
  Do
    i1 = i1 + 1
  Loop Until z(i1) >= Z1 Or i = n

  i2 = i1
  Do
    i2 = i2 + 1
  Loop Until z(i2) >= Z2 Or i = n

' Now calculate weighted sum
  Vsum = (V1 + V(i1)) * (z(i1) - Z1)
  For i = i1 To i2 - 1
    Vsum = Vsum + (V(i) + V(i + 1)) * (z(i + 1) - z(i))
  Next i
  Vsum = Vsum + (V(i2) + V2) * (Z2 - z(i2))
  fVaverage = Vsum * (Z2 - Z1) / 2#

End Function

Function fGCD(LAT1!, LON1!, LAT2!, LON2!) As Single
' Calculate Great Circle Distance between two points in km
Dim D!

D = ACN(Sin(LAT1 * rpd) * Sin(LAT2 * rpd) + Cos(LAT1 * rpd) * Cos(LAT2 * rpd) * Cos((LON1 - LON2) * rpd))
fGCD = 1.852 * 60 * D / rpd 'km (1.852 km / nm, and 1 minute = 1 nm)

End Function


Function fNonNumeric(A$) As Boolean
Dim i%, n%

  For i = 1 To Len(A$)
    n = Asc(Mid$(A$, i, 1))
    If n < 48 Or n > 57 Then fNonNumeric = True: Exit Function
  Next i
  fNonNumeric = False

End Function


Function fNormal() As Single
' Calculate normally distributed random number
' Be sure to call RANDOMIZE before this routine
Dim V1!, V2!, r!, fAC!
Static Iset As Boolean, Gset!

  If Not Iset Then
    Do
      V1 = 2# * Rnd(1) - 1#
      V2 = 2# * Rnd(1) - 1#
      r = V1 ^ 2 + V2 ^ 2
    Loop Until r < 1
    fAC = Sqr(-2# * Log(r) / r)
    Gset = V1 * fAC
    fNormal = V2 * fAC
    Iset = True
  Else
    fNormal = Gset
    Iset = False
  End If
  
End Function

Sub FIT(x!(), y!(), nData%, sig!(), Mwt As Boolean, A!, b!, SigA!, SigB!, Chi2!, q!)
' Fit Y = A + Bx
' Sig ... SD on Y
' SigA .. SD on A
' SigB .. SD on B
' Chi2 .. Chi Squared
' Q ..... GoodneS of fit probability that would have Chi2 or larger
' If Mwt = 0 then Sig aSumed to be unavailable

Dim Sx!, sY!, Sxx!, Sxy!, s!, Wt!, Delta!, Sxoss!, GammQ!, SigDat!, T!
Dim i%
  Sx = 0#
  sY = 0#
  Sxx = 0#
  b = 0#
  If Mwt Then
    s = 0#
    For i = 1 To nData
      Wt = 1# / (sig(i) ^ 2)
      s = s + Wt
      Sx = Sx + x(i) * Wt
      sY = sY + y(i) * Wt
    Next i
  Else
    For i = 1 To nData
      Sx = Sx + x(i)
      sY = sY + y(i)
    Next i
    s = nData
  End If
  Sxoss = Sx / s      'average x value
  If Mwt Then
    For i = 1 To nData
      T = (x(i) - Sxoss) / sig(i)
      Sxx = Sxx + T * T
      b = b + T * y(i) / sig(i)
    Next i
  Else
    For i = 1 To nData
      T = x(i) - Sxoss
      Sxx = Sxx + x(i) ^ 2
      Sxy = Sxy + x(i) * y(i)
    Next i
  End If
  Delta = s * Sxx - Sx ^ 2
  
  A = (Sxx * sY - Sx * Sxy) / Delta
  b = (s * Sxy - Sx * sY) / Delta
'  B = B / Sxx
'  A = (Sy - Sx * B) / S
  SigA = Sqr((1# + Sx * Sx / (s * Sxx)) / s)
  SigB = Sqr(1# / Sxx)
  
  Chi2 = 0#
  If Not Mwt Then
    For i = 1 To nData
      Chi2 = Chi2 + (y(i) - A - b * x(i)) ^ 2
    Next i
    q = 1#
    SigDat = Sqr(Chi2 / (nData - 2))
    SigA = SigA * SigDat
    SigB = SigB * SigDat
  Else
    For i = 1 To nData
      Chi2 = Chi2 + ((y(i) - A - b * x(i)) / sig(i)) ^ 2
    Next i
    q = 0 'GammQ(0.5 * (Ndata - 2), 0.5 * Chi2)
  End If

End Sub
Sub AvgRMS(ByRef A!(), ByVal n%, ByRef AVG!, ByRef RMS!)
Dim Sum1!, Sum2!, x!, i%
' Calculate average and rms of n numbers in A()
  If n < 2 Then Exit Sub
  For i = 1 To n
    x = A(i)
    Sum1 = Sum1 + x         'Sum time difference
    Sum2 = Sum2 + x ^ 2     'Sum square of time differences
  Next i
  AVG = Sum1 / n            'Calculate average
  If Sum2 - AVG ^ 2 * n > 0 Then RMS = Sqr((Sum2 - AVG ^ 2 * n) / (n - 1)) Else RMS = 0# 'Calculate RMS
  
End Sub

Sub FIT2(x!(), y!(), nData%, sig!(), Mwt As Boolean, A!, b!, SigA!, SigB!, Chi2!, q!)
' Fit Y = A + Bx
' Sig ... SD on Y
' SigA .. SD on A
' SigB .. SD on B
' Chi2 .. Chi Squared
' Q ..... GoodneS of fit probability that would have Chi2 or larger
' If Mwt = 0 then Sig aSumed to be unavailable

Dim Sx!, sY!, Sxx!, s!, Wt!, Sxoss!, GammQ!, SigDat!, T!
Dim i%
  Sx = 0#
  sY = 0#
  Sxx = 0#
  b = 0#
  If Mwt Then
    s = 0#
    For i = 1 To nData
      Wt = 1# / (sig(i) ^ 2)
      s = s + Wt
      Sx = Sx + x(i) * Wt
      sY = sY + y(i) * Wt
    Next i
  Else
    For i = 1 To nData
      Sx = Sx + x(i)
      sY = sY + y(i)
    Next i
    s = nData
  End If
  Sxoss = Sx / s      'average x value
  If Mwt Then
    For i = 1 To nData
      T = (x(i) - Sxoss) / sig(i)
      Sxx = Sxx + T * T
      b = b + T * y(i) / sig(i)
    Next i
  Else
    For i = 1 To nData
      T = x(i) - Sxoss
      Sxx = Sxx + T * T
      b = b + T * y(i)
    Next i
  End If
  
  b = b / Sxx
  A = (sY - Sx * b) / s
  SigA = Sqr((1# + Sx * Sx / (s * Sxx)) / s)
  SigB = Sqr(1# / Sxx)
  
  Chi2 = 0#
  If Not Mwt Then
    For i = 1 To nData
      Chi2 = Chi2 + (y(i) - A - b * x(i)) ^ 2
    Next i
    q = 1#
    SigDat = Sqr(Chi2 / (nData - 2))
    SigA = SigA * SigDat
    SigB = SigB * SigDat
  Else
    For i = 1 To nData
      Chi2 = Chi2 + ((y(i) - A - b * x(i)) / sig(i)) ^ 2
    Next i
    q = 0 'GammQ(0.5 * (Ndata - 2), 0.5 * Chi2)
  End If
    
End Sub

Sub Fpoly(x!, P!(), NP%)
Dim j%
      
  P(1) = 1#
  For j = 2 To NP
    P(j) = P(j - 1) * x
  Next j

End Sub

Sub LFIT(x!(), y!(), sig!(), nData%, A!(), mA%, ListA%(), mFit%, CoVar!(), nCVM%, ChiSq!)
Dim k%, kk%, i%, j%, Afunc!(1 To 6), YM#, SIG2I#, Sum#
Dim beta!(1 To 6, 1), iHit%, Wt#

' Code from Numerical Recipes
  kk = mFit + 1
  For j = 1 To mA
    iHit = 0
    For k = 1 To mFit
      If ListA(k) = j Then iHit = iHit + 1
    Next k
    If iHit = 0 Then
      ListA(kk) = j
      kk = kk + 1
    Else
      If iHit > 1 Then MsgBox "Improper set in LISTA", vbOKOnly
    End If
  Next j
  If (kk <> (mA + 1)) Then MsgBox "Improper set in LISTA", vbOKOnly
    For j = 1 To mFit
      For k = 1 To mFit
        CoVar(j, k) = 0#
      Next k
      beta(j, 1) = 0#
    Next j
    For i = 1 To nData
      Call Fpoly(x(i), Afunc, mA)
      YM = y(i)
      If (mFit < mA) Then
        For j = mFit + 1 To mA
          YM = YM - A(ListA(j)) * Afunc(ListA(j))
        Next j
      End If
      SIG2I = 1# / sig(i) ^ 2
      For j = 1 To mFit
        Wt = Afunc(ListA(j)) * SIG2I
        For k = 1 To j
          CoVar(j, k) = CoVar(j, k) + Wt * Afunc(ListA(k))
        Next k
        beta(j, 1) = beta(j, 1) + YM * Wt
      Next j
    Next i
    If (mFit > 1) Then
      For j = 2 To mFit
        For k = 1 To j - 1
          CoVar(k, j) = CoVar(j, k)
        Next k
      Next j
    End If
    Call GAUSSJ(CoVar(), mFit, nCVM, beta(), 1, 1)
    For j = 1 To mFit
      A(ListA(j)) = beta(j, 1)
    Next j
    ChiSq = 0#
    For i = 1 To nData
      Call Fpoly(x(i), Afunc, mA)
      Sum = 0#
      For j = 1 To mA
        Sum = Sum + A(j) * Afunc(j)
      Next j
      ChiSq = ChiSq + ((y(i) - Sum) / sig(i)) ^ 2
    Next i
    Call COVSRT(CoVar(), nCVM, mA, ListA(), mFit)
 
 End Sub
Sub GAUSSJ(A!(), n%, NP%, b!(), M%, MP%)
Dim IPIV%(1 To 10), INDXR%(1 To 10), INDXC%(1 To 10)
Dim i%, j%, k%, L%, iRow%, iCol%, BIG!, DUM!, PIVINV!, ll%
' Code from Numberical Recipes
For j = 1 To n: IPIV(j) = 0: Next j
  For i = 1 To n
    BIG = 0#
    For j = 1 To n
      If (IPIV(j) <> 1) Then
        For k = 1 To n
          If (IPIV(k) = 0) Then
            If (Abs(A(j, k)) >= BIG) Then
              BIG = Abs(A(j, k))
              iRow = j
              iCol = k
            End If
          Else
            If (IPIV(k) > 1) Then MsgBox "Singular Matrix!", vbOKOnly
          End If
        Next k
      End If
    Next j
    IPIV(iCol) = IPIV(iCol) + 1
    If (iRow <> iCol) Then
      For L = 1 To n
        DUM = A(iRow, L)
        A(iRow, L) = A(iCol, L)
        A(iCol, L) = DUM
      Next L
      For L = 1 To M
        DUM = b(iRow, L)
        b(iRow, L) = b(iCol, L)
        b(iCol, L) = DUM
      Next L
    End If
    INDXR(i) = iRow
    INDXC(i) = iCol
    If (A(iCol, iCol) = 0#) Then
      MsgBox "Singular Matrix!", vbOKOnly
      Exit Sub
    End If
    PIVINV = 1# / A(iCol, iCol)
    A(iCol, iCol) = 1#
    For L = 1 To n
      A(iCol, L) = A(iCol, L) * PIVINV
    Next L
    For L = 1 To M
      b(iCol, L) = b(iCol, L) * PIVINV
    Next L
    For ll = 1 To n
      If (ll <> iCol) Then
        DUM = A(ll, iCol)
        A(ll, iCol) = 0#
        For L = 1 To n
          A(ll, L) = A(ll, L) - A(iCol, L) * DUM
        Next L
        For L = 1 To M
          b(ll, L) = b(ll, L) - b(iCol, L) * DUM
        Next L
      End If
    Next ll
  Next i
  For L = n To 1 Step -1
    If (INDXR(L) <> INDXC(L)) Then
      For k = 1 To n
        DUM = A(k, INDXR(L))
        A(k, INDXR(L)) = A(k, INDXC(L))
        A(k, INDXC(L)) = DUM
      Next k
    End If
  Next L
  
End Sub
Sub Qfit(x!(), y!(), n%, Coeff!(), nC%)
Dim nData%, A!(1 To 10), mA%, CoVar!(1 To 10, 1 To 10), ChiSq!
Dim sig!(1 To 100), ListA%(1 To 10), nCVM%, mFit%, i%
' Routine to do quadratic fit to 3 TB errors due to RAOB bias
'x(1) = -25#: x(2) = 0#: x(3) = 25#
'y(1) = 1.59: y(2) = 0: y(3) = -2.11
'A(2)= 0.:A(2)= -0.074:A(3)= -4.159999E-04  agrees will Excel fit
nData = n
mA = nC
nCVM = nC
mFit = nC
For i = 1 To nData: sig(i) = 1#: ListA(i) = i: Next i

Call LFIT(x!(), y!(), sig!(), nData%, A!(), mA%, ListA%(), mFit%, CoVar!(), nCVM%, ChiSq!)

'Debug.Print x(1); x(2); x(3); y(1); y(2); y(3); A(1); A(2); A(3)
For i = 1 To nC
  Coeff(i) = A(i)
Next i
End Sub

Sub COVSRT(CoVar!(), nCVM%, mA%, ListA%(), mFit%)
Dim i%, j%, k%, Swap!
' Numberical Recipes code called by LFIT
  For j = 1 To mA - 1
    For i = j + 1 To mA
      CoVar(i, j) = 0#
    Next i
  Next j
  For i = 1 To mFit - 1
    For j = i + 1 To mFit
      If (ListA(j) > ListA(i)) Then
        CoVar(ListA(j), ListA(i)) = CoVar(i, j)
      Else
        CoVar(ListA(i), ListA(j)) = CoVar(i, j)
      End If
    Next j
  Next i
  Swap = CoVar(1, 1)
  For j = 1 To mA
    CoVar(1, j) = CoVar(j, j)
    CoVar(j, j) = 0#
  Next j
  CoVar(ListA(1), ListA(1)) = Swap
  For j = 2 To mFit
    CoVar(ListA(j), ListA(j)) = CoVar(1, j)
  Next j
  For j = 2 To mA
    For i = 1 To j - 1
      CoVar(i, j) = CoVar(j, i)
    Next i
  Next j

End Sub

Sub Cinv(x1!, y1!, x2!, y2!)
'
Dim Mag!
  Mag = x1 ^ 2 + y1 ^ 2
  x2 = x1 / Mag
  y2 = -y1 / Mag
End Sub


Sub CpC(x1!, y1!, x2!, y2!, x3!, y3!)

x3 = x1 + x2
y3 = y1 + y2
  
End Sub

Sub CmC(x1!, y1!, x2!, y2!, x3!, y3!)
x3 = x1 - x2
y3 = y1 - y2
End Sub



Sub CxC(x1!, y1!, x2!, y2!, x3!, y3!)
Dim Mag!
x3 = x1 * x2 - y1 * y2
y3 = x1 * y2 + x2 * y1

End Sub

Function fLinterp(z!(), V!(), n%, zo!) As Single
' Do linear interpolation in z to find value of at zo
Dim i%, it%, ib%

  i = 0
  Do
    i = i + 1
  Loop Until z(i) >= zo Or i = n
  If z(i) = zo Then fLinterp = V(i): Exit Function
  If i > n Then fLinterp = V(n): Exit Function
  it = i
  ib = i - 1
  If ib = 0 Then
    fLinterp = V(it)
  Else
    fLinterp = V(ib) + (V(it) - V(ib)) * (zo - z(ib)) / (z(it) - z(ib))
  End If

End Function
Function LapseRate(z, TZ, z0, T0)
'Lapse rate from (z0,T0) to (z,T)
    LapseRate = (TZ - T0) / (z - z0)
End Function


Function Calc_LR_Level(z0!(), T0!(), T2#(), j%, LRt!) As Single
' Enter with j% as index to level BELOW where LR=LRt
' Function returns altitude where LR=LRt
' Z0, T0 is T profile
' T2 are second derivatives from spline fit
Dim dT#, dZ#, AA#, Bb#, cC#, Arg#, SolnP#, SolnM#

  dT = T0(j% + 1) - T0(j%)
  dZ = z0(j% + 1) - z0(j%)
  AA = (T2(j% + 1) - T2(j%))
  Bb = -2 * T2(j% + 1)
  If dZ <> 0# Then
    cC = T2(j% + 1) - (AA + (6 / dZ) * (LRt - dT / dZ)) / 3
  Else
    cC = 0#
  End If
  Arg = Bb ^ 2 - 4 * AA * cC
  If Arg > 0 Then            'Avoid arithmetic fault
    SolnP = z0(j% + 1) - dZ * (-Bb + Sqr(Arg)) / (2 * AA)
    SolnM = z0(j% + 1) - dZ * (-Bb - Sqr(Arg)) / (2 * AA)
    If SolnP >= z0(j) And SolnP <= z0(j + 1) Then
      Calc_LR_Level = SolnP
    Else
      Calc_LR_Level = SolnM
    End If
    'Debug.Print z0(j); z0(j + 1); SolnP; SolnM; Calc_LR_Level
  Else
    Calc_LR_Level = 99.9: Exit Function
  End If
End Function


Function fHexToDec(HexNo$) As Integer
Dim V%
' Convert HexNo$ to Decimal Number
  V = Val("&H" + HexNo$)
  If V < 0 Then V = 65536 + V  'MSB is not sign, add 2^16
  fHexToDec = V
End Function


Sub INC(j%)
j% = j% + 1
End Sub


Sub RBCaverage(n%, Count%, V!())
' Perform running box car average over N cycles on V() which has Count elements
' Return with V containg running box car average
' Note, N should be an odd number
Dim x!(1 To 10000), i%, j%, n1%, n2%, n3%, Sum!

  If Int(n / 2) = n / 2 Then
    MsgBox "RBCaverage, N must be an odd integer!", vbOKOnly
    Exit Sub
  End If
  n1 = Int(n / 2)
  n2 = n1 + 1
' Save V
  For i = 1 To Count
    x(i) = V(i)
  Next i
' Average first n1 cycles
  For i = 1 To n1
    Sum = 0#
    n3 = 2 * (i - 1) + 1
    For j = 1 To n3
      Sum = Sum + x(j)
    Next j
    V(i) = Sum / n3
  Next i
' Average from n1+1 to count-n1
  For i = n1 + 1 To Count - n1
    Sum = 0#
    For j = i - n1 To i + n1
      Sum = Sum + x(j)
    Next j
    V(i) = Sum / n
  Next i
' Average last n1 cycles
  For i = Count - n1 + 1 To Count
    Sum = 0#
    n3 = -2 * (i - Count) + 1
    For j = 1 To n3
      Sum = Sum + x(Count - j + 1)
    Next j
    V(i) = Sum / n3
  Next i

End Sub

Function SPLINT(z0!(), T0!(), TD2#(), M%, n%, z, R_flag As Boolean)
Dim dZ#, A#, b#, klo%, khi%, k%
'This procedure calculates temperature (T) for a specified altitude (z)
'Routine SPLINE must be run once before using this routine
'Input Parameters:
' z0 .. Measured altitude observables
' T0 .. Measured temperature observables
' TD2 .. Second derivatives of T wrt z (from SPLINE)
' m% ... First non-zero element of z0 if R_flag=TRUE
'        Current index if R_flag=FALSE
' n% ... Last non-zero element of z0
' z ... Altitude for which temperature T is needed
' R_flag TRUE for random Splints, FALSE if consecutive
'Output parameters:
' T .... Temperature at specified altitude (z)

'Bound the current fit point by retrieved values
  klo% = M%
  khi% = n%
  If R_flag Then
    While (khi% - klo% > 1)        'Perform binary search
      k% = (khi% + klo%) / 2
      If (z0(k%) > z) Then: khi% = k%: Else: klo% = k%
    Wend
  Else
    While z0(klo%) <= z
      klo% = klo% + 1
      If klo% > khi% Then Exit Function
    Wend
    M% = klo%
  End If

  dZ = z0(khi%) - z0(klo%)
  If (dZ = 0) Then dZ = 0.0001 'Stop              'Bad Z0() input.'
  A = (z0(khi%) - z) / dZ
  b = (z - z0(klo%)) / dZ
  If khi <= n Then SPLINT = A * T0(klo%) + b * T0(khi%) + ((A ^ 3 - A) * TD2(klo%) + (b ^ 3 - b) * TD2(khi%)) * (dZ ^ 2) / 6

End Function


Sub DEC(j%)
j% = j% - 1
End Sub
Sub SPLINE(z0!(), T0!(), TD1#(), TD2#(), M%, n%)
'
' Input Parameters:
' z0 ... Measured altitude observables
' T0 ... Measured temperature observables
' M% ... First non-zero element in z0
' N% ... Last non-zero element in z0
' Output parameters:
' TD1 ... Calculated first derivatives of T wrt Z
' TD2 ... Calculated second derivatives of T wrt Z
Dim dZ#, sig#, i%, P#, dT#, U#(200)

If (n%) < 2 Then Exit Sub
'ReDim U(1 To n%)     mjmmjm

' Do one-time Spline fit for input parameters
' Use natural fit; 2nd derivative=0 at end points
  TD2(M%) = 0
  U(M%) = 0
  For i% = M% + 1 To n% - 1
    If i > n Then Exit For
    If z0(i + 1) <> z0(i - 1) Then
      sig = (z0(i%) - z0(i% - 1)) / (z0(i% + 1) - z0(i% - 1))
    Else
      sig = 0
    End If
    
    P = sig * TD2(i% - 1) + 2
    TD2(i%) = (sig - 1) / P
    If sig <> 0 And z0(i) <> z0(i + 1) Then
      U(i%) = (6 * ((T0(i% + 1) - T0(i%)) / (z0(i% + 1) - z0(i%)) - (T0(i%) - T0(i% - 1)) / (z0(i%) - z0(i% - 1))) / (z0(i% + 1) - z0(i% - 1)) - sig * U(i% - 1)) / P
    Else
      U(i) = 0#
    End If
 ' Debug.Print i%; U(i%); p; SIG
  Next
  If n < 200 Then TD2(n%) = 0

  For i% = n% - 1 To M% Step -1
    If i > n Then Exit For
    dT = T0(i% + 1) - T0(i%)
    dZ = z0(i% + 1) - z0(i%)
    If dZ <> 0 Then
      TD1(i%) = dT / dZ - (dZ / 6) * (2 * TD2(i%) + TD2(i% + 1))
    Else
      TD1(i) = 0#
    End If
    
    TD2(i%) = TD2(i%) * TD2(i% + 1) + U(i%)
  Next

End Sub



Sub trapzd(A!, b!, s!, n%)
Dim TNM%, j%, Sum!, x!, DEL!
Static it%

If n = 1 Then
'  S = 0.5 * (b - a) * (FUNC(a) + FUNC(b))
  it = 1
Else
  TNM = it
  DEL = (b - A) / TNM
  x = A + 0.5 * DEL
  Sum = 0#
  For j = 1 To it
'    Sum = Sum + FUNC(x)
    x = x + DEL
  Next
  s = 0.5 * (s + (b - A) * Sum / TNM)
  it = 2 * it
End If

End Sub


Function DCOS(E)
Dim C!
 C = 180 / 3.14159265358979

DCOS = Cos(E / C)

End Function

Function DSExyz(Tilt!, Az!, Pitchoff!, P!, r!, E!)

Static T#, C!, sinA!, sinP!, sinR!, sinE!, sinF!, sinT!
Static cosA!, cosP!, cosR!, cosE!, cosT!, cosF!, cosFs!
Static tanF!, tanFs!
Dim s!, g!, f!, sinAlpha!, cosAlphas!, cosAlpha!, tanAlpha!, alpha!
Dim AlphaM!, sinG!, cosGs!, cosG!, tanG!, gamma!, sinGamma!
Dim CosI!, SinIs!, SinI!, TanI!, i!, SinS!, CosS!, CosSs!, CosSm!
Dim TanS!, Delta!, SinDelta!, SinDeltaS!, CosDeltaS!, CosDelta!
Dim TanDelta!, DSE!

 T# = Tilt
 C = 180 / 3.14159265358979
 sinA = Sin(Az / C): cosA = Cos(Az / C)
 sinP = Sin(P / C): cosP = Cos(P / C)
 sinR = Sin(r / C): cosR = Cos(r / C)
 sinE = Sin(E / C): cosE = Cos(E / C)
 sinT = Sin(T# / C): cosT = Cos(T# / C)

 sinF = sinP * cosA - cosP * sinA * sinR
 cosFs = 1 - sinF ^ 2:
 If cosFs < 0 Then cosFs = 0.00001 'added 960204 as fix as precaution
 If cosFs > 1 Then cosFs = 0.99999 'added 960204 as fix as precaution
 cosF = Sqr(cosFs):
 If cosFs = 0 Then cosFs = 0.00001 'added 960204 as fix as precaution
 tanF = sinF / cosF
 f = C * Atn(tanF)                                            'we have f

 sinAlpha = cosR * cosP / cosF
 If sinAlpha > 0.99999 Then sinAlpha = 0.99999 'added 960204 as fix to FailLite
 cosAlphas = 1 - sinAlpha ^ 2:
 If cosAlphas < 0 Then cosAlpha = 0.00001 'added 960204 as fix to FailLite
 If cosAlphas > 1 Then cosAlpha = 0.99999 'added 960204 as fix to FailLite
 cosAlpha = Sqr(cosAlphas):
    If cosAlpha = 0 Then cosAlpha = 0.00001
 tanAlpha = sinAlpha / cosAlpha
 alpha = C * Atn(tanAlpha): AlphaM = 180 - alpha              'we have Alpha

    'code for choosing Alpha vs Alpham
     If cosA <= 0 Then cosA = 0.00001  'added 960204 as fix as precaution
     sinG = sinP / cosA
     cosGs = 1 - sinG ^ 2:
     If cosGs <= 0 Then cosGs = 0.00001 'added 960204 as fix as precaution
     cosG = Sqr(cosGs):
     If cosG = 0 Then cosG = 0.00001 'added 960204 as fix as precaution
     tanG = sinG / cosG
     g = C * Atn(tanG)
     If f > g Then alpha = AlphaM

 gamma = alpha + Tilt - 90                                    'we have Gamma

 sinGamma = Sin(gamma / C)
 CosI = sinGamma * cosF:
 If CosI = 0 Then CosI = 0.00001     'added 960204 as fix as precaution
 SinIs = 1 - CosI ^ 2:
 If SinIs < 0 Then SinIs = 0.00001   'added 960204 as fix as precaution
 If SinIs > 1 Then SinIs = 0.99999   'added 960204 as fix as precaution
 SinI = Sqr(SinIs):
 If SinI = 0 Then SinI = 0.00001     'added 960204 as fix as precaution
 TanI = SinI / CosI
 i = C * Atn(TanI)                                            'we have i

 SinS = sinF / SinI
 CosSs = 1 - SinS ^ 2:
 If CosSs <= 0 Then CosSs = 0.00001  'added 960204 as fix as precaution
 If CosSs > 1 Then CosSs = 0.99999   'added 960204 as fix as precaution
 CosS = Sqr(CosSs): CosSm = -CosS

 TanS = SinS / CosS
 s = C * Atn(TanS)                                           'we have S

 If sinE > SinI Then  'chk for "can't get there"
   If E >= 0 Then  'go to highest possible
     Delta = 90
   End If

   If E < 0 Then  'go to lowest possible
     Delta = -90
   End If
 End If

 If E > 0 And sinE < SinI Then      'there's a solution
  SinDelta = sinE / SinI
   CosDeltaS = 1 - SinDelta ^ 2:
   If CosDeltaS < 0 Then CosDeltaS = 0.00001 'added 960204 as fix as precaution
   If CosDeltaS > 1 Then CosDeltaS = 0.99999 'added 960204 as fix as precaution
   CosDelta = Sqr(CosDeltaS):
   If CosDelta = 0 Then CosDelta = 0.00001  'added 960204 as fix as precaution
   TanDelta = SinDelta / CosDelta
   Delta = C * Atn(TanDelta)
   'IF E < 0 THEN Delta = -Delta   '!!!!!!!!!
 End If

 If E < 0 And -sinE < SinI Then      'there's a solution
  SinDelta = sinE / SinI
   CosDeltaS = 1 - SinDelta ^ 2:
   If CosDeltaS < 0 Then CosDeltaS = 0.00001 'added 960204 as fix as precaution
   If CosDeltaS > 1 Then CosDeltaS = 0.99999 'added 960204 as fix as precaution
   CosDelta = Sqr(CosDeltaS):
   If CosDelta = 0 Then CosDelta = 0.00001 'added 960204 as fix as precaution
   TanDelta = SinDelta / CosDelta
   Delta = C * Atn(TanDelta)
   'IF E < 0 THEN Delta = -Delta   '!!!!!!!!!
 End If

 If E < 0 And -sinE > SinI Then   'no solution; must go to lowest possible
   Delta = -90
 End If                                                 'we have Delta

 DSE = Delta - s
 If DSE < -90 Then DSE = -90
 If DSE > 90 Then DSE = 90                              'we have DSE

LeaveSub0:

'DSExyz = -((180 + 10.3) - DSE) 'refer DSE to RefTarget = 0 degrees
'nmprint "Q, P, R --> " + STR$(navqual) + " " + STR$(P) + " " + STR$(R), 25, 1
' removed above and inserted below mjm 20000831
'ER2: DSExyz = -((180 + 10.3) - DSE) 'refer DSE to RefTarget = 0 degrees
'WAM:
DSExyz = -(180 + Pitchoff - DSE) 'refer DSE to RefTarget = 0 degrees
'nmprint "Q, P, R --> " + Form$(NavQual, 1, 0) + " " + Form$(P, 5, 1) + " " + Form$(R, 5, 1), 25, 1
End Function

Function DSIN(E)
Dim C!
  C = 180 / 3.14159265358979
  DSIN = Sin(E / C)
End Function
Function fPTinterp(z!(), V!(), n%, zo!)
' Do linear interpolation in z to find value of at zo
Dim i%, it%, ib%

  i = n + 1
  Do
    i = i - 1
  Loop Until z(i) <= zo Or i = 1
  If z(i) = zo Then fPTinterp = V(i): Exit Function
  If z(1) > zo Then fPTinterp = -1: Exit Function
  ib = i
  it = i + 1
  If it > n Then
    fPTinterp = -1 'V(N)
  Else
    fPTinterp = V(ib) + (V(it) - V(ib)) * (zo - z(ib)) / (z(it) - z(ib))
  End If
  
End Function

