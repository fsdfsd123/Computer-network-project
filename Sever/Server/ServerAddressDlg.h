#if !defined(AFX_SERVERADDRESSDLG_H__8F2F090C_25B1_4483_892F_844320920F10__INCLUDED_)
#define AFX_SERVERADDRESSDLG_H__8F2F090C_25B1_4483_892F_844320920F10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerAddressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerAddressDlg dialog

class CServerAddressDlg : public CDialog
{
// Construction
public:

	CServerAddressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerAddressDlg)
	enum { IDD = IDD_SERVER_ADDRESS_DLG };
	CIPAddressCtrl	m_ctlServIP;
	CEdit	m_ctlServPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerAddressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerAddressDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_strServIP;	//服务器IP地址
	SHORT m_shServPort;	//服务器端口
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERADDRESSDLG_H__8F2F090C_25B1_4483_892F_844320920F10__INCLUDED_)
