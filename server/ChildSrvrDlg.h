#if !defined(AFX_CHILDSRVRDLG_H__0739DC27_3979_434E_BC18_73ED7CFF9589__INCLUDED_)
#define AFX_CHILDSRVRDLG_H__0739DC27_3979_434E_BC18_73ED7CFF9589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildSrvrDlg.h : header file
//

#include "rapi_lib/remoteapi_server.h"
#include "libraries/InterProcessCommunication.h"	// Added by ClassView

typedef struct
{
  union
  {
    SOCKET socket;
    DWORD  isocket;
  };
  HWND   ParentHandle;
  LPVOID Param;
} tChildServerStartupInfo;


void ChildApiTransportServerThreadProc(void *);
bool processServerCommunication(tRemoteApiSession &);

/////////////////////////////////////////////////////////////////////////////
// CChildSrvrDlg dialog

class CChildSrvrDlg : public CDialog
{
// Construction
public:
	static void ChildApiTransportServerThreadProc(void *param);
  void PrintMessageParent(LPCTSTR Format, ...);
	void PrintMessage(LPCTSTR Format, ...);
	tChildServerStartupInfo m_cssi;
	static void RunChildServer(tChildServerStartupInfo *cssi);
	CChildSrvrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChildSrvrDlg)
	enum { IDD = IDD_CHILDSRVRDLG_DIALOG };
	CEdit	m_txtChildLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildSrvrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChildSrvrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	long m_nTimeout;
	CInterProcessCommunication m_ipc;
	bool m_bDialogInitialized;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDSRVRDLG_H__0739DC27_3979_434E_BC18_73ED7CFF9589__INCLUDED_)
