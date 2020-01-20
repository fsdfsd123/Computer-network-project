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
// 	CString str("�ҳԷ�ȥ�ˡ�");
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
 * ��ʼ��
 */
BOOL CClientDlg::Init(void)
{
	
	InitListCtrlSetting();	//��ʼ���б���ͼ�ؼ�	

	BegingListen();			//��ʼ����
 	
	if(!ConnectToServer())	//���ӷ�����
	return FALSE;

	return TRUE;
}

/*
 * ���ӷ�����
 */
BOOL CClientDlg::ConnectToServer(void)
{
	//����CChatSocket����
	m_pSocket = new CChatSocket(this);	
	if (!m_pSocket->Create())//�����׽��֣��󶨣�ע�������¼�
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return FALSE;
	}
	m_pFile = new CSocketFile(m_pSocket);					//����CSocketFile����
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);	//���������ĵ�����
	

	CClientLoginDlg loginDlg(this);//��¼�Ի���
	if (IDOK == loginDlg.DoModal())
	{
		//�������˿�
		SHORT shServPort = (SHORT)atoi((LPCTSTR)loginDlg.m_strServerPort);
		m_servAddr.sin_family = AF_INET;									//��ַ����
		m_servAddr.sin_addr.S_un.S_addr = htonl((u_long)loginDlg.m_dwIP);	//��ַ
		m_servAddr.sin_port = htons(shServPort);							//�˿�
	
		//���ӷ�����
		while (!m_pSocket->Connect((SOCKADDR*)&m_servAddr, sizeof(m_servAddr)))
		{
			if (AfxMessageBox(_T("���ӷ�����ʧ�ܣ��Ƿ��ٴγ������ӡ�"),MB_YESNO) == IDNO)
			{
				return FALSE;
			}
		}

		//��ȡ�����׽��ֵ�ַ
		SOCKADDR_IN hostAddr;
		int nSockaddLen = sizeof(SOCKADDR_IN);
		m_pSocket->GetSockName((SOCKADDR*)&hostAddr, &nSockaddLen);
		m_hostAddr.sin_addr.S_un.S_addr = hostAddr.sin_addr.S_un.S_addr;//������ַ
	
	
		CChatPacket packet;															//���ݰ�

		packet.m_type = CChatPacket::USERLIST;										//����
		packet.m_UserInfo.m_strName = loginDlg.m_strName;							//����
		packet.m_UserInfo.m_strPassword = loginDlg.m_strPassword;					//����
		packet.m_UserInfo.m_eStatus = CUserInfo::LOGIN;								//�û�״̬
		packet.m_UserInfo.m_lIP = hostAddr.sin_addr.S_un.S_addr;					//IP
		packet.m_UserInfo.m_nPort = m_hostAddr.sin_port;							//�˿�
		packet.m_UserInfo.m_time = CTime::GetCurrentTime();							//��¼ʱ��		
		m_UserInfo = packet.m_UserInfo;												//�����û���Ϣ

		//�����û��б�
		SendPacket(packet);
		return TRUE;
	}else
	{
		return FALSE;
	}
}


/*
 * �ȴ�������Ϣ
 */
void CClientDlg::ProcessPendingRead(void)
{
	do
	{
		ReadPacket();
		if (m_pSocket == NULL)
			return;
	}
	while(!m_pArchiveIn->IsBufferEmpty());//������������ȫ������
}

/*
 * ���շ���������
 */
void CClientDlg::ReadPacket(void)
{
	CObList		obList;//��ʱ����
	CChatPacket	packet;//���ݰ�
	packet.m_pUserList = &obList;

	TRY
	{
		packet.Serialize(*m_pArchiveIn);					//��������

		if (packet.m_type == CChatPacket::MESSAGE)			//������Ϣ
		{
			ShowOfflineMsg(&packet);		

		}else if (packet.m_type == CChatPacket::USERLIST)	//�û��б�
		{
			CreateUserList(&obList);						//�����û�����

			UpdateClientListCtl();							//�����û�����

			m_UserInfo.m_eStatus = CUserInfo::ONLINE;		//�޸��û�״̬

		}else if (packet.m_type == CChatPacket::SERVERMSG)	//��¼ʧ����Ϣ
		{
			 MessageBox(packet.m_strMsg, "Server Messages", MB_OK);			
			 EndDialog(IDCANCEL);							//�˳�		
		}		
	}
	CATCH (CFileException, e)
	{
		MessageBox(_T("����chatter�������"));		
	}
	END_CATCH
}

/*
 * �����û��б�
 */
