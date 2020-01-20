; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChatDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "client.h"
LastPage=0

ClassCount=8
Class1=CChatDlg
Class2=CChatSocket
Class3=CClientApp
Class4=CClientDlg
Class5=CClientLogin

ResourceCount=3
Resource1=IDD_CLIENT_CHAT_DIALOG
Resource2=IDD_CLIENT_LOGIN_DIALOG
Class6=CListenSocket
Class7=CPeerSocket
Class8=CClientReloginDlg
Resource3=IDD_CLIENT_MAIN_DIALOG

[CLS:CChatDlg]
Type=0
BaseClass=CDialog
HeaderFile=ChatDlg.h
ImplementationFile=ChatDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CChatDlg

[CLS:CChatSocket]
Type=0
BaseClass=CSocket
HeaderFile=ChatSocket.h
ImplementationFile=ChatSocket.cpp

[CLS:CClientApp]
Type=0
BaseClass=CWinApp
HeaderFile=Client.h
ImplementationFile=Client.cpp

[CLS:CClientDlg]
Type=0
BaseClass=CDialog
HeaderFile=ClientDlg.h
ImplementationFile=ClientDlg.cpp
Filter=D
VirtualFilter=dWC

[CLS:CClientLogin]
Type=0
BaseClass=CDialog
HeaderFile=ClientLogin.h
ImplementationFile=ClientLogin.cpp

[DLG:IDD_CLIENT_CHAT_DIALOG]
Type=1
Class=CChatDlg
ControlCount=2
Control1=IDC_CLIENT_CHAT_IPUT_EDIT,edit,1352732676
Control2=IDC_CLIENT_CHAT_OUTPUT_EDIT,edit,1352730628

[DLG:IDD_CLIENT_MAIN_DIALOG]
Type=1
Class=CClientDlg
ControlCount=1
Control1=IDC_CLIENT_LIST,SysListView32,1350647813

[DLG:IDD_CLIENT_LOGIN_DIALOG]
Type=1
Class=CClientLogin
ControlCount=12
Control1=IDC_CLIENT_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_CLIENT_NAME_EDIT,edit,1350631552
Control3=IDC_CLIENT_PASSWORD_EDIT,edit,1350639776
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_CLIENT_NAME_STATIC,static,1342308352
Control7=IDC_CLIENT_PASSWORD_STATIC,static,1342308352
Control8=IDC_CLIENT_GROUPBOX_IP,button,1342177287
Control9=IDC_CLIENT_GROUPBOX_USER,button,1342177287
Control10=IDC_CLIENT_PORT_EDIT,edit,1350639616
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:CListenSocket]
Type=0
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp
BaseClass=CSocket
Filter=N

[CLS:CPeerSocket]
Type=0
HeaderFile=PeerSocket.h
ImplementationFile=PeerSocket.cpp
BaseClass=CSocket
Filter=N
LastObject=CPeerSocket

[CLS:CClientReloginDlg]
Type=0
HeaderFile=ClientReloginDlg.h
ImplementationFile=ClientReloginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CLIENT_RELOGIN_NAME_EDIT
VirtualFilter=dWC

