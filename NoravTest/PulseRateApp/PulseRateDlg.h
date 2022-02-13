
// PulseRateDlg.h : header file
//

#pragma once
#include "ResizeDlg.h"
#include "GraphCtl.h"
#include "PulseDataProvider.h"

// CPulseRateDlg dialog
class CPulseRateDlg : public CResizeDlg, public IPulseDataConsumer
{
// Construction
public:
	CPulseRateDlg(CWnd* pParent = nullptr);	// standard constructor

	// IPulseDataConsumer
	void OnPulseData(long val) { m_graph.AddValue(val); }
	void OnTestFinished() { m_bStarted = false; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PULSERATEAPP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual bool QueryNewCtrlPos(int nCtrlID, int cxDlgInit, int cyDlgInit, int cxDlgNew, int cyDlgNew,
		const CRect* pCtrlRectInit, LPRECT pCtrlRectNew);

// Implementation
protected:
	HICON m_hIcon;
	void ReportError(const CString& strMsg);

private:
	bool m_bStarted = false;
	CGraphCtl m_graph;
	void Start();
	void Stop();

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateCmdUIBtnStartstop(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBtnStartstop();
	afx_msg void OnCbnSelchangeCmbViewmode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnUpdateCmdUIPulseRate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdUIProbeInterval(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedBtnPdf();
	afx_msg void OnUpdateCmdUIPdf(CCmdUI* pCmdUI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
