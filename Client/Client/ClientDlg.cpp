// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "ClientLogin.h"
#include "ChatDlg.h"
#include "ChatSocket.h"
#include "ListenSocket.h"
#include "PeerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImageList = NULL;	
	m_pArchiveIn = NULL;	
	m_pArchiveOut = NULL;	
	m_pFile = NULL;
	m_pSocket = NULL;

	memset(&m_hostAddr, 0, sizeof(SOCKADDR_IN));
}

CClientDlg::~CClientDlg()
{
	
	if (NULL != m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	if (NULL != m_pArchiveIn)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	if (NULL != m_pArchiveOut)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
	
	if (NULL != m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	if (m_pSocket != NULL)
	{
		BYTE Buffer[50];
		m_pSocket->ShutDown();
		
		while(m_pSocket->Receive(Buffer,50) > 0);//????	
		delete m_pSocket;
		m_pSocket = NULL;
	}



}
void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_CLIENT_LIST, m_ctlUserList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_CLIENT_LIST, OnDblclkClientList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	

	if (!Init())
		EndDialog(IDCANCEL);
	return FALSE;


//#define WM_USER                         0x0400
// 	CUserInfo userInfo;
// 	userInfo.m_time = CTime::GetCurrentTime();
// 	userInfo.m_strName = "sunhm";
// 	CString str("我吃饭去了。");
// 	ShowOfflineMsg(userInfo, str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
 * 初始化
 */
BOOL CClientDlg::Init(void)
{
	
	InitListCtrlSetting();	//初始化列表视图控件	

	BegingListen();			//开始监听
 	
	if(!ConnectToServer())	//连接服务器
	return FALSE;

	return TRUE;
}

/*
 * 连接服务器
 */
BOOL CClientDlg::ConnectToServer(void)
{
	//创建CChatSocket对象
	m_pSocket = new CChatSocket(this);	
	if (!m_pSocket->Create())//创建套接字，绑定，注册网络事件
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(_T("创建套接字失败！"));
		return FALSE;
	}
	m_pFile = new CSocketFile(m_pSocket);					//创建CSocketFile对象
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);	//创建读入文档对象
	

	CClientLoginDlg loginDlg(this);//登录对话框
	if (IDOK == loginDlg.DoModal())
	{
		//服务器端口
		SHORT shServPort = (SHORT)atoi((LPCTSTR)loginDlg.m_strServerPort);
		m_servAddr.sin_family = AF_INET;									//地址家族
		m_servAddr.sin_addr.S_un.S_addr = htonl((u_long)loginDlg.m_dwIP);	//地址
		m_servAddr.sin_port = htons(shServPort);							//端口
	
		//连接服务器
		while (!m_pSocket->Connect((SOCKADDR*)&m_servAddr, sizeof(m_servAddr)))
		{
			if (AfxMessageBox(_T("连接服务器失败，是否再次尝试连接。"),MB_YESNO) == IDNO)
			{
				return FALSE;
			}
		}

		//获取本机套接字地址
		SOCKADDR_IN hostAddr;
		int nSockaddLen = sizeof(SOCKADDR_IN);
		m_pSocket->GetSockName((SOCKADDR*)&hostAddr, &nSockaddLen);
		m_hostAddr.sin_addr.S_un.S_addr = hostAddr.sin_addr.S_un.S_addr;//主机地址
	
	
		CChatPacket packet;															//数据包

		packet.m_type = CChatPacket::USERLIST;										//类型
		packet.m_UserInfo.m_strName = loginDlg.m_strName;							//名称
		packet.m_UserInfo.m_strPassword = loginDlg.m_strPassword;					//密码
		packet.m_UserInfo.m_eStatus = CUserInfo::LOGIN;								//用户状态
		packet.m_UserInfo.m_lIP = hostAddr.sin_addr.S_un.S_addr;					//IP
		packet.m_UserInfo.m_nPort = m_hostAddr.sin_port;							//端口
		packet.m_UserInfo.m_time = CTime::GetCurrentTime();							//登录时间		
		m_UserInfo = packet.m_UserInfo;												//保存用户信息

		//请求用户列表
		SendPacket(packet);
		return TRUE;
	}else
	{
		return FALSE;
	}
}


/*
 * 等待接收消息
 */
void CClientDlg::ProcessPendingRead(void)
{
	do
	{
		ReadPacket();
		if (m_pSocket == NULL)
			return;
	}
	while(!m_pArchiveIn->IsBufferEmpty());//将缓冲区数据全部读入
}

/*
 * 接收服务器数据
 */
void CClientDlg::ReadPacket(void)
{
	CObList		obList;//临时链表
	CChatPacket	packet;//数据包
	packet.m_pUserList = &obList;

	TRY
	{
		packet.Serialize(*m_pArchiveIn);					//接收数据

		if (packet.m_type == CChatPacket::MESSAGE)			//离线消息
		{
			ShowOfflineMsg(&packet);		

		}else if (packet.m_type == CChatPacket::USERLIST)	//用户列表
		{
			CreateUserList(&obList);						//创建用户链表

			UpdateClientListCtl();							//更新用户界面

			m_UserInfo.m_eStatus = CUserInfo::ONLINE;		//修改用户状态

		}else if (packet.m_type == CChatPacket::SERVERMSG)	//登录失败消息
		{
			 MessageBox(packet.m_strMsg, "Server Messages", MB_OK);			
			 EndDialog(IDCANCEL);							//退出		
		}		
	}
	CATCH (CFileException, e)
	{
		MessageBox(_T("读入chatter链表错误"));		
	}
	END_CATCH
}

