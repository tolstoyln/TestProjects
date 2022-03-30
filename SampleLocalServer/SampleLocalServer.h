
// SampleLocalServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


// CSampleLocalServerApp:
// See SampleLocalServer.cpp for the implementation of this class
//

class CSampleLocalServerApp : public CWinApp
{
public:
	CSampleLocalServerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSampleLocalServerApp theApp;
