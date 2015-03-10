Attribute VB_Name = "Module2"
Dim Packet2Ah As String, Packet2Bh(1 To 5) As String

' Packet 2Bh Parameters
Public pALT!, OAT!, CC2&(1 To 5)
Public Heading!, WindSpeed!, WindDir!, Pitch!, Roll!, Zgps!

' Packet 2Ah Parameters
Public Latitude!, Longitude!, UTks!, HHMMSS$, CC1&

Sub Decode2Bh(Packet2Bh As String, n%, Status2Bh)
Dim i%, High!, Low!
' All  2Bh parameters are 2 bytes long and scaled
' T,  P,  TAS, pALT, DA,  Roll, TH,   B,    Pitch,AA,   SA,   B,    Wsd,  Wdir, B,    CC2
' 1-2,3-4,5-6, 7-8,  9-10,11-12,13-14,15-16,17-18,19-20,21-22,23-24,25-26,27-28,29-30,31-32

' Status Word 2 (deg)
  High = Asc(Mid$(Packet2Bh, 32, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet2Bh, 33, 1)) ' Get Low Byte
  CC2(n) = 256 * High + Low
' Check for valid status
  Status2Bh = 1  'Good status word
  Mask = 12392          '8 + 32 + 64 + 4096 + 8192
  If Mask And CC2(n) < Mask Then Status2Bh = 0
  
' 1 T, 2 P, 3 TAS, 4 pALT, 5 DA, 6 Roll, 7 TH, 8 080-1
' 9 Pitch, 10 AA, 11 SA, 12 ADC, 13 Wind, 14 LSS, 15 DS, 16 080-1
' Outside air temperature (C)
  OAT = fGetNo(Packet2Bh, 2) * 1024# / 32768#
' Pressure Altitude (meters)
  pALT = fGetNo(Packet2Bh, 8)
' True Roll(deg)
  Roll = fGetNo(Packet2Bh, 12) * 180# / 32768#
' True Heading (deg)
  Heading = fGetNo(Packet2Bh, 14) * 180# / 32768#
' True Pitch (deg)
  Pitch = fGetNo(Packet2Bh, 18) * 180# / 32768#
' Wind Speed (deg)
  WindSpeed = fGetNo(Packet2Bh, 26) * 128# / 32768#
' Wind Direction (deg)
  WindDir = fGetNo(Packet2Bh, 28) * 180# / 32768#

End Sub
Function fGetNo(Packet$, Index%)
Dim High%, Low%, Minus As Boolean
  Minus = False
  High = Asc(Mid$(Packet$, Index, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet$, Index + 1, 1)) ' Get Low Byte
' Take twos-complement of negative number if necessary
  If (High And 128) Then
    Minus = True
    High = High - 128
  End If
  If Minus Then fGetNo = -1 * (High * 256 + Low) Else fGetNo = High * 256 + Low
End Function

Sub ReadNextFrame(NoData%)
Dim i%, j%

  NoData = 0   '1 if not enough data in buffer
' Find 2Ah Packet
  Call FindCode("&H2A")
' And read it
  If MSComm1.InBufferCount > 46 Then
    For j = 1 To 46
      Packet2Ah = Packet2Ah + MSComm1.Input
    Next j
  Else
    NoData = 1
    Exit Sub
  End If
  
' Find 2Bh Packet
  EOFflag = False
  For i = 1 To 5
    FindCode ("&H2B")
    If EOF(INPlu) Then EOFflag = True: Exit Sub
    If MSComm1.InBufferCount > 32 Then
      For j = 1 To 32
        Packet2Bh(i) = Packet2Bh(i) + MSComm1.Input
      Next j
    Else
      NoData = 1
      Exit Sub
    End If
  Next i

End Sub
Sub Decode2Ah(Packet2Ah$, Status2Ah%)
Dim char1 As String, char2 As String, High!, Low!
Dim Lat$, Lon$, Deg$, Min$, Mask%

' Read CpK(2),GMT(9),LAT(10),AA(2),LON(11),GS(5),TA(3),Zgps(2),CC1(2)
'      1-2    3-11   12-21   22-23 24-34   35-39 40-42 43-44   45-46
' Read Status Byte CC1
  High = Asc(Mid$(Packet2Ah, 46, 1))  ' Get High Byte
  Low = Asc(Mid$(Packet2Ah, 47, 1)) ' Get Low Byte
  CC1 = 256 * High + Low
  Mask = 294  ' 2 + 4 + 32 + 256
' Check status
  Status2Ah = 1
  If CC1 And Mask < Mask Then Status2Ah = 0
' Decode GMT
'  HHMMSS$ = Mid$(Packet2Ah, 3, 9)
  HHMMSS$ = Mid$(Packet2Ah, 3, 2) & ":" & Mid$(Packet2Ah, 5, 2) & ":" & Mid$(Packet2Ah, 7, 5)
  UTks = 3.6 * Val(Mid$(Packet2Ah, 3, 2)) + 0.06 * Val(Mid$(Packet2Ah, 5, 2)) + Val(Mid$(Packet2Ah, 7, 5)) / 1000#

' Decode Latitude
  Deg$ = Mid$(Packet2Ah, 13, 2)
  Min$ = Mid$(Packet2Ah, 15, 7)
  Latitude = Val(Deg$) + Val(Min$) / 60#
  If Mid$(Packet2Ah, 12, 1) = "S" Then Latitude = -Latitude
  
' Skip reserved word AAh
   
' Read Longitude
  Deg$ = Mid$(Packet2Ah, 25, 3)
  Min$ = Mid$(Packet2Ah, 28, 7)
  Longitude = Val(Deg$) + Val(Min$) / 60#
  If Mid$(Packet2Ah, 24, 1) = "E" Then Longitude = -Longitude

' Read GPS Altitude
  Zgps = fGetNo(Packet2Ah, 43)
  txtZgps = Zgps

End Sub


Sub FindCode(Code%)
Dim Char$, CharCode%, LastCode%
  
  EOFflag = False
  If MSComm1.InBufferCount > 0 Then
    CharCode = Asc(MSComm1.Input)
  Else
    NoComPortData = True
    Exit Sub
  End If
  
  Do    ' Loop until end of file.
    LastCode = CharCode
    If MSComm1.InBufferCount > 0 Then
      CharCode = Asc(MSComm1.Input)
    Else
      MsgBox "There is no data on COM Port " + Str(COMport) + "!", vbOKOnly
      NoComPortData = True
      Exit Sub
    End If
  Loop Until EOF(INPlu) Or (CharCode = Code) 'LastCode = "&H10" And

End Sub

