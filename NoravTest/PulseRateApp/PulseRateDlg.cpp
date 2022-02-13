
// PulseRateDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GdiUtil.h"
#include "PulseRateApp.h"
#include "PulseRateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPulseRateDlg dialog

CPulseRateDlg::CPulseRateDlg(CWnd* pParent /*=nullptr*/)
	: CResizeDlg(IDD_PULSERATEAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPulseRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPHSAMPLECTRL, m_graph);
}

bool CPulseRateDlg::QueryNewCtrlPos(int nCtrlID,
	int cxDlgInit,
	int cyDlgInit,
	int cxDlgNew,
	int cyDlgNew,
	const CRect* pCtrlRectInit,
	LPRECT pCtrlRectNew)
{
	switch (nCtrlID)
	{
	case IDC_GRAPHSAMPLECTRL:
		pCtrlRectNew->left = pCtrlRectInit->left;
		pCtrlRectNew->top = pCtrlRectInit->top;
		pCtrlRectNew->right = pCtrlRectInit->right + cxDlgNew - cxDlgInit;
		pCtrlRectNew->bottom = pCtrlRectInit->bottom + cyDlgNew - cyDlgInit;
		return true;

	case ID_BTN_STARTSTOP:
	case ID_STC_PROBEINTERVAL:
	case ID_CMB_PROBEINTERVAL:
	case ID_STC_VIEWMODE:
	case ID_CMB_VIEWMODE:
	case ID_STC_PULSERATE:
	case ID_BTN_PDF:
		pCtrlRectNew->left = pCtrlRectInit->left + cxDlgNew - cxDlgInit;
		pCtrlRectNew->top = pCtrlRectInit->top;
		pCtrlRectNew->right = pCtrlRectInit->right + cxDlgNew - cxDlgInit;
		pCtrlRectNew->bottom = pCtrlRectInit->bottom;
		return true;

	case ID_STC_PATIENT:
		pCtrlRectNew->left = pCtrlRectInit->left;
		pCtrlRectNew->top = pCtrlRectInit->top;
		pCtrlRectNew->right = pCtrlRectInit->right + cxDlgNew - cxDlgInit;
		pCtrlRectNew->bottom = pCtrlRectInit->bottom;
		return true;

	case IDCANCEL:
		pCtrlRectNew->left = pCtrlRectInit->left + cxDlgNew - cxDlgInit;
		pCtrlRectNew->top = pCtrlRectInit->top + cyDlgNew - cyDlgInit;
		pCtrlRectNew->right = pCtrlRectInit->right + cxDlgNew - cxDlgInit;
		pCtrlRectNew->bottom = pCtrlRectInit->bottom + cyDlgNew - cyDlgInit;
		return true;

	default:
		break;
	}

	return false;
}

BEGIN_MESSAGE_MAP(CPulseRateDlg, CResizeDlg)
	ON_UPDATE_COMMAND_UI(ID_BTN_STARTSTOP, &CPulseRateDlg::OnUpdateCmdUIBtnStartstop)
	ON_BN_CLICKED(ID_BTN_STARTSTOP, &CPulseRateDlg::OnBnClickedBtnStartstop)
	ON_CBN_SELCHANGE(ID_CMB_VIEWMODE, &CPulseRateDlg::OnCbnSelchangeCmbViewmode)
	ON_BN_CLICKED(IDCANCEL, &CPulseRateDlg::OnBnClickedCancel)
	ON_UPDATE_COMMAND_UI(ID_STC_PULSERATE, &CPulseRateDlg::OnUpdateCmdUIPulseRate)
	ON_UPDATE_COMMAND_UI(ID_CMB_PROBEINTERVAL, &CPulseRateDlg::OnUpdateCmdUIProbeInterval)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_PDF, &CPulseRateDlg::OnBnClickedBtnPdf)
	ON_UPDATE_COMMAND_UI(ID_BTN_PDF, &CPulseRateDlg::OnUpdateCmdUIPdf)
END_MESSAGE_MAP()


// CPulseRateDlg message handlers

