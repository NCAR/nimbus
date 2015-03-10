Attribute VB_Name = "Formatting"
Option Explicit

Sub DateTtoStrings(DandT As Date, YYYYMMDD As String, HHMMSS As String, UTsec&, Separators As Boolean)
Dim Iyear%, Imonth%, Iday%, Ihour%, Imin%, Isec%
' Convert DandT as Date to YYYYMMDD$ and HHMMSS$
  Iyear = DatePart("yyyy", DandT)
'  Idoy = DatePart("y", DandT)
  Imonth = DatePart("m", DandT)
  Iday = DatePart("d", DandT)
  If Separators Then
    YYYYMMDD$ = Format(Iyear, "0000") + "-" + Format(Imonth, "00") + "-" + Format(Iday, "00")
  Else
    YYYYMMDD$ = Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")
  End If
  Ihour = DatePart("h", DandT)
  Imin = DatePart("n", DandT)
  Isec = DatePart("s", DandT)
  If Separators Then
    HHMMSS$ = Format(Ihour, "00") + ":" + Format(Imin, "00") + ":" + Format(Isec, "00")
    UTsec = fTstringToSec(HHMMSS$, True)
  Else
    HHMMSS$ = Format(Ihour, "00") + Format(Imin, "00") + Format(Isec, "00")
    UTsec = fTstringToSec(HHMMSS$, False)
  End If
End Sub
Function fTstamp2(Abbreviate As Boolean, IncludeTime As Boolean) As String
Dim i%, j%, A$, M%, D$, b$
' Write timestamp as either:
' January 1, 2008 12:12:12 AM or
' Jan 1, 2008 12:12:12 AM depending on whether Abbreviate is FALSE or TRUE
' with option to drop time part if IncludeTime is FALSE
  D$ = Date
  i = InStr(D$, "/")
  j = InStr(i, D$, "/")
  A$ = MonthName(Val(Left$(D$, i - 1)), Abbreviate)
  b$ = Mid$(D$, i + 1, j)
  A$ = A$ + " " + b$ + ", " + Right$(D$, 4) + " "
  If IncludeTime Then
    b$ = Time
    fTstamp2 = A$ + b$
  Else
    fTstamp2 = A$
  End If
  
End Function
Function fDelimiterCharacter2(ByVal Char$, ByVal Delim$) As Boolean
' Set TRUE is Char is in Delim$
  
  If InStr(Delim$, Char$) > 0 Then
    fDelimiterCharacter2 = True
  Else
    fDelimiterCharacter2 = False
  End If
 
End Function

Function fYYYYMMDD(DateVar As Date) As String
' Enter with Date variable and return date string
Dim iY%, iM%, iD%
  iY = Year(DateVar)
  iM = Month(DateVar)
  iD = Day(DateVar)
  fYYYYMMDD = Format(iY, "0000") + Format(iM, "00") + Format(iD, "00")
End Function

Sub Parse(ByRef StrgIn$, ByRef Par$, ByRef s$(), ByRef n%)
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
Dim Delim%, CharsFound%, C$, LastC$, i%

  For i = 0 To 40: s$(i) = "": Next  'CLEAR OUT OLD MATRIX
  n = 1
  Delim = 1
  CharsFound = 0

  For i = 1 To Len(StrgIn$)
    C$ = Mid$(StrgIn$, i, 1)
    If InStr(Par$, C$) > 0 Then   'Is this character a delimiter?
      If (C$ = ",") And (LastC$ = ",") Then Delim = 0: n = n + 1 'Yes
      Delim = 1
      LastC$ = C$
      n = n + 1
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

Sub CopyFolder(FromPath$, ToPath$)
Dim Filename$
  
  If Len(Dir(FromPath$, vbDirectory)) = 0 Then
    MsgBox FromPath$ + " does not exist!", vbOKOnly
    Exit Sub
  End If
' Make sure ToPath exist
  CreateDirectoryTree ToPath$, False
  
  Filename = Dir(FromPath) ' Retrieve the first entry.
  Do While Filename <> "" ' Start the loop.
    FileCopy FromPath$ + Filename, ToPath$ + Filename
    Filename = Dir
  Loop
  
End Sub

