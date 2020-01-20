// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "ServerAddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pListenSocket = NULL;	//监听套接字
	m_pImageList = NULL;	//图标列表

}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_CLIENT_LIST, m_ctlUserList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_CLIENT_LIST, OnRclickClientList)
	ON_COMMAND(ID_SERVER_LIST_DELETE_MENUITEM, OnServerListDeleteMenuitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//初始化
	Init();

	return TRUE;  
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CServerDlg::~CServerDlg()
{

}

/*
 * 接受客户端连接请求
 */
void CServerDlg::ProcessPendingAccept(void)
{
	CClientSocket *pClient = new CClientSocket(this);//创建实例
	SOCKADDR_IN		clientAddr;
	int socketLen = sizeof(SOCKADDR_IN);

	m_pListenSocket->Accept(*pClient,(SOCKADDR*)&clientAddr, &socketLen);//接受客户端的连接
	
	pClient->Init();					//初始化
	pClient->SaveUserInfo(clientAddr);	//保存用户的地址
	
	m_csChatterList.Lock();
	m_ChatterList.AddTail(pClient);		//加入在线用户链表
	m_csChatterList.Unlock();
}

/*
 * 初始化
 */
BOOL CServerDlg::Init(void)
{			

	InitListCtrlSetting();				//定义列表控件

	InitUserList();						//初始化用户列表

	LoadOfflineMsg(m_OfflineMsgList);	//读取离线消息

	return TRUE;
}

/*
 * 启动服务
 */
BOOL CServerDlg::StartService(void)
{
	CServerAddressDlg servAddrDlg;		//服务器地址和端口对话框
	if (IDOK != servAddrDlg.DoModal())
	{
		return FALSE;
	}

	//获取服务器地址
	in_addr servAdd;
	servAdd.S_un.S_addr = htonl(servAddrDlg.m_strServIP);	//主机字节转换为网络字节
	CString strServIP = inet_ntoa(servAdd);					//转换为点格式

	//创建CListenSocket对象
	m_pListenSocket = new CListenSocket(this);				

	//创建套接字
	if (m_pListenSocket->Create(servAddrDlg.m_shServPort, SOCK_STREAM,strServIP))
	{
		//开始监听
		if (!m_pListenSocket->Listen())
		{
			delete m_pListenSocket;
			m_pListenSocket = NULL;
			AfxMessageBox(_T("创建套接字失败！"));
			return FALSE;			
		}
	}else
	{
		delete m_pListenSocket;
		m_pListenSocket = NULL;
		AfxMessageBox(_T("创建套接字失败！"));
		return FALSE;	
	}	
	return TRUE;
}


/*
 * 定义列表控件
 */
void CServerDlg::InitListCtrlSetting(void)
{
	m_pImageList = new CImageList();
	ASSERT(m_pImageList != NULL);  
	
	//32*32，8位，初始化为2个图标，每次增长2个图标
	m_pImageList->Create(32, 32, ILC_COLOR8|ILC_MASK, 2, 2);
	CWinApp* pApp = AfxGetApp();								//获得应用程序指针
	HICON hIconOnline = pApp->LoadIcon(IDI_CHATTER_ONLINE);		//在线图标
	HICON hIconOffline = pApp->LoadIcon(IDI_CHATTER_OFFLINE);	//离线图标
	
	m_pImageList->Add(hIconOnline);								//加入图标
	m_pImageList->Add(hIconOffline);							//加入图标
	
	//将图标列表赋值给列表控件
	m_ctlUserList.SetImageList(m_pImageList, LVSIL_SMALL);
	
	//设置列表头
	m_ctlUserList.InsertColumn(0, _T("名称"), LVCFMT_IMAGE|LVCFMT_LEFT);
	m_ctlUserList.InsertColumn(1, _T("密码"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(2, _T("地址"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(3, _T("端口"), LVCFMT_CENTER);
	m_ctlUserList.InsertColumn(4, _T("时间"), LVCFMT_CENTER);
	
	//设置各列的宽度
	CRect rect;
	m_ctlUserList.GetClientRect(&rect);//获取列表视图控件的宽度

	int nWidth = rect.Width();
	m_ctlUserList.SetColumnWidth(0, nWidth * 25 / 100);//名称占25%
	m_ctlUserList.SetColumnWidth(1, nWidth * 13 / 100);//密码占13%
	m_ctlUserList.SetColumnWidth(2, nWidth * 25 / 100);//地址占25%
	m_ctlUserList.SetColumnWidth(3, nWidth * 12 / 100);//端口占12%
	m_ctlUserList.SetColumnWidth(4, nWidth * 25 / 100);//时间占25%
}

/*
 * 初始化用户列表
 */
void CServerDlg::InitUserList(void)
{
	LoadUserList(m_UserList);//读取用户信息

	//遍历链表的每个节点，读取数据，初始化用户列表
	POSITION pos;
	int nLine = 0;
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		CUserInfo *pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);
		if (NULL != pUserInfo)
		{
			m_ctlUserList.InsertItem(nLine,pUserInfo->m_strName,1);			//名称，用户初始化为离线状态
			m_ctlUserList.SetItemText(nLine,1,pUserInfo->m_strPassword);	//密码
			
			in_addr userAddr;
			userAddr.S_un.S_addr = (long)pUserInfo->m_lIP;
			CString strUserAddr(inet_ntoa(userAddr));						//IP
			m_ctlUserList.SetItemText(nLine,2,strUserAddr);
			
			CString strUserPort;											//端口
			strUserPort.Format("%d",ntohs(pUserInfo->m_nPort));	
			m_ctlUserList.SetItemText(nLine,3,strUserPort);
			
			CString strUserTime = pUserInfo->m_time.Format("%c");			//端口
			m_ctlUserList.SetItemText(nLine,4,strUserTime);
			nLine++;
		}
	}
}

/*
 * 保存用户信息
 */
void CServerDlg::SaveUserList(CObList &obList)
{
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath +="\\";
	filePath += USERLISTINI;	

	//删除原来的文件
	DeleteFile(filePath);

	POSITION	pos;
	int			nIndex = 0;

	//遍历整个用户链表
	for(pos = obList.GetHeadPosition(); pos != NULL; )
	{
		//获取数据
		CUserInfo *pUserInfo = (CUserInfo*)obList.GetNext(pos);//用户信息
		if (NULL == pUserInfo)
		{
			break;
		}

		CString strSection("section");								//section
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;

		in_addr userAddr;
		userAddr.S_un.S_addr = (long)pUserInfo->m_lIP;
		CString strUserAddr(inet_ntoa(userAddr));					//用户IP

		CString strUserPort;										//用户端口
		strUserPort.Format("%d",pUserInfo->m_nPort);

		CString strUserTime;										//用户登录线时间
		strUserTime.Format("%ld", (long)pUserInfo->m_time.GetTime());

		//写用户名字
		WritePrivateProfileString(strSection, _T("Name"), pUserInfo->m_strName, filePath);	
		//写用户密码
		WritePrivateProfileString(strSection, _T("Password"), pUserInfo->m_strPassword, filePath);	
		//写用户地址
		WritePrivateProfileString(strSection, _T("Address"), strUserAddr,filePath);	
		//写用户端口
		WritePrivateProfileString(strSection, _T("Port"), strUserPort,filePath);					
		//写用时间
		WritePrivateProfileString(strSection, _T("Time"), strUserTime,filePath);					

		delete pUserInfo;//删除对象
		pUserInfo = NULL;
		nIndex++;
	}

	obList.RemoveAll();//删除链表所有节点
}

/*
 * 读取用户信息
 */
void CServerDlg::LoadUserList(CObList &obList)
{
#define  TEMP_BUF_SIZE 32//缓冲区长度
	
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath += "\\";
	filePath += USERLISTINI;
	
	int nIndex = 0;
	while (TRUE)
	{
		CString strSection("section");
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;
		
		//用户名称
		CString strSectionKey = "Name";					
		CString strValue = _T("");
		char cBuf[TEMP_BUF_SIZE];
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		
		CUserInfo *pUserInfo = new CUserInfo();//新建CUserInfo对象
		pUserInfo->m_strName = cBuf;
		
		//读取用户密码
		strSectionKey = "Password";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		pUserInfo->m_strPassword = cBuf;
		
		//读取用户地址
		strSectionKey = "Address";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		u_long clinetAddr = inet_addr(cBuf);
		pUserInfo->m_lIP = (DWORD)clinetAddr;
		
		//读取用户端口
		strSectionKey = "Port";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		pUserInfo->m_nPort = atoi(cBuf);

		//读取用户时间
		strSectionKey = "Time";
		memset(cBuf, 0, TEMP_BUF_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		time_t time = atol(cBuf);
		pUserInfo->m_time = time;
		
		obList.AddTail(pUserInfo);		//加入链表
		nIndex++;
	}	
}


/*
 * 保存离线消息
 */
void CServerDlg::SaveOfflineMsg(CObList &obList)
{
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath +="\\";
	filePath += OFFLINEMSGLIST;	

	//删除原来的文件
	DeleteFile(filePath);
	
	POSITION pos;
	int nIndex = 0;
	for(pos = obList.GetHeadPosition(); pos != NULL; )
	{
		CChatPacket *pPacket = (CChatPacket*)obList.GetNext(pos);//用户信息
		if (NULL == pPacket)
		{
			break;
		}

		CString strSection("section");							//section
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;

		CString strTime;
		strTime.Format("%ld",(long)pPacket->m_time.GetTime());

		pPacket->m_strMsg.Replace("\r\n", "$");//取代回车符为$
		//写发送消息的用户名称 
		WritePrivateProfileString(strSection, _T("SendMsgUser"), pPacket->m_UserInfo.m_strName, filePath);		//发送消息用户
		//写接收消息的用户名称
		WritePrivateProfileString(strSection, _T("RecvMsgUser"), pPacket->m_OfflineUserInfo.m_strName, filePath);	//接收消息用户
		//写发送消息 的时间
		WritePrivateProfileString(strSection, _T("Time"), strTime,filePath);									//发送消息时间
		//写发送的消息
		WritePrivateProfileString(strSection, _T("Message"), pPacket->m_strMsg, filePath);						//发送的消息
		delete pPacket;
		pPacket = NULL;
		nIndex++;
	}
	obList.RemoveAll();


}

/*
 * 读取离线消息
 */
void CServerDlg::LoadOfflineMsg(CObList &obList)
{
	m_csOfflineList.Lock();	
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//保存Ini文件名
	filePath.Format("%s",appPath);
	filePath += "\\";
	filePath += OFFLINEMSGLIST;
	
	int nIndex = 0;
	while (TRUE)
	{
		CString strSection("section");						//section
		CString strIndex;
		strIndex.Format("%d",nIndex);
		strSection += strIndex;
		
		//读取发送消息的用户名称
		CString strSectionKey = "SendMsgUser";
		CString strValue = _T("");
		char cBuf[MAX_MSG_SIZE];
		memset(cBuf, 0, MAX_MSG_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		
		CChatPacket *pPacket = new CChatPacket();	//新建CChatPacket对象
		pPacket->m_type = CChatPacket::MESSAGE;
		pPacket->m_UserInfo.m_strName = cBuf;		//发送消息的用户名
		
		
		//读取接收消息的用户名称
		strSectionKey = "RecvMsgUser";
		memset(cBuf, 0, MAX_MSG_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		pPacket->m_OfflineUserInfo.m_strName = cBuf;
		

		//读取发送消息时间
		strSectionKey = "Time";
		memset(cBuf, 0, MAX_MSG_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}
		time_t time = atol(cBuf);
		pPacket->m_time = time;


		//读取发送消息
		strSectionKey = "Message";
		memset(cBuf, 0, MAX_MSG_SIZE);
		if(0 == GetPrivateProfileString (strSection, strSectionKey, NULL, cBuf, TEMP_BUF_SIZE, filePath))
		{
			break;
		}

		////取代$为回车符
		CString strMsg(cBuf);
		strMsg.Replace( "$","\r\n");
		pPacket->m_strMsg = strMsg;
		

		obList.AddTail(pPacket);//加入链表
		nIndex++;
	}
	
	m_csOfflineList.Unlock();
}

/*
 * 处理数据
 */
BOOL CServerDlg::ProcessPendingRead(CArchive *pArchiveIn, CArchive *pArchiveOut, CClientSocket *pClientSocket)
{	
	do 
	{		
		CChatPacket chatPact;//接收数据包
		chatPact.Serialize(*pArchiveIn);
		
		if (CChatPacket::MESSAGE == chatPact.m_type)//发给离线用户的消息
		{
			m_csOfflineList.Lock();
		
			//保存离线用户消息
			CChatPacket *pChatPacket = new CChatPacket();
			//包类型
			pChatPacket->m_type = CChatPacket::MESSAGE;	
			//发送消息用户
			pChatPacket->m_UserInfo.m_strName = chatPact.m_UserInfo.m_strName;				
			//接收消息用户
			pChatPacket->m_OfflineUserInfo.m_strName = chatPact.m_OfflineUserInfo.m_strName;
			//发送的消息
			pChatPacket->m_strMsg = chatPact.m_strMsg;	
			//发包时间
			pChatPacket->m_time = chatPact.m_time;											
			
			//加入离线消息链表
			m_OfflineMsgList.AddTail(pChatPacket);											

			m_csOfflineList.Unlock();			
			
		}else if (CChatPacket::USERLIST == chatPact.m_type)//请求用户列表--用户刚登录
		{
			//保存用户信息
			pClientSocket->SaveUserInfo(chatPact.m_UserInfo);
			
			//户更新信息
			if (!UpdateUserList(chatPact.m_UserInfo, /*pArchiveOut,*/ pClientSocket))
				return FALSE;
			
			//向所有的在线用户发送用户列表
			SendUserList();
			
			//转发离线消息
			TransmitMsg(chatPact, /*pArchiveOut*/pClientSocket);
			
			//更新服务器界面
			UpdateServerListCtl(m_UserList);
			
		}
		
	} while(!pArchiveIn->IsBufferEmpty());
	
	return TRUE;
}

/*
 * 发送用户列表
 */
void CServerDlg::SendUserList()
{
	//复制用户链表
 	CObList tempChatterlist;
	CopyUserList(tempChatterlist);

 	CChatPacket packet;
	packet.m_type = CChatPacket::USERLIST;	//包的类型
 	packet.m_pUserList = &tempChatterlist;	//用户链表

	m_csChatterList.Lock();//上锁

	//向所有在线用户发送列表
	POSITION pos = NULL;
	for (pos = m_ChatterList.GetHeadPosition(); NULL!=pos;)
	{
		CClientSocket *pClientSocket = (CClientSocket*)m_ChatterList.GetNext(pos);
		pClientSocket->SendUserList(&packet);//发送数据包
	}
	m_csChatterList.Unlock();//解锁

	DeleteTempUserList(tempChatterlist);//删除临时链表
}

/*
 * 复制用户链表
 */
void CServerDlg::CopyUserList(CObList &obList)
{
	if (m_UserList.IsEmpty())
	{
		return ;
	}

	POSITION pos = NULL;
	CUserInfo *pUserInfo = NULL;
	CUserInfo *pTempUserInfo = NULL;
	for (pos = m_UserList.GetHeadPosition(); pos != NULL;)
	{
		pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);
		
		//不要复制用户的密码
		pTempUserInfo = new CUserInfo();
		pTempUserInfo->m_strName = pUserInfo->m_strName;	//名称
		pTempUserInfo->m_eStatus = pUserInfo->m_eStatus;	//状态
		pTempUserInfo->m_lIP = pUserInfo->m_lIP;			//IP
		pTempUserInfo->m_nPort = pUserInfo->m_nPort;		//端口

		obList.AddTail(pTempUserInfo);
	}
}

/*
 * 删除临时链表
 */
void CServerDlg::DeleteTempUserList(CObList &obList)
{
	if (obList.IsEmpty())
	{
		return;
	}
	
	POSITION pos = NULL;
	CUserInfo *pUserInfo = NULL;

	for (pos = obList.GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)obList.GetNext(pos);
		delete pUserInfo;
		pUserInfo = NULL;
	}

	obList.RemoveAll();
}

/*
 * 更新用户链表
 */
BOOL CServerDlg::UpdateUserList(const CUserInfo &userInfo, CClientSocket *pClientSocket)
{

	BOOL retVal = TRUE;				//返回值
	m_csUserList.Lock();			//上锁

	POSITION	pos;				//位置变量		
	CUserInfo	*pUserInfo = NULL;	//用户对象指针
	BOOL bFind = FALSE;				//是否找到该用户
	
	//该用户是否存在于链表中
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);//节点下移		
		
		if (pUserInfo->m_strName == userInfo.m_strName )//名字相同
		{
			if (pUserInfo->m_strPassword == userInfo.m_strPassword)	//在册用户
			{
				if(CUserInfo::ONLINE == pUserInfo->m_eStatus)		//用户已经登录
				{
					//从在线链表中删除该节点
					DeleteChatter(pClientSocket);
					
					//发送“用户已经登录”消息
					CChatPacket packet;
					packet.m_type = CChatPacket::SERVERMSG;		//数据包类型
					packet.m_strMsg = _T("    用户已经登录!   ");//消息
					packet.m_time = CTime::GetCurrentTime();	//时间

					pClientSocket->SendUserMsg(&packet);		//发送数据
		
					m_csUserList.Unlock();						//解锁	
					return FALSE;
					
				}else//用户登录
				{
					pUserInfo->m_eStatus = CUserInfo::ONLINE;	//修改用户状态
					pUserInfo->m_lIP = userInfo.m_lIP;			//IP地址
					pUserInfo->m_nPort = userInfo.m_nPort;		//端口
					bFind = TRUE;
					break;						
				}
					
			}else//密码错误
			{
				//从在线链表中删除该节点
				DeleteChatter(pClientSocket);
				
				//发送“密码错误”消息
				CChatPacket packet;
				packet.m_type = CChatPacket::SERVERMSG;						//数据包类型
				packet.m_strMsg = _T("    密码错误\t\n    请重新登录!    "); //消息	
				packet.m_time = CTime::GetCurrentTime();					//时间

				pClientSocket->SendUserMsg(&packet);//发送数据
				
				m_csUserList.Unlock();//解锁
				return FALSE;				
			}			
		}
	}

	if (FALSE == bFind)//注册新用户
	{
		CUserInfo *pUserInfo = new CUserInfo();
		pUserInfo->m_strName = userInfo.m_strName;			//姓名
		pUserInfo->m_strPassword = userInfo.m_strPassword;	//密码
		pUserInfo->m_eStatus = CUserInfo::ONLINE;			//状态
		pUserInfo->m_lIP = userInfo.m_lIP;					//地址
		pUserInfo->m_nPort = userInfo.m_nPort;				//端口
		pUserInfo->m_time = userInfo.m_time;				//时间

		m_UserList.AddTail(pUserInfo);						//加入链表
	}

	m_csUserList.Unlock();//解锁
	return retVal;	
}

/*
 * 转发离线消息
 */
void CServerDlg::TransmitMsg(const CChatPacket &packet, CClientSocket *pClientSocket)
{
	m_csOfflineList.Lock();
	POSITION pos1 = NULL;		//位置变量
	POSITION pos2 = NULL;		//位置变量
	CChatPacket *pPacket = NULL;//数据包

	//遍历整个离线消息链表
	for (pos1 = m_OfflineMsgList.GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		//取出数据包
		pPacket = (CChatPacket*)m_OfflineMsgList.GetNext(pos1);

		//登录用户的名字和接收离线消息的用户名字相同
		if (pPacket->m_OfflineUserInfo.m_strName == packet.m_UserInfo.m_strName)
		{
			pClientSocket->SendUserMsg(pPacket);//发送离线消息
			m_OfflineMsgList.RemoveAt(pos2);	//删除链表节点
			delete pPacket;						//删除对象
			pPacket = NULL;					
		}
	}
	m_csOfflineList.Unlock();
}

/*
 * 更新服务器界面
 */
void CServerDlg::UpdateServerListCtl(const CObList &obList)
{
	m_csUserList.Lock();//上锁

	//删除原来数据项
	m_ctlUserList.DeleteAllItems();
	
	POSITION	pos = NULL;		//位置变量
	int			nIndex = 0;		//序号
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		//取出用户信息
		CUserInfo *pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);
		if (CUserInfo::ONLINE == pUserInfo->m_eStatus)//在线状态
		{
			//插入数据项 
			m_ctlUserList.InsertItem(nIndex, pUserInfo->m_strName,CUserInfo::ONLINE);
			
		}else//离线状态 		
		{
			//插入数据项 
			m_ctlUserList.InsertItem(nIndex, pUserInfo->m_strName,CUserInfo::OFFLINE);
		}
		
		m_ctlUserList.SetItemText(nIndex,1,pUserInfo->m_strPassword);	//用户密码
		
		in_addr userAddr;
		userAddr.S_un.S_addr = (long)pUserInfo->m_lIP;
		CString strUserAddr(inet_ntoa(userAddr));						//用户IP
		m_ctlUserList.SetItemText(nIndex,2,strUserAddr);
		
		CString strUserPort;											//用户端口
		strUserPort.Format("%d",ntohs(pUserInfo->m_nPort));	
		m_ctlUserList.SetItemText(nIndex,3,strUserPort);
		
		CString strUserTime = pUserInfo->m_time.Format("%c");			//登录时间
		m_ctlUserList.SetItemText(nIndex,4,strUserTime);
		nIndex++;		
	}
	m_csUserList.Unlock();//解锁

}

