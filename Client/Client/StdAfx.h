// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__60C168D2_487E_4103_B2D5_E91852487DCA__INCLUDED_)
#define AFX_STDAFX_H__60C168D2_487E_4103_B2D5_E91852487DCA__INCLUDED_

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



//该程序定义

#include <afxsock.h>
#include "ChatPacket.h"
#include "UserInfo.h"

#define	SERVERPORT		5558
#define	CHATLOG			"chatlog.txt" 
#pragma comment(lib, "ws2_32.lib")




#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__60C168D2_487E_4103_B2D5_E91852487DCA__INCLUDED_)
