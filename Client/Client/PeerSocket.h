#if !defined(AFX_PEERSOCKET_H__3842AA97_CDF8_4316_BCB6_D1CE55135E8F__INCLUDED_)
#define AFX_PEERSOCKET_H__3842AA97_CDF8_4316_BCB6_D1CE55135E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeerSocket.h : header file
//


class CClientDlg;
/////////////////////////////////////////////////////////////////////////////
// CPeerSocket command target

class CPeerSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CPeerSocket(CClientDlg *pServerDlg);
	virtual ~CPeerSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeerSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPeerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	protected:
	virtual void OnAccept(int nErrorCode);

protected:
	CClientDlg	*m_pClientDlg;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEERSOCKET_H__3842AA97_CDF8_4316_BCB6_D1CE55135E8F__INCLUDED_)
