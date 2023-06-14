
// NativeInprocServerClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NativeInprocServerClient.h"
#include "NativeInprocServerClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNativeInprocServerClientDlg dialog



CNativeInprocServerClientDlg::CNativeInprocServerClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NATIVEINPROCSERVERCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNativeInprocServerClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNativeInprocServerClientDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CNativeInprocServerClientDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CNativeInprocServerClientDlg message handlers

BOOL CNativeInprocServerClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}


class CSampleDispatchWrapper : public COleDispatchDriver
{
public:
	CSampleDispatchWrapper() { CreateDispatch(_T("SampleDotNetInprocServer.SampleComClass")); }
	~CSampleDispatchWrapper() { ReleaseDispatch(); }
	void SampleComMethodCall()
	{
		InvokeHelper(1, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
};


void CNativeInprocServerClientDlg::OnBnClickedButtonTest()
{
	CSampleDispatchWrapper disp;
	disp.SampleComMethodCall();
}
