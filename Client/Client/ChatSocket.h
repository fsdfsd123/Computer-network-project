#if !defined(AFX_CHATSOCKET_H__F18E600A_6DBF_4D2A_A41C_4EC622B82B84__INCLUDED_)
#define AFX_CHATSOCKET_H__F18E600A_6DBF_4D2A_A41C_4EC622B82B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatSocket.h : header file
//

class CClientDlg;
class CChatDlg;

/////////////////////////////////////////////////////////////////////////////
// CChatSocket command target

class CChatSocket : public CSocket
{
	// Attributes
public:
	
	// Operations
public:
	CChatSocket(CClientDlg *pClientDlg);
	CChatSocket(CChatDlg *m_pChatDlg);

	virtual ~CChatSocket();
	
	// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatSocket)
	//}}AFX_VIRTUAL
	
	// Generated message map functions
	//{{AFX_MSG(CChatSocket)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	// Implementation
protected:
		virtual void OnReceive(int nErrorCode);

protected:

private:
	CClientDlg		*m_pClientDlg;	//主对话框指针
	CChatDlg		*m_pChatDlg;	//聊天对话框指针
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSOCKET_H__F18E600A_6DBF_4D2A_A41C_4EC622B82B84__INCLUDED_)
