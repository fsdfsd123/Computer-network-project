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
 * 初始化
 */
void CClientSocket::Init(void)
{
	//创建CSocketFile对象
	m_pFile = new CSocketFile(this);
}

/*
 * 接收数据
 */
void CClientSocket::OnReceive(int nErrorCode)
{
	CArchive *pArchiveIn = new CArchive(m_pFile,CArchive::load);				//创建读入文档对象
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);				//创建写入文档对象	
	BOOL reVal = m_pServDlg->ProcessPendingRead(pArchiveIn, pArchiveOut, this);	//接收数据

	delete pArchiveIn;	//删除读入文档对象
	pArchiveIn = NULL;
	delete pArchiveOut;	//删除写入文档对象
	pArchiveOut = NULL;

	if (FALSE == reVal)	//用户密码错误，或者重复登录
	{
		delete this;	//删除自身
	}
}

/*
 * 关闭套接字
 */
void CClientSocket::OnClose(int nErrorCode)
{
	//通知用户不再发送数据
	ShutDown(sends);

	//更新用户状态
	m_pServDlg->ReleaseChatter(this);

	delete this;
}

/*
 * 发送用户列表
 */
void CClientSocket::SendUserList(CChatPacket *pPacket)
{
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);	//初始化写入流	
	pPacket->Serialize(*pArchiveOut);
	pArchiveOut->Flush();
	delete pArchiveOut;
	pArchiveOut = NULL;
}

/*
 * 发送消息
 */
void CClientSocket::SendUserMsg(CChatPacket *pPacket)
{
	
	CArchive *pArchiveOut = new CArchive(m_pFile,CArchive::store);//创建写入流	
	pPacket->Serialize(*pArchiveOut);	//发送数据 
	pArchiveOut->Flush();
	
	delete pArchiveOut;	//删除写入流
	pArchiveOut = NULL;
}

/*
 * 保存用户地址
 */
void CClientSocket::SaveUserInfo(SOCKADDR_IN clientAddr)
{
	m_userInfo.m_lIP = clientAddr.sin_addr.S_un.S_addr;
	m_userInfo.m_nPort = clientAddr.sin_port;
}


/*
 * 保存用户状态信息
 */
void CClientSocket::SaveUserInfo(const CUserInfo &userInfo)
{
	m_userInfo.m_eStatus = userInfo.m_eStatus;
	m_userInfo.m_strName = userInfo.m_strName;
	m_userInfo.m_strPassword = userInfo.m_strPassword;
	m_userInfo.m_time = userInfo.m_time;
}

/*
 * 保存用户信息
 */
CUserInfo CClientSocket::GetUserInfo(void)
{
	return m_userInfo;
}