; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildServerConfigPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "RApiServer.h"

ClassCount=10
Class1=CRApiSrvrApp
Class2=CRApiSrvrDlg
Class3=CAboutDlg

ResourceCount=10
Resource1=IDD_INPUTBOX1
Resource2=IDR_MAINFRAME
Resource3=IDD_RAPISERVER_DIALOG
Resource4=IDD_GENERALCONFIGPAGE_DIALOG
Class4=CChildSrvrDlg
Class5=CConfigurationSheet
Resource5=IDD_CONFIGSERVERPAGE_DIALOG
Class6=CConfigServerPage
Resource6=IDD_CHILDSERVERCONFIGPAGE_DIALOG
Class7=CChildServerConfigPage
Resource7=IDD_CHILDSRVRDLG_DIALOG
Class8=CGeneralConfigPage
Resource8=IDD_ABOUTBOX
Class9=CInputBox1Dlg
Resource9=IDD_DIALOGTESTS_DIALOG
Class10=CTestDialogs
Resource10=IDR_MAINMENU

[CLS:CRApiSrvrApp]
Type=0
HeaderFile=RApiServer.h
ImplementationFile=RApiServer.cpp
Filter=N
LastObject=CRApiSrvrApp

[CLS:CRApiSrvrDlg]
Type=0
HeaderFile=RApiServerDlg.h
ImplementationFile=RApiServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CRApiSrvrDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=RApiServerDlg.h
ImplementationFile=RApiServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RAPISERVER_DIALOG]
Type=1
Class=CRApiSrvrDlg
ControlCount=1
Control1=IDC_EDT_LOG,edit,1353779396

[DLG:IDD_CHILDSRVRDLG_DIALOG]
Type=1
Class=CChildSrvrDlg
ControlCount=1
Control1=IDC_TXT_CHILDLOG,edit,1353779396

[CLS:CChildSrvrDlg]
Type=0
HeaderFile=ChildSrvrDlg.h
ImplementationFile=ChildSrvrDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CChildSrvrDlg
VirtualFilter=dWC

[CLS:CConfigurationSheet]
Type=0
HeaderFile=ConfigurationSheet.h
ImplementationFile=ConfigurationSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CConfigurationSheet

[DLG:IDD_CONFIGSERVERPAGE_DIALOG]
Type=1
Class=CConfigServerPage
ControlCount=4
Control1=IDC_SERVER_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_EDIT2,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352

[CLS:CConfigServerPage]
Type=0
HeaderFile=ConfigServerPage.h
ImplementationFile=ConfigServerPage.cpp
BaseClass=CDialog
Filter=D
LastObject=CConfigServerPage

[MNU:IDR_MAINMENU]
Type=1
Class=CRApiSrvrDlg
Command1=IDOK
Command2=ID_MNU_EDIT_CLEAR
Command3=ID_MNU_EDIT_SELECT_ALL
Command4=IDM_MENU_SERVERLISTEN
Command5=ID_MNU_OPTIONS_CONFIGURATION
Command6=ID_MENUDEBUG_PASSDATASOCKETTOCHILD
Command7=IDC_MENU_TESTING
CommandCount=7

[DLG:IDD_CHILDSERVERCONFIGPAGE_DIALOG]
Type=1
Class=CChildServerConfigPage
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[CLS:CChildServerConfigPage]
Type=0
HeaderFile=ChildServerConfigPage.h
ImplementationFile=ChildServerConfigPage.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENUDEBUG_PASSDATASOCKETTOCHILD

[DLG:IDD_GENERALCONFIGPAGE_DIALOG]
Type=1
Class=CGeneralConfigPage
ControlCount=2
Control1=IDC_CHK_DBGLVL_WARNING,button,1342242819
Control2=IDC_CHK_DBGLVL_APICALLS,button,1342242819

[CLS:CGeneralConfigPage]
Type=0
HeaderFile=GeneralConfigPage.h
ImplementationFile=GeneralConfigPage.cpp
BaseClass=CDialog
Filter=D
LastObject=CGeneralConfigPage

[DLG:IDD_INPUTBOX1]
Type=1
Class=CInputBox1Dlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDT_INPUTBOXDLG,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CInputBox1Dlg]
Type=0
HeaderFile=InputBox1Dlg.h
ImplementationFile=InputBox1Dlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInputBox1Dlg

[DLG:IDD_DIALOGTESTS_DIALOG]
Type=1
Class=CTestDialogs
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_SHOW_CONFIGSHEET,button,1342242816

[CLS:CTestDialogs]
Type=0
HeaderFile=TestDialogs.h
ImplementationFile=TestDialogs.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BTN_SHOW_CONFIGSHEET
VirtualFilter=dWC

