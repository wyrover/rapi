#if !defined(AFX_GENERALCONFIGPAGE_H__B0F8A0AD_AEC0_446F_B872_FD5C313F42CC__INCLUDED_)
#define AFX_GENERALCONFIGPAGE_H__B0F8A0AD_AEC0_446F_B872_FD5C313F42CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralConfigPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralConfigPage dialog

class CGeneralConfigPage : public CPropertyPage
{
// Construction
public:
	CGeneralConfigPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneralConfigPage)
	enum { IDD = IDD_GENERALCONFIGPAGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralConfigPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALCONFIGPAGE_H__B0F8A0AD_AEC0_446F_B872_FD5C313F42CC__INCLUDED_)
