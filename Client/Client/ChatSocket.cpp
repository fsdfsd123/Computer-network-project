// ChatSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ChatSocket.h"
#include "ClientDlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatSocket

CChatSocket::CChatSocket(CClientDlg *pClientDlg)
{
	
	m_pClientDlg = pClientDlg;
	m_pChatDlg = NULL;
	
}

CChatSocket::CChatSocket(CChatDlg *pChatDlg )
{
	m_pChatDlg = pChatDlg;
	m_pClientDlg = NULL;
}

CChatSocket::~CChatSocket()
{
	m_pClientDlg = NULL;
	m_pChatDlg = NULL;

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatSocket, CSocket)
	//{{AFX_MSG_MAP(CChatSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatSocket member functions

void CChatSocket::OnReceive(int nErrorCode)
{
	if (NULL != m_pClientDlg)
	{
		m_pClientDlg->ProcessPendingRead();
	}else if (NULL != m_pChatDlg)
	{
		m_pChatDlg->ProcessPendingRead();
	}
}