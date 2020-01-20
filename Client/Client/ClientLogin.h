#if !defined(AFX_CLIENTLOGIN_H__3E755F15_AF3D_47F1_8EE6_AEA60FD96526__INCLUDED_)
#define AFX_CLIENTLOGIN_H__3E755F15_AF3D_47F1_8EE6_AEA60FD96526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientLogin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientLogin dialog

class CClientLoginDlg : public CDialog
{
// Construction
public:
	CClientLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClientLogin)
	enum { IDD = IDD_CLIENT_LOGIN_DIALOG };
	CIPAddressCtrl	m_serverIP;
	CString	m_strServerPort;
	CString	m_strName;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual BOOL OnInitDialog();
	virtual void OnOK(void);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClientLogin)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DWORD	m_dwIP;	//·þÎñÆ÷µØÖ·
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTLOGIN_H__3E755F15_AF3D_47F1_8EE6_AEA60FD96526__INCLUDED_)
