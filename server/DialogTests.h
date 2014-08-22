// DialogTests.h : main header file for the DIALOGTESTS application
//

#if !defined(AFX_DIALOGTESTS_H__F69B280B_089C_489D_B44D_61CB8084EBC0__INCLUDED_)
#define AFX_DIALOGTESTS_H__F69B280B_089C_489D_B44D_61CB8084EBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDialogTestsApp:
// See DialogTests.cpp for the implementation of this class
//

class CDialogTestsApp : public CWinApp
{
public:
	CDialogTestsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTestsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDialogTestsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTESTS_H__F69B280B_089C_489D_B44D_61CB8084EBC0__INCLUDED_)
