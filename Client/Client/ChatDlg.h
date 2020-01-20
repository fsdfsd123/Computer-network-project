#if !defined(AFX_CHATDLG_H__2ED704A1_5774_4073_9B3F_F005A1407CA3__INCLUDED_)
#define AFX_CHATDLG_H__2ED704A1_5774_4073_9B3F_F005A1407CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog
#include "resource.h"
#include "ChatSocket.h"
class CChatDlg : public CDialog
{
// Construction
public:
	CChatDlg(CWnd* pParent = NULL);						//被动聊天窗口
	CChatDlg(CWnd* pParent, const CUserInfo &peerInfo,
				const CUserInfo &userInfor);			//在线/离线窗口
	CChatDlg(CWnd* pParent, CChatPacket *pPacket);		//显示离线消息窗口
	
	virtual ~CChatDlg();
// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_CLIENT_CHAT_DIALOG };
	CEdit	m_ctlOutput;
	CEdit	m_ctlInput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);		//处理键盘消息
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL	InitSocket(void);							//创建套接字
	BOOL	ConnectToPeer(void);						//连接对方
	void	ProcessPendingRead(void);					//接收数据

	void	DisplayRecvMessage(CChatPacket *pPacket);	//显示消息
	void	DisplayOfflineMessage(CChatPacket *pPacket);//显示离线消息
	
	void	SendPeerMessage(void);						//发送消息	
	
	void	InitArchive(void);							//初始化读入文档
	void	SetPeerInfor(CUserInfo &peerInfo);			//设置对方信息
	void	SetUserInfor(CUserInfo &userInfo);			//设置用户信息
	CChatSocket *GetChatSocket(void);					//返回CChatSocket指针
protected:
 	void DisplaySentMessage();							//显示发送的离线消息
	void DisplaySentMessage(CChatPacket *pPacket);		//显示发送的在线消息


	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString		m_strRec;			//聊天记录
private:
	CChatSocket	*m_pSocket;			//套接字指针
	CSocketFile	*m_pFile;			//文件指针	
	CArchive	*m_pArchiveIn;		//读文档指针
	CArchive	*m_pArchiveOut;		//写文档指针
	CUserInfo	m_PeerInfo;			//对方信息
	CUserInfo	m_UserInfo;			//用户信息
	CChatPacket *m_pOfflinePacket;	//离线数据包
	BOOL		m_bOfflineWnd;		//判断是否为显示离线消息窗口
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__2ED704A1_5774_4073_9B3F_F005A1407CA3__INCLUDED_)
