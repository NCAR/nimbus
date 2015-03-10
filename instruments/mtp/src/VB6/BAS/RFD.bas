Attribute VB_Name = "RFD"



Sub DecodeM01()
Dim s$(40), n%, i%, Label$(8), fact$(8), T
  
  Parse M01dat$, " ", s$(), n
  
  fact$(7) = -5.1: Label$(7) = "-15V PS   "
  fact$(6) = 2#: Label$(6) = "VCC  PS   "
  fact$(5) = 5.1: Label$(5) = "+15V PS   "
  fact$(4) = 7.79: Label$(4) = "+24V Syn  "
  fact$(3) = 7.79: Label$(3) = "+24V Step "
  fact$(2) = 2.78: Label$(2) = "+8V  PS   "
  fact$(1) = 1: Label$(1) = "Video V.  "
  fact$(0) = -2.73: Label$(0) = "-8V  PS   "
  
  lstMux1.Clear
  Parse M01dat$, " ", s$(), n
  lstMux1.AddItem "Channel  Counts  Volts"
  For i = 2 To n
    T = fact$(i - 2) * (Val(s$(i)) / 1000)
    lstMux1.AddItem Label$(i - 2) + s$(i) + Format$(T, " +00.00V; -00.00V")
  Next i

End Sub

Sub DecodeM02()
Dim s$(40), n%, i%, Label$(8), cnt, RR, Rt, T
Dim A, B, C ' thermistor constants
  A = 0.0009376: B = 0.0002208: C = 0.0000001276
  Parse M02dat$, " ", s$(), n
  Label$(0) = "Acceler   "
  Label$(1) = "T Data    "
  Label$(2) = "T Motor   "
  Label$(3) = "T Pod Air "
  Label$(4) = "T Scan    "
  Label$(5) = "T Pwr Sup "
  Label$(6) = "T N/C     "
  Label$(7) = "T Synth   "
  lstMux2.Clear
  Parse M02dat$, " ", s$(), n
  lstMux2.AddItem "Channel  Counts  Value"
  For i = 2 To n
    Select Case i
      Case Is = 2
        T = -((Val(s$(i)) * 0.001) - 2.5) / 0.4  ' MMA1250D accellerometer 2.5V +/- .25V @ 0G
        lstMux2.AddItem Label$(i - 2) + s$(i) + Format$(T, "  +0.00 g;  -0.00 g")
      Case Else
        cnt = Val(s$(i))
        If (cnt = 4095) Or (cnt = 0) Then
          lstMux2.AddItem Label$(i - 2) + s$(i) + "   N/A   "
        Else
          cnt = 4096 - cnt
          RR = (1 / (cnt / 4096)) - 1
          Rt = 34800 * RR
          T = (1 / (A + B * Log(Rt) + C * Log(Rt) ^ 3) - 273.16)
          lstMux2.AddItem Label$(i - 2) + s$(i) + Format$(T, " +00.00 C; -00.00 C")
      End If
    End Select
  Next i

End Sub
'PARSE SUB for chopping up strings into sub strings using delimiting
'characters.  'View parse() for details.

'DECLARE SUB parse (strgin$, par$, s$(), n!)

'****************************************************************
' VARIABLES PASSED BY PARSE SUB
'Global StrgIn$, Par$, s$(30), n
'****************************************************************

