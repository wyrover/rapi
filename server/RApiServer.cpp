// RApiServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RApiServer.h"
#include "RApiServerDlg.h"
#include "ChildSrvrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrApp

BEGIN_MESSAGE_MAP(CRApiSrvrApp, CWinApp)
	//{{AFX_MSG_MAP(CRApiSrvrApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrApp construction

CRApiSrvrApp::CRApiSrvrApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRApiSrvrApp object

CRApiSrvrApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrApp initialization

BOOL CRApiSrvrApp::InitInstance()
{
  // initialize winsocks
  WSADATA ws;
  WSAStartup(MAKEWORD(1, 1), &ws);

  tChildServerStartupInfo cssi;

  // should we be running a child server?
  if ((__argc==3) &&
      (stricmp(__argv[1], "child")==0)
      )
  {
    cssi.socket = (SOCKET) atol(__argv[2]);
    CChildSrvrDlg::RunChildServer(&cssi);
  }
  else if ( (__argc==2) &&
           (stricmp(__argv[1], "lastchild")==0)
          )
  {
    cssi.isocket = 0;
    CChildSrvrDlg::RunChildServer(&cssi);
  }
  // run the listening server
  else
  {
  	CRApiSrvrDlg dlg;
    dlg.DoModal();
  }

  // Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CRApiSrvrApp::ExitInstance()
{
  // de-initialize winsocks
  WSACleanup();
  return CWinApp::ExitInstance();
}