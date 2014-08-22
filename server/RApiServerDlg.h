// RApiServerDlg.h : header file
//

#if !defined(AFX_RAPISERVERDLG_H__A2ED43D9_0FC8_4901_B521_310D332C4058__INCLUDED_)
#define AFX_RAPISERVERDLG_H__A2ED43D9_0FC8_4901_B521_310D332C4058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRApiSrvrDlg dialog
#include "libraries/interprocesscommunication.h"

typedef struct
{
    HANDLE hProcess;
    HANDLE hThread;
    DWORD  ProcessId;
    SOCKET sd;
    SOCKET dupSocket;
    LPVOID Param;
} tChildServerHandles, *pChildServerHandles;

class CRApiSrvrDlg : public CDialog
{
    // Construction
public:
    void PrintMessageStartQueue();
    void PrintMessage(unsigned DebugLevel, LPCTSTR Format, ...);
    ~CRApiSrvrDlg();
    void ShowInternalVariables();
    static unsigned __stdcall WaitForChildServerThreadProc(void *);
    BOOL StartChildServer(SOCKET);
    bool StartListeningThread();
    static unsigned __stdcall ListeningThreadProc(void *);
    void PrintMessage(LPCTSTR Format, ...);
    bool m_bDialogInitialized;
    CRApiSrvrDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    //{{AFX_DATA(CRApiSrvrDlg)
    enum { IDD = IDD_RAPISERVER_DIALOG };
    CEdit	m_txtLog;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRApiSrvrDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CRApiSrvrDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMenuOptionsConfiguration();
    afx_msg void OnMenuServerlisten();
    afx_msg void OnMenuTesting();
    afx_msg void OnMnuEditClear();
    afx_msg void OnMnuEditSelectAll();
    afx_msg void OnDestroy();
    afx_msg void OnClose();
    afx_msg void OnMenutestsManualchildspawn();
    afx_msg void OnMenudebugPassdatasockettochild();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void OnOK();
    void PrintMessageStopQueue();
    CString m_strQueuedPrintMessage;
    bool m_bQueuePrintMessage;
    CInterProcessCommunication  m_ipc;
    eRApiStatusCodes            m_LastError;
    void                        StopListeningThread();
    CMenu                       *m_Menu;
    LPTSTR   m_szPrintMessageBuffer1;
    LPTSTR   m_szPrintMessageBuffer2;

    bool     m_bAutoListen;
    SOCKET   m_hLastDataSocket;
    DWORD    m_PID;

    bool     m_bManualChildServer;
    bool     m_bAlreadyListening;
    bool     m_bStopListening;
    SOCKET   m_hListeningSocket;
    USHORT   m_nServerPort;

    CString  m_strServerAddress;
    unsigned m_hListeningThread;
    unsigned m_dwListeningThreadID;
    WSAEVENT m_wsaevListen[2];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAPISERVERDLG_H__A2ED43D9_0FC8_4901_B521_310D332C4058__INCLUDED_)
