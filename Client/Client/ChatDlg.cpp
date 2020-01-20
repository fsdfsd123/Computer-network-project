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
 * 在线/离线窗口
 */
CChatDlg::CChatDlg(CWnd* pParent, const CUserInfo &peerInfo, const CUserInfo &userInfor)
:CDialog(CChatDlg::IDD, pParent),m_strRec("")
{
	m_PeerInfo = peerInfo;	//对方用户信息
	m_UserInfo = userInfor;	//用户信息
	m_pSocket = NULL;		//CChatSocket指针
	m_pFile = NULL;			//文件对象
	m_pArchiveIn = NULL;	//读入文档对象
	m_pArchiveOut = NULL;	//写入文档对象
	m_pOfflinePacket = NULL;//离线数据包
	m_bOfflineWnd = FALSE;	//接收离线窗口
}

/*
 * 显示离线消息窗口
 */
CChatDlg::CChatDlg(CWnd* pParent, CChatPacket *pPacket)
:CDialog(CChatDlg::IDD, pParent),m_strRec("")
{
	m_pOfflinePacket = pPacket;	//离线数据包
	m_bOfflineWnd = TRUE;		//接收离线窗口

	m_pSocket = NULL;			//CChatSocket指针
	m_pFile = NULL;				//文件对象
	m_pArchiveIn = NULL;		//读入文档对象
	m_pArchiveOut = NULL;		//写入文档对象

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
 * 接收消息
 */
void CChatDlg::ProcessPendingRead(void)
{
	do 
	{		
		CChatPacket chatPact;
		chatPact.Serialize(*m_pArchiveIn);		
		if (CChatPacket::MESSAGE == chatPact.m_type)//消息
		{		
			
			DisplayRecvMessage(&chatPact);	
		}	
		
	} while(!m_pArchiveIn->IsBufferEmpty());
}

/*
 * WM_INITDIALOG消息响应函数
 */
BOOL CChatDlg::OnInitDialog() 
{
	//调用基类函数
	CDialog::OnInitDialog();	

	if (NULL != m_pOfflinePacket && TRUE == m_bOfflineWnd)	//显示离线消息
	{
		DisplayOfflineMessage(m_pOfflinePacket);	
		
	}else if (m_PeerInfo.m_eStatus == CUserInfo::ONLINE)	//在线聊天
	{
		SetWindowText(_T("与") + m_PeerInfo.m_strName + _T("聊天中"));
		
	}else													//发送离线消息
	{
		SetWindowText(m_PeerInfo.m_strName + _T("不在线哦"));
	}	
	return TRUE;  
}

/*
 * 连接对方
 */
BOOL CChatDlg::ConnectToPeer(void)
{
	//创建CChatSocket对象
	if(!InitSocket())
	{
		return FALSE;
	}

	//创建套接字，绑定
	if (!m_pSocket->Create())
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(_T("创建套接字失败！"));
		return FALSE;
	}

	m_pFile = new CSocketFile(m_pSocket);				//创建文件对象
 	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);//创建读入文档对象
	
	
	//连接对方
	SOCKADDR_IN peerAddr;
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.S_un.S_addr = m_PeerInfo.m_lIP;
	peerAddr.sin_port = m_PeerInfo.m_nPort;
	while (!m_pSocket->Connect((SOCKADDR*)&peerAddr, sizeof(SOCKADDR_IN)))
	{
		if (AfxMessageBox(_T("连接对方失败，是否再次尝试连接。"),MB_YESNO) == IDNO)
		{
			return FALSE;
		}
	}	
	return TRUE;
}

BOOL CChatDlg::InitSocket(void)
{
	//创建套接字
	m_pSocket = new CChatSocket(this);
	return TRUE;
}

/*
 * 显示主动聊天接收到的消息
 */
void CChatDlg::DisplayRecvMessage(CChatPacket *pPacket)
{
	CString chatHeader = pPacket->m_UserInfo.m_strName;//发送消息的用户名称

	//格式化日期和时间
	CTime timeChatting(pPacket->m_time);				
	int year = timeChatting.GetYear();
	int month = timeChatting.GetMonth();
	int day = timeChatting.GetDay();
	int hour = timeChatting.GetHour();
	int minute = timeChatting.GetMinute();
	int second = timeChatting.GetSecond();
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);

	//显示消息头
	chatHeader += chatTime;
	CString strItem = chatHeader + "\r\n"+ pPacket->m_strMsg + "\r\n";

	//显示消息
	int len = m_ctlOutput.GetWindowTextLength();
	m_ctlOutput.SetSel(len,len);
	m_ctlOutput.ReplaceSel(strItem);

}

/*
 * 处理键盘消息
 */
BOOL CChatDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_ctlInput.GetSafeHwnd())	//输入消息窗口    
	{  
		if (pMsg->wParam == VK_RETURN)			//return键  
		{ 		
			if (TRUE == m_bOfflineWnd )			//显示离线消息窗口
			{
				return TRUE;

			}else								//在线/离线聊天窗口
			{
				SendPeerMessage();				//发送消息
				m_ctlInput.SetWindowText("");	//清空输入消息窗口内容
			}

			return TRUE;
		}  
	}  
	return CDialog::PreTranslateMessage(pMsg); //其他消息
}


