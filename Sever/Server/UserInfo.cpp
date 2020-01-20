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
IMPLEMENT_SERIAL(CUserInfo, CObject, 1)//newһ������Ȼ�����Ĭ�Ϲ��캯��->init

CUserInfo::CUserInfo()
{
	Init();
}

CUserInfo::~CUserInfo()
{

}

/*
 * ��ʼ��
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
 * ���л�
 */
void CUserInfo::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);//���û�������л�����
	
	if (ar.IsStoring())//��������
	{
		ar << m_strName;				//����
		ar << m_strPassword;			//����

		WORD byStatus = 0;
		byStatus = (WORD)m_eStatus;
		ar << byStatus;					//״̬

		ar << m_lIP;					//��ַ
		ar << m_nPort;					//�˿�
	
		long  lTime = m_time.GetTime();	//���ں�ʱ��
		ar << lTime;	
	}else
	{
		ar >> m_strName;				//����
		ar >> m_strPassword;			//����
	
		WORD byStatus = 0;		
		ar >> byStatus;		
		m_eStatus = (USERSTATUE)byStatus;//״̬
	
		ar >> m_lIP;					//��ַ
		ar >> m_nPort;					//�˿�
	
		long lTime;
		ar >> lTime;					//���ں�ʱ��
		m_time = CTime((time_t)lTime);
	}
}


/*
 * ��ֵ���캯��
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
 * ��ֵ���캯��
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