// RApiChildServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RApiChildServer.h"
#include "ChildSrvrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRApiChildSrvrApp

BEGIN_MESSAGE_MAP(CRApiChildSrvrApp, CWinApp)
	//{{AFX_MSG_MAP(CRApiChildSrvrApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRApiChildSrvrApp construction

CRApiChildSrvrApp::CRApiChildSrvrApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRApiChildSrvrApp object

CRApiChildSrvrApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRApiChildSrvrApp initialization

BOOL CRApiChildSrvrApp::InitInstance()
{
  // initialize winsocks
  WSADATA ws;
  WSAStartup(MAKEWORD(1, 1), &ws);

  #ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
  #else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
  #endif
   
  // should we be running a child server?
  if ((__argc==3) &&
      (stricmp(__argv[1], "child")==0)
      )
  {
    tChildServerStartupInfo cssi;
    cssi.socket = (SOCKET) atol(__argv[2]);
    CChildSrvrDlg::RunChildServer(&cssi);
  }
  else
  {
    AfxMessageBox("Invalid usage!");
  }
  // Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CRApiChildSrvrApp::ExitInstance()
{
  // de-initialize winsocks
  WSACleanup();
  return CWinApp::ExitInstance();
}
