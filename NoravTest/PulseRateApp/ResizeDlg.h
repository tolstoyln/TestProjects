
#pragma once

#include "CmdUIDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg dialog

class CResizeDlg : public CCmdUIDlg
{
    DECLARE_DYNAMIC(CResizeDlg)

// Construction
public:
    CResizeDlg(bool bResizeAllow = true, bool bModifyStyle = true);
	CResizeDlg(UINT idTemplate, CWnd* pParent = NULL, bool bResizeAllow = true, bool bModifyStyle = true);

// Implementation
protected:
    int m_nGripSize;

	// Generated message map functions
	//{{AFX_MSG(CResizeDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    virtual bool QueryNewCtrlPos(int nCtrlID, int cxDlgInit, int cyDlgInit, int cxDlgNew, int cyDlgNew,
        const CRect* pCtrlRectInit, LPRECT pCtrlRectNew) {return false;}

    bool GetInitRect(int nID, LPRECT pRect);    // nID = 0 - the dialog itself
    bool GetInitRect(CWnd* pCtrl, LPRECT pRect);
    inline INT_PTR GetCtrlCount() {return m_mapInitRects.GetCount() - 1;}
    inline const CRect& GetGripRect() {return m_rectGrip;}

private:
    void DrawSizeGrip(CDC* pDC);
    CRect m_rectGrip;
	bool m_bResizeAllow;
	bool m_bModifyStyle;

    CMapWordToPtr m_mapInitRects;

    void RepositionControls(int cx, int cy);
};
