// guiclient.h : main header file for the GUICLIENT application
//

#if !defined(AFX_GUICLIENT_H__D6C5CAA4_B461_4D86_B7E2_1888FE540AE6__INCLUDED_)
#define AFX_GUICLIENT_H__D6C5CAA4_B461_4D86_B7E2_1888FE540AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CGuiclientApp:
// See guiclient.cpp for the implementation of this class
//

class CGuiclientApp : public CWinApp
{
public:
	CGuiclientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiclientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGuiclientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUICLIENT_H__D6C5CAA4_B461_4D86_B7E2_1888FE540AE6__INCLUDED_)
