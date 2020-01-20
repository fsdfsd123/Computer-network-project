// ClientLogin.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientLogin dialog


CClientLoginDlg::CClientLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientLogin)
	m_strServerPort.Format("%d",SERVERPORT);
	m_strName = _T("myChat");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


void CClientLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientLogin)
	DDX_Control(pDX, IDC_CLIENT_IPADDRESS, m_serverIP);
	DDX_Text(pDX, IDC_CLIENT_PASSWORD_EDIT, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 10);
	DDX_Text(pDX, IDC_CLIENT_NAME_EDIT, m_strName);
	DDV_MaxChars(pDX, m_strName, 10);
	DDX_Text(pDX, IDC_CLIENT_PORT_EDIT, m_strServerPort);
	DDV_MaxChars(pDX, m_strServerPort, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClientLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CClientLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientLogin message handlers
BOOL CClientLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_serverIP.SetAddress(127, 0, 0, 1);
	return TRUE; 
}


void CClientLoginDlg::OnOK(void)
{
	UpdateData(TRUE);
	m_serverIP.GetAddress(m_dwIP);

	CDialog::OnOK();
}