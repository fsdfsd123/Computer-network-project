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
		ONLINE,			//在线
		OFFLINE,		//离线
		LOGIN,			//登录
		UNKNOWN			//未知
	};

public:
	void	Init(void);									//初始化
	virtual void Serialize(CArchive& ar);				//序列化 
	CUserInfo &operator = (const CUserInfo &userInfo);	//赋值函数
	
public:
	CString		m_strName;		//名称
	CString		m_strPassword;	//密码
	USERSTATUE	m_eStatus;		//状态
	DWORD		m_lIP;			//IP地址
	SHORT		m_nPort;		//端口
	CTime		m_time;			//日期和时间
};

#endif // !defined(AFX_USERINFO_H__6FE7D9D8_ED87_4452_98C6_DB61D2BD6196__INCLUDED_)
