VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "msinet.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   2505
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6555
   LinkTopic       =   "Form1"
   ScaleHeight     =   2505
   ScaleWidth      =   6555
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtHostName 
      Height          =   285
      IMEMode         =   3  'DISABLE
      Left            =   1320
      TabIndex        =   1
      Text            =   "ftp://www.mysite.com"
      Top             =   480
      Width           =   3615
   End
   Begin VB.TextBox txtUserName 
      Height          =   285
      IMEMode         =   3  'DISABLE
      Left            =   1320
      TabIndex        =   2
      Text            =   "user_name"
      Top             =   840
      Width           =   3615
   End
   Begin VB.TextBox txtPassword 
      Height          =   285
      IMEMode         =   3  'DISABLE
      Left            =   1320
      PasswordChar    =   "*"
      TabIndex        =   3
      Top             =   1200
      Width           =   3615
   End
   Begin VB.CommandButton cmdUploadFile 
      Caption         =   "Upload File"
      Height          =   375
      Left            =   2640
      TabIndex        =   5
      Top             =   2040
      Width           =   1095
   End
   Begin InetCtlsObjects.Inet inetFtp 
      Left            =   5160
      Top             =   480
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
   Begin VB.TextBox txtFromFile 
      Height          =   285
      Left            =   1320
      TabIndex        =   0
      Top             =   120
      Width           =   3615
   End
   Begin VB.TextBox txtToFile 
      Height          =   285
      Left            =   1320
      TabIndex        =   4
      Text            =   "/public_html/test.gif"
      Top             =   1560
      Width           =   3615
   End
   Begin VB.Label Label1 
      Caption         =   "Host Name"
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   10
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "User Name"
      Height          =   255
      Index           =   3
      Left            =   120
      TabIndex        =   9
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "Password"
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   8
      Top             =   1200
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "To File"
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   7
      Top             =   1560
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "From File"
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Upload the file.
Private Sub cmdUploadFile_Click()
    Screen.MousePointer = vbHourglass
    DoEvents

    If UploadFile(txtFromFile.Text, txtToFile.Text, txtHostName.Text, txtUserName.Text, txtPassword.Text) Then
        MsgBox "Upload Complete", _
            vbOKOnly Or vbInformation, _
            "Done"
    End If

    Screen.MousePointer = vbDefault
End Sub

Private Sub Form_Load()
Dim app_path As String

    app_path = App.Path
    If Right$(app_path, 1) <> "\" Then app_path = app_path & "\"
    txtFromFile.Text = app_path & "test.gif"
End Sub

Private Sub Form_Resize()
Dim wid As Single
Dim hgt As Single

    wid = ScaleWidth - txtToFile.Left - 120
    If wid < 120 Then wid = 120
    txtFromFile.Width = wid
    txtHostName.Width = wid
    txtUserName.Width = wid
    txtPassword.Width = wid
    txtToFile.Width = wid

    cmdUploadFile.Left = (ScaleWidth - cmdUploadFile.Width) / 2
End Sub

' Upload a file. Return True if we are successful.
Private Function UploadFile(ByVal source_file As String, ByVal dest_file As String, ByVal host_name As String, ByVal user_name As String, ByVal passwd As String) As Boolean
    ' Get the file's contents.
    On Error GoTo UploadError

    ' You must set the URL before the user name and
    ' password. Otherwise the control cannot verify
    ' the user name and password and you get the error:
    '
    '       Unable to connect to remote host
    If LCase$(Left$(host_name, 6)) <> "ftp://" Then host_name = "ftp://" & host_name
    inetFtp.URL = host_name
    inetFtp.UserName = user_name
    inetFtp.Password = passwd

    ' Do not include the host name here. That will make
    ' the control try to use its default user name and
    ' password and you'll get the error again.
    Debug.Print "Put " & source_file & " " & dest_file
    inetFtp.Execute , "Put " & source_file & " " & dest_file

    UploadFile = True
    Exit Function

UploadError:
    MsgBox "Error " & Err.Number & _
        " uploading file '" & _
        source_file & "' to '" & _
        dest_file & "'." & vbCrLf & Err.Description, _
        vbExclamation Or vbOKOnly, _
        "Download Error"
    UploadFile = False
    Exit Function
End Function
