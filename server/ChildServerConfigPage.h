#if !defined(AFX_CHILDSERVERCONFIGPAGE_H__A4CCA24F_6990_4798_A528_5EF60B7557E8__INCLUDED_)
#define AFX_CHILDSERVERCONFIGPAGE_H__A4CCA24F_6990_4798_A528_5EF60B7557E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildServerConfigPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildServerConfigPage dialog

class CChildServerConfigPage : public CPropertyPage
{
// Construction
public:
	CChildServerConfigPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChildServerConfigPage)
	enum { IDD = IDD_CHILDSERVERCONFIGPAGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildServerConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChildServerConfigPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDSERVERCONFIGPAGE_H__A4CCA24F_6990_4798_A528_5EF60B7557E8__INCLUDED_)
