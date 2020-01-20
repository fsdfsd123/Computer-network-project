#if !defined(AFX_LISTENSOCKET_H__55C9A35A_A582_4C63_8EDF_10868A5F07A2__INCLUDED_)
#define AFX_LISTENSOCKET_H__55C9A35A_A582_4C63_8EDF_10868A5F07A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target
//
//监听客户端的连接请求
//
//
class CServerDlg;

class CListenSocket : public CSocket
{
// Attributes
public:
	CListenSocket(CServerDlg* pServerDlg);
	virtual ~CListenSocket();
// Operations
public:


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
public:

//	virtual void OnClose(int nErrorCode);

// Overridable callbacks
protected:
	virtual void OnAccept(int nErrorCode);

protected:
	CServerDlg	*m_pServerDlg;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__55C9A35A_A582_4C63_8EDF_10868A5F07A2__INCLUDED_)
