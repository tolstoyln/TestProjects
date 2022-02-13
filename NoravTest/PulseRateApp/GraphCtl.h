#pragma once

/////////////////////////////////////////////////////////////////////////////
// Graph Sample OCX wrapper class

class CGraphCtl : public CWnd
{
public:
	void SetMode(long val)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, val);
	}
	void SetInterval(long val)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, val);
	}
	void Clear()
	{
		InvokeHelper(10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddValue(long val)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, val);
	}
	CString WritePDF(LPCTSTR patient, LPCTSTR pdfFile)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		CString result;
		InvokeHelper(30, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, patient, pdfFile);
		return result;
	}
};
