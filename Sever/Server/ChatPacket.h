// ChatPacket.h: interface for the CChatPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATPACKET_H__7F5E67FB_273B_4679_8A59_23575978DDCC__INCLUDED_)
#define AFX_CHATPACKET_H__7F5E67FB_273B_4679_8A59_23575978DDCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "UserInfo.h"

class CUserInfo;

class CChatPacket : public CObject  
{
public:
	CChatPacket();
	virtual ~CChatPacket();
	DECLARE_SERIAL( CChatPacket )

	enum PACKETTYPE		//��������							
	{
			MESSAGE,	//������Ϣ
			USERLIST,	//�û��б�
			SERVERMSG,	//���������͸��û�����Ϣ
			UNKNOWN		//δ֪����
	};
	
public:
	void	Init(void);						//��ʼ����Ա����
	virtual void Serialize(CArchive& ar);	//���л�  
	
public:
	PACKETTYPE	m_type;				//������
	CUserInfo	m_UserInfo;			//�û���Ϣ
	CString		m_strMsg;			//��Ϣ
	CObList		*m_pUserList;		//�û�����
	CUserInfo	m_OfflineUserInfo;	//�����û���Ϣ
	CTime		m_time;				//���ں�ʱ��
};

#endif // !defined(AFX_CHATPACKET_H__7F5E67FB_273B_4679_8A59_23575978DDCC__INCLUDED_)
