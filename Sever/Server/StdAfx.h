// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7A83B24B_8A6A_450F_A57E_EF4CED9BAF3C__INCLUDED_)
#define AFX_STDAFX_H__7A83B24B_8A6A_450F_A57E_EF4CED9BAF3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

//程序定义
#include "UserInfo.h"
#include "ChatPacket.h"
#include <afxmt.h>
#define	USERLISTINI			"userlist.ini"
#define	OFFLINEMSGLIST		"offlinemsglist.ini"
#define SERVERPORT			5558
#define MAX_MSG_SIZE		128						//发送消息的最大长度




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7A83B24B_8A6A_450F_A57E_EF4CED9BAF3C__INCLUDED_)
