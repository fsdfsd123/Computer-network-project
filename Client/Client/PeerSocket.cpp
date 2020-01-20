// PeerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "PeerSocket.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeerSocket

CPeerSocket::CPeerSocket(CClientDlg *pClientDlg)
{
}

CPeerSocket::~CPeerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPeerSocket, CSocket)
	//{{AFX_MSG_MAP(CPeerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPeerSocket::OnAccept( int nErrorCode )
{
		m_pClientDlg->ProcessPendingAccept();	
}
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPeerSocket member functions
