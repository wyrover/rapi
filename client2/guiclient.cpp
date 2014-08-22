// guiclient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "guiclient.h"
#include "guiclientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../rapi_lib/remoteapi_client.h"

/////////////////////////////////////////////////////////////////////////////
// CGuiclientApp

BEGIN_MESSAGE_MAP(CGuiclientApp, CWinApp)
	//{{AFX_MSG_MAP(CGuiclientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiclientApp construction

CGuiclientApp::CGuiclientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGuiclientApp object

CGuiclientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGuiclientApp initialization

BOOL CGuiclientApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
  WSADATA ws;

  WSAStartup(MAKEWORD(1,1), &ws);   

	CGuiclientDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
  WSACleanup();
	return FALSE;
}