void CClientDlg::UpdateClientListCtl(void)
{
	//ɾ��ԭ���û�����		
	m_ctlUserList.DeleteAllItems(); 
	
	POSITION pos;
	int nIndex = 0;
	for(pos = m_UserList.GetHeadPosition(); pos != NULL;)//������������
	{
		//��ȡÿ���û���Ϣ
		CUserInfo userInfo;
		userInfo =*(CUserInfo*)m_UserList.GetNext(pos);
		
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT |LVIF_IMAGE |LVIF_PARAM;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.pszText = userInfo.m_strName.GetBuffer(10);	
		if (userInfo.m_eStatus == CUserInfo::ONLINE)	//����״̬
		{
			lvItem.iImage = CUserInfo::ONLINE;
		}else										
		{
			lvItem.iImage = CUserInfo::OFFLINE;			//����״̬	
		}		
		lvItem.lParam = (LPARAM)userInfo.m_strName.GetBuffer(10);

		m_ctlUserList.InsertItem(&lvItem);				//���������

		userInfo.m_strName.ReleaseBuffer();		
		nIndex++;		
	}	
}


/*
 * ��ʾ������Ϣ
 */
void CClientDlg::ShowOfflineMsg(CChatPacket *pPacket)
{
	CChatDlg chatDlg(this, pPacket);//������ʾ������Ϣ�Ի���
	chatDlg.DoModal();	
	SaveChatLog(&chatDlg);			//���������¼
}


/*
 * ��������
 */
void  CClientDlg::SendPacket(CChatPacket &packet)
{
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);	//��ʼ��д����	
	
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
			MessageBox(_T("�������ݰ�����"));	
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

	//ɾ��ԭ������
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)m_UserList.GetNext(pos);
		delete pUserInfo;
		pUserInfo = NULL;
	}
	m_UserList.RemoveAll();

	//��������
	CUserInfo *pNewUserInfo = NULL;
	for (pos = pObList->GetHeadPosition(); NULL != pos;)
	{
		pUserInfo = (CUserInfo*)pObList->GetNext(pos);
		pNewUserInfo= new CUserInfo();
		pNewUserInfo->m_strName = pUserInfo->m_strName;	//����
		pNewUserInfo->m_eStatus = pUserInfo->m_eStatus;	//״̬
		pNewUserInfo->m_lIP = pUserInfo->m_lIP;			//��ַ
		pNewUserInfo->m_nPort = pUserInfo->m_nPort;		//�˿�
		m_UserList.AddTail(pNewUserInfo);				//��������
		delete pUserInfo;
		pUserInfo = NULL;
	}
	
	pObList->RemoveAll();
}

/*
 * ˫���б���ͼ�ؼ���Ϣ��Ϣ��Ӧ����
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
 * �������촰��
 */
void CClientDlg::CreateChatDlg( CString strPeerName )
{
	//�����Լ�������Ϣ
	if (strPeerName == m_UserInfo.m_strName)
		return;

	//���û��������ҵ����û�
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

	ASSERT(TRUE == bFinder);//�ҵ����û�
	if (pPeerInfo->m_eStatus == CUserInfo::ONLINE)		//�û�����
	{
		CChatDlg chatDlg(this, *pPeerInfo, m_UserInfo);	//����Ի���
		if (chatDlg.ConnectToPeer())					//���ӶԷ�
		{		
			chatDlg.DoModal();//��ʾ���촰��
			//���������¼
			SaveChatLog(&chatDlg);
		}
	}else//����������Ϣ
	{
		CChatDlg chatDlg(this, *pPeerInfo, m_UserInfo);//����Ի���
		chatDlg.DoModal();//��ʾ���촰��
		
		//�������ݰ�
		CChatPacket packet;
		packet.m_type = CChatPacket::MESSAGE;	//����
		packet.m_UserInfo = m_UserInfo;			//����������Ϣ���û���Ϣ
		packet.m_OfflineUserInfo = *pPeerInfo;	//����������Ϣ���û���Ϣ
		packet.m_strMsg = chatDlg.m_strRec;		//������Ϣ
		packet.m_time = CTime::GetCurrentTime();//������Ϣ��ʱ��

		//�����������ݰ�
		SendPacket(packet);	
		//���������¼
		SaveChatLog(&chatDlg);
	}	
}

/*
 * ��ʼ����
 */
BOOL CClientDlg::BegingListen( void )
{
	m_pListenSocket = new CListenSocket(this);	//������������
	if (m_pListenSocket->Create())				//�������׽���
	{
		if (!m_pListenSocket->Listen())			//��ʼ����
		{
			delete m_pListenSocket;
			m_pListenSocket = NULL;
			AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			return FALSE;			
		}
	}
	//��ȡ��ַ��Ϣ
	SOCKADDR_IN hostAddr;
	int hostAddrLen = sizeof(SOCKADDR_IN);
	m_pListenSocket->GetSockName((SOCKADDR*)&hostAddr,&hostAddrLen);

	m_hostAddr.sin_port = hostAddr.sin_port;//��������˿�
	return TRUE;
}

