// ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Client.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog


CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent),m_strRec("")
{
	//{{AFX_DATA_INIT(CChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSocket = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	m_bOfflineWnd = FALSE;
	m_pOfflinePacket = NULL;

}

/*
 * ����/���ߴ���
 */
CChatDlg::CChatDlg(CWnd* pParent, const CUserInfo &peerInfo, const CUserInfo &userInfor)
:CDialog(CChatDlg::IDD, pParent),m_strRec("")
{
	m_PeerInfo = peerInfo;	//�Է��û���Ϣ
	m_UserInfo = userInfor;	//�û���Ϣ
	m_pSocket = NULL;		//CChatSocketָ��
	m_pFile = NULL;			//�ļ�����
	m_pArchiveIn = NULL;	//�����ĵ�����
	m_pArchiveOut = NULL;	//д���ĵ�����
	m_pOfflinePacket = NULL;//�������ݰ�
	m_bOfflineWnd = FALSE;	//�������ߴ���
}

/*
 * ��ʾ������Ϣ����
 */
CChatDlg::CChatDlg(CWnd* pParent, CChatPacket *pPacket)
:CDialog(CChatDlg::IDD, pParent),m_strRec("")
{
	m_pOfflinePacket = pPacket;	//�������ݰ�
	m_bOfflineWnd = TRUE;		//�������ߴ���

	m_pSocket = NULL;			//CChatSocketָ��
	m_pFile = NULL;				//�ļ�����
	m_pArchiveIn = NULL;		//�����ĵ�����
	m_pArchiveOut = NULL;		//д���ĵ�����

}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_CLIENT_CHAT_OUTPUT_EDIT, m_ctlOutput);
	DDX_Control(pDX, IDC_CLIENT_CHAT_IPUT_EDIT, m_ctlInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers
/*
 * ������Ϣ
 */
void CChatDlg::ProcessPendingRead(void)
{
	do 
	{		
		CChatPacket chatPact;
		chatPact.Serialize(*m_pArchiveIn);		
		if (CChatPacket::MESSAGE == chatPact.m_type)//��Ϣ
		{		
			
			DisplayRecvMessage(&chatPact);	
		}	
		
	} while(!m_pArchiveIn->IsBufferEmpty());
}

/*
 * WM_INITDIALOG��Ϣ��Ӧ����
 */
BOOL CChatDlg::OnInitDialog() 
{
	//���û��ຯ��
	CDialog::OnInitDialog();	

	if (NULL != m_pOfflinePacket && TRUE == m_bOfflineWnd)	//��ʾ������Ϣ
	{
		DisplayOfflineMessage(m_pOfflinePacket);	
		
	}else if (m_PeerInfo.m_eStatus == CUserInfo::ONLINE)	//��������
	{
		SetWindowText(_T("��") + m_PeerInfo.m_strName + _T("������"));
		
	}else													//����������Ϣ
	{
		SetWindowText(m_PeerInfo.m_strName + _T("������Ŷ"));
	}	
	return TRUE;  
}

/*
 * ���ӶԷ�
 */
BOOL CChatDlg::ConnectToPeer(void)
{
	//����CChatSocket����
	if(!InitSocket())
	{
		return FALSE;
	}

	//�����׽��֣���
	if (!m_pSocket->Create())
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return FALSE;
	}

	m_pFile = new CSocketFile(m_pSocket);				//�����ļ�����
 	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);//���������ĵ�����
	
	
	//���ӶԷ�
	SOCKADDR_IN peerAddr;
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.S_un.S_addr = m_PeerInfo.m_lIP;
	peerAddr.sin_port = m_PeerInfo.m_nPort;
	while (!m_pSocket->Connect((SOCKADDR*)&peerAddr, sizeof(SOCKADDR_IN)))
	{
		if (AfxMessageBox(_T("���ӶԷ�ʧ�ܣ��Ƿ��ٴγ������ӡ�"),MB_YESNO) == IDNO)
		{
			return FALSE;
		}
	}	
	return TRUE;
}

