// GraphSampleCtrl.cpp : Implementation of the CGraphSampleCtrl ActiveX Control class.

#include "pch.h"
#include "framework.h"
#include <sstream>
#include <iomanip>
#include "GraphSample.h"
#include "GraphSampleCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGraphSampleCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CGraphSampleCtrl, COleControl)
	//ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// Dispatch map

BEGIN_DISPATCH_MAP(CGraphSampleCtrl, COleControl)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_PROPERTY_EX_ID(CGraphSampleCtrl, "Mode", dispidMode, GetMode, SetMode, VT_I4)
	DISP_PROPERTY_EX_ID(CGraphSampleCtrl, "Interval", dispidInterval, GetInterval, SetInterval, VT_I4)
	DISP_FUNCTION_ID(CGraphSampleCtrl, "Clear", dispidClear, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CGraphSampleCtrl, "AddValue", dispidAddValue, AddValue, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CGraphSampleCtrl, "WritePDF", dispidWritePDF, WritePDF, VT_BSTR, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

// Event map

BEGIN_EVENT_MAP(CGraphSampleCtrl, COleControl)
END_EVENT_MAP()

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGraphSampleCtrl, "NoravTest.GraphSampleCtrl",
	0x015662fe,0xfbd6,0x4ffb,0xa1,0x54,0xcb,0x6b,0xbc,0x9a,0xd4,0x55)

// Type library ID and version

IMPLEMENT_OLETYPELIB(CGraphSampleCtrl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs

const IID IID_DGraphSample = {0x5cbf15d6,0xa09b,0x470b,{0xa7,0x8c,0xa0,0x36,0x48,0xc0,0xe2,0x91}};
const IID IID_DGraphSampleEvents = {0x6c5b52be,0xc011,0x4993,{0xb0,0x4f,0x6d,0x42,0x5b,0x52,0x40,0x35}};

// Control type information

static const DWORD _dwGraphSampleOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CGraphSampleCtrl, IDS_GRAPHSAMPLE, _dwGraphSampleOleMisc)

// CGraphSampleCtrl::CGraphSampleCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CGraphSampleCtrl

BOOL CGraphSampleCtrl::CGraphSampleCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GRAPHSAMPLE,
			0,
			afxRegApartmentThreading,
			_dwGraphSampleOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CGraphSampleCtrl::CGraphSampleCtrl - Constructor

CGraphSampleCtrl::CGraphSampleCtrl()
{
	InitializeIIDs(&IID_DGraphSample, &IID_DGraphSampleEvents);
	// TODO: Initialize your control's instance data here.
}

// CGraphSampleCtrl::~CGraphSampleCtrl - Destructor

CGraphSampleCtrl::~CGraphSampleCtrl()
{
	// TODO: Cleanup your control's instance data here.
}

// CGraphSampleCtrl::OnDraw - Drawing function

void CGraphSampleCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	CDC memDC;
	memDC.CreateCompatibleDC(pdc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdc, rcBounds.Width(), rcBounds.Height());
	memDC.SelectObject(bmp);

	// Border:
	CBrush br(TranslateColor(GetBackColor()));
	memDC.FillRect(rcBounds, &br);
	memDC.Rectangle(rcBounds);

	m_rcPlot = rcBounds;
	m_rcPlot.DeflateRect(36, 32);

	// Plot
	DrawAxes(&memDC, rcBounds);
	DrawData(&memDC);

	pdc->BitBlt(0, 0, rcBounds.Width(), rcBounds.Height(), &memDC, 0, 0, SRCCOPY);
}

