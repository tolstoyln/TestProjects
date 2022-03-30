// SampleAutomationClient.cpp : Defines the initialization routines and the export API for the DLL.
//

#include "stdafx.h"
#include "SampleDispatchWrapper.h"
#include "SampleAutomationClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleAutomationClientApp

BEGIN_MESSAGE_MAP(CSampleAutomationClientApp, CWinApp)
END_MESSAGE_MAP()


// CSampleAutomationClientApp construction

CSampleAutomationClientApp::CSampleAutomationClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSampleAutomationClientApp object

CSampleAutomationClientApp theApp;


// CSampleAutomationClientApp initialization

BOOL CSampleAutomationClientApp::InitInstance()
{
	CWinApp::InitInstance();

	CoInitialize(NULL);

	return TRUE;
}

int CSampleAutomationClientApp::ExitInstance()
{
	m_disp.ReleaseDispatch();

	return CWinApp::ExitInstance();
}


CSampleDispatchWrapper* CSampleAutomationClientApp::GetServerDispatch()
{
	if (m_disp.m_lpDispatch == NULL)
	{
		m_disp.CreateDispatch("SampleLocalServer.SampleAutomationObject");
	}
	return m_disp.m_lpDispatch != NULL ? &m_disp : NULL;
}


// Export API

extern "C" BOOL WINAPI EXPORT ShowMessage(LPCTSTR czsMsgText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSampleDispatchWrapper* pDisp = theApp.GetServerDispatch();

	if (pDisp == NULL)
	{
		return FALSE;
	}

	return pDisp->ShowMessage(czsMsgText);
}