BOOL CChatDlg::InitSocket(void)
{
	//�����׽���
	m_pSocket = new CChatSocket(this);
	return TRUE;
}

/*
 * ��ʾ����������յ�����Ϣ
 */
void CChatDlg::DisplayRecvMessage(CChatPacket *pPacket)
{
	CString chatHeader = pPacket->m_UserInfo.m_strName;//������Ϣ���û�����

	//��ʽ�����ں�ʱ��
	CTime timeChatting(pPacket->m_time);				
	int year = timeChatting.GetYear();
	int month = timeChatting.GetMonth();
	int day = timeChatting.GetDay();
	int hour = timeChatting.GetHour();
	int minute = timeChatting.GetMinute();
	int second = timeChatting.GetSecond();
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);

	//��ʾ��Ϣͷ
	chatHeader += chatTime;
	CString strItem = chatHeader + "\r\n"+ pPacket->m_strMsg + "\r\n";

	//��ʾ��Ϣ
	int len = m_ctlOutput.GetWindowTextLength();
	m_ctlOutput.SetSel(len,len);
	m_ctlOutput.ReplaceSel(strItem);

}

/*
 * ���������Ϣ
 */
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_ctlInput.GetSafeHwnd())	//������Ϣ����    
	{  
		if (pMsg->wParam == VK_RETURN)			//return��  
		{ 		
			if (TRUE == m_bOfflineWnd )			//��ʾ������Ϣ����
			{
				return TRUE;

			}else								//����/�������촰��
			{
				SendPeerMessage();				//������Ϣ
				m_ctlInput.SetWindowText("");	//���������Ϣ��������
			}

			return TRUE;
		}  
	}  
	return CDialog::PreTranslateMessage(pMsg); //������Ϣ
}


/*
 * ������Ϣ
 */
void CChatDlg::SendPeerMessage( void )
{
	//������Ϣ��������Ϊ��
	if (0 == m_ctlInput.GetWindowTextLength())
	{
		return;
	}
	
	//�Է�����
	if(m_PeerInfo.m_eStatus == CUserInfo::ONLINE)	//����������Ϣ
	{
		CChatPacket packet;
		packet.m_UserInfo = this->m_UserInfo;		//������Ϣ�û�����
		packet.m_type = CChatPacket::MESSAGE;		//��������
		m_ctlInput.GetWindowText(packet.m_strMsg);	//��Ϣ
		packet.m_time = CTime::GetCurrentTime();	//����ʱ��
		
		//������Ϣ
		m_pArchiveOut = new CArchive(m_pFile,CArchive::store);//����д���ĵ�����	
		packet.Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;	
		
		//��ʾ���͵���Ϣ
		DisplaySentMessage(&packet);

	}else//�Է�����
	{
		//��ʾ���͵�������Ϣ
		DisplaySentMessage();
	}	
	
}


/*
 * ��ʾ���͵���Ϣ
 */
void CChatDlg::DisplaySentMessage(CChatPacket *pPacket)
{
	CString chatHeader = pPacket->m_UserInfo.m_strName;	//������Ϣ�û�����

	//��ʽ��������Ϣ��ʱ��
	CTime timeChatting =  pPacket->m_time;				
	int year = timeChatting.GetYear();		//��
	int month = timeChatting.GetMonth();	//��
	int day = timeChatting.GetDay();		//��
	int hour = timeChatting.GetHour();		//ʱ
	int minute = timeChatting.GetMinute();	//��
	int second = timeChatting.GetSecond();	//��
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);
	
	chatHeader += chatTime;//��Ϣͷ
	//��ȡ������Ϣ
	CString strMsg;
	m_ctlInput.GetWindowText(strMsg);

	//��Ϣͷ����Ϣ������ʾ
	CString strItem = chatHeader + "\r\n"+ pPacket->m_strMsg + "\r\n";

	//�ڵ�ǰ��Ϣ��������ʾ��Ϣ
	int len = m_ctlOutput.GetWindowTextLength();//��ǰ�ı�����
	m_ctlOutput.SetSel(len,len);				//�ƶ����������ǰ�ı����
	m_ctlOutput.ReplaceSel(strItem);			//�ڵ�ǰ�ı��������Ϣ
}