void CGraphSampleCtrl::DrawAxes(CDC* pdc, const CRect& rcBounds)
{
	CPen pnAxe(PS_SOLID, 4, GetSysColor(COLOR_3DDKSHADOW));
	CPen* pPenOld = pdc->SelectObject(&pnAxe);
	pdc->Rectangle(m_rcPlot);

	CPen pnGrid(PS_DOT, 1, GetSysColor(COLOR_3DDKSHADOW));
	CPen pnTick(PS_SOLID, 2, GetSysColor(COLOR_3DDKSHADOW));

	CRect rc(m_rcPlot);
	rc.DeflateRect(2, 2);

	// Vertical
	for (int i = 1; i < 20; ++i)
	{
		pPenOld = pdc->SelectObject(&pnGrid);
		pdc->MoveTo(rc.left, rc.top + rc.Height() * i / 20);
		pdc->LineTo(rc.right, rc.top + rc.Height() * i / 20);

		pdc->SelectObject(&pnTick);
		pdc->MoveTo(rc.left, rc.top + rc.Height() * i / 20);
		pdc->LineTo(rc.left + 8, rc.top + rc.Height() * i / 20);

		if (!(i & 1))
		{
			CRect rcTxt(rcBounds.left, rc.top + rc.Height() * i / 20, m_rcPlot.left - 4, rc.top + rc.Height() * i / 20);
			rcTxt.InflateRect(0, 20);
			CString str;
			str.Format(_T("%d"), PRT_PULSE_RATE_MAX - MulDiv(i, PRT_PULSE_RATE_MAX - PRT_PULSE_RATE_MIN, 20));
			pdc->DrawTextEx(str, const_cast<LPRECT>((LPCRECT)rcTxt), DT_SINGLELINE | DT_RIGHT | DT_VCENTER, NULL);
		}
	}

	// Horizontal
	for (int i = 0; i <= TimeSpans(); ++i)
	{
		if (i > 0 && i < TimeSpans())
		{
			pPenOld = pdc->SelectObject(&pnGrid);
			pdc->MoveTo(rc.left + rc.Width() * i / TimeSpans(), rc.top);
			pdc->LineTo(rc.left + rc.Width() * i / TimeSpans(), rc.bottom);

			pdc->SelectObject(&pnTick);
			pdc->MoveTo(rc.left + rc.Width() * i / TimeSpans(), rc.bottom);
			pdc->LineTo(rc.left + rc.Width() * i / TimeSpans(), rc.bottom - 10);
		}

		if (!(i & 1))
		{
			CRect rcTxt(rc.left + rc.Width() * i / TimeSpans(), m_rcPlot.bottom, rc.left + rc.Width() * i / TimeSpans(), rcBounds.bottom);
			rcTxt.InflateRect(40, 0);
			CString str;
			int t = OriginTime() + i * (m_nMode / TimeSpans());
			str.Format(_T("%02d:%02d"), t / 60, t % 60);
			pdc->DrawTextEx(str, const_cast<LPRECT>((LPCRECT)rcTxt), DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL);
		}
	}

	pdc->SelectObject(pPenOld);
}

void CGraphSampleCtrl::DrawData(CDC* pdc)
{
	if (!m_data.empty())
	{
		CRect rc(m_rcPlot);
		rc.DeflateRect(2, 2);

		COLORREF clr(RGB(0, 0, 255));
		CPen pn(PS_SOLID, 3, clr);
		CPen* pPenOld = pdc->SelectObject(&pn);

		// Graph
		std::vector<POINT> pts;
		for (auto itStart = std::next(m_data.begin(), OriginTime() / m_nInterval), it = itStart; it != m_data.end(); ++it)
		{
			CPoint pt(rc.left + MulDiv(rc.Width(), m_nInterval * std::distance(itStart, it), m_nMode),
				rc.bottom - MulDiv(rc.Height(), *it - PRT_PULSE_RATE_MIN, PRT_PULSE_RATE_MAX - PRT_PULSE_RATE_MIN));
			pts.push_back(pt);
		}
		pdc->Polyline(pts.data(), pts.size());

		// Dots
		if (m_nMode <= PRT_VIEW_MODE_1)
		{
			CBrush br(clr);
			for (auto pt : pts)
			{
				CRect r(pt, pt);
				r.InflateRect(4, 4);
				pdc->FillRect(&r, &br);
			}
		}

		pdc->SelectObject(pPenOld);
	}
}

