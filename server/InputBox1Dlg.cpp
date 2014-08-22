// InputBox1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RApiServer.h"
#include "InputBox1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBox1Dlg dialog


CInputBox1Dlg::CInputBox1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBox1Dlg)
	m_edtValue = _T("");
	//}}AFX_DATA_INIT
}


void CInputBox1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBox1Dlg)
	DDX_Text(pDX, IDC_EDT_INPUTBOXDLG, m_edtValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBox1Dlg, CDialog)
	//{{AFX_MSG_MAP(CInputBox1Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBox1Dlg message handlers

UINT CInputBox1Dlg::Launch(
    LPCTSTR Title, 
    LPCTSTR DefaultValue, 
    LPTSTR Buffer,
    size_t BufSz)
{
  CInputBox1Dlg dlg;

  dlg.m_strValue = DefaultValue;
  dlg.m_strTitle = Title;

  UINT rc = dlg.DoModal();

  if (rc == IDOK)
      _tcsncpy(Buffer, dlg.m_strValue, BufSz);

  return rc;
}

BOOL CInputBox1Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  SetWindowText(m_strTitle);
  m_edtValue = m_strValue;
	UpdateData(FALSE);

  GetDlgItem(IDC_EDT_INPUTBOXDLG)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBox1Dlg::OnOK() 
{
	UpdateData(TRUE);
  m_strValue = m_edtValue;

	CDialog::OnOK();
}
