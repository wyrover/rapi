// guiclientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guiclient.h"
#include "guiclientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "../rapi_lib/remoteapi_client.h"
#include "../rapi_lib/remotedebug_util.h"
#include ".\guiclientdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CGuiclientDlg dialog

CGuiclientDlg::CGuiclientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuiclientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuiclientDlg)
	m_strServer = _T("127.0.0.1");
	m_nPort = 1001;
	m_edtProcess = _T("c:\\temp\\notepad1.exe");
	m_edtParam = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGuiclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuiclientDlg)
	DDX_Control(pDX, IDC_LBSTATUS, m_lbStatus);
	DDX_Text(pDX, IDC_EDTSERVER, m_strServer);
	DDX_Text(pDX, IDC_EDTPORT, m_nPort);
	DDX_Text(pDX, IDC_EDTPROCESS, m_edtProcess);
	DDX_Text(pDX, IDC_EDTPARAM, m_edtParam);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGuiclientDlg, CDialog)
	//{{AFX_MSG_MAP(CGuiclientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNCONNECT, OnBtnconnect)
	ON_BN_CLICKED(IDC_BTNRUN, OnBtnrun)
	ON_BN_CLICKED(IDC_BTNSUSPEND, OnBtnsuspend)
	ON_BN_CLICKED(IDC_BTNRESUME, OnBtnresume)
	ON_BN_CLICKED(IDC_BTNTERMINATE, OnBtnterminate)
	ON_BN_CLICKED(IDC_BTNSETTHREADCONTEXT, OnBtnsetthreadcontext)
	ON_BN_CLICKED(IDC_BTNGETTHREADCONTEXT, OnBtngetthreadcontext)
	ON_BN_CLICKED(IDC_BTNDISCONNECT, OnBtndisconnect)
	ON_BN_CLICKED(IDC_BTNCLOSESERVER, OnBtncloseserver)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNSTATUSCLEAR, OnBtnstatusclear)
	ON_BN_CLICKED(IDC_BTNRESTARTSERVER, OnBtnrestartserver)
	ON_BN_CLICKED(IDC_BTN_DEBUGPROCESS, OnBtnDebugprocess)
	ON_BN_CLICKED(IDC_BTN_BENCHMARK, OnBtnBenchmark)
	ON_BN_CLICKED(IDC_BTN_FINDMODULE, OnBtnFindmodule)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BTNTERMINATE2, OnBnClickedBtnterminate2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiclientDlg message handlers

BOOL CGuiclientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGuiclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGuiclientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGuiclientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGuiclientDlg::OnBtnconnect() 
{
  UpdateData(TRUE);
  switch(RApiClient_Init(m_session, m_strServer, m_nPort))
  {
    case 1:
      m_lbStatus.AddString("Could not resolve");
      break;
    case 2:
      m_lbStatus.AddString("Could not connect");
      break;
    case 0:
      m_lbStatus.AddString("Connected to server");
      break;
  };
}

void CGuiclientDlg::OnBtnrun() 
{
	// TODO: Add your control notification handler code here
  UpdateData(TRUE);
  
  memset(&m_si, 0, sizeof(m_si));
  m_si.cb = sizeof(m_si);

  m_rc = RApiClientCreateProcess(
                        m_session,
                        m_edtProcess,
                        (m_edtParam == "") ? NULL : (LPSTR) (LPCTSTR) m_edtParam,
                        NULL,
                        NULL,
                        FALSE,
                        NORMAL_PRIORITY_CLASS,
                        GetEnvironmentStrings(),
                        NULL,
                        &m_si,
                        &m_pi);
  m_lbStatus.AddString(m_rc ? "Created process" : "Failed to create process");
}

void CGuiclientDlg::OnBtnsuspend() 
{
  RApiClientSuspendThread(m_session, m_pi.hThread);

  m_lbStatus.AddString("Suspended...");
}

void CGuiclientDlg::OnBtnresume() 
{
  RApiClientResumeThread(m_session, m_pi.hThread);	
  m_lbStatus.AddString("Resume...");
}

void CGuiclientDlg::OnBtnterminate() 
{
  RApiClientTerminateProcess(m_session, m_pi.hProcess, 0);

  m_lbStatus.AddString("Terminated...");  	
}