// CGraphSampleCtrl::DoPropExchange - Persistence support

void CGraphSampleCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}


// CGraphSampleCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CGraphSampleCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();

	// The control's output is not being clipped.
	// The control guarantees that it will not paint outside its
	// client rectangle.
	dwFlags &= ~clipPaintDC;
	return dwFlags;
}


// CGraphSampleCtrl::OnResetState - Reset control to default state

void CGraphSampleCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}

void CGraphSampleCtrl::SetMode(long val)
{
	m_nMode = val;
	Invalidate(FALSE);
	UpdateWindow();
}

void CGraphSampleCtrl::Clear()
{
	m_data.clear();
	Invalidate(FALSE);
	UpdateWindow();
}

void CGraphSampleCtrl::AddValue(long val)
{
	m_data.push_back(val);
	Invalidate(FALSE);
	UpdateWindow();
}

BSTR CGraphSampleCtrl::WritePDF(LPCTSTR cszPatient, LPCTSTR cszPdfFile)
{
	USES_CONVERSION;
	PdfLib::PDF pdf;

	int marginX = 40, marginY = 60;

	// Patient name
	pdf.setFont(PdfLib::PDF::HELVETICA_BOLD, 20);
	std::string strPatient(W2A(cszPatient));
	int cx = pdf.stringWidth(strPatient);
	pdf.showTextXY(strPatient, (pdf.getWidth() - cx) / 2, pdf.getHeight() - marginY);

	// Plot
	CRect rcPlot(marginX + 10, pdf.getHeight() / 2 - 40, pdf.getWidth() - marginX, pdf.getHeight() - marginY - 40);
	PlotToPDF(pdf, rcPlot);

	// Data table
	CRect rcTable(CPoint(marginX, marginY), CSize(pdf.getWidth() - 2 * marginX, pdf.getHeight() - 2 * marginY));
	for (auto it = m_data.begin(); it != m_data.end();)
	{
		pdf.newPage();
		DataToPDF(pdf, rcTable, it);
	}

	// Save file
	std::string errMsg;
	pdf.writeToFile(W2A(cszPdfFile), errMsg);
	return CComBSTR(errMsg.c_str()).Detach();
}

static std::string _FormatTime(int sec)	// min:sec
{
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << sec / 60 << ":"
		<< std::setw(2) << std::setfill('0') << sec % 60;
	return oss.str();
}