/*
 *���ܶԷ�������
 */
void CClientDlg::ProcessPendingAccept(void)
{
	CChatDlg	chatDlg(this);	//����Ի���
	chatDlg.InitSocket();		//����CChatSocket����

	//���ܶԷ�����
	CChatSocket *pChatSocket = chatDlg.GetChatSocket();
	SOCKADDR_IN	peerAddr;
	int socketLen = sizeof(SOCKADDR_IN);
 	m_pListenSocket->Accept(*pChatSocket,(SOCKADDR*)&peerAddr, &socketLen);

	//���û������в��Ҹ��û�
	POSITION	pos;
	CUserInfo	*pPeerInfo = NULL;
	BOOL		bFinder = FALSE;
	for (pos = m_UserList.GetHeadPosition(); NULL != pos;)
	{
		pPeerInfo = (CUserInfo*)m_UserList.GetNext(pos);
		if (pPeerInfo->m_lIP == peerAddr.sin_addr.S_un.S_addr &&//�ҵ����û�
			pPeerInfo->m_eStatus == CUserInfo::ONLINE)				
		{
			bFinder = TRUE;
			break;
		}
	}

	ASSERT(TRUE == bFinder);

	//��ȡ�Է���Ϣ
	CUserInfo peerInfo;
	peerInfo.m_strName = pPeerInfo->m_strName;		//����
	peerInfo.m_eStatus = pPeerInfo->m_eStatus;		//״̬
	peerInfo.m_lIP = peerAddr.sin_addr.S_un.S_addr;	//��ַ
	peerInfo.m_nPort = peerAddr.sin_port;			//�˿�


	chatDlg.SetPeerInfor(peerInfo);					//���öԷ���Ϣ	
	chatDlg.SetUserInfor(m_UserInfo);				//�����û���Ϣ
	chatDlg.InitArchive();							//�����׽��ֶ����ĵ�

	//������Ի���
	chatDlg.DoModal();	
	//���������¼
	SaveChatLog(&chatDlg);	
}



/*
 * ��ʼ���б���ͼ�ؼ�
 */
BOOL CClientDlg::InitListCtrlSetting( void )
{
	//����ͼ���б�
	m_pImageList = new CImageList();
	
	//32*32��8λ����ʼ��Ϊ2��ͼ�꣬ÿ������2��ͼ��
	m_pImageList->Create(32, 32, ILC_COLOR8|ILC_MASK, 2, 2);
	CWinApp* pApp = AfxGetApp();								//���Ӧ�ó���ָ��
	HICON hIconOnline = pApp->LoadIcon(IDI_CHATTER_ONLINE);		//����ͼ��
	HICON hIconOffline = pApp->LoadIcon(IDI_CHATTER_OFFLINE);	//����ͼ��

	m_pImageList->Add(hIconOnline);								//����ͼ��
	m_pImageList->Add(hIconOffline);							//����ͼ��
	
	//��ͼ���б�ֵ���б�ؼ�
	m_ctlUserList.SetImageList(m_pImageList, LVSIL_SMALL);
	
	//�����б�ͷ
	m_ctlUserList.InsertColumn(0,_T("Chatter"),LVCFMT_IMAGE|LVCFMT_LEFT);

	//���õ�һ�еĿ��Ϊ����б�ؼ��Ŀ��
	CRect rect;
	m_ctlUserList.GetClientRect(&rect);
	m_ctlUserList.SetColumnWidth(0,rect.Width());

	return TRUE;
	
}

/*
 * ���������¼
 */
void CClientDlg::SaveChatLog(CChatDlg *pChatDlg)
{
	//û��������Ϣ
	if (pChatDlg->m_strRec.IsEmpty())
		return;
	
	//���������¼
	char appPath[256];
	GetCurrentDirectory(256,appPath);	//ȡ��Ӧ�ó���ǰ·��
	CString  filePath;					//�ļ���
	filePath.Format("%s",appPath);
	filePath +="\\";
	filePath += CHATLOG;
	
	TRY
	{
		CFile file;
		if (!file.Open(filePath, CFile::modeReadWrite))					//�ļ��Ѿ�����
		{
			file.Open(filePath, CFile::modeCreate|CFile::modeReadWrite);//�½��ļ�			
			
		}else
		{
			file.SeekToEnd();//�ƶ��ļ�ָ�뵽�ļ�β��
		}
		
		int nRecLen = pChatDlg->m_strRec.GetLength();	//�����¼����			
		file.Write(pChatDlg->m_strRec, nRecLen);		//д������
		file.Flush();
		file.Close();									//�ر��ļ� 		
	}
	CATCH(CFileException, e)
	{
		MessageBox(_T("���������¼�쳣"),MB_OK, -1);
	}
	END_CATCH	
}