/*
 * 更新用户列表
 */
void CClientDlg::UpdateClientListCtl(void)
{
	//删除原来用户链表		
	m_ctlUserList.DeleteAllItems(); 
	
	POSITION pos;
	int nIndex = 0;
	for(pos = m_UserList.GetHeadPosition(); pos != NULL;)//遍历整个链表
	{
		//获取每个用户信息
		CUserInfo userInfo;
		userInfo =*(CUserInfo*)m_UserList.GetNext(pos);
		
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT |LVIF_IMAGE |LVIF_PARAM;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.pszText = userInfo.m_strName.GetBuffer(10);	
		if (userInfo.m_eStatus == CUserInfo::ONLINE)	//在线状态
		{
			lvItem.iImage = CUserInfo::ONLINE;
		}else										
		{
			lvItem.iImage = CUserInfo::OFFLINE;			//离线状态	
		}		
		lvItem.lParam = (LPARAM)userInfo.m_strName.GetBuffer(10);

		m_ctlUserList.InsertItem(&lvItem);				//添加数据项

		userInfo.m_strName.ReleaseBuffer();		
		nIndex++;		
	}	
}


/*
 * 显示离线消息
 */
void CClientDlg::ShowOfflineMsg(CChatPacket *pPacket)
{
	CChatDlg chatDlg(this, pPacket);//创建显示离线消息对话框
	chatDlg.DoModal();	
	SaveChatLog(&chatDlg);			//保存聊天记录
}


/*
 * 发送数据
 */
void  CClientDlg::SendPacket(CChatPacket &packet)
{
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);	//初始化写入流	
	
	if (NULL != m_pArchiveOut)
	{
		TRY
		{
			packet.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
		}
		CATCH(CFileException, e)
		{
			m_pArchiveOut->Abort();
			delete m_pArchiveOut;
			m_pArchiveOut = NULL;
			MessageBox(_T("发送数据包错误"));	
		}
		END_CATCH
	}
	
	if (NULL != m_pArchiveOut)
	{
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
	
}		


void CClientDlg::CreateUserList(CObList *pObList)
{
	POSITION pos = NULL;
	CUserInfo *pUserInfo = NULL;

	//删除原来链表
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);
		delete pUserInfo;
		pUserInfo = NULL;
	}
	m_UserList.RemoveAll();

	//复制链表
	CUserInfo *pNewUserInfo = NULL;
	for (pos = pObList->GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)pObList->GetNext(pos);
		pNewUserInfo= new CUserInfo();
		pNewUserInfo->m_strName = pUserInfo->m_strName;	//名称
		pNewUserInfo->m_eStatus = pUserInfo->m_eStatus;	//状态
		pNewUserInfo->m_lIP = pUserInfo->m_lIP;			//地址
		pNewUserInfo->m_nPort = pUserInfo->m_nPort;		//端口
		m_UserList.AddTail(pNewUserInfo);				//加入链表
		delete pUserInfo;
		pUserInfo = NULL;
	}
	
	pObList->RemoveAll();
}

/*
 * 双击列表视图控件消息消息响应函数
 */
void CClientDlg::OnDblclkClientList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		int nSelectItem = pNMListView->iItem;
		CString strPeerName = m_ctlUserList.GetItemText(nSelectItem, 0);
		CreateChatDlg(strPeerName);
	}
	*pResult = 0;
}


/*
 * 创建聊天窗口
 */
void CClientDlg::CreateChatDlg( CString strPeerName )
{
	//不给自己发送消息
	if (strPeerName == m_UserInfo.m_strName)
		return;

	//在用户链表中找到该用户
	CUserInfo *pPeerInfo = NULL;
	POSITION pos = NULL;
	BOOL bFinder = FALSE;
	for (pos = m_UserList.GetHeadPosition(); NULL != pos; )
	{
		pPeerInfo = (CUserInfo*)m_UserList.GetNext(pos);
		if (pPeerInfo->m_strName == strPeerName)
		{
			bFinder = TRUE;
			break;
		}
	}

	ASSERT(TRUE == bFinder);//找到该用户
	if (pPeerInfo->m_eStatus == CUserInfo::ONLINE)		//用户在线
	{
		CChatDlg chatDlg(this, *pPeerInfo, m_UserInfo);	//定义对话框
		if (chatDlg.ConnectToPeer())					//连接对方
		{		
			chatDlg.DoModal();//显示聊天窗口
			//保存聊天记录
			SaveChatLog(&chatDlg);
		}
	}else//发送离线消息
	{
		CChatDlg chatDlg(this, *pPeerInfo, m_UserInfo);//定义对话框
		chatDlg.DoModal();//显示聊天窗口
		
		//离线数据包
		CChatPacket packet;
		packet.m_type = CChatPacket::MESSAGE;	//类型
		packet.m_UserInfo = m_UserInfo;			//发送离线消息的用户信息
		packet.m_OfflineUserInfo = *pPeerInfo;	//接收离线消息的用户信息
		packet.m_strMsg = chatDlg.m_strRec;		//离线消息
		packet.m_time = CTime::GetCurrentTime();//发送消息的时间

		//发送离线数据包
		SendPacket(packet);	
		//保存聊天记录
		SaveChatLog(&chatDlg);
	}	
}

