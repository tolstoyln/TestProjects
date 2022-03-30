#include "stdafx.h"
#include "SampleAutomationObject.h"
#include "SampleAutomationLib_h.h"
#include "SampleAutomationLib_i.c"

IMPLEMENT_DYNCREATE(CSampleAutomationObject, CCmdTarget)
IMPLEMENT_OLECREATE(CSampleAutomationObject, "SampleLocalServer.SampleAutomationObject", 0x709FF59B, 0xCDA4, 0x4D7E, 0x88, 0x32, 0x25, 0x1D, 0xFA, 0x15, 0x56, 0x56)

CSampleAutomationObject::CSampleAutomationObject()
{
	EnableAutomation();
	AfxOleLockApp();
}

CSampleAutomationObject::~CSampleAutomationObject()
{
	AfxOleUnlockApp();
}

void CSampleAutomationObject::OnFinalRelease()
{
	CCmdTarget::OnFinalRelease();
}

BEGIN_INTERFACE_MAP(CSampleAutomationObject, CCmdTarget)
	INTERFACE_PART(CSampleAutomationObject, IID_ISampleAutomationObject, Dispatch)
END_INTERFACE_MAP()

BEGIN_DISPATCH_MAP(CSampleAutomationObject, CCmdTarget)
	DISP_FUNCTION_ID(CSampleAutomationObject, "ShowMessage", 1, ShowMessage, VT_BOOL, VTS_BSTR)
END_DISPATCH_MAP()


// ISampleAutomationObject methods:

BOOL CSampleAutomationObject::ShowMessage(LPCTSTR message)
{
	MessageBox(NULL, message, _T("SampleAutomationObject"), MB_OK);
	return TRUE;
}
