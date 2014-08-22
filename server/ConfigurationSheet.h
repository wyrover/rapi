#if !defined(AFX_CONFIGURATIONSHEET_H__AA51656B_7A4F_4748_BA44_6957C201DC33__INCLUDED_)
#define AFX_CONFIGURATIONSHEET_H__AA51656B_7A4F_4748_BA44_6957C201DC33__INCLUDED_

#include "ConfigServerPage.h"	// Added by ClassView
#include "ChildServerConfigPage.h"
#include "generalconfigpage.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigurationSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigurationSheet

class CConfigurationSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigurationSheet)

// Construction
public:
	CConfigurationSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CConfigurationSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigurationSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	static UINT ShowModal();
	virtual ~CConfigurationSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConfigurationSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CChildServerConfigPage m_dlg_childservercfg;
	CConfigServerPage      m_dlg_servercfg;
  CGeneralConfigPage     m_dlg_generalcfg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATIONSHEET_H__AA51656B_7A4F_4748_BA44_6957C201DC33__INCLUDED_)
