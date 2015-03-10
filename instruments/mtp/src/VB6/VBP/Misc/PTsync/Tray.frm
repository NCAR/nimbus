VERSION 5.00
Begin VB.Form frmTray 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "System Tray Icon"
   ClientHeight    =   3090
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4680
   Icon            =   "Tray.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   206
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows Default
   Visible         =   0   'False
End
Attribute VB_Name = "frmTray"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Copyright 2004 Karen Kenworthy
' All Rights Reserved
' http://www.karenware.com/

Private Const WM_MOUSEMOVE = &H200
Private Const WM_LBUTTONDOWN = &H201     'Button down
Private Const WM_LBUTTONUP = &H202       'Button up
Private Const WM_LBUTTONDBLCLK = &H203   'Double-click
Private Const WM_RBUTTONDOWN = &H204     'Button down
Private Const WM_RBUTTONUP = &H205       'Button up
Private Const WM_RBUTTONDBLCLK = &H206   'Double-click
Private Const WM_MBUTTONDOWN = &H207
Private Const WM_MBUTTONUP = &H208
Private Const WM_MBUTTONDBLCLK = &H209
Private Const WM_XBUTTONDOWN = &H20B
Private Const WM_XBUTTONUP = &H20C
Private Const WM_XBUTTONDBLCLK = &H20D
Private Const WM_MOUSEHOVER = &H2A1
Private Const WM_MOUSELEAVE = &H2A3

Public HomeForm As Object
Public TrayID As Long
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim result As Long
    Dim Action As PT_MOUSE_ACTION

    If Y <> 0 Then Exit Sub      ' mouse not over system tray icon

    'the value of X will vary depending upon the scalemode setting
    If Me.ScaleMode = vbPixels Then
        Action = X
    Else
        Action = X / Screen.TwipsPerPixelX
    End If

    HomeForm.TrayEvent TrayID, Action
End Sub
