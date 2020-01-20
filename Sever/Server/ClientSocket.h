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
 	void	Init();									//��ʼ��
	void	SendUserList(CChatPacket *pPacket);		//�����û��б�
	void	SendUserMsg(CChatPacket *pPacket);		//������Ϣ
	void	SaveUserInfo(SOCKADDR_IN clientAddr);	//�����û���ַ
	void	SaveUserInfo(const CUserInfo &userInfo);//�����û�״̬��Ϣ	
	CUserInfo	GetUserInfo(void);					//��ȡ�û���Ϣ

protected:
	virtual void OnReceive(int nErrorCode);			//��������
	virtual void OnClose(int nErrorCode);			//�ر��׽���

protected:
	CServerDlg		*m_pServDlg;					//CServerDlgָ��
 	CSocketFile		*m_pFile;						//�ļ�ָ��
	CUserInfo		m_userInfo;						//�û���Ϣ
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__6DC91793_99EA_4206_B699_FB322092824B__INCLUDED_)
