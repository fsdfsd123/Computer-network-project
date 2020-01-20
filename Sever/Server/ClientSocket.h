#if !defined(AFX_CLIENTSOCKET_H__6DC91793_99EA_4206_B699_FB322092824B__INCLUDED_)
#define AFX_CLIENTSOCKET_H__6DC91793_99EA_4206_B699_FB322092824B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//


class CServerDlg;

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket(CServerDlg *pServDlg);
	virtual ~CClientSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation


// Operations
public:
 	void	Init();									//初始化
	void	SendUserList(CChatPacket *pPacket);		//发送用户列表
	void	SendUserMsg(CChatPacket *pPacket);		//发送消息
	void	SaveUserInfo(SOCKADDR_IN clientAddr);	//保存用户地址
	void	SaveUserInfo(const CUserInfo &userInfo);//保存用户状态信息	
	CUserInfo	GetUserInfo(void);					//获取用户信息

protected:
	virtual void OnReceive(int nErrorCode);			//接收数据
	virtual void OnClose(int nErrorCode);			//关闭套接字

protected:
	CServerDlg		*m_pServDlg;					//CServerDlg指针
 	CSocketFile		*m_pFile;						//文件指针
	CUserInfo		m_userInfo;						//用户信息
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__6DC91793_99EA_4206_B699_FB322092824B__INCLUDED_)
