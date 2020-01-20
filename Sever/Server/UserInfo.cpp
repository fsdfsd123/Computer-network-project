// UserInfo.cpp: implementation of the CUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNCREATE(CUserInfo, CObject)
IMPLEMENT_SERIAL(CUserInfo, CObject, 1)//new一个对象然后调用默认构造函数->init

CUserInfo::CUserInfo()
{
	Init();
}

CUserInfo::~CUserInfo()
{

}

/*
 * 初始化
 */
void CUserInfo::Init(void)
{
	m_strName = _T("");
	m_strPassword = _T("");
	m_eStatus = UNKNOWN;
	m_lIP = 0;
	m_nPort = 0;
	m_time = CTime::GetCurrentTime();	
}


/*
 * 序列化
 */
void CUserInfo::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);//调用基类的序列化函数
	
	if (ar.IsStoring())//发送数据
	{
		ar << m_strName;				//名字
		ar << m_strPassword;			//密码

		WORD byStatus = 0;
		byStatus = (WORD)m_eStatus;
		ar << byStatus;					//状态

		ar << m_lIP;					//地址
		ar << m_nPort;					//端口
	
		long  lTime = m_time.GetTime();	//日期和时间
		ar << lTime;	
	}else
	{
		ar >> m_strName;				//名字
		ar >> m_strPassword;			//密码
	
		WORD byStatus = 0;		
		ar >> byStatus;		
		m_eStatus = (USERSTATUE)byStatus;//状态
	
		ar >> m_lIP;					//地址
		ar >> m_nPort;					//端口
	
		long lTime;
		ar >> lTime;					//日期和时间
		m_time = CTime((time_t)lTime);
	}
}


/*
 * 赋值构造函数
 */
CUserInfo &CUserInfo::operator = (const CUserInfo &userInfo)
{
	m_strName = userInfo.m_strName;
	m_strPassword = userInfo.m_strPassword;
	m_eStatus = userInfo.m_eStatus;
	m_lIP = userInfo.m_lIP;
	m_nPort = userInfo.m_nPort;
	m_time = userInfo.m_time;

	return *this;
}

/*
 * 赋值构造函数
 */
CUserInfo::CUserInfo(const CUserInfo& userInfo)
{
	m_strName = userInfo.m_strName;
	m_strPassword = userInfo.m_strPassword;
	m_eStatus = userInfo.m_eStatus;
	m_lIP = userInfo.m_lIP;
	m_nPort = userInfo.m_nPort;
	m_time = userInfo.m_time;
}