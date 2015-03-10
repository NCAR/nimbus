VERSION 5.00
Begin VB.Form frmKeyCode 
   Caption         =   "Key Codes"
   ClientHeight    =   2460
   ClientLeft      =   2955
   ClientTop       =   4695
   ClientWidth     =   3810
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   2460
   ScaleWidth      =   3810
   Begin VB.TextBox InKey 
      Height          =   285
      Left            =   1440
      TabIndex        =   2
      Top             =   630
      Width           =   255
   End
   Begin VB.Label Labe2 
      Caption         =   "Base:      10   16"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   2040
      Width           =   2985
   End
   Begin VB.Label Label5 
      Caption         =   "KeyAscii"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1680
      Width           =   2985
   End
   Begin VB.Label Label1 
      Caption         =   "Press Any Key to See its Codes"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   3645
   End
   Begin VB.Label Label4 
      Caption         =   "Shift:"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   960
      Width           =   2985
   End
   Begin VB.Label Label3 
      Caption         =   "KeyCode"
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   1320
      Width           =   2985
   End
   Begin VB.Label Label2 
      Caption         =   "Character: "
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   570
      Width           =   1125
   End
End
Attribute VB_Name = "frmKeyCode"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'KeyCode Constants
   
'The followingconstants can be used anywhere in your code in place of the actual values:

'Constant Value Description
'vbKeyLButton 0x1 Left mouse button
'vbKeyRButton 0x2 Right mouse button
'vbKeyCancel 0x3 CANCEL key
'vbKeyMButton 0x4 Middle mouse button
'vbKeyBack 0x8 BACKSPACE key
'vbKeyTab 0x9 TAB key
'vbKeyClear 0xC CLEAR key
'vbKeyReturn 0xD ENTER key
'vbKeyShift 0x10 SHIFT key
'vbKeyControl 0x11 CTRL key
'vbKeyMenu 0x12 MENU key
'vbKeyPause 0x13 PAUSE key
'vbKeyCapital 0x14 CAPS LOCK key
'vbKeyEscape 0x1B ESC key
'vbKeySpace 0x20 SPACEBAR key
'vbKeyPageUp 0x21 PAGE UP key
'vbKeyPageDown 0x22 PAGE DOWN key
'vbKeyEnd 0x23 END key
'vbKeyHome 0x24 HOME key
'vbKeyLeft 0x25 LEFT ARROW key
'vbKeyUp 0x26 UP ARROW key
'vbKeyRight 0x27 RIGHT ARROW key
'vbKeyDown 0x28 DOWN ARROW key
'vbKeySelect 0x29 SELECT key
'vbKeyPrint 0x2A PRINT SCREEN key
'vbKeyExecute 0x2B EXECUTE key
'vbKeySnapshot 0x2C SNAPSHOT key
'vbKeyInsert 0x2D INSERT key
'vbKeyDelete 0x2E DELETE key
'vbKeyHelp 0x2F HELP key
'vbKeyNumlock 0x90 NUM LOCK key


'The A key through the Z key are the same as the ASCII equivalents A – Z:

'Constant Value Description
'vbKeyA 65 A key
'vbKeyB 66 B key
'vbKeyC 67 C key
'vbKeyD 68 D key
'vbKeyE 69 E key
'vbKeyF 70 F key
'vbKeyG 71 G key
'vbKeyH 72 H key
'vbKeyI 73 I key
'vbKeyJ 74 J key
'vbKeyK 75 K key
'vbKeyL 76 L key
'vbKeyM 77 M key
'vbKeyN 78 N key
'vbKeyO 79 O key
'vbKeyP 80 P key
'vbKeyQ 81 Q key
'vbKeyR 82 R key
'vbKeyS 83 S key
'vbKeyT 84 T key
'vbKeyU 85 U key
'vbKeyV 86 V key
'vbKeyW 87 W key
'vbKeyX 88 X key
'vbKeyY 89 Y key
'vbKeyZ 90 Z key


'The 0 key through 9 key are the same as their ASCII equivalents 0 – 9:

'Constant Value Description
'vbKey0 48 0 key
'vbKey1 49 1 key
'vbKey2 50 2 key
'vbKey3 51 3 key
'vbKey4 52 4 key
'vbKey5 53 5 key
'vbKey6 54 6 key
'vbKey7 55 7 key
'vbKey8 56 8 key
'vbKey9 57 9 key


'The following constants represent keys on the numeric keypad:

'Constant Value Description
'vbKeyNumpad0 0x60 0 key
'vbKeyNumpad1 0x61 1 key
'vbKeyNumpad2 0x62 2 key
'vbKeyNumpad3 0x63 3 key
'vbKeyNumpad4 0x64 4 key
'vbKeyNumpad5 0x65 5 key
'vbKeyNumpad6 0x66 6 key
'vbKeyNumpad7 0x67 7 key
'vbKeyNumpad8 0x68 8 key
'vbKeyNumpad9 0x69 9 key
'vbKeyMultiply 0x6A MULTIPLICATION SIGN (*) key
'vbKeyAdd 0x6B PLUS SIGN (+) key
'vbKeySeparator 0x6C ENTER key
'vbKeySubtract 0x6D MINUS SIGN (–) key
'vbKeyDecimal 0x6E DECIMAL POINT (.) key
'vbKeyDivide 0x6F DIVISION SIGN (/) key


'The following constants represent function keys:

'Constant Value Description
'vbKeyF1 0x70 F1 key
'vbKeyF2 0x71 F2 key
'vbKeyF3 0x72 F3 key
'vbKeyF4 0x73 F4 key
'vbKeyF5 0x74 F5 key
'vbKeyF6 0x75 F6 key
'vbKeyF7 0x76 F7 key
'vbKeyF8 0x77 F8 key
'vbKeyF9 0x78 F9 key
'vbKeyF10 0x79 F10 key
'vbKeyF11 0x7A F11 key
'vbKeyF12 0x7B F12 key
'vbKeyF13 0x7C F13 key
'vbKeyF14 0x7D F14 key
'vbKeyF15 0x7E F15 key
'vbKeyF16 0x7F F16 key
Private Sub Form_Load()

Show

End Sub

Private Sub InKey_KeyDown(KeyCode As Integer, Shift As Integer)
InKey.Text = "" 'Clear previous character if any
'NB KeyAscii passes key to active form if KeyPreview is FALSE

Label3.Caption = "KeyCode:  " & Str$(Val(KeyCode)) & "   " & (Hex(Val(KeyCode)))
Select Case Shift
Case 0
  Label4.Caption = "Shift: "
Case 1
  Label4.Caption = "Shift:     SHIFT"
Case 2
  Label4.Caption = "Shift:     CTRL"
Case 3
  Label4.Caption = "Shift:     SHIFT+CTRL"
Case 4
  Label4.Caption = "Shift:     ALT"
Case 5
  Label4.Caption = "Shift:     SHIFT+ALT"
Case 6
  Label4.Caption = "Shift:     CTRL+ALT"
Case 7
  Label4.Caption = "Shift:     SHIFT+CTRL+ALT"
End Select
If KeyAscii = 0 Then Label5.Caption = "KeyAscii: "
End Sub



'Base       10  16
Private Sub InKey_KeyPress(KeyAscii As Integer)
Label5.Caption = "KeyAscii: " + Format(KeyAscii, "000") + "   " + (Hex(KeyAscii))
End Sub


Private Sub InKey_KeyUp(KeyCode As Integer, Shift As Integer)

End Sub

