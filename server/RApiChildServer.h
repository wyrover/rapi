// RApiServer.h : main header file for the RAPISERVER application
//

#if !defined(AFX_RAPISERVER_H__E9D0DB65_9B51_4AEE_9A2B_17E4D96E5BB3__INCLUDED_)
#define AFX_RAPISERVER_H__E9D0DB65_9B51_4AEE_9A2B_17E4D96E5BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRApiChildSrvrApp:
// See RApiServer.cpp for the implementation of this class
//

class CRApiChildSrvrApp : public CWinApp
{
public:
	int ExitInstance();
	CRApiChildSrvrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRApiChildSrvrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRApiChildSrvrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAPISERVER_H__E9D0DB65_9B51_4AEE_9A2B_17E4D96E5BB3__INCLUDED_)
