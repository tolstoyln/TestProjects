// SampleDllClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define _EXPLICIT_LINK

#ifdef _EXPLICIT_LINK
	#include "ExportCall.hpp"
#else
	#ifdef _DEBUG
		#pragma comment(lib,"../Debug/SampleDll.lib")
	#else
		#pragma comment(lib,"../Release/SampleDll.lib")
	#endif
	extern "C" __declspec(dllimport) BOOL WINAPI ShowMessage(LPCTSTR cszMsgText);
#endif // _EXPLICIT_LINK

int main()
{
#ifdef _EXPLICIT_LINK
	export_call("SampleDll.dll", "ShowMessage", FALSE, _T("It works!"));
#else
	ShowMessage(_T("It works!"));
#endif // _EXPLICIT_LINK
}

