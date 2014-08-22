// guiclientDlg.h : header file
//

#if !defined(AFX_GUICLIENTDLG_H__6944DA5A_211C_4EE9_8B44_4A67A11E011B__INCLUDED_)
#define AFX_GUICLIENTDLG_H__6944DA5A_211C_4EE9_8B44_4A67A11E011B__INCLUDED_

#include "../rapi_lib/remoteapi.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGuiclientDlg dialog

class CGuiclientDlg : public CDialog
{
// Construction
public:
	void AddStatusLine(LPCTSTR Format, ...);
	CONTEXT m_ctx;
	BOOL m_rc;
	STARTUPINFO m_si;
  tRemoteApiSession m_session;
	PROCESS_INFORMATION m_pi;
	CGuiclientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGuiclientDlg)
	enum { IDD = IDD_GUICLIENT_DIALOG };
	CListBox	m_lbStatus;
	CString	m_strServer;
	int		m_nPort;
	CString	m_edtProcess;
	CString	m_edtParam;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiclientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGuiclientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnconnect();
	afx_msg void OnBtnrun();
	afx_msg void OnBtnsuspend();
	afx_msg void OnBtnresume();
	afx_msg void OnBtnterminate();
	afx_msg void OnBtnsetthreadcontext();
	afx_msg void OnBtngetthreadcontext();
	afx_msg void OnBtndisconnect();
	afx_msg void OnBtncloseserver();
	afx_msg void OnDestroy();
	afx_msg void OnBtnstatusclear();
	afx_msg void OnBtnrestartserver();
	afx_msg void OnBtnDebugprocess();
	afx_msg void OnBtnBenchmark();
	afx_msg void OnBtnFindmodule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
public:
  afx_msg void OnBnClickedBtnterminate2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUICLIENTDLG_H__6944DA5A_211C_4EE9_8B44_4A67A11E011B__INCLUDED_)
