// ChatPacket.cpp: implementation of the CChatPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNCREATE(CChatPacket, CObject)
IMPLEMENT_SERIAL(CChatPacket, CObject, 1 )


CChatPacket::CChatPacket()
{
	Init();

}

CChatPacket::~CChatPacket()
{
}

/*
 * 初始化
 */
void CChatPacket::Init(void)
{
	m_type = UNKNOWN;			//为知类型
	m_strMsg = _T("");			//清空
	m_pUserList = NULL;			//清空	
	m_time = CTime::GetCurrentTime();	
}

/*
 * 序列化
 */
void CChatPacket::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);//调用基类的序列化函数

	if (ar.IsStoring())//发送数据
	{
		BYTE byType = m_type;
		ar << byType;					//包的类型
		ar << m_strMsg;					//消息
	
		long  lTime = m_time.GetTime();	//日期和时间
		ar << lTime;
	}else//接收数据
	{
		BYTE byType;			
		ar >> byType;	
		m_type = (PACKETTYPE)byType;	//包的类型
		ar >> m_strMsg;					//消息
		
		long lTime;
		ar >> lTime;					//日期和时间
		m_time = CTime((time_t)lTime);
	}

	m_UserInfo.Serialize(ar);			//序列化用户信息
	m_OfflineUserInfo.Serialize(ar);	//序列化离线用户信息

	if (m_type == USERLIST && NULL != m_pUserList)//序列化用户列表
	{
		m_pUserList->Serialize(ar);			
	}
}
