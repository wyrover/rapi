// ConfigurationSheet.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "ConfigurationSheet.h"

// server configuration page


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigurationSheet

IMPLEMENT_DYNAMIC(CConfigurationSheet, CPropertySheet)

CConfigurationSheet::CConfigurationSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CConfigurationSheet::CConfigurationSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  AddPage(&m_dlg_servercfg);
  AddPage(&m_dlg_childservercfg);
  AddPage(&m_dlg_generalcfg);
}

CConfigurationSheet::~CConfigurationSheet()
{

}


BEGIN_MESSAGE_MAP(CConfigurationSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigurationSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigurationSheet message handlers

UINT CConfigurationSheet::ShowModal()
{
  UINT rc;

  CConfigurationSheet cfg(_T("Configuration"));
  rc = cfg.DoModal();
  return rc;
}