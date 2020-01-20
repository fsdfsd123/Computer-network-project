// UserInfo.h: interface for the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINFO_H__6FE7D9D8_ED87_4452_98C6_DB61D2BD6196__INCLUDED_)
#define AFX_USERINFO_H__6FE7D9D8_ED87_4452_98C6_DB61D2BD6196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserInfo : public CObject  
{
public:
	CUserInfo();
	CUserInfo(const CUserInfo& userInfo);
	virtual ~CUserInfo();
	DECLARE_SERIAL(CUserInfo)

	enum USERSTATUE{
		ONLINE,			//����
		OFFLINE,		//����
		LOGIN,			//��¼
		UNKNOWN			//δ֪
	};

public:
	void	Init(void);									//��ʼ��
	virtual void Serialize(CArchive& ar);				//���л� 
	CUserInfo &operator = (const CUserInfo &userInfo);	//��ֵ����
	
public:
	CString		m_strName;		//����
	CString		m_strPassword;	//����
	USERSTATUE	m_eStatus;		//״̬
	DWORD		m_lIP;			//IP��ַ
	SHORT		m_nPort;		//�˿�
	CTime		m_time;			//���ں�ʱ��
};

#endif // !defined(AFX_USERINFO_H__6FE7D9D8_ED87_4452_98C6_DB61D2BD6196__INCLUDED_)
