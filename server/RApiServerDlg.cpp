// RApiServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "RApiServerDlg.h"
#include "ConfigurationSheet.h"
#include "InputBox1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrDlg dialog

CRApiSrvrDlg::CRApiSrvrDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CRApiSrvrDlg::IDD, pParent), m_ipc(LC_IPC_NAME, 1024)
{
    //{{AFX_DATA_INIT(CRApiSrvrDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bDialogInitialized = false;

    m_bAutoListen = true;

    m_PID = GetCurrentProcessId();

    // Allocate memory for printing messages
    m_szPrintMessageBuffer1 = new TCHAR[4096];
    m_szPrintMessageBuffer2 = new TCHAR[4096];


    CT2A serverAddress(m_strServerAddress);

    DWORD ret = GetHostIpAddress(serverAddress.m_psz);
    if (!ret)
        m_strServerAddress = "127.0.0.1";

    m_nServerPort = 1001;

    m_bQueuePrintMessage = false;
    PrintMessageStopQueue();
}

CRApiSrvrDlg::~CRApiSrvrDlg()
{
    delete[] m_szPrintMessageBuffer1;
    delete[] m_szPrintMessageBuffer2;
}

void CRApiSrvrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRApiSrvrDlg)
    DDX_Control(pDX, IDC_EDT_LOG, m_txtLog);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRApiSrvrDlg, CDialog)
    //{{AFX_MSG_MAP(CRApiSrvrDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_COMMAND(ID_MNU_OPTIONS_CONFIGURATION, OnMenuOptionsConfiguration)
    ON_COMMAND(IDM_MENU_SERVERLISTEN, OnMenuServerlisten)
    ON_COMMAND(IDC_MENU_TESTING, OnMenuTesting)
    ON_COMMAND(ID_MNU_EDIT_CLEAR, OnMnuEditClear)
    ON_COMMAND(ID_MNU_EDIT_SELECT_ALL, OnMnuEditSelectAll)
    ON_WM_DESTROY()
    ON_WM_CLOSE()
    ON_COMMAND(ID_MENUDEBUG_PASSDATASOCKETTOCHILD, OnMenudebugPassdatasockettochild)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrDlg message handlers

BOOL CRApiSrvrDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    //-----------------------------------
    m_bDialogInitialized = true;
    m_bAlreadyListening = false;

    m_bManualChildServer = ((__argc == 2) && (_tcsicmp((LPCTSTR)__argv[1], _T("manualchildserver")) == 0));

    m_Menu = GetMenu();

    ShowInternalVariables();
    if (m_bAutoListen)
        OnMenuServerlisten();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRApiSrvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRApiSrvrDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

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
HCURSOR CRApiSrvrDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

//-------------------------------------------------------------------------------------------
//
// Prints a message into the listener window
void CRApiSrvrDlg::PrintMessage(
    unsigned int DebugLevel, 
    LPCTSTR Format, 
    ...)
{
    va_list parameters;
    va_start(parameters, Format);
    PrintMessage((LPCTSTR)NULL, Format, parameters);
}

//-------------------------------------------------------------------------------------------
//
// Prints a message into the listener window
//
//
void CRApiSrvrDlg::PrintMessage(LPCTSTR Format, ...)
{
    LPTSTR fmt;
    va_list parameters, vparam;

    if (m_szPrintMessageBuffer1 == NULL)
        return;

    va_start(parameters, Format);

    if (Format == NULL)
    {
        fmt = va_arg(parameters, LPTSTR);
        vparam = va_arg(parameters, va_list);
    }
    else
    {
        vparam = parameters;
        fmt = (LPTSTR)Format;
    }
    _vstprintf(m_szPrintMessageBuffer1, fmt, vparam);

    FixMultiLine(m_szPrintMessageBuffer1, m_szPrintMessageBuffer2, true);

    if (!m_bQueuePrintMessage)
    {
        int nLen = m_txtLog.GetWindowTextLength();
        m_txtLog.SetSel(nLen, nLen, TRUE);
        m_txtLog.ReplaceSel(m_szPrintMessageBuffer2);
    }
    else
    {
        m_strQueuedPrintMessage += m_szPrintMessageBuffer2;
    }
}

//------------------------------------------------------
// Resizes the log text window as the dialog resizes
//
//
void CRApiSrvrDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (!m_bDialogInitialized)
        return;

    RECT r;
    GetWindowRect(&r);
    m_txtLog.SetWindowPos(this, 0, 0, cx, cy, SWP_NOZORDER);
}


