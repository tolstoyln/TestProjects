
// NativeInprocServerClientDlg.h : header file
//

#pragma once


// CNativeInprocServerClientDlg dialog
class CNativeInprocServerClientDlg : public CDialogEx
{
// Construction
public:
	CNativeInprocServerClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NATIVEINPROCSERVERCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
};
