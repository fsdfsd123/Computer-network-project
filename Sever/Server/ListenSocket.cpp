// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ListenSocket.h"
#include "ServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket(CServerDlg* pServerDlg)
{
	m_pServerDlg = pServerDlg;
}

CListenSocket::~CListenSocket()
{
	m_pServerDlg = NULL;
	//		Close();

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions


/*	
 * 接受客户端的链接请求
 */
void CListenSocket::OnAccept(int nErrorCode)
{
	m_pServerDlg->ProcessPendingAccept();
}



