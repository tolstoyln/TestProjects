#pragma once

class CSampleDispatchWrapper : public COleDispatchDriver
{
public:
	BOOL ShowMessage(LPCTSTR cszMsg)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, cszMsg);
		return result;
	}
};
