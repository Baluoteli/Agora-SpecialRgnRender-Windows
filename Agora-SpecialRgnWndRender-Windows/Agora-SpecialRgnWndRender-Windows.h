
// Agora-SpecialRgnWndRender-Windows.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAgoraSpecialRgnWndRenderWindowsApp:
// See Agora-SpecialRgnWndRender-Windows.cpp for the implementation of this class
//

class CAgoraSpecialRgnWndRenderWindowsApp : public CWinApp
{
public:
	CAgoraSpecialRgnWndRenderWindowsApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAgoraSpecialRgnWndRenderWindowsApp theApp;