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
	BOOL	Init(void);								//��ʼ��
	BOOL	InitListCtrlSetting(void);				//��ʼ�б�ؼ�
	BOOL	ConnectToServer(void);					//���ӷ�����
	BOOL	BegingListen(void);						//����

	void	ProcessPendingAccept(void);				//���ܶԷ�����
	void	ProcessPendingRead(void);				//�������ݰ�
	void	ReadPacket(void);						//���շ���������
	void	SendPacket(CChatPacket &packet);		//��������

	void	UpdateClientListCtl(void);				//�����û��б�
	void	ShowOfflineMsg(CChatPacket* pPacket);	//��ʾ������Ϣ

	void	CreateUserList(CObList *pObList);		//�����û�����
	void	CreateChatDlg(CString strUserName);		//�������촰��

	void	SaveChatLog(CChatDlg *pChatDlg);		//���������¼


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
	SOCKADDR_IN		m_servAddr;			//��������ַ
	SOCKADDR_IN		m_hostAddr;			//������ַ
	CChatSocket		*m_pSocket;			//���ӷ����׽���ָ��
	CListenSocket	*m_pListenSocket;	//�����׽���ָ��
	CSocketFile		*m_pFile;			//�ļ�ָ��	
	CArchive		*m_pArchiveIn;		//���ĵ�ָ��
	CArchive		*m_pArchiveOut;		//д�ĵ�ָ��
	CUserInfo		m_UserInfo;			//�û���Ϣ
	CObList			m_UserList;			//�û��б�
	CImageList		*m_pImageList;		//ͼ���б�
	//CObList		m_ChatterList;		//chat����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__F7EB6ACC_8633_4637_B8D0_9EA51F55F6C1__INCLUDED_)
