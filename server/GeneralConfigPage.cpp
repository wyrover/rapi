// GeneralConfigPage.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "GeneralConfigPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralConfigPage dialog


CGeneralConfigPage::CGeneralConfigPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CGeneralConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralConfigPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGeneralConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralConfigPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralConfigPage, CDialog)
	//{{AFX_MSG_MAP(CGeneralConfigPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralConfigPage message handlers
