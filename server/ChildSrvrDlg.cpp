// ChildSrvrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "ChildSrvrDlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildSrvrDlg dialog


CChildSrvrDlg::CChildSrvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChildSrvrDlg::IDD, pParent), m_ipc(LC_IPC_NAME, 1024)
{
	//{{AFX_DATA_INIT(CChildSrvrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_bDialogInitialized = false;
  m_nTimeout = 10;
}


void CChildSrvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildSrvrDlg)
	DDX_Control(pDX, IDC_TXT_CHILDLOG, m_txtChildLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildSrvrDlg, CDialog)
	//{{AFX_MSG_MAP(CChildSrvrDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildSrvrDlg message handlers

//-------------------------------------------------------------------------------------------
//
void CChildSrvrDlg::PrintMessageParent(LPCTSTR Format, ...)
{

}

//-------------------------------------------------------------------------------------------
//
// Prints a message into a given text box
//
//
void CChildSrvrDlg::PrintMessage(LPCTSTR Format, ...)
{
  TCHAR Buf[1024], Buf2[1024];
  va_list marker;

  va_start(marker, Format);
  _vstprintf(Buf, Format, marker);
  FixMultiLine(Buf, Buf2);

  _tcscat(Buf2, _T("\r\n"));

  int nLen = m_txtChildLog.GetWindowTextLength();
  m_txtChildLog.SetSel(nLen, nLen, TRUE);
  m_txtChildLog.ReplaceSel(Buf2);
}

//-------------------------------------------------------------------------------------------
// Handles on window resize message
//
//
void CChildSrvrDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
  if (!m_bDialogInitialized)
    return;  

  RECT r;
  GetWindowRect(&r);
  m_txtChildLog.SetWindowPos(this, 0, 0, cx, cy, SWP_NOZORDER);	
}

//--------------------------------------------------------------------------------------------
//
//
//
void CChildSrvrDlg::RunChildServer(tChildServerStartupInfo *cssi)
{
  CChildSrvrDlg dlg;

  dlg.m_cssi = *cssi;
  dlg.DoModal();
}

//----------------------------------------------------------------------------
BOOL CChildSrvrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_bDialogInitialized = true;

  if (m_cssi.isocket == 0)
  {
    if (!m_ipc.InterConnect())
    {
      PrintMessage(_T("Could not InterConnect\n"));
      return TRUE;
    }
    else
    {
      m_ipc.ReceiveBuffer(&m_cssi.socket, sizeof(SOCKET));
      PrintMessage(_T("InterConnection success, handle received = %ld\n"), m_cssi.socket);
      //CloseHandle((HANDLE)m_cssi.socket);
    }
  }

	SetWindowText(FormatString(_T("Serving socket: %d"), m_cssi.isocket));

  if (!_beginthread(ChildApiTransportServerThreadProc, NULL, (void *)this))
    PrintMessage(_T("Could not create serving thread"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CChildSrvrDlg *g_dlg;

void CChildSrvrDlg::ChildApiTransportServerThreadProc(void *param)
{
  bool rc = true;

  CChildSrvrDlg *dlg = (CChildSrvrDlg *) param;

  g_dlg = dlg;
  tRemoteApiSession s;

  s.sd = dlg->m_cssi.socket;

  while (processServerCommunication(s));

  //dlg->PrintMessage("WSAGetLastError()=%ld\n", WSAGetLastError());
  dlg->PrintMessage(_T("Finished serving..."));
  dlg->SetForegroundWindow();
  dlg->SetTimer(ID_TIMER_CHILD_AUTOCLOSE, 1000, NULL);
  return;
}



void CChildSrvrDlg::OnTimer(UINT nIDEvent) 
{
  switch (nIDEvent)
  {
    case ID_TIMER_CHILD_AUTOCLOSE:
      m_nTimeout--;
      if (m_nTimeout < 0)
      {
        KillTimer(ID_TIMER_CHILD_AUTOCLOSE);
        EndDialog(IDOK);
      }
      SetWindowText(FormatString(_T("Closing in %d second(s)"), m_nTimeout));
    break;
  }
	CDialog::OnTimer(nIDEvent);
}