/*
 * 当用户离线时删除用户
 */
void CServerDlg::ReleaseChatter(CClientSocket *pClientSocket)
{
	m_csChatterList.Lock();
	POSITION pos = NULL;

	//找到该用户
	if ((pos = m_ChatterList.Find(pClientSocket)) != NULL)
	{
		ASSERT((CClientSocket*)m_ChatterList.GetAt(pos) == pClientSocket);
		
		//获取该用户信息
		CUserInfo userInfo = pClientSocket->GetUserInfo();
		
		//更新该用户信息
		POSITION posUser = NULL;
		for (posUser = m_UserList.GetHeadPosition(); posUser != NULL;)
		{
			CUserInfo *pUserInfo = (CUserInfo*)m_UserList.GetNext(posUser);
			if (pUserInfo->m_strName == userInfo.m_strName)
			{
				pUserInfo->m_eStatus = CUserInfo::OFFLINE;	//修改状态为离线
				pUserInfo->m_lIP = userInfo.m_lIP;			//修改用户地址			
				pUserInfo->m_nPort = userInfo.m_nPort;		//修改用户端口
				pUserInfo->m_time = CTime::GetCurrentTime();//设置离线时间
				break;
			}
		}

		//删除该在线用户
		m_ChatterList.RemoveAt(pos);

		//通知所有在线用户更新用户列表
		SendUserList();
		
		//更新服务器界面
		UpdateServerListCtl(m_UserList);	
	}

	m_csChatterList.Unlock();
}


