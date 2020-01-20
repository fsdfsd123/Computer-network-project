#if !defined(AFX_LISTENSOCKET_H__78A890C4_C6D7_4D2B_997B_EF6145E94357__INCLUDED_)
#define AFX_LISTENSOCKET_H__78A890C4_C6D7_4D2B_997B_EF6145E94357__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

class CClientDlg;

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CListenSocket(CClientDlg *pServerDlg);
	virtual ~CListenSocket();

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
protected:

public:
	virtual void OnAccept(int nErrorCode);

protected:
	CClientDlg	*m_pClientDlg;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__78A890C4_C6D7_4D2B_997B_EF6145E94357__INCLUDED_)
