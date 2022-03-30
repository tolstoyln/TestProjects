
// SampleLocalServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SampleLocalServer.h"
#include "HiddenWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleLocalServerApp

BEGIN_MESSAGE_MAP(CSampleLocalServerApp, CWinApp)
END_MESSAGE_MAP()


// CSampleLocalServerApp construction

CSampleLocalServerApp::CSampleLocalServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSampleLocalServerApp object

CSampleLocalServerApp theApp;


// CSampleLocalServerApp initialization

BOOL CSampleLocalServerApp::InitInstance()
{
	if (!CWinApp::InitInstance())
	{
		return FALSE;
	}

	if (!AfxOleInit())
	{
		return FALSE;
	}

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!cmdInfo.m_bRunEmbedded && !cmdInfo.m_bRunAutomated)
	{
		COleObjectFactory::UpdateRegistryAll(cmdInfo.m_nShellCommand != CCommandLineInfo::AppUnregister);	// Command line: /UnregServer to unregister
		return FALSE;
	}

	if (!theWnd.Create())
	{
		return FALSE;
	}
	m_pMainWnd = &theWnd;

	COleObjectFactory::RegisterAll();
	
	return TRUE;
}

int CSampleLocalServerApp::ExitInstance()
{
	COleObjectFactory::RevokeAll();

	return CWinApp::ExitInstance();
}
