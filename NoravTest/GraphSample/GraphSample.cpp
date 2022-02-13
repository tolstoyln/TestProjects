// GraphSample.cpp : Implementation of CGraphSampleApp and DLL registration.

#include "pch.h"
#include "framework.h"
#include "GraphSample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CGraphSampleApp theApp;

const GUID CDECL _tlid = {0x7a08b7d7,0xca21,0x464b,{0x81,0x9f,0xe6,0xe5,0x7b,0x3a,0x22,0xd3}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CGraphSampleApp::InitInstance - DLL initialization

BOOL CGraphSampleApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CGraphSampleApp::ExitInstance - DLL termination

int CGraphSampleApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
