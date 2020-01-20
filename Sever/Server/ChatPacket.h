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

	enum PACKETTYPE		//包的类型							
	{
			MESSAGE,	//聊天消息
			USERLIST,	//用户列表
			SERVERMSG,	//服务器发送给用户的消息
			UNKNOWN		//未知类型
	};
	
public:
	void	Init(void);						//初始化成员变量
	virtual void Serialize(CArchive& ar);	//序列化  
	
public:
	PACKETTYPE	m_type;				//包类型
	CUserInfo	m_UserInfo;			//用户信息
	CString		m_strMsg;			//消息
	CObList		*m_pUserList;		//用户链表
	CUserInfo	m_OfflineUserInfo;	//离线用户信息
	CTime		m_time;				//日期和时间
};

#endif // !defined(AFX_CHATPACKET_H__7F5E67FB_273B_4679_8A59_23575978DDCC__INCLUDED_)
