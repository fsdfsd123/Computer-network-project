// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ClientSocket.h"
#include "ServerDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CServerDlg *pServDlg)
{
	m_pServDlg = pServDlg;

}

CClientSocket::~CClientSocket()
{
	m_pServDlg = NULL;

	if (m_pFile != NULL)
		delete m_pFile;	
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

/*
 * ��ʼ��
 */
void CClientSocket::Init(void)
{
	//����CSocketFile����
	m_pFile = new CSocketFile(this);
}

/*
 * ��������
 */
void CClientSocket::OnReceive(int nErrorCode)
{
	CArchive *pArchiveIn = new CArchive(m_pFile,CArchive::load);				//���������ĵ�����
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);				//����д���ĵ�����	
	BOOL reVal = m_pServDlg->ProcessPendingRead(pArchiveIn, pArchiveOut, this);	//��������

	delete pArchiveIn;	//ɾ�������ĵ�����
	pArchiveIn = NULL;
	delete pArchiveOut;	//ɾ��д���ĵ�����
	pArchiveOut = NULL;

	if (FALSE == reVal)	//�û�������󣬻����ظ���¼
	{
		delete this;	//ɾ������
	}
}

/*
 * �ر��׽���
 */
void CClientSocket::OnClose(int nErrorCode)
{
	//֪ͨ�û����ٷ�������
	ShutDown(sends);

	//�����û�״̬
	m_pServDlg->ReleaseChatter(this);

	delete this;
}

/*
 * �����û��б�
 */
void CClientSocket::SendUserList(CChatPacket *pPacket)
{
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);	//��ʼ��д����	
	pPacket->Serialize(*pArchiveOut);
	pArchiveOut->Flush();
	delete pArchiveOut;
	pArchiveOut = NULL;
}

/*
 * ������Ϣ
 */
void CClientSocket::SendUserMsg(CChatPacket *pPacket)
{
	
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);//����д����	
	pPacket->Serialize(*pArchiveOut);	//�������� 
	pArchiveOut->Flush();
	
	delete pArchiveOut;	//ɾ��д����
	pArchiveOut = NULL;
}

/*
 * �����û���ַ
 */
void CClientSocket::SaveUserInfo(SOCKADDR_IN clientAddr)
{
	m_userInfo.m_lIP = clientAddr.sin_addr.S_un.S_addr;
	m_userInfo.m_nPort = clientAddr.sin_port;
}


/*
 * �����û�״̬��Ϣ
 */
void CClientSocket::SaveUserInfo(const CUserInfo &userInfo)
{
	m_userInfo.m_eStatus = userInfo.m_eStatus;
	m_userInfo.m_strName = userInfo.m_strName;
	m_userInfo.m_strPassword = userInfo.m_strPassword;
	m_userInfo.m_time = userInfo.m_time;
}

/*
 * �����û���Ϣ
 */
CUserInfo CClientSocket::GetUserInfo(void)
{
	return m_userInfo;
}