void CGuiclientDlg::OnBtnsetthreadcontext() 
{
	RApiClientSuspendThread(m_session, m_pi.hThread);
  OnBtngetthreadcontext();
  m_ctx.Eax++;
  m_ctx.Eip++;
  RApiClientSetThreadContext(m_session, m_pi.hThread, &m_ctx);
  OnBtngetthreadcontext();
  m_ctx.Eax--;
  m_ctx.Eip--;
  RApiClientSetThreadContext(m_session, m_pi.hThread, &m_ctx);
  OnBtngetthreadcontext();
  RApiClientResumeThread(m_session, m_pi.hThread);
}

void CGuiclientDlg::OnBtngetthreadcontext() 
{
	m_ctx.ContextFlags = CONTEXT_FULL;
  m_rc = RApiClientGetThreadContext(m_session, m_pi.hThread, &m_ctx);

  CString s;
  s.Format("EAX=%08lX ECX=%08lX EDX=%08lX ESI=%08lX EIP=%08lX ESP=%08lX",
            m_ctx.Eax, m_ctx.Ecx, m_ctx.Edx, m_ctx.Esi, m_ctx.Eip, m_ctx.Esp);

  m_lbStatus.AddString(m_rc ? s : "Failed to get Context");
}

void CGuiclientDlg::OnBtndisconnect() 
{
  RApiClient_Close(m_session);
}

void CGuiclientDlg::OnBtncloseserver() 
{
  RApiClient_CloseServer(m_session);	
}

void CGuiclientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
  RApiClient_Close(m_session);	
}

void CGuiclientDlg::OnBtnstatusclear() 
{
  m_lbStatus.ResetContent();	
}



void CGuiclientDlg::OnBtnrestartserver() 
{
  RApiClient_RestartServer(m_session);	
}

