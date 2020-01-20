// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__F7EB6ACC_8633_4637_B8D0_9EA51F55F6C1__INCLUDED_)
#define AFX_CLIENTDLG_H__F7EB6ACC_8633_4637_B8D0_9EA51F55F6C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

#include "ChatDlg.h"
class CChatSocket;
class CListenSocket;

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CClientDlg(void);
// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_MAIN_DIALOG };
	CListCtrl	m_ctlUserList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	BOOL	Init(void);								//初始化
	BOOL	InitListCtrlSetting(void);				//初始列表控件
	BOOL	ConnectToServer(void);					//连接服务器
	BOOL	BegingListen(void);						//监听

	void	ProcessPendingAccept(void);				//接受对方连接
	void	ProcessPendingRead(void);				//接收数据包
	void	ReadPacket(void);						//接收服务器数据
	void	SendPacket(CChatPacket &packet);		//发送数据

	void	UpdateClientListCtl(void);				//更新用户列表
	void	ShowOfflineMsg(CChatPacket* pPacket);	//显示离线消息

	void	CreateUserList(CObList *pObList);		//创建用户链表
	void	CreateChatDlg(CString strUserName);		//创建聊天窗口

	void	SaveChatLog(CChatDlg *pChatDlg);		//保存聊天记录


// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg void OnDestroy();
	afx_msg void OnDblclkClientList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	SOCKADDR_IN		m_servAddr;			//服务器地址
	SOCKADDR_IN		m_hostAddr;			//本机地址
	CChatSocket		*m_pSocket;			//连接服务套接字指针
	CListenSocket	*m_pListenSocket;	//监听套接字指针
	CSocketFile		*m_pFile;			//文件指针	
	CArchive		*m_pArchiveIn;		//读文档指针
	CArchive		*m_pArchiveOut;		//写文档指针
	CUserInfo		m_UserInfo;			//用户信息
	CObList			m_UserList;			//用户列表
	CImageList		*m_pImageList;		//图标列表
	//CObList		m_ChatterList;		//chat链表
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__F7EB6ACC_8633_4637_B8D0_9EA51F55F6C1__INCLUDED_)
