// ServerAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerAddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerAddressDlg dialog


CServerAddressDlg::CServerAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerAddressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerAddressDlg)

	//}}AFX_DATA_INIT
}


void CServerAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerAddressDlg)
	DDX_Control(pDX, IDC_SERVER_IPADDRESS, m_ctlServIP);
	DDX_Control(pDX, IDC_SERVER_PORT_EDIT, m_ctlServPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerAddressDlg, CDialog)
	//{{AFX_MSG_MAP(CServerAddressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerAddressDlg message handlers

void CServerAddressDlg::OnOK() 
{
	//获取服务器IP和端口
	m_ctlServIP.GetAddress(m_strServIP);

	CString strServPort;
	m_ctlServPort.GetWindowText(strServPort);
	m_shServPort = atoi(strServPort);		//将字符串转换为SHORT

	CDialog::OnOK();
}

BOOL CServerAddressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctlServIP.SetAddress(127, 0, 0, 1);
	CString strServIP;
	strServIP.Format("%d",SERVERPORT);
	m_ctlServPort.SetWindowText(strServIP);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
