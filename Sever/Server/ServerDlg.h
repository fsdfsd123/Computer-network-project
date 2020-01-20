// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__A83EBE53_9C93_4AEE_AED5_5ED55BB94A31__INCLUDED_)
#define AFX_SERVERDLG_H__A83EBE53_9C93_4AEE_AED5_5ED55BB94A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListenSocket;
class CClientSocket;
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


class CServerDlg : public CDialog
{

// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	//构造函数
	virtual ~CServerDlg();				//析构函数
// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_MAIN_DIALOG };
	CListCtrl	m_ctlUserList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	Init(void);											//初始化
	BOOL	StartService(void);									//启动服务
	BOOL	ProcessPendingRead(CArchive *pArchiveIn,
			CArchive *pArchiveOut,CClientSocket *pClientSocket);//接收数据
	void	ProcessPendingAccept(void);							//接受客户端连接请求	

	void	ReleaseChatter(CClientSocket *pClientSocket);		//当用户离线时删除用户
	void	DeleteChatter(CClientSocket *pClientSocket);		//用户登录密码错误或者重复登录时删除用户
	void	DeleteAllChatter(void);								//删除所有在线用户

protected:

	void	InitListCtrlSetting(void);							//定义列表控件
	void	InitUserList(void);									//初始化用户列表

	void	SaveUserList(CObList &obList);						//保存用户信息
	void	LoadUserList(CObList &obList);						//读取用户信息

	void	SaveOfflineMsg(CObList &obList);					//保存离线消息
	void	LoadOfflineMsg(CObList &obList);					//读取离线消息

	void	SendUserList(void);									//发送用户列表
	void	CopyUserList(CObList &obList);						//复制用链表
	void	DeleteTempUserList(CObList &obList);				//删除临时链表

	BOOL	UpdateUserList(const CUserInfo &userInfo, CClientSocket *pClientSocket);//更新用户链表
	void	TransmitMsg(const CChatPacket &packet, CClientSocket *pClientSocket);	//转发离线消息
	void	UpdateServerListCtl(const CObList &obList);								//更新服务器界面

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnRclickClientList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnServerListDeleteMenuitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CListenSocket	*m_pListenSocket;	//CListenSocket指针
	CImageList		*m_pImageList;		//图标列表指针
	
	CObList			m_UserList;			//用户链表
	CObList			m_OfflineMsgList;	//离线信息链表
	CObList			m_ChatterList;		//在线用户链表

	CCriticalSection	m_csUserList;	//保护用户链表临界区对象
	CCriticalSection	m_csOfflineList;//保护离线信息链表临界区对象
	CCriticalSection	m_csChatterList;//保护在线用户链表临界区对象
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__A83EBE53_9C93_4AEE_AED5_5ED55BB94A31__INCLUDED_)
