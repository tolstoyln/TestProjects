// SampleDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#pragma comment(linker, "/export:ShowMessage=_ShowMessage@4")

extern "C" __declspec(dllexport) BOOL WINAPI ShowMessage(LPCTSTR cszMsgText)
{
	return MessageBox(NULL, cszMsgText, _T("Sample DLL"), MB_OK);
}