Sub ParseX(ByRef StrgIn$, ByRef Par$, ByRef s$(), ByRef n%)
'*************************************************************************
' MAY 1993, R. F. Denning
' PARSES THE INPUT STRGIN$ INTO N SUBSTRINGS(S$()), CONTAINING CONSECUTIVE
' CHARS BOUNDED BY ANY DELIMITING CHARS FROM PAR$. CONSECUTIVE DELIMITERS
' (FOR EXAMPLE TWO SPACES IN A ROW) ARE TREATED AS ONE, EXCEPT THAT  ADJACENT
' COMMAS ARE DETECTED AND WILL PRODUCE NULL STRINGS. COMMAS FOLLOWING OTHER
' DELIMITERS ARE SKIPPED.
'
' AN EXAMPLE MIGHT BE:
' STRGIN$ = "A 12345%54321, 66666,,,     93/05/10,09:11:32 RANDOM TEXT"
' PAR$="% /,:" 'I.E. PERCENT, SPACE, SLASH, COMMA, COLON (IN ANY ORDER)
' PARSE STRGIN$, PAR$, S$(), N
' FOR I=1 TO N
' PRINT S$(I)
' NEXT I
'
' THIS WILL PRODUCE THE FOLLOWING OUTPUT:
' A
' 12345
' 54321
' 66666
' [NULL$]
' [NULL$]
' 93
' 05
' 10
' 09
' 11
' 32
' RANDOM
' TEXT
'
' RETURNING WITH N=0 MEANS NO SUBSTRINGS COULD BE FOUND. N=1 means strgin$ was
' all one string without delimiters.
' strgin$, par$ are not modified, s$() is cleared on entry.
'
'*************************************************************************
Dim Delim%, CharsFound%, C$, LastC$

  For i = 0 To n: s$(i) = "": Next  'CLEAR OUT OLD MATRIX
  n = 1
  Delim = 1
  CharsFound = 0

  For i = 1 To Len(StrgIn$)
    C$ = Mid$(StrgIn$, i, 1)
    If InStr(Par$, C$) > 0 Then   'Is this character a delimiter?
      If (C$ = ",") And (LastC$ = ",") Then Delim = 0: n = n + 1 'Yes
      Delim = 1
      LastC$ = C$
    Else                                                 'No
      Delim = 0
      s$(n) = s$(n) + C$                                 'Add character to S$
      CharsFound = CharsFound + 1
    End If
  Next i
  If CharsFound = 0 Then n = 0

' TEST CODE:
'StrgIn$ = "Now:Is%The/Time,( 00:00:00, 5/29/1993 ), For All         Good Men To Come To The Aid Of Their Country"
'par$ = "% /,:()"'I.E. SPACE,SLASH, COMMA, COLON, PARENS (IN ANY ORDER)
'parse strgin$, par$, s$(), n
'CLS
'PRINT "Input String:"
'PRINT strgin$
'FOR i = 1 TO n
'PRINT s$(i),
'NEXT i
'PRINT
'PRINT

'PRINT "Parsing TIME$, DATE$:"
'PRINT "Hours, Minutes Seconds now:"
'parse TIME$, ":", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT

'PRINT "Month, Date, Year now:"
'parse DATE$, "-", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT

'PRINT "Parsing DATE$+TIME$"
'parse DATE$ + "/" + TIME$, ":-/", s$(), n
'FOR i = 1 TO n: PRINT USING "####.# "; VAL(s$(i)); : NEXT i: PRINT


End Sub

Sub DecodePt()
Dim s$(40), n%, i%, Label$(8), Ct(8), r(8), rslop, T(8)
Dim A As Double, B As Double, C As Double, D As Double
  Label$(0) = "Rref 350 "
  Label$(1) = "Target 1 "
  Label$(2) = "Target 2 "
  Label$(3) = "Window   "
  Label$(4) = "Mixer    "
  Label$(5) = "Dblr Amp "
  Label$(6) = "Noise D. "
  Label$(7) = "Rref 600 "
  r(0) = 350 'rref low
  r(7) = 600  'rref hi
  A = -244.3364635: B = 0.462418: C = 0.0000588: D = -0.000000013
  Parse PtDat$, " ", s$(), n

  For i = 2 To n
    Ct(i - 2) = Val(s$(i))
  Next i
  rslop = (Ct(7) - Ct(0)) / (r(7) - r(0))
  For i = 1 To 6
    r(i) = 350 + (Ct(i) - Ct(0)) / rslop
    T(i) = A + B * r(i) + C * r(i) ^ 2 + D * r(i) ^ 3
    TPt(i) = T(i)
  Next i

  lstMuxPt.Clear
  Parse PtDat$, " ", s$(), n
  lstMuxPt.AddItem "Channel  Counts  Ohms  Temp"
  For i = 0 To 8
    If Ct(i) = 16383 Then
        lstMuxPt.AddItem Label$(i) + Format$(Ct(i), " 00000") + "  N/A  "
    Else
        lstMuxPt.AddItem Label$(i) + Format$(Ct(i), " 00000") + Format$(r(i), " 000.00") + Format$(T(i), " +00.00; -00.00")
    End If
  Next i

End Sub

