// TestDialogs.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "TestDialogs.h"
#include "configurationsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDialogs dialog


CTestDialogs::CTestDialogs(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDialogs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDialogs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestDialogs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDialogs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDialogs, CDialog)
	//{{AFX_MSG_MAP(CTestDialogs)
	ON_BN_CLICKED(IDC_BTN_SHOW_CONFIGSHEET, OnBtnShowConfigsheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDialogs message handlers

void CTestDialogs::OnBtnShowConfigsheet() 
{
  CConfigurationSheet::ShowModal();
}