/*
 * 登录失败处理
 */
void CServerDlg::DeleteChatter(CClientSocket *pClientSocket)
{

	m_csChatterList.Lock();
	POSITION pos = NULL;
	if ((pos = m_ChatterList.Find(pClientSocket)) != NULL)
	{
		ASSERT((CClientSocket*)m_ChatterList.GetAt(pos) == pClientSocket);		
		//删除该在线用户
		m_ChatterList.RemoveAt(pos);	
	}

	m_csChatterList.Unlock();
	
}


/*
 * WM_DESTROY消息相应函数
 */
void CServerDlg::OnDestroy()
{
	SaveUserList(m_UserList);			//保存用户信息

	SaveOfflineMsg(m_OfflineMsgList);	//保存离线信息

	DeleteAllChatter();					//删除在线用户

	if (NULL != m_pListenSocket)		//删除CListenSocket类对象
	{
		delete m_pListenSocket;
		m_pListenSocket = NULL;
	}
	if (NULL != m_pImageList)			//删除CImageList类对象
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

/*
 * 右击列表控件响应函数
 */
void CServerDlg::OnRclickClientList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();					//鼠标获取位置
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
		
		CMenu menu;
		VERIFY( menu.LoadMenu(IDR_SERVER_LIST_MENU));	//装载菜单资源
		CMenu* popup = menu.GetSubMenu(0);				//获取菜单项
		ASSERT( popup != NULL );

		//弹出快捷菜单
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	}
	*pResult = 0;
}