//-------------------------------------------------------------------------------------------
// This thread will accept all incomming connections and then serves each connection
// with a corresponding child server
//
//
unsigned __stdcall CRApiSrvrDlg::ListeningThreadProc(void *param)
{
    CRApiSrvrDlg *dlg = (CRApiSrvrDlg *)param;

    SOCKET &listenSocket = dlg->m_hListeningSocket;

    // Keep accepting connection until asked otherwise
    while (true)
    {
        //
        dlg->PrintMessage(_T("Waiting for a connection on %s:%d...\n"),
            dlg->m_strServerAddress, dlg->m_nServerPort);

        DWORD ret = WSAWaitForMultipleEvents(
            2,
            dlg->m_wsaevListen,
            FALSE,
            INFINITE,
            FALSE);
        if (ret == WSA_WAIT_FAILED)
        {
            dlg->PrintMessage(_T("Failed to wait for accept() events!\n"));
            break;
        }

        // Stop event signaled ?
        if (ret == WSA_WAIT_EVENT_0 + 1)
        {
            dlg->PrintMessage(_T("Listening server shutting down....\n"));
            break;
        }

        // not our accept() or close() event?
        if (ret != WSA_WAIT_EVENT_0)
        {
            dlg->PrintMessage(_T("Unkown event signaled!! aborting!\n"));
            break;
        }

        // Guess what network event occured w/ our event1
        WSANETWORKEVENTS ne;
        ret = WSAEnumNetworkEvents(listenSocket, dlg->m_wsaevListen[0], &ne);
        if ((ret == SOCKET_ERROR) || ((ret & FD_ACCEPT) != FD_ACCEPT) || ((ret & FD_CLOSE) != FD_CLOSE))
            dlg->PrintMessage(_T("warning: unkown event or a non-accept or close event occured\n"));

        // connection closed unexpectedly ?
        if ((ne.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
        {
            dlg->PrintMessage(_T("connection closed unexpectedly\n"));
            break;
        }

        // address of address that will connect to us
        sockaddr_in sinRemote;

        // data socket
        SOCKET sd = AcceptConnection(listenSocket, sinRemote);

        // Invalid socket returned ?!
        if (sd == INVALID_SOCKET)
        {
            dlg->PrintMessage(_T("warning: connection closed unexpectedly\n"));
            break;
        }

        // Accepted socket will also be associated with the same event handle as
        // the listen socket.  You don't want this, so disassociate event from the
        // accepted socket.
        // Note: that this will make your socket a non-block socket too!
        ret = WSAEventSelect(sd, NULL, 0);
        // Switch socket back to blocking mode!
        { u_long blocking = 0; ret = ioctlsocket(sd, FIONBIO, &blocking); }
        if (ret == SOCKET_ERROR)
        {
            dlg->PrintMessage(_T("Could not turn data socket %d into blocking mode!\n"), sd);
            break;
        }

        dlg->PrintMessage(_T("Accepted connection; sockethandle: %ld\n"), sd);

        // Start a child server to process all the requests
        dlg->m_hLastDataSocket = sd;

        if (!dlg->StartChildServer(sd))
            dlg->PrintMessage(_T("Could not server this connection!\n"));
    }
    return 0;
}


//---------------------------------------------------------------------------------
//
// m_wsaevListen[0] = FD_ACCEPT or FD_CLOSE event
// m_wsaevListen[1] = my own event to tell listening thread to stop listening
//
bool CRApiSrvrDlg::StartListeningThread()
{
    // Start listening
    CT2A serverAddress(m_strServerAddress);
    SOCKET listenSocket = m_hListeningSocket =
        SetUpListener(serverAddress.m_psz,
        htons(m_nServerPort));

    // opened a listening socket?
    if (listenSocket == INVALID_SOCKET)
    {
        PrintMessage(
            _T("Could not start listening socket (%s:%d)\n"),
            m_strServerAddress,
            m_nServerPort);

        return false;
    }

    // create accept();close() and "Stop accept" events
    for (int i = 0; i < 2; i++)
    {
        if ((m_wsaevListen[i] = WSACreateEvent()) == WSA_INVALID_EVENT)
        {
            PrintMessage(_T("Could not create WSAEvent[%d]\n"), i);
            return false;
        }
    }

    // Associate the event1 w/ Accept() or Close() events
    if (WSAEventSelect(listenSocket, m_wsaevListen[0], FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
    {
        PrintMessage(_T("Failed to setup events! closing listening socket!\n"));
        WSACloseEvent(m_wsaevListen[0]);
        WSACloseEvent(m_wsaevListen[1]);
        closesocket(listenSocket);
        m_hListeningSocket = INVALID_SOCKET;
        return false;
    }

    // create the connection handler thread
    m_hListeningThread = _beginthreadex(NULL,
        NULL,
        ListeningThreadProc,
        (void *)this,
        NULL,
        &m_dwListeningThreadID);

    if (m_hListeningThread == -1)
    {
        PrintMessage(_T("Could not start listening thread\n"));
        return false;
    }
    return (m_bAlreadyListening = true);
}


//--------------------------------------------------------------------
//
void CRApiSrvrDlg::OnMenuOptionsConfiguration()
{
    TCHAR buf[100];

    if (CInputBox1Dlg::Launch(_T("Server IP"), _T("127.0.0.1:1001"), buf, _countof(buf)) == IDOK)
    {
        TCHAR serverAddress[100];
        _stscanf(buf, _T("%[^:]:%d"), serverAddress, &m_nServerPort);
        m_strServerAddress = serverAddress;
    }
}

//--------------------------------------------------------------------
//
//
void CRApiSrvrDlg::OnMenuServerlisten()
{
    if (m_bAlreadyListening)
    {
        StopListeningThread();
        m_Menu->CheckMenuItem(IDM_MENU_SERVERLISTEN, MF_UNCHECKED);
    }
    else
    {
        if (!m_strServerAddress.GetLength())
            OnMenuOptionsConfiguration();
        // If it could start listening, set menu checkmark
        if (StartListeningThread())
            m_Menu->CheckMenuItem(IDM_MENU_SERVERLISTEN, MF_CHECKED);
    }

}

//--------------------------------------------------------------------
//
// Launches another process to handle the data socket.
// It duplicates the handle and passes it to the other process
// This method is Win9x compatible as described in 
// http://support.microsoft.com/default.aspx?scid=KB;en-us;q150523
//
BOOL CRApiSrvrDlg::StartChildServer(SOCKET datasocket)
{
    STARTUPINFO          si;
    PROCESS_INFORMATION  pi;
    SOCKET               DuplicateSocket;
    TCHAR                cmdline[MAX_PATH];
    unsigned             dummy;

    if (m_bManualChildServer)
    {
        m_hLastDataSocket = datasocket;
        return TRUE;
    }

    m_LastError = RASC_CANNOTCREATECHILDSERVER;

    //
    // Make this handle duplicatable
    // The new duplicatable handle will be stored in duplicateSocket
    //
    if (!DuplicateHandle(GetCurrentProcess(),
        (HANDLE)datasocket,
        GetCurrentProcess(),
        (LPHANDLE)&DuplicateSocket,
        0,
        TRUE,
        DUPLICATE_SAME_ACCESS))
    {
        GetLastError();
        return FALSE;
    }

    //
    // compose command line
    //
    TCHAR Self[MAX_PATH];
    GetModuleFileName(NULL, Self, _countof(Self));
    wsprintf(
        cmdline, 
        _T("\"%s\" child %ld"), Self, DuplicateSocket);

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(
        NULL,
        cmdline,
        NULL, NULL,
        TRUE, // inherit handles
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        GetLastError();
        if (!ShutdownConnection(datasocket))
            closesocket(datasocket);

        closesocket(DuplicateSocket);
        return FALSE;
    }

    // create a new struct that will be used on our child server waiting thread
    pChildServerHandles pcsh = new tChildServerHandles;

    // prepare parameters to be passed
    pcsh->dupSocket = DuplicateSocket;
    pcsh->sd = datasocket;
    pcsh->hThread = pi.hThread;
    pcsh->hProcess = pi.hProcess;
    pcsh->ProcessId = pi.dwProcessId;
    pcsh->Param = (LPVOID)this;

    PrintMessage(_T("Child server created; PID=%d\n"), pi.dwProcessId);

    // failed to create child server?
    if (_beginthreadex(NULL,
        NULL,
        WaitForChildServerThreadProc,
        (void *)pcsh,
        NULL,
        &dummy) == -1)
    {
        PrintMessage(_T("Could not create the thread that will monitor the child server!\n"));
        ShutdownConnection(datasocket);
        closesocket(DuplicateSocket);
        delete pcsh;
        return FALSE;
    }
    //
    m_LastError = RASC_SUCCESS;
    return TRUE;
}


//---------------------------------------------------------------------------------------------
//
// A worker thread that waits for a given server child to terminate
//
//
unsigned __stdcall CRApiSrvrDlg::WaitForChildServerThreadProc(void *param)
{
    // take a copy of param
    tChildServerHandles pcsh = *(pChildServerHandles)param;

    // free memory allocated by the parameters
    delete param;

    CRApiSrvrDlg *dlg = (CRApiSrvrDlg *)pcsh.Param;

    // Wait until the child server finishes
    WaitForSingleObject(pcsh.hProcess, INFINITE);

    // close data socket
    closesocket(pcsh.sd);

    //
    CloseHandle(pcsh.hProcess);
    CloseHandle(pcsh.hThread);

    // close Duplicated socket
    CloseHandle((HANDLE)pcsh.dupSocket);

    // status messages
    dlg->PrintMessage(_T("Finished waiting for child server %d\n"), pcsh.ProcessId);

    return 0;
}


//----------------------------------------------------------------------------------------
//
//
//
void CRApiSrvrDlg::ShowInternalVariables()
{
    PrintMessage(
        _T("-----------------------------------------------------------------------------\n")
        _T("RemoteApi Server Build: %S %S\n")
        _T("Window handle: %ld (0x%08lX)\n")
        _T("ProcessId: %ld\n")
        _T("Command line: [%s]\n")
        _T("Manual child server: %s\n")
        _T("------------------------------------------------------------------------------\n"),
        __DATE__,
        __TIME__,
        m_hWnd,
        m_hWnd,
        m_PID,
        GetCommandLine(),
        m_bManualChildServer ? _T("yes") : _T("no"));
}


void CRApiSrvrDlg::OnMenuTesting()
{
}

//-------------------------------------------------------------------------
void CRApiSrvrDlg::OnMnuEditClear()
{
    // clear text
    m_txtLog.SetWindowText(_T(""));

    ShowInternalVariables();
}

void CRApiSrvrDlg::OnMnuEditSelectAll()
{
    // Select all text
    m_txtLog.SetSel(0, -1, TRUE);
}


void CRApiSrvrDlg::OnDestroy()
{
    if (m_bAlreadyListening)
        StopListeningThread();

    CDialog::OnDestroy();
}

void CRApiSrvrDlg::OnClose()
{
    if (m_bAlreadyListening)
        StopListeningThread();
    CDialog::OnClose();
}


//--------------------------------------------------------------------------------
// Stops the listening thread
//
void CRApiSrvrDlg::StopListeningThread()
{
    // signal listening thread that we want to stop
    WSASetEvent(m_wsaevListen[1]);

    // signal listening thread to terminate & shutdown connection
    closesocket(m_hListeningSocket);

    // invalidate socket
    m_hListeningSocket = INVALID_SOCKET;

    // wait for ListeniningThread to terminate (couple of seconds)
    PrintMessageStartQueue();
    WaitForSingleObject((HANDLE)m_hListeningThread, 3000);
    PrintMessageStopQueue();

    PrintMessage(_T("Stopped listening to incomming connections\n"));

    // Clear the wsaevListen events
    for (int i = 0; i < 2; i++)
    {
        WSACloseEvent(m_wsaevListen[i]);
        m_wsaevListen[i] = NULL;
    }

    // reset thread
    m_hListeningThread = NULL;

    m_bAlreadyListening = false;
}


//------------------------------------------------------------------------------
// Passes the last data socket to the child server instance using CIpc
//
void CRApiSrvrDlg::OnMenudebugPassdatasockettochild()
{
    if (!m_bManualChildServer)
        return;

    if (!m_ipc.InterConnect())
    {
        PrintMessage(_T("Child server not ready and waiting yet! Could not pass!\n"));
        return;
    }

    HANDLE h = (HANDLE)m_hLastDataSocket;

    PrintMessage(_T("Data socket handle is %d\n"), h);

    DWORD destPID = m_ipc.GetOtherProcessId();

    HANDLE destProcess = OpenProcess(PROCESS_ALL_ACCESS,
        TRUE,
        destPID);
    if (destProcess == NULL)
    {
        PrintMessage(_T("Failed to open child server process\n"));
        return;
    }

    HANDLE rh;

    if (!DuplicateHandle(
        GetCurrentProcess(),
        h,
        destProcess,
        &rh,
        0,
        TRUE,
        DUPLICATE_SAME_ACCESS))
    {
        PrintMessage(_T("Failed to duplicate handle\n"));
        CloseHandle(destProcess);
        return;
    }

    CloseHandle(destProcess);


    // Send handle to child server
    m_ipc.SendBuffer(&rh, sizeof(rh));
    PrintMessage(_T("Handle %ld duplicated and passed for PID(%d) as %d\n"),
        h,
        destPID,
        rh);

    CloseHandle(h);
}

void CRApiSrvrDlg::PrintMessageStartQueue()
{
    m_bQueuePrintMessage = true;
    m_strQueuedPrintMessage = "";
}

void CRApiSrvrDlg::PrintMessageStopQueue()
{
    // If was previously queued
    if (m_bQueuePrintMessage)
    {
        m_bQueuePrintMessage = false;
        PrintMessage(m_strQueuedPrintMessage);
    }
    m_strQueuedPrintMessage = "";
}

void CRApiSrvrDlg::OnOK()
{
    if (m_bAlreadyListening)
        StopListeningThread();
    CDialog::OnOK();
}
