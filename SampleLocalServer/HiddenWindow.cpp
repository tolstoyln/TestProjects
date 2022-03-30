
// HiddenWindow.cpp : implementation file
//

#include "stdafx.h"
#include "SampleLocalServer.h"
#include "HiddenWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHiddenWindow window
CHiddenWindow theWnd;

BEGIN_MESSAGE_MAP(CHiddenWindow, CWnd)
	
END_MESSAGE_MAP()

BOOL CHiddenWindow::Create()
{
	return CreateEx(NULL, AfxRegisterWndClass(NULL, NULL, NULL, NULL), NULL, WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL);
}

// CHiddenWindow message handlers