/*
 * 开始监听
 */
BOOL CClientDlg::BegingListen( void )
{
	m_pListenSocket = new CListenSocket(this);	//创建监听对象
	if (m_pListenSocket->Create())				//创建绑定套接字
	{
		if (!m_pListenSocket->Listen())			//开始监听
		{
			delete m_pListenSocket;
			m_pListenSocket = NULL;
			AfxMessageBox(_T("创建套接字失败！"));
			return FALSE;			
		}
	}
	//获取地址信息
	SOCKADDR_IN hostAddr;
	int hostAddrLen = sizeof(SOCKADDR_IN);
	m_pListenSocket->GetSockName((SOCKADDR*)&hostAddr,&hostAddrLen);

	m_hostAddr.sin_port = hostAddr.sin_port;//保存监听端口
	return TRUE;
}

/*
 *接受对方的连接
 */
void CClientDlg::ProcessPendingAccept(void)
{
	CChatDlg	chatDlg(this);	//聊天对话框
	chatDlg.InitSocket();		//创建CChatSocket对象

	//接受对方连接
	CChatSocket *pChatSocket = chatDlg.GetChatSocket();
	SOCKADDR_IN	peerAddr;
	int socketLen = sizeof(SOCKADDR_IN);
 	m_pListenSocket->Accept(*pChatSocket,(SOCKADDR*)&peerAddr, &socketLen);

	//在用户链表中查找该用户
	POSITION	pos;
	CUserInfo	*pPeerInfo = NULL;
	BOOL		bFinder = FALSE;
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		pPeerInfo = (CUserInfo*)m_UserList.GetNext(pos);
		if (pPeerInfo->m_lIP == peerAddr.sin_addr.S_un.S_addr &&//找到该用户
			pPeerInfo->m_eStatus == CUserInfo::ONLINE)				
		{
			bFinder = TRUE;
			break;
		}
	}

	ASSERT(TRUE == bFinder);

	//获取对方信息
	CUserInfo peerInfo;
	peerInfo.m_strName = pPeerInfo->m_strName;		//名字
	peerInfo.m_eStatus = pPeerInfo->m_eStatus;		//状态
	peerInfo.m_lIP = peerAddr.sin_addr.S_un.S_addr;	//地址
	peerInfo.m_nPort = peerAddr.sin_port;			//端口


	chatDlg.SetPeerInfor(peerInfo);					//设置对方信息	
	chatDlg.SetUserInfor(m_UserInfo);				//设置用户信息
	chatDlg.InitArchive();							//设置套接字读入文档

	//打开聊天对话框
	chatDlg.DoModal();	
	//保存聊天记录
	SaveChatLog(&chatDlg);	
}



/*
 * 初始化列表视图控件
 */
BOOL CClientDlg::InitListCtrlSetting( void )
{
	//创建图标列表
	m_pImageList = new CImageList();
	
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
	m_ctlUserList.InsertColumn(0,_T("Chatter"),LVCFMT_IMAGE|LVCFMT_LEFT);

	//设置第一列的宽度为这个列表控件的宽度
	CRect rect;
	m_ctlUserList.GetClientRect(&rect);
	m_ctlUserList.SetColumnWidth(0,rect.Width());

	return TRUE;
	
}

/*
 * 保存聊天记录
 */
void CClientDlg::SaveChatLog(CChatDlg *pChatDlg)
{
	//没有聊天消息
	if (pChatDlg->m_strRec.IsEmpty())
		return;
	
	//保存聊天记录
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//取得应用程序当前路径
	CString  filePath;					//文件名
	filePath.Format("%s",appPath);
	filePath +="\\";
	filePath += CHATLOG;
	
	TRY
	{
		CFile file;
		if (!file.Open(filePath, CFile::modeReadWrite))					//文件已经存在
		{
			file.Open(filePath, CFile::modeCreate|CFile::modeReadWrite);//新建文件			
			
		}else
		{
			file.SeekToEnd();//移动文件指针到文件尾部
		}
		
		int nRecLen = pChatDlg->m_strRec.GetLength();	//聊天记录长度			
		file.Write(pChatDlg->m_strRec, nRecLen);		//写入数据
		file.Flush();
		file.Close();									//关闭文件 		
	}
	CATCH(CFileException, e)
	{
		MessageBox(_T("保存聊天记录异常"),MB_OK, -1);
	}
	END_CATCH	
}

