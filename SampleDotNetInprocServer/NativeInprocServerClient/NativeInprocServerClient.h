
// NativeInprocServerClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNativeInprocServerClientApp:
// See NativeInprocServerClient.cpp for the implementation of this class
//

class CNativeInprocServerClientApp : public CWinApp
{
public:
	CNativeInprocServerClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNativeInprocServerClientApp theApp;