// ConfigServerPage.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "ConfigServerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigServerPage dialog


CConfigServerPage::CConfigServerPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CConfigServerPage::IDD)
{
	//{{AFX_DATA_INIT(CConfigServerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigServerPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigServerPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigServerPage, CDialog)
	//{{AFX_MSG_MAP(CConfigServerPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigServerPage message handlers
