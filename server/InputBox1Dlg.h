#if !defined(AFX_INPUTBOX1DLG_H__68F4F04E_9D3E_4084_9AB3_1DCF4329E63D__INCLUDED_)
#define AFX_INPUTBOX1DLG_H__68F4F04E_9D3E_4084_9AB3_1DCF4329E63D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBox1Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBox1Dlg dialog

class CInputBox1Dlg : public CDialog
{
// Construction
public:
	static UINT Launch(
        LPCTSTR Title, 
        LPCTSTR DefaultValue, 
        LPTSTR Buffer,
        size_t BufSz);
	CInputBox1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBox1Dlg)
	enum { IDD = IDD_INPUTBOX1 };
	CString	m_edtValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBox1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBox1Dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strValue;
	bool m_bDialogInitialized;
	CString m_strTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOX1DLG_H__68F4F04E_9D3E_4084_9AB3_1DCF4329E63D__INCLUDED_)
