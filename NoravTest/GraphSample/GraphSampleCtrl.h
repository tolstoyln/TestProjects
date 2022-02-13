#pragma once

#include <vector>
#include "../Shared/inc/GlobalDefs.h"
#include "../PdfLib/pdf.h"

// GraphSampleCtrl.h : Declaration of the CGraphSampleCtrl ActiveX Control class.


// CGraphSampleCtrl : See GraphSampleCtrl.cpp for implementation.

class CGraphSampleCtrl : public COleControl
{
	DECLARE_DYNCREATE(CGraphSampleCtrl)

// Constructor
public:
	CGraphSampleCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CGraphSampleCtrl();

	DECLARE_OLECREATE_EX(CGraphSampleCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CGraphSampleCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CGraphSampleCtrl)		// Type name and misc status

	afx_msg long GetMode() const { return m_nMode; }
	afx_msg void SetMode(long val);
	afx_msg long GetInterval() const { return m_nInterval; }
	afx_msg void SetInterval(long val) { m_nInterval = val; }
	afx_msg void Clear();
	afx_msg void AddValue(long val);
	afx_msg BSTR WritePDF(LPCTSTR cszPatient, LPCTSTR cszPdfFile);

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

protected:
	std::vector<long> m_data;
	long m_nMode = PRT_VIEW_MODE_0;
	long m_nInterval = PRT_PROBE_INTERVAL_0;
	CRect m_rcPlot;
	int TimeSpans() { return m_nMode == PRT_VIEW_MODE_0 || m_nMode == PRT_VIEW_MODE_3 ? 12 : 10; }
	int OriginTime() { return max(0, (static_cast<int>(m_data.size()) - 1) * m_nInterval - m_nMode); }
	void DrawAxes(CDC* pdc, const CRect& rcBounds);
	void DrawData(CDC* pdc);
	void PlotToPDF(PdfLib::PDF& pdf, const CRect& rcPlot);
	void DataToPDF(PdfLib::PDF& pdf, const CRect& rcTable, std::vector<long>::iterator& it);

// Dispatch and event IDs
public:
	enum
	{
		dispidMode = 1,
		dispidInterval = 2,
		dispidClear = 10,
		dispidAddValue = 20,
		dispidWritePDF = 30
	};
};