/*
 * ��ʾ���͵���Ϣ
 */
void CChatDlg::DisplaySentMessage(void)
{
	CString chatHeader = m_UserInfo.m_strName;
	CTime timeChatting =  CTime::GetCurrentTime();
	int year = timeChatting.GetYear();		//��
	int month = timeChatting.GetMonth();	//��
	int day = timeChatting.GetDay();		//��
	int hour = timeChatting.GetHour();		//ʱ
	int minute = timeChatting.GetMinute();	//��
	int second = timeChatting.GetSecond();	//��
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);

	
	chatHeader += chatTime;//��Ϣͷ

	CString strMsg;
	m_ctlInput.GetWindowText(strMsg);
	CString strItem = chatHeader + "\r\n"+ strMsg + "\r\n";

	//��ʾ��Ϣ
	int len = m_ctlOutput.GetWindowTextLength();
	m_ctlOutput.SetSel(len,len);
	m_ctlOutput.ReplaceSel(strItem);

}


void CChatDlg::InitArchive(void)
{
	//��ʼ������������
	m_pFile = new CSocketFile(m_pSocket);					//��ʼ��CSocketFile
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);	//��ʼ��������
}

void CChatDlg::SetPeerInfor( CUserInfo &peerInfo )
{
	m_PeerInfo = peerInfo;
}

void CChatDlg::SetUserInfor( CUserInfo &userInfo )
{
	m_UserInfo = userInfo;
}


void CChatDlg::OnDestroy() 
{
	m_ctlOutput.GetWindowText(m_strRec);//�����¼
	CDialog::OnDestroy();

}

CChatDlg::~CChatDlg()
{
	if(FALSE == m_bOfflineWnd)
	{
		if (NULL != m_pArchiveIn)
		{
			delete m_pArchiveIn;
			m_pArchiveIn = NULL;
		}
		if (NULL != m_pFile)
		{
			delete m_pFile;
			m_pFile = NULL;
		}	
		
		if (NULL != m_pSocket)
		{
			delete m_pSocket;
			m_pSocket = NULL;
		}		
	}
}

CChatSocket *CChatDlg::GetChatSocket( void )
{
	return m_pSocket;
}

/*
 * ��ʾ������Ϣ
 */
void CChatDlg::DisplayOfflineMessage( CChatPacket *pPacket )
{
	CRect inputWndRect;
	m_ctlInput.GetWindowRect(&inputWndRect);//��ȡ������Ϣ��������		
	m_ctlInput.MoveWindow(0, 0, 0, 0);		//���øô��ڴ�СΪ0
	
	CRect chatDlgRect;
	GetClientRect(&chatDlgRect);			//��ȡ�Ի���ͻ�����С
	MoveWindow(0, 0, chatDlgRect.Width(),	//�޸ĶԻ���ĸ߶�
		chatDlgRect.Height() - inputWndRect.Height());
	
	GetClientRect(&chatDlgRect);			//��ȡ�Ի���ͻ�����С
	m_ctlOutput.MoveWindow(5, 5,			//�޸���ʾ��Ϣ���ڸ߶�
		chatDlgRect.Width() -10, chatDlgRect.Height() -10);
	
	m_PeerInfo = pPacket->m_UserInfo;								//������Ϣ�û�����							
	SetWindowText(m_PeerInfo.m_strName + _T("���͸�����������Ϣ"));	 //���ñ���
	m_ctlOutput.SetWindowText(pPacket->m_strMsg );					//������Ϣ
}