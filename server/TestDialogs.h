#if !defined(AFX_TESTDIALOGS_H__05270390_1687_4494_936E_263E9C655BA5__INCLUDED_)
#define AFX_TESTDIALOGS_H__05270390_1687_4494_936E_263E9C655BA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDialogs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestDialogs dialog

class CTestDialogs : public CDialog
{
// Construction
public:
	CTestDialogs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDialogs)
	enum { IDD = IDD_DIALOGTESTS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDialogs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDialogs)
	afx_msg void OnBtnShowConfigsheet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDIALOGS_H__05270390_1687_4494_936E_263E9C655BA5__INCLUDED_)
