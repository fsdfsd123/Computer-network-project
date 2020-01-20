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
	CServerDlg(CWnd* pParent = NULL);	//���캯��
	virtual ~CServerDlg();				//��������
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
	BOOL	Init(void);											//��ʼ��
	BOOL	StartService(void);									//��������
	BOOL	ProcessPendingRead(CArchive *pArchiveIn,
			CArchive *pArchiveOut,CClientSocket *pClientSocket);//��������
	void	ProcessPendingAccept(void);							//���ܿͻ�����������	

	void	ReleaseChatter(CClientSocket *pClientSocket);		//���û�����ʱɾ���û�
	void	DeleteChatter(CClientSocket *pClientSocket);		//�û���¼�����������ظ���¼ʱɾ���û�
	void	DeleteAllChatter(void);								//ɾ�����������û�

protected:

	void	InitListCtrlSetting(void);							//�����б�ؼ�
	void	InitUserList(void);									//��ʼ���û��б�

	void	SaveUserList(CObList &obList);						//�����û���Ϣ
	void	LoadUserList(CObList &obList);						//��ȡ�û���Ϣ

	void	SaveOfflineMsg(CObList &obList);					//����������Ϣ
	void	LoadOfflineMsg(CObList &obList);					//��ȡ������Ϣ

	void	SendUserList(void);									//�����û��б�
	void	CopyUserList(CObList &obList);						//����������
	void	DeleteTempUserList(CObList &obList);				//ɾ����ʱ����

	BOOL	UpdateUserList(const CUserInfo &userInfo, CClientSocket *pClientSocket);//�����û�����
	void	TransmitMsg(const CChatPacket &packet, CClientSocket *pClientSocket);	//ת��������Ϣ
	void	UpdateServerListCtl(const CObList &obList);								//���·���������

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

	CListenSocket	*m_pListenSocket;	//CListenSocketָ��
	CImageList		*m_pImageList;		//ͼ���б�ָ��
	
	CObList			m_UserList;			//�û�����
	CObList			m_OfflineMsgList;	//������Ϣ����
	CObList			m_ChatterList;		//�����û�����

	CCriticalSection	m_csUserList;	//�����û������ٽ�������
	CCriticalSection	m_csOfflineList;//����������Ϣ�����ٽ�������
	CCriticalSection	m_csChatterList;//���������û������ٽ�������
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__A83EBE53_9C93_4AEE_AED5_5ED55BB94A31__INCLUDED_)