Sub DirCheck(DirPath$, Folder$)
Dim ans As Variant

  If Not fDirExists(DirPath$, Folder$) Then
    ans = MsgBox("Path: " + DirPath$ + Folder$ + "\" + " does not exist!" + vbLf + vbCrLf + "Do you wish to create it?", vbYesNo)
    If ans = vbYes Then
      CreateDirectoryTree DirPath$ + Folder$, False
    Else
      Exit Sub
    End If
  End If
  
End Sub

Function fGetPathToDot(ByVal File$) As String
Dim L%, Char$
' Get filename up to last ., excluding it
L = Len(File$)
Do
  Char$ = Mid$(File$, L, 1)
  If Char$ = "." Then Exit Do
  L = L - 1
Loop Until L = 0
If L > 0 Then
  fGetPathToDot = Left$(File$, L - 1)
Else
  fGetPathToDot = ""
End If
End Function
Function fGetExtension(ByVal File$) As String
Dim L%, Char$
' Get filename up to last ., excluding it
  L = Len(File$)
  Do
    Char$ = Mid$(File$, L, 1)
    If Char$ = "." Then Exit Do
    L = L - 1
  Loop Until L = 0
  If L > 0 Then
    fGetExtension = Right$(File$, Len(File$) - L)
  Else
    fGetExtension = ""
  End If
End Function
Sub FindDifference(A$, b$, C$, i%)
Dim nA%, nb%, nC%

  If b$ = "" Then b$ = A$  'Handle first pass
  Select Case i
  Case 1
'AD106:22:32:55 +34.480 -136.900  013 +00.0 +00.0 01100 01100 +17.1 0110   0270 +180.0 1 000 041129 223255
    C$ = b$
    
  Case 2, 3
'B 07317 06788 06536 07334 06805 06559 07320 06788 06545 07323 06787 06538 07316 06767 06528
'C 07318 06774 06531 07318 06775 06541 07323 06773 06541 07315 06777 06520 07306 06766 06523
' 123456123456
    If i = 2 Then C$ = "B" Else C$ = "C"
    For i = 1 To 15
      nA = Val(Mid$(A$, (i - 1) * 6 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 6 + 2, 6))
      nC = nA - nb
      C$ = C$ + fInteger(nC, 6)
    Next i
  
  Case 4
'     Tnd   Tamp   Tsyn   Tmix  Ttgt1  Ttgt2   Acc+   Twin   Tmot  Spare   Vref  Tdat1  Tdat2  Vcc/2  15V/n Tplate   Acc-
'd +40.58 +46.92 +32.80 +41.23 +26.24 +26.58 +01.02 +07.19 +36.48 +01.96 +02.04 +34.33 +40.81 +02.48 +01.90 +41.67 +01.02
' 12345671234567
    C$ = "D"
    For i = 1 To 17
      nA = Val(Mid$(A$, (i - 1) * 7 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 7 + 2, 6))
      nC = nA - nb
      If nC < 0 Then
        C$ = C$ + " " + Format(nC, "-00.00")
      Else
        C$ = C$ + " " + Format(nC, "+00.00")
      End If
    Next i

  Case 6
'E 13825 12345 09650 07339 06812 06567
    C$ = "E"
    For i = 1 To 6
      nA = Val(Mid$(A$, (i - 1) * 6 + 2, 6))
      nb = Val(Mid$(b$, (i - 1) * 6 + 2, 6))
      nC = nA - nb
      C$ = C$ + fInteger(nC, 6)
    Next i

  End Select

End Sub

Function fInteger$(n%, Count%)
Dim AbsN$, StrN$, Nspace%

  AbsN$ = Trim(Abs(n))
  StrN$ = Trim(Str(AbsN))
  Nspace = Count - Len(AbsN)
  If n < 0 Then
    fInteger$ = Space(Nspace - 1) + "-" + StrN$
  Else
    fInteger$ = Space(Nspace - 1) + "+" + StrN$
  End If
  
End Function

Function fDirExists(DirPath$, SearchName$) As Boolean
Dim DirName$

  DirName = Dir(DirPath, vbDirectory) ' Retrieve the first entry.
  Do While DirName <> "" ' Start the loop.
  ' Ignore the current directory and the encompassing directory.
    If DirName = SearchName$ Then
      fDirExists = True
      Exit Function               ' SearchName$ already exists
    End If
    DirName = Dir                 ' Get next entry.
  Loop
  fDirExists = False              ' Failed to find SearchName$

End Function

Function fAC(ByVal AC$) As String
' Get default mission directory name for a given platform
  Select Case AC$
  Case "DC": fAC = AC$ + "8"
  Case "ER": fAC = AC$ + "2"
  Case "WB": fAC = AC$ + "57"
  Case "EL": fAC = AC$ + "EC"
  Case "M5": fAC = AC$ + "5"
  Case "NG": fAC = AC$ + "V"
  Case "HA": fAC = AC$ + "LO"
  Case "GH": fAC = AC$
  End Select

End Function

Function fDate(ByVal YYYYMMDD$) As Date
' Convert yyyymmdd$ to a Date data type

  fDate = DateValue(Mid$(YYYYMMDD$, 5, 2) + "/" + Right$(YYYYMMDD$, 2) + "/" + Left$(YYYYMMDD$, 4))

End Function

Function fTime(ByVal HHMMSS$)
' Convert hhmmss$ to a Date data type

  fTime = DateValue(Left$(HHMMSS$, 2) + ":" + Mid$(HHMMSS$, 3, 2) + ":" + Right$(HHMMSS$, 2))

End Function

Function fDelimiterCharacter(ByVal Char$) As Boolean
' Set TRUE is Char is Tab, Carriage Return, Linefeed, Comma, Space or Colon
  
  Select Case Char$
  Case vbTab, vbCr, vbLf, ",", " ", ":": fDelimiterCharacter = True
  Case Else: fDelimiterCharacter = False
  End Select
  
End Function


Function fIsNumber(ByVal X$) As Boolean
' Check if x$ contains only 0-9, +, - or .
' +=43 -=45 .=46 0=48 9=57
Dim i%, n%

  For i = 1 To Len(X$)
    n = Asc(Mid$(X$, i, 1))
  
    If n < 43 Or n > 57 Or n = 44 Or n = 47 Then '0 thru 9
      fIsNumber = False
      Exit Function
    End If
  Next i
  fIsNumber = True

End Function

Function fMMMtoNUM(MMM$)
' Convert 3-letter month to number
  Select Case MMM$
  Case "JAN": fMMMtoNUM = 1
  Case "FEB": fMMMtoNUM = 2
  Case "MAR": fMMMtoNUM = 3
  Case "APR": fMMMtoNUM = 4
  Case "MAY": fMMMtoNUM = 5
  Case "JUN": fMMMtoNUM = 6
  Case "JUL": fMMMtoNUM = 7
  Case "AUG": fMMMtoNUM = 8
  Case "SEP": fMMMtoNUM = 9
  Case "OCT": fMMMtoNUM = 10
  Case "NOV": fMMMtoNUM = 11
  Case "DEC": fMMMtoNUM = 12
  End Select
  
End Function

Function formats(ByVal X!, ByVal f$) As String
' Format a number x using the format f$ so that
' negative numbers have minus sign (they would anyway) and
' positive numbers and zero have a plus sign

  If X < 0 Then
    formats = "-" + Format(Abs(X), f$)
  Else
    formats = "+" + Format(X, f$)
  End If

End Function


Sub CenterChildForm(ParentForm As Form, ChildForm As Form)
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Name       : CenterChildForm
' Purpose    : Used for centering one form on another form
' Parameters : the Parent form and the form you would like to center
' Return val : NA
' Algorithm  : moves the form to center of screen after calculating position
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Dim iLeft   As Integer
Dim iTop    As Integer

  iLeft = ParentForm.Left + (ParentForm.Width - ChildForm.Width) / 2
  iTop = ParentForm.Top + (ParentForm.Height - ChildForm.Height) / 2
  If (iLeft < 0) Then iLeft = 0
  If (iTop < 0) Then iTop = 0
  ChildForm.Move iLeft, iTop

End Sub

Function fACext(ByVal AC$) As String
' Get default filename extension for a specified platform

  fACext = "." + fAC(AC$)
  If Mission$ = "TexAQS" Then fACext = ".ELEC"  'DC8 instrument was on NCAR Electra
  
End Function

Function fExtension(ByVal File$) As String
Dim i%, j%
' Get File$ extension without "."

  fExtension = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "." Then
      fExtension = Right$(File$, j - i)
      Exit Function
    End If
  Loop

End Function


Function fDegToDMS(ByVal Deg!) As String
Dim D%, M%, s%, A!, pm%
' Convert decimal degrees to SDDD AM AS
  
  pm = Sgn(Deg)
  A = Abs(Deg)
  D = Int(A)
  s = Int((A - D) * 3600 + 0.5)
  M = Int(s / 60)
  s = Int(s - 60 * M)
  fDegToDMS$ = Format$(pm * D, "####") + Format$(M, " 00") + Format$(s, " 00")
End Function

Function fDeSpace(ByVal X$) As String
' Get rid of trailing spaces

If Asc(Left$(X$, 1)) <> 0 Then
  If InStr(1, X$, " ", 1) > 0 Then
    fDeSpace$ = Left$(X$, InStr(1, X$, " ", 1) - 1)
  End If
Else
  fDeSpace$ = ""
End If

End Function

Function fGetPath(ByVal File$) As String
' Get path to last "/" or "\"
Dim i%, j%

  fGetPath = ""
  j = Len(File$)
  i = j + 1
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "\" Or Mid$(File$, i, 1) = "/" Then
      fGetPath = Left$(File$, i)
      Exit Function
    End If
  Loop Until i = 1

End Function
Function fGetFilename(ByRef File$) As String
' Get Filename from last "/" or "\"
Dim i%, j%
  
  fGetFilename = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "\" Or Mid$(File$, i, 1) = "/" Then
      fGetFilename = Mid$(File$, i + 1, Len(File$))
      Exit Function
    End If
  Loop Until i = 1
  
End Function
Function fRemoveCharacter(ByVal A$, ByVal character$) As String
Dim i%, b$, C$, Length%, out$, LastCharacter$
' Remove the character in character$ from A$

  Length = Len(A$)
  out$ = ""
  LastCharacter = ""
  For i = 1 To Length
    b$ = Mid$(A$, i, 1)
    If b$ <> character$ Then out$ = out$ + b$
  Next i
  fRemoveCharacter = out$

End Function

Function fRemoveRepeatCharacters(ByVal A$, ByVal character$) As String
Dim i%, b$, C$, Length%, out$, LastCharacter$
' Remove any multiple occurances of character$ from A$

  Length = Len(A$)
  out$ = ""
  LastCharacter = ""
  For i = 1 To Length
    b$ = Mid$(A$, i, 1)
    If b$ = character$ Then
      If b$ <> LastCharacter Then out$ = out$ + b$
    Else
      out$ = out$ + b$
    End If
    LastCharacter = b$
  Next i
  fRemoveRepeatCharacters = out$

End Function

Function fReplaceExtension(ByVal File$, ByVal Ext$) As String
' Replace extension on File$ with Ext$, do not include "."
Dim i%, j%

  fReplaceExtension = ""
  j = Len(File$)
  i = j
  Do
    i = i - 1
    If Mid$(File$, i, 1) = "." Then
      fReplaceExtension = Left$(File$, i) + Ext$
      Exit Function
    End If
  Loop Until i = 1

End Function


Function fTstamp() As String
' Generate time stamp in format: yyyymmdd_hhmmss
Dim ymd$, hms$

  ymd$ = Right$(Date$, 4) + Left$(Date$, 2) + Mid$(Date$, 4, 2)
  hms$ = Time$
  hms$ = Left$(hms$, 2) + Mid$(hms$, 4, 2) + Right$(hms$, 2)
  fTstamp = ymd$ + "_" + hms$

End Function

Function fFullFileName(ByVal Prefix$) As String
' Assumes Drive$, Rdir$, Mission$, AC$, yyyymmdd$ are known
' Generates: "C:\M55\EUPLEX\20030211\TD\TD20030211.M55" type filename
' where Prefix$="TD"
Dim Extension$

  fFullFileName = MNpath$ + Prefix$ + "\" + Prefix$ + YYYYMMDD$ + fACext(AC$)

End Function

Function fUsing2(ByVal fmt$, ByRef V!()) As String
Dim L%, M%, i%, j%, k%, f$, out$, n$
  M% = 1
  L% = Len(fmt$)
  i% = InStr(1, fmt$, "#", 1) ' find first field start =#
  out$ = Space(i% - 1)        ' save leading spaces before # if any
  
  Do
    j% = InStr(i%, fmt$, ",", 1)   'find next field location (delimited by comma)
    If j% = 0 Then j% = L% + 1     'none if end of fmt$
    k% = j% - i%                   'number of character in current field
    f$ = Mid$(fmt$, i%, k%)
    n$ = Format$(V(M%), f$)        'format number
    M% = M% + 1
    out$ = out$ + Space(k% - Len(n$)) + "," + n$  ' add spaces so field has correct length
    i% = InStr(j%, fmt$, "#", 1)  'Get next field
    If i% <> 0 Then out$ = out$ + Space(i% - j%)
  Loop Until i% = 0
  fUsing2$ = out$
End Function

Function fUsingN(ByVal fmt$, ByVal X!, ByVal n%) As String
Dim A$
' Format x using fmt$ and add leading spaces to make total length n

  A$ = Format(X!, fmt$)
  If Len(A$) < n% Then A$ = Space(n - Len(A$)) + A$
  fUsingN = A$

End Function


Function fYMDtoDOY(ByVal Year%, ByVal Month%, ByVal Day%) As Integer
' Given year, month, and day, returns the day of the year (DOY).

  Day = Day + (Month - 1) * 30# + Int((Month + 1) * 0.61) - 2
  If (Month <= 2) Then
    Day = Day + Month
  Else
    If (Not fLeap_Year(Year)) Then Day = Day - 1
  End If
  fYMDtoDOY = Day

End Function

Function fYYYYMMDDtoDOY(ByVal YYYYMMDD$) As Integer
Dim Year%, Month%, Day%
' Convert yyyymmdd$ to DOY
  
  Year = Left$(YYYYMMDD$, 4)
  Month = Mid$(YYYYMMDD$, 5, 2)
  Day = Right$(YYYYMMDD$, 2)
  fYYYYMMDDtoDOY = fYMDtoDOY(Year, Month, Day)

End Function

Function fLeap_Year(ByVal Year%) As Boolean
'  This routine returns TRUE if the year is a leap year.
'  Otherwise FALSE is returned.

  If (Year Mod 4 = 0 And Year Mod 100 <> 0) Or (Year Mod 400 = 0) Then
    fLeap_Year = True
  Else
    fLeap_Year = False
  End If

End Function

Sub ListBoxRemoveItem(L As ListBox, Remove As Boolean)
' Generic routine to remove items from a list box and select next item
Dim i%

  Select Case L.SelCount
  Case Is < 1                 'Empty list
  
  Case 1                      'Remove single item
    If Remove Then
      i = L.ListIndex
      L.RemoveItem (L.ListIndex)
      If i >= 0 And i < L.ListCount - 1 Then
        L.Selected(i) = True
      Else
        If L.ListCount > 0 Then L.Selected(L.ListCount - 1) = True
      End If
    Else                      'Keep selected item
      i = -1
      Do
        i = i + 1
        If Not L.Selected(i) Then L.RemoveItem (i): i = i - 1
      Loop Until i = L.ListCount - 1
    End If
    
  Case Is > 1                  'Keep or Remove multiple items
    i = -1
    If Remove Then
      Do
        i = i + 1
        If L.Selected(i) Then L.RemoveItem (i): i = i - 1
      Loop Until i = L.ListCount - 1
    Else
      Do
        i = i + 1
        If Not L.Selected(i) Then L.RemoveItem (i): i = i - 1
      Loop Until i = L.ListCount - 1
    End If
  End Select

End Sub

Sub ParseString(ByVal A$, ByRef n%, ByRef b$())
Dim i%, j%, L%, C$, iStart%
' Fill array B$ with n% delimited strings (Tab, CR, LF, "  ", ":") in A$
  L% = Len(A$)
  i% = 0
  n% = 0
  Do While i <= L
    i% = i% + 1
    j% = i% - 1
    Do
      j% = j% + 1
      C$ = Mid$(A$, j, 1)
    Loop Until fDelimiterCharacter(C$) Or j = L
    If j = L Then Exit Sub
    
    C$ = Mid$(A$, i, j - i)
    If j > i And IsNumeric(C$) Then
      n = n + 1
      b$(n) = C$
    End If
    i = j
  Loop

End Sub

Sub ParseString2(ByVal A$, ByVal Delim$, ByRef b$(), ByRef n%)
Dim i%, j%, L%, C$, iStart%
' Fill array B$ with n% delimited numberic only strings, Delim$ in A$
  L% = Len(A$)
  i% = 0
  n% = 0
  Do While i < L
    i% = i% + 1
    j% = i% - 1
    Do
      j% = j% + 1
      C$ = Mid$(A$, j, 1)
    Loop Until fDelimiterCharacter2(C$, Delim$) Or j = L
    ' If j=L need to add last character, which is normally a delimiter
    If j = L Then
      C$ = Mid$(A$, i, j - i + 1)
    Else
      C$ = Mid$(A$, i, j - i)
    End If
    ' Only save numeric strings
    If j > i And IsNumeric(C$) Then
      n = n + 1
      b$(n) = C$
    End If
    i = j
  Loop

End Sub

Sub RgbToHsv(ByVal r%, ByVal g%, ByVal b%, ByRef H%, ByRef s%, ByRef V%)
' Convert RGB to HSV values
Dim vRed!, vGreen!, vBlue!
Dim Mx!, Mn!, Va!, Sa!, rc!, gc!, bc!

  vRed = r / 255
  vGreen = g / 255
  vBlue = b / 255

  Mx = vRed
  If vGreen > Mx Then Mx = vGreen
  If vBlue > Mx Then Mx = vBlue

  Mn = vRed
  If vGreen < Mn Then Mn = vGreen
  If vBlue < Mn Then Mn = vBlue

  Va = Mx
  If Mx Then
    Sa = (Mx - Mn) / Mx
  Else
    Sa = 0
  End If
  If Sa = 0 Then
    H = 0
  Else
    rc = (Mx - vRed) / (Mx - Mn)
    gc = (Mx - vGreen) / (Mx - Mn)
    bc = (Mx - vBlue) / (Mx - Mn)
    Select Case Mx
    Case vRed:   H = bc - gc
    Case vGreen: H = 2 + rc - bc
    Case vBlue:  H = 4 + gc - rc
    End Select
    H = H * 60
    If H < 0 Then H = H + 360
  End If

  s = Sa * 100
  V = Va * 100
End Sub


Function fSecToTstring(hms&, ColonFlag As Boolean) As String
Dim hr!, Min!, sec!, D&, T$, C$
' Convert time in seconds to  HHMMSS string with colon if colon.flag% is TRUE
  
  hr = Int(hms / 3600)
  Min = Int((hms - 3600 * hr) / 60)
  sec = Int(hms - 3600 * hr - 60 * Min + 0.5)
  If sec = 60 Then sec = 0: Min = Min + 1
  If Min = 60 Then Min = 0: hr = hr + 1
  If ColonFlag Then C$ = ":" Else C$ = ""
  D = 1000000 + 10000 * hr + 100 * Min + sec   'Avoid loosing leading zeros!
  T$ = Str$(D): T$ = Right$(T$, 6)
  fSecToTstring$ = Left$(T$, 2) + C$ + Mid$(T$, 3, 2) + C$ + Right$(T$, 2)

End Function

Function fUsing(ByVal fmt$, ByRef V!()) As String
Dim L%, M%, i%, j%, k%, f$, out$, n$, comma$, k1%
' Generate a string using the format in fmt$ and the order parameters in V()
  
  M% = 1
  L% = Len(fmt$)
  i% = InStr(1, fmt$, "#", 1) ' find first field start =#
  out$ = Space(i% - 1)        ' save leading spaces before # if any
  
  Do
    j% = InStr(i%, fmt$, " ", 1)   'find next field location (delimited by space)
    If j% = 0 Then j% = L% + 1     'none if end of fmt$
    k% = j% - i%                   'number of character in current field
    f$ = Mid$(fmt$, i%, k%)
    If Right$(f$, 1) = "," Then
      comma$ = ","
      k1 = k - 1
      f$ = Left(f$, k1)
    Else
      k1 = k
      comma$ = ""
    End If
    If V(M) <> -9999 Then
      n$ = Format$(V(M%), f$)        'format number
    Else
      n$ = "-9999"
    End If
    M% = M% + 1
    If k1 - Len(n$) >= 0 Then
      out$ = out$ + Space(k1% - Len(n$)) + n$  ' add spaces so field has correct length
    Else
      out$ = out$ + n$
    End If
    out$ = out$ + comma$
    i% = InStr(j%, fmt$, "#", 1)   'Get next field
    If i% <> 0 Then out$ = out$ + Space(i% - j%)
  Loop Until i% = 0
  fUsing$ = out$

End Function

Function fUsingCSV(ByVal fmt$, ByRef V!()) As String

End Function

Function fNumeric(ByVal X$) As Boolean
Dim i%, n%
' Checks if x$ is an unsigned integer number

  For i = 1 To Len(X$)
    n = Asc(Mid$(X$, i, 1))
    If n < 48 Or n > 57 Then  '0 thru 9
      fNumeric = False
      Exit Function
    End If
  Next i
  fNumeric = True
  
End Function


Function fTstringToSec(ByVal T$, ByVal ColonFlag As Boolean) As Long
Dim i%, hr%, Min%, sec%
'Convert a time string (T$) to seconds
'If Colon_Flag is TRUE, T$ format is assumed to be hh:mm:ss; otherwise, hhmmss
  
  If ColonFlag Then i% = 3 Else i% = 2
  hr = Val(Mid$(T$, 1, 2))
  Min = Val(Mid$(T$, 1 + i%, 2))
  sec = Val(Mid$(T$, 1 + 2 * i%, 2))
  fTstringToSec& = 3600# * hr + 60# * Min + sec

End Function

Sub SameFormat(ByVal OUTlu%, ByVal fmt$, ByVal Repeat%, ByRef Value!(), ByVal n%)
Dim L%, i%, X$, V!(1 To 20), j%
' Write n% values in Value!() to OUTlu% Repeat%/line using the format fmt$ for each value
  j = n% Mod Repeat%
  For L = 1 To n% - j Step Repeat%
    X$ = ""
    For i = 1 To Repeat%
      X$ = X$ + fmt$
      V(i) = Value(L + i - 1)
      'Debug.Print L; i; v(i)
    Next i
    Print #OUTlu, fUsing(X$, V())
  Next L
  
' Write leftovers
  If j > 0 Then
    X$ = ""
    For i = 1 To j
      X$ = X$ + fmt$
      V(i) = Value(n% - j + i)
    Next i
    Print #OUTlu, fUsing(X$, V())
  End If

End Sub

Sub SameFormat2(LB As ListBox, ByVal fmt$, ByVal Repeat%, ByRef Value!(), ByVal n%)
Dim L%, i%, X$, V!(1 To 20), j%
' Write n% values in Value!() to OUTlu% Repeat%/line using the format fmt$ for each value
  j = n% Mod Repeat%
  For L = 1 To n% - j Step Repeat%
    X$ = ""
    For i = 1 To Repeat%
      X$ = X$ + fmt$
      V(i) = Value(L + i - 1)
      'Debug.Print L; i; v(i)
    Next i
    LB.AddItem fUsing(X$, V())
  Next L
  
' Write leftovers
  If j > 0 Then
    X$ = ""
    For i = 1 To j
      X$ = X$ + fmt$
      V(i) = Value(n% - j + i)
    Next i
    LB.AddItem fUsing(X$, V())
  End If

End Sub

Sub HsvToRgb(ByVal H%, ByVal s%, ByVal V%, ByRef r%, ByRef g%, ByRef b%)
' Convert HSV to RGB values
Dim Sa!, Va!, Hue!, i%, f!, P!, q!, T!

  Sa = s / 100
  Va = V / 100
  If s = 0 Then
    r = Va
    g = Va
    b = Va
  Else
    Hue = H / 60
    If Hue = 6 Then Hue = 0
    i = Int(Hue)
    f = Hue - i
    P = Va * (1 - Sa)
    q = Va * (1 - (Sa * f))
    T = Va * (1 - (Sa * (1 - f)))
    Select Case i
    Case 0
      r = Va
      g = T
      b = P
    Case 1
      r = q
      g = Va
      b = P
    Case 2
      r = P
      g = Va
      b = T
    Case 3
      r = P
      g = q
      b = Va
    Case 4
      r = T
      g = P
      b = Va
    Case 5
      r = Va
      g = P
      b = q
    End Select
  End If
    
  r = Int(255.9999 * r)
  g = Int(255.9999 * g)
  b = Int(255.9999 * b)

End Sub

Function fDOYtoYMDold(Iyear%, Doy%) As String
'       SUBROUTINE W3FS26(JLDAYN, Iyear, Month, Iday, IDAYWK, IDAYYR)
'C$$$   SUBPROGRAM  DOCUMENTATION  BLOCK
'c
'C SUBPROGRAM: W3FS26         YEAR, MONTH, DAY FROM JULIAN DAY NUMBER
'C   AUTHOR: JONES,R.E.       ORG: W342       DATE: 87-03-29
'c
'C ABSTRACT: COMPUTES YEAR (4 DIGITS), MONTH, DAY, DAY OF WEEK, DAY
'C   OF YEAR FROM JULIAN DAY NUMBER. THIS SUBROUTINE WILL WORK
'C   FROM 1583 A.D. TO 3300 A.D.
'c
'C PROGRAM HISTORY LOG:
'C   87-03-29  R.E.JONES
'C   88-07-06  R.E.JONES  CHANGE TO MICROSOFT FORTRAN 4.10
'C   90-06-11  R.E.JONES  CHANGE TO SUN FORTRAN 1.3
'C   91-03-29  R.E.JONES  CONVERT TO SiliconGraphics FORTRAN
'c
'c USAGE:  Call W3FS26(JLDAYN, Iyear, Month, Iday, IDAYWK, IDAYYR)
'c
'C   INPUT VARIABLES:
'C     NAMES  INTERFACE DESCRIPTION OF VARIABLES AND TYPES
'C     ------ --------- -----------------------------------------------
'C     JLDAYN ARG LIST  INTEGER*4 JULIAN DAY NUMBER
'c
'C   OUTPUT VARIABLES:
'C     NAMES  INTERFACE DESCRIPTION OF VARIABLES AND TYPES
'C     ------ --------- -----------------------------------------------
'C     IYEAR  ARG LIST  INTEGER*4 YEAR  (4 DIGITS)
'C     MONTH  ARG LIST  INTEGER*4 MONTH
'C     IDAY   ARG LIST  INTEGER*4 DAY
'C     IDAYWK ARG LIST  INTEGER*4 DAY OF WEEK (1 IS SUNDAY, 7 IS SAT)
'C     IDAYYR ARG LIST  INTEGER*4 DAY OF YEAR (1 TO 366)
'c
'C   REMARKS: A JULIAN DAY NUMBER CAN BE COMPUTED BY USING ONE OF THE
'C     FOLLOWING STATEMENT FUNCTIONS. A DAY OF WEEK CAN BE COMPUTED
'C     FROM THE JULIAN DAY NUMBER. A DAY OF YEAR CAN BE COMPUTED FROM
'C     A JULIAN DAY NUMBER AND YEAR.
'c
'C      IYEAR (4 DIGITS)
'c
'c JDN(Iyear, Month, Iday) = Iday - 32075
'c X + 1461 * (Iyear + 4800 + (Month - 14) / 12) / 4
'c y + 367 * (Month - 2 - (Month - 14) / 12 * 12) / 12
'c z - 3 * ((Iyear + 4900 + (Month - 14) / 12) / 100) / 4
'c
'C      IYR (4 DIGITS) , IDYR(1-366) DAY OF YEAR
'c
'c JULIAN(IYR, IDYR) = -31739 + 1461 * (IYR + 4799) / 4
'c X - 3 * ((IYR + 4899) / 100) / 4 + IDYR
'c
'C      DAY OF WEEK FROM JULIAN DAY NUMBER, 1 IS SUNDAY, 7 IS SATURDAY.
'c
'C      JDAYWK(JLDAYN) = MOD((JLDAYN + 1),7) + 1
'c
'C      DAY OF YEAR FROM JULIAN DAY NUMBER AND 4 DIGIT YEAR.
'c
'C      JDAYYR(JLDAYN,IYEAR) = JLDAYN -
'c X(-31739 + 1461 * (Iyear + 4799) / 4 - 3 * ((Iyear + 4899) / 100) / 4)
'c
'C      THE FIRST FUNCTION WAS IN A LETTER TO THE EDITOR COMMUNICATIONS
'C      OF THE ACM  VOLUME 11 / NUMBER 10 / OCTOBER, 1968. THE 2ND
'C      FUNCTION WAS DERIVED FROM THE FIRST. THIS SUBROUTINE WAS ALSO
'C      INCLUDED IN THE SAME LETTER. JULIAN DAY NUMBER 1 IS
'C      JAN 1,4713 B.C. A JULIAN DAY NUMBER CAN BE USED TO REPLACE A
'C      DAY OF CENTURY, THIS WILL TAKE CARE OF THE DATE PROBLEM IN
'C      THE YEAR 2000, OR REDUCE PROGRAM CHANGES TO ONE LINE CHANGE
'C      OF 1900 TO 2000. JULIAN DAY NUMBERS CAN BE USED FOR FINDING
'C      RECORD NUMBERS IN AN ARCHIVE OR DAY OF WEEK, OR DAY OF YEAR.
'c
'c Attributes:
'C   LANGUAGE: SiliconGraphics 3.3 FORTRAN 77
'C   MACHINE:  SiliconGraphics IRIS-4D/25
'c
'C$$$
'c
'c
'       Save
'c
Dim JLDAYN&, L&, n&, i&, j&, IDAYWK&, IDAYYR&, Month&
Select Case Iyear
Case 1997: JLDAYN = 2450448.5
Case 1998: JLDAYN = 2450813.5
Case 1999: JLDAYN = 2451178.5
Case Else
  Call MsgBox(Str$(Iyear) + " is not supported by this function!", vbOKOnly)
  Exit Function
End Select

  L = JLDAYN + 68569 + Doy + 1

  n = (4 * L) \ 146097
  L = L - (146097 * n + 3) \ 4
  i = 4000 * (L + 1) \ 1461001
  L = L - 1461 * i \ 4 + 31
  j = 80 * L \ 2447
  Iday = L - 2447 * j \ 80
  L = j \ 11
  Month = j + 2 - 12 * L
  Iyear = 100 * (n - 49) + i + L
  IDAYWK = ((JLDAYN + 1) Mod 7) + 1
  IDAYYR = JLDAYN - (-31739 + 1461 * (Iyear + 4799) / 4 - 3 * ((Iyear + 4899) / 100) / 4)
  fDOYtoYMDold = Format(Iyear, "0000") + Format(Month, "00") + Format(Iday, "00")
End Function

Function fDOYtoYMD(Iyear%, Doy%) As String
Dim LY%, Imonth%, Iday%

  If fLeap_Year(Iyear) Then LY = 1 Else LY = 0
  
  Select Case Doy
  Case Is < 32:    Iday = Doy: Imonth = 1   'Jan
  Case Is < 60 + LY: Iday = Doy - 31: Imonth = 2  'Feb
  Case Is < 91 + LY: Iday = Doy - 59 - LY: Imonth = 3 'Mar
  Case Is < 121 + LY: Iday = Doy - 90 - LY: Imonth = 4 'Apr
  Case Is < 152 + LY: Iday = Doy - 120 - LY: Imonth = 5 'May
  Case Is < 182 + LY: Iday = Doy - 151 - LY: Imonth = 6 'Jun
  Case Is < 213 + LY: Iday = Doy - 181 - LY: Imonth = 7 'Jul
  Case Is < 244 + LY: Iday = Doy - 212 - LY: Imonth = 8 'Aug
  Case Is < 274 + LY: Iday = Doy - 243 - LY: Imonth = 9 'Sep
  Case Is < 305 + LY: Iday = Doy - 273 - LY: Imonth = 10 'Oct
  Case Is < 335 + LY: Iday = Doy - 304 - LY: Imonth = 11 'Nov
  Case Else: Iday = Doy - 334 - LY: Imonth = 12 'Dec
  End Select
  
  fDOYtoYMD = Format(Iyear, "0000") + Format(Imonth, "00") + Format(Iday, "00")

End Function

Sub StringsToDate(YYYYMMDD$, HHMMSS$, DandT As Date)
Dim Iyear%, Imonth%, Iday%, Ihour%, Imin%, Isec%
' Convert YYYYMMDD$ and HHMMSS$ strings to DandT Date type
  Iyear = Left$(YYYYMMDD$, 4)
  Imonth = Mid$(YYYYMMDD$, 5, 2)
  Iday = Right$(YYYYMMDD$, 2)
  Ihour = Left$(HHMMSS$, 2)
  Imin = Mid$(HHMMSS$, 3, 2)
  Isec = Right$(HHMMSS$, 2)
  DandT = DateSerial(Iyear, Imonth, Iday) + TimeSerial(Ihour, Imin, Isec)

End Sub

Sub Wait(seconds!)
Dim T As Date
' Wait for seconds in this (lousy) routine (Avoid using!)
  T = Time
  T = T + TimeSerial(0, 0, seconds)
  Do
    DoEvents
  Loop Until Time > T
  
End Sub


Sub CreateDirectoryTree(Path$, Ask As Boolean)
Dim i%, PathSave$, ans As Variant
' Create the complete directory structure specified by Path$
' Append final slash if necessary
' If Ask = TRUE then user is prompted before creating a new folder

' Replace forward slash with backslash
  Path$ = fReplaceChar(Path$, "/", "\")
' Check if last character is "\" and add
  If Right$(Path$, 1) <> "\" Then Path$ = Path$ + "\"
  PathSave$ = Path$
' Check if path exists
  If Len(Dir(Path$)) = 0 Then
    i = 0
    Do
      i = InStr(i + 1, Path$, "\")
      If Len(Dir(Left$(Path$, i), vbDirectory)) = 0 Then
        If Ask Then
          ans = MsgBox("Path: " + Left$(Path$, i) + " does not exist!" + vbLf + vbCrLf + "Do you wish to create it?", vbYesNo)
          If ans = vbYes Then
            MkDir Left$(Path$, i)
          Else
            Exit Sub  'Don't go any further if answer is NO
          End If
        Else
          MkDir Left$(Path$, i)
        End If
      End If
    Loop Until i = Len(PathSave$)
  End If

End Sub

Function fReplaceChar(Filename$, char1$, char2$)
' Replace char1 with char2 anywhere in filename$
Dim i&

For i = 1 To Len(Filename$)
  If Mid$(Filename$, i, 1) = char1$ Then Mid$(Filename$, i, 1) = char2$
Next i
fReplaceChar = Filename$

End Function

