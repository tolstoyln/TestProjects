// SampleAutomationClient.h : main header file for the SampleAutomationClient DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSampleAutomationClientApp
// See SampleAutomationClient.cpp for the implementation of this class
//

class CSampleAutomationClientApp : public CWinApp
{
public:
	CSampleAutomationClientApp();
	CSampleDispatchWrapper* GetServerDispatch();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

private:
	CSampleDispatchWrapper m_disp;
};