void CGuiclientDlg::OnBtnDebugprocess() 
{
  UpdateData(TRUE);

  PROCESS_INFORMATION pi;
  STARTUPINFO si;

  DWORD nbRW;
  DEBUG_EVENT de;
  CHAR buf[1024];

  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  BOOL rc;

  rc = RApiClientCreateProcess(m_session,
                        m_edtProcess,
                        "", // command line
                        NULL,
                        NULL,
                        FALSE,
                        DEBUG_PROCESS,
                        GetEnvironmentStrings(), // env
                        "C:\\", // cur dir
                        &si,
                        &pi);
  GetLastError();   
  if (!rc)
  {
    sprintf(buf,"failed to create process! (gle=%ld)", GetLastError());
    m_lbStatus.AddString(buf);
    return;
  }

  sprintf(buf,"Process ID:%08lX ThreadID:%08lX\n", pi.dwProcessId, pi.dwThreadId);
  m_lbStatus.AddString(buf);

  BOOL b=TRUE;
  DWORD gle;
  while (b)
  {
    b = RApiClientWaitForDebugEvent(m_session, &de, INFINITE);
    if (gle=GetLastError())
    {
      if (gle==0x12B)
        b = TRUE;
      sprintf(buf, "WaitForDebugEvent failed: (GLE=%d)\n", GetLastError());
      m_lbStatus.AddString(buf);
    }

    // skip all debug events that are not ours!
    if (de.dwProcessId != pi.dwProcessId)
      RApiClientContinueDebugEvent(m_session, de.dwProcessId, de.dwThreadId, DBG_CONTINUE);

    switch (de.dwDebugEventCode)
    {
      //
      //
      //
      case EXCEPTION_DEBUG_EVENT:
        sprintf(buf, "Exception: (address=%08lX)\n", de.u.Exception.ExceptionRecord.ExceptionAddress);
        m_lbStatus.AddString(buf);
        break;
      //
      //
      //
      case CREATE_THREAD_DEBUG_EVENT:
        sprintf(buf, "CreateThread: (hThread:%08lX LocalBase:%08lX StartAddress:%08lX)\n", 
                de.u.CreateThread.hThread,
                de.u.CreateThread.lpThreadLocalBase,
                de.u.CreateThread.lpStartAddress);
        m_lbStatus.AddString(buf);
        break;
      //
      //
      //
      case CREATE_PROCESS_DEBUG_EVENT:
        strcpy(buf, "?");
        if (!GetRemoteDebugeeModuleName(m_session,
                                        de.u.CreateProcessInfo.hProcess, 
                                        de.u.CreateProcessInfo.lpImageName, 
                                        de.u.CreateProcessInfo.fUnicode,
                                        &buf, 
                                        sizeof(buf)))
        {
          // ...  
        }
        sprintf(buf, "CreateProcess: (\n\tModule:%s\n\thFile:%ld)\n", 
                buf, 
                de.u.CreateProcessInfo.hFile);
        m_lbStatus.AddString(buf);
        break;
      //
      //
      //
      case OUTPUT_DEBUG_STRING_EVENT:
        if (!RApiClientReadProcessMemory(m_session, pi.hProcess, de.u.DebugString.lpDebugStringData, buf, 
                                    (de.u.DebugString.fUnicode ? 2 : 1) * de.u.DebugString.nDebugStringLength,
                                    &nbRW))
          strcpy(buf, "NULL");
        sprintf(buf, "OutDebugString(%s)\n", buf);
        m_lbStatus.AddString(buf);
        break;
      case LOAD_DLL_DEBUG_EVENT:
        strcpy(buf, "?");
        if (!GetRemoteDebugeeModuleName(m_session, pi.hProcess, 
                                        de.u.LoadDll.lpImageName, 
                                        de.u.LoadDll.fUnicode,
                                        &buf, sizeof(buf)))
        {
          //RApiClientReadProcessMemory(pi.hProcess, (LPVOID)((DWORD)de.u.LoadDll.lpBaseOfDll+0x1000), buf, sizeof(buf), &nbRW);
          //RApiClientGetModuleFileNameEx(pi.hProcess, (HMODULE)de.u.LoadDll.lpBaseOfDll, buf, sizeof(buf));
         }
        sprintf(buf, "LoadDll: (Module: %s Base:%08lX)\n", buf, de.u.LoadDll.lpBaseOfDll);         
        m_lbStatus.AddString(buf);
        {
          MEMORY_BASIC_INFORMATION mbi;
          RApiClientVirtualQueryEx(m_session, pi.hProcess, (LPVOID)0x401000, &mbi, sizeof(mbi));
        }
        break;
      case EXIT_PROCESS_DEBUG_EVENT:
        b = FALSE;
        sprintf(buf, "Process has exited (code=%d)!\n", de.u.ExitProcess.dwExitCode);
        m_lbStatus.AddString(buf);
        break;
      case UNLOAD_DLL_DEBUG_EVENT:
        sprintf(buf, "Unload dll: (Base: %08lX)\n", de.u.UnloadDll.lpBaseOfDll);
        m_lbStatus.AddString(buf);
        break;
    }
    RApiClientContinueDebugEvent(m_session, de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
  }
  RApiClientTerminateProcess(m_session, pi.hProcess, 0);
}

void CGuiclientDlg::OnBtnBenchmark() 
{
  LPVOID address = (LPVOID) 0x401000;
  DWORD dummy;
  DWORD val;
  int i;
  char buf[100];

  UpdateData(TRUE);

  int nInstructions = atoi(m_edtParam);
  if (nInstructions == 0)
    nInstructions = 1000;
  DWORD tcS = GetTickCount();

  for (i=0;i<nInstructions;i++)
  {
    RApiClientReadProcessMemory(m_session, m_pi.hProcess, address, &val, 1, &dummy);
    sprintf(buf, "Read 1 byte @ %08lX", address);
    address = (LPVOID)((DWORD)address+1);
    m_lbStatus.AddString(buf);
    m_lbStatus.RedrawWindow();
  }	

  DWORD tcE = GetTickCount();
  sprintf(buf, "Took %ld seconds or %ld millisecond(s) to carry %d instruction(s)", (tcE-tcS)/1000, tcE-tcS, nInstructions);
  AddStatusLine(buf);
}

void CGuiclientDlg::AddStatusLine(LPCTSTR Format, ...)
{
  TCHAR buf[1024];

  _tcscpy(buf, Format);
  m_lbStatus.AddString(buf);
  m_lbStatus.SetSel(m_lbStatus.GetCount()-1, TRUE);
}


void CGuiclientDlg::OnBtnFindmodule() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuiclientDlg::OnBnClickedBtnterminate2()
{
  // TODO: Add your control notification handler code here
}
