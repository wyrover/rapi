#if !defined(AFX_CONFIGSERVERPAGE_H__D11799B9_D5BD_4110_84F9_BFAB4B34A57C__INCLUDED_)
#define AFX_CONFIGSERVERPAGE_H__D11799B9_D5BD_4110_84F9_BFAB4B34A57C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigServerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigServerPage dialog

class CConfigServerPage : public CPropertyPage
{
// Construction
public:
	CConfigServerPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigServerPage)
	enum { IDD = IDD_CONFIGSERVERPAGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigServerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigServerPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGSERVERPAGE_H__D11799B9_D5BD_4110_84F9_BFAB4B34A57C__INCLUDED_)