BOOL CPulseRateDlg::OnInitDialog()
{
	CResizeDlg::OnInitDialog();

	SetIcon(m_hIcon, FALSE);		// Set small icon

	GdiUtil::SetFontWeight(GetDlgItem(ID_STC_PATIENT), FW_MEDIUM);
	GdiUtil::FitFont(GetDlgItem(ID_STC_PATIENT));

	GdiUtil::SetFontWeight(GetDlgItem(ID_STC_PULSERATE), FW_SEMIBOLD);
	GdiUtil::FitFont(GetDlgItem(ID_STC_PULSERATE));

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(CRect(cxScreen / 6, cyScreen / 6, cxScreen * 5 / 6, cyScreen * 5 / 6));

	static_cast<CComboBox*>(GetDlgItem(ID_CMB_PROBEINTERVAL))->SetCurSel(0);
	static_cast<CComboBox*>(GetDlgItem(ID_CMB_VIEWMODE))->SetCurSel(0);

	return TRUE;
}

void CPulseRateDlg::OnUpdateCmdUIBtnStartstop(CCmdUI* pCmdUI)
{
	CString str;
	str.LoadString(m_bStarted ? IDS_STOP : IDS_START);
	pCmdUI->SetText(str);
}

void CPulseRateDlg::OnBnClickedBtnStartstop()
{
	if (m_bStarted)
		Stop();
	else
		Start();
	m_bStarted = !m_bStarted;
}

void CPulseRateDlg::OnCbnSelchangeCmbViewmode()
{
	static const long _modes[] = { PRT_VIEW_MODE_0, PRT_VIEW_MODE_1, PRT_VIEW_MODE_2, PRT_VIEW_MODE_3 };
	m_graph.SetMode(_modes[static_cast<CComboBox*>(GetDlgItem(ID_CMB_VIEWMODE))->GetCurSel()]);
}

void CPulseRateDlg::OnUpdateCmdUIPulseRate(CCmdUI* pCmdUI)
{
	CString str;
	str.Format(_T("%d bpm"), CPulseDataProvider::/*Average*/Last());
	pCmdUI->SetText(str);
}

void CPulseRateDlg::OnUpdateCmdUIProbeInterval(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

HBRUSH CPulseRateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == ID_STC_PULSERATE)
	{
		COLORREF clr = CPulseDataProvider::IsNormal() ? RGB(0, 255, 0) : RGB(255, 102, 102);
		pDC->SetBkColor(clr);
		return GdiUtil::GetBrush(clr);
	}
	return __super::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPulseRateDlg::OnBnClickedCancel()
{
	Stop();
	OnCancel();
}

void CPulseRateDlg::Start()
{
	static const long _intervals[] = { PRT_PROBE_INTERVAL_0, PRT_PROBE_INTERVAL_1, PRT_PROBE_INTERVAL_2, PRT_PROBE_INTERVAL_3 };
	long nInterval = _intervals[static_cast<CComboBox*>(GetDlgItem(ID_CMB_PROBEINTERVAL))->GetCurSel()];
	m_graph.Clear();
	m_graph.SetInterval(nInterval);
	CPulseDataProvider::Start(this, nInterval);
}
void CPulseRateDlg::Stop()
{
	CPulseDataProvider::Stop();
}

void CPulseRateDlg::OnUpdateCmdUIPdf(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CPulseDataProvider::HasData());
}

void CPulseRateDlg::OnBnClickedBtnPdf()
{
	CString strPatient;
	GetDlgItemText(ID_STC_PATIENT, strPatient);

	CFileDialog fd(FALSE, _T(".pdf"), strPatient, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("PDF Files (*.pdf)|*.pdf||"), this);

	if (fd.DoModal() == IDOK)
	{
		CString strPDF(fd.GetPathName());
		CString strErr = m_graph.WritePDF(strPatient, strPDF);

		if (strErr.IsEmpty())
		{
			SHELLEXECUTEINFO se = { 0 };
			se.cbSize = sizeof(SHELLEXECUTEINFO);
			se.lpVerb = _T("open");
			se.lpFile = strPDF;
			se.nShow = SW_SHOWNORMAL;
			ShellExecuteEx(&se);
		}
		else
		{
			ReportError(strErr);
		}
	}
}

void CPulseRateDlg::ReportError(const CString& strMsg)
{
	CString strTitle;
	GetWindowText(strTitle);
	MessageBox(strMsg, strTitle, MB_ICONERROR | MB_OK);
}