/*
 * 处理菜单消息
 */
void CServerDlg::OnServerListDeleteMenuitem() 
{
	//获得被选中数据项的位置
	POSITION pos = m_ctlUserList.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ctlUserList.GetNextSelectedItem(pos);
	//获取用户名称
	CString strPeerName = m_ctlUserList.GetItemText(nSelectedItem, 0);
	
	POSITION pos1 = NULL;		//位置变量 
	POSITION pos2 = NULL;		//位置变量
	BOOL bDeleteUser = FALSE;	//是否找到该用户
	
	//在用户链表中查找
	for (pos1 = m_UserList.GetHeadPosition();(pos2=pos1)!= NULL;)
	{
		//取出数据
		CUserInfo* pUserInfo = (CUserInfo*)m_UserList.GetNext(pos1);
		if (pUserInfo->m_strName == strPeerName)			//名称相同
		{
			if (pUserInfo->m_eStatus == CUserInfo::ONLINE)	//不能删除在线用户
			{
				AfxMessageBox("不能删除在线用户！",MB_OK, -1);
				return ;
			}else//删除不在线用户
			{
				CUserInfo *pUserInfo = (CUserInfo*)m_UserList.GetAt(pos2);
				m_UserList.RemoveAt(pos2);	//删除指针
				delete pUserInfo;			//删除对象
				pUserInfo = NULL;			
				
				bDeleteUser = TRUE;
				//更新服务器界面
				UpdateServerListCtl(m_UserList);
				
			}
		}
	}

	if (TRUE == bDeleteUser)
	{
		//通知客户端更新成员列表
		SendUserList();
	}	
}

/*
 * 删除所有在线用户
 */
void CServerDlg::DeleteAllChatter( void )
{
	POSITION pos;
	CClientSocket *pClient = NULL;
	for (pos = m_ChatterList.GetHeadPosition(); NULL != pos; )
	{
		pClient = (CClientSocket*)m_ChatterList.GetNext(pos);
		delete pClient;
		pClient = NULL;
	}
	m_ChatterList.RemoveAll();
}