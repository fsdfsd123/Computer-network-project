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
 * ��ʼ��
 */
void CChatPacket::Init(void)
{
	m_type = UNKNOWN;			//Ϊ֪����
	m_strMsg = _T("");			//���
	m_pUserList = NULL;			//���	
	m_time = CTime::GetCurrentTime();	
}

/*
 * ���л�
 */
void CChatPacket::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);//���û�������л�����

	if (ar.IsStoring())//��������
	{
		BYTE byType = m_type;
		ar << byType;					//��������
		ar << m_strMsg;					//��Ϣ
	
		long  lTime = m_time.GetTime();	//���ں�ʱ��
		ar << lTime;
	}else//��������
	{
		BYTE byType;			
		ar >> byType;	
		m_type = (PACKETTYPE)byType;	//��������
		ar >> m_strMsg;					//��Ϣ
		
		long lTime;
		ar >> lTime;					//���ں�ʱ��
		m_time = CTime((time_t)lTime);
	}

	m_UserInfo.Serialize(ar);			//���л��û���Ϣ
	m_OfflineUserInfo.Serialize(ar);	//���л������û���Ϣ

	if (m_type == USERLIST && NULL != m_pUserList)//���л��û��б�
	{
		m_pUserList->Serialize(ar);			
	}
}