/*
 * 发送消息
 */
void CChatDlg::SendPeerMessage( void )
{
	//输入消息窗口内容为空
	if (0 == m_ctlInput.GetWindowTextLength())
	{
		return;
	}
	
	//对方在线
	if(m_PeerInfo.m_eStatus == CUserInfo::ONLINE)	//发送在线消息
	{
		CChatPacket packet;
		packet.m_UserInfo = this->m_UserInfo;		//发送消息用户名称
		packet.m_type = CChatPacket::MESSAGE;		//包的类型
		m_ctlInput.GetWindowText(packet.m_strMsg);	//消息
		packet.m_time = CTime::GetCurrentTime();	//发包时间
		
		//发送消息
		m_pArchiveOut = new CArchive(m_pFile,CArchive::store);//创建写入文档对象	
		packet.Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;	
		
		//显示发送的消息
		DisplaySentMessage(&packet);

	}else//对方离线
	{
		//显示发送的离线消息
		DisplaySentMessage();
	}	
	
}


/*
 * 显示发送的消息
 */
void CChatDlg::DisplaySentMessage(CChatPacket *pPacket)
{
	CString chatHeader = pPacket->m_UserInfo.m_strName;	//发送消息用户名称

	//格式化发送消息的时间
	CTime timeChatting =  pPacket->m_time;				
	int year = timeChatting.GetYear();		//年
	int month = timeChatting.GetMonth();	//月
	int day = timeChatting.GetDay();		//日
	int hour = timeChatting.GetHour();		//时
	int minute = timeChatting.GetMinute();	//分
	int second = timeChatting.GetSecond();	//秒
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);
	
	chatHeader += chatTime;//消息头
	//获取输入消息
	CString strMsg;
	m_ctlInput.GetWindowText(strMsg);

	//消息头与消息分行显示
	CString strItem = chatHeader + "\r\n"+ pPacket->m_strMsg + "\r\n";

	//在当前消息行下面显示消息
	int len = m_ctlOutput.GetWindowTextLength();//当前文本长度
	m_ctlOutput.SetSel(len,len);				//移动插入符到当前文本最后
	m_ctlOutput.ReplaceSel(strItem);			//在当前文本后插入消息
}

/*
 * 显示发送的消息
 */
void CChatDlg::DisplaySentMessage(void)
{
	CString chatHeader = m_UserInfo.m_strName;
	CTime timeChatting =  CTime::GetCurrentTime();
	int year = timeChatting.GetYear();		//年
	int month = timeChatting.GetMonth();	//月
	int day = timeChatting.GetDay();		//日
	int hour = timeChatting.GetHour();		//时
	int minute = timeChatting.GetMinute();	//分
	int second = timeChatting.GetSecond();	//秒
	CString chatTime;
	chatTime.Format(" (%d-%d-%d  %d:%d:%d)", year, month, day, hour, minute, second);

	
	chatHeader += chatTime;//消息头

	CString strMsg;
	m_ctlInput.GetWindowText(strMsg);
	CString strItem = chatHeader + "\r\n"+ strMsg + "\r\n";

	//显示消息
	int len = m_ctlOutput.GetWindowTextLength();
	m_ctlOutput.SetSel(len,len);
	m_ctlOutput.ReplaceSel(strItem);

}


void CChatDlg::InitArchive(void)
{
	//初始化输入和输出流
	m_pFile = new CSocketFile(m_pSocket);					//初始化CSocketFile
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);	//初始化读入流
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
	m_ctlOutput.GetWindowText(m_strRec);//聊天记录
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
 * 显示离线消息
 */
void CChatDlg::DisplayOfflineMessage( CChatPacket *pPacket )
{
	CRect inputWndRect;
	m_ctlInput.GetWindowRect(&inputWndRect);//获取输入消息窗口区域		
	m_ctlInput.MoveWindow(0, 0, 0, 0);		//设置该窗口大小为0
	
	CRect chatDlgRect;
	GetClientRect(&chatDlgRect);			//获取对话框客户区大小
	MoveWindow(0, 0, chatDlgRect.Width(),	//修改对话框的高度
		chatDlgRect.Height() - inputWndRect.Height());
	
	GetClientRect(&chatDlgRect);			//获取对话框客户区大小
	m_ctlOutput.MoveWindow(5, 5,			//修改显示消息窗口高度
		chatDlgRect.Width() -10, chatDlgRect.Height() -10);
	
	m_PeerInfo = pPacket->m_UserInfo;								//发送消息用户名称							
	SetWindowText(m_PeerInfo.m_strName + _T("发送给您的离线消息"));	 //设置标题
	m_ctlOutput.SetWindowText(pPacket->m_strMsg );					//设置消息
}