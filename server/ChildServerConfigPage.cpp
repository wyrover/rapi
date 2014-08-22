// ChildServerConfigPage.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "ChildServerConfigPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildServerConfigPage dialog


CChildServerConfigPage::CChildServerConfigPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CChildServerConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CChildServerConfigPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChildServerConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildServerConfigPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildServerConfigPage, CDialog)
	//{{AFX_MSG_MAP(CChildServerConfigPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildServerConfigPage message handlers