void CGraphSampleCtrl::PlotToPDF(PdfLib::PDF& pdf, const CRect& rcPlot)
{
	pdf.setLineWidth(2);
	pdf.setLineColor(96, 96, 96);
	pdf.drawRect(rcPlot.left, rcPlot.top, rcPlot.Width(), rcPlot.Height());

	CRect rc(rcPlot);
	rc.DeflateRect(1, 1);

	pdf.setLineWidth(1);
	pdf.setFont(PdfLib::PDF::HELVETICA, 10);

	// Vertical axes
	for (int i = 1; i < 20; ++i)
	{
		int y = rc.top + rc.Height() * i / 20;
		pdf.setLineColor(96, 96, 96);
		pdf.drawLine(rc.left, y, rc.left + 4, y);
		pdf.setLineColor(224, 224, 224);
		pdf.drawLine(rc.left + 4, y, rc.right, y);

		if (!(i & 1))
		{
			pdf.rightJustifyTextXY(std::to_string(PRT_PULSE_RATE_MIN + MulDiv(i, PRT_PULSE_RATE_MAX - PRT_PULSE_RATE_MIN, 20)),
				rc.left - 5, y - 3);
		}
	}

	// Horizontal axes
	for (int i = 0; i <= TimeSpans(); ++i)
	{
		int x = rc.left + rc.Width() * i / TimeSpans();

		if (i > 0 && i < TimeSpans())
		{
			pdf.setLineColor(96, 96, 96);
			pdf.drawLine(x, rc.top, x, rc.top + 4);
			pdf.setLineColor(224, 224, 224);
			pdf.drawLine(x, rc.top + 4, x, rc.bottom);
		}

		if (!(i & 1))
		{
			std::string strTime = _FormatTime(OriginTime() + i * (m_nMode / TimeSpans()));
			int cx = pdf.stringWidth(strTime);
			pdf.showTextXY(strTime, x - cx / 2, rc.top - 15);
		}
	}

	std::vector<PdfLib::XY> pts;

	// Graph
	pdf.setLineColor(0, 0, 204);
	for (auto itStart = std::next(m_data.begin(), OriginTime() / m_nInterval), it = itStart; it != m_data.end(); ++it)
	{
		PdfLib::XY pt(rc.left + MulDiv(rc.Width(), m_nInterval * std::distance(itStart, it), m_nMode),
			rc.top + MulDiv(rc.Height(), *it - PRT_PULSE_RATE_MIN, PRT_PULSE_RATE_MAX - PRT_PULSE_RATE_MIN));
		pts.push_back(pt);
	}
	pdf.drawLine(pts);

	// Dots
	if (m_nMode <= PRT_VIEW_MODE_1)
	{
		pdf.setFillColor(0, 0, 204);
		for (auto pt : pts)
		{
			pdf.fillRect(pt.mX - 2, pt.mY - 2, 4, 4);
		}
	}
}

void CGraphSampleCtrl::DataToPDF(PdfLib::PDF& pdf, const CRect& rcTable, std::vector<long>::iterator& it)
{
	pdf.setLineWidth(1);
	pdf.setLineColor(160, 160, 160);

	for (int col = 0; col < 4; ++col)
	{
		CRect rcCol(CPoint(rcTable.left + rcTable.Width() * col / 4, rcTable.top), CSize(rcTable.Width() / 4, rcTable.Height()));
		CRect rcCell(CPoint(rcCol.left + rcCol.Width() / 2 - 50, rcCol.bottom - 16), CSize(100, 16));

		pdf.drawLine(rcCol.left + rcCol.Width() / 2, rcCol.top, rcCol.left + rcCol.Width() / 2, rcCol.bottom);
		pdf.drawLine(rcCell.left, rcCell.top, rcCell.left + rcCell.Width(), rcCell.top);

		pdf.setFont(PdfLib::PDF::HELVETICA_BOLD, 11);
		std::string str("Min:Sec");
		int cx = pdf.stringWidth(str);
		pdf.showTextXY(str, rcCell.left + (rcCell.Width() / 2 - cx) / 2, rcCell.top + 5);
		str = "BPM";
		cx = pdf.stringWidth(str);
		pdf.showTextXY(str, rcCell.right - (rcCell.Width() / 2 + cx) / 2, rcCell.top + 5);

		int rows = (rcCol.Height() - rcCell.Height()) / rcCell.Height();

		pdf.setFont(PdfLib::PDF::HELVETICA, 10);

		for (int row = 1; row <= rows; ++row)
		{
			rcCell.OffsetRect(0, -rcCell.Height());

			if (it != m_data.end())
			{
				str = _FormatTime(m_nInterval * std::distance(m_data.begin(), it));
				cx = pdf.stringWidth(str);
				pdf.showTextXY(str, rcCell.left + (rcCell.Width() / 2 - cx) / 2, rcCell.top + 5);

				str = std::to_string(*it);
				cx = pdf.stringWidth(str);
				pdf.showTextXY(str, rcCell.right - (rcCell.Width() / 2 + cx) / 2, rcCell.top + 5);

				++it;
			}
			else
			{
				return;
			}
		}
	}
}

// CGraphSampleCtrl message handlers
