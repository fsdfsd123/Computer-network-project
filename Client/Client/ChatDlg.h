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
	CChatDlg(CWnd* pParent = NULL);						//�������촰��
	CChatDlg(CWnd* pParent, const CUserInfo &peerInfo,
				const CUserInfo &userInfor);			//����/���ߴ���
	CChatDlg(CWnd* pParent, CChatPacket *pPacket);		//��ʾ������Ϣ����
	
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);		//���������Ϣ
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL	InitSocket(void);							//�����׽���
	BOOL	ConnectToPeer(void);						//���ӶԷ�
	void	ProcessPendingRead(void);					//��������

	void	DisplayRecvMessage(CChatPacket *pPacket);	//��ʾ��Ϣ
	void	DisplayOfflineMessage(CChatPacket *pPacket);//��ʾ������Ϣ
	
	void	SendPeerMessage(void);						//������Ϣ	
	
	void	InitArchive(void);							//��ʼ�������ĵ�
	void	SetPeerInfor(CUserInfo &peerInfo);			//���öԷ���Ϣ
	void	SetUserInfor(CUserInfo &userInfo);			//�����û���Ϣ
	CChatSocket *GetChatSocket(void);					//����CChatSocketָ��
protected:
 	void DisplaySentMessage();							//��ʾ���͵�������Ϣ
	void DisplaySentMessage(CChatPacket *pPacket);		//��ʾ���͵�������Ϣ


	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString		m_strRec;			//�����¼
private:
	CChatSocket	*m_pSocket;			//�׽���ָ��
	CSocketFile	*m_pFile;			//�ļ�ָ��	
	CArchive	*m_pArchiveIn;		//���ĵ�ָ��
	CArchive	*m_pArchiveOut;		//д�ĵ�ָ��
	CUserInfo	m_PeerInfo;			//�Է���Ϣ
	CUserInfo	m_UserInfo;			//�û���Ϣ
	CChatPacket *m_pOfflinePacket;	//�������ݰ�
	BOOL		m_bOfflineWnd;		//�ж��Ƿ�Ϊ��ʾ������Ϣ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__2ED704A1_5774_4073_9B3F_F005A1407CA3__INCLUDED_)
