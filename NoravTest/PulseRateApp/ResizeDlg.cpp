#include "pch.h"
#include "framework.h"
#include "ResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg dialog

IMPLEMENT_DYNAMIC(CResizeDlg, CCmdUIDlg)

CResizeDlg::CResizeDlg(bool bResizeAllow /* = true */, bool bModifyStyle /*= true*/) : CCmdUIDlg(),
    m_nGripSize(13),
    m_rectGrip(0,0,0,0),
	m_bResizeAllow(bResizeAllow),
	m_bModifyStyle(bModifyStyle)
{
}

CResizeDlg::CResizeDlg(UINT idTemplate,
                       CWnd* pParent /* = NULL */,
                       bool bResizeAllow /* = true */,
                       bool bModifyStyle /*= true*/) :
      CCmdUIDlg(idTemplate, pParent),
      m_nGripSize(13),
      m_rectGrip(0,0,0,0),
	  m_bResizeAllow(bResizeAllow),
	  m_bModifyStyle(bModifyStyle)
{
}


BEGIN_MESSAGE_MAP(CResizeDlg, CCmdUIDlg)
	//{{AFX_MSG_MAP(CResizeDlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeDlg message handlers

void CResizeDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bResizeAllow && m_bModifyStyle)
	{
		if (!m_rectGrip.IsRectEmpty())
		{
			InvalidateRect(m_rectGrip, TRUE);
		}
		if (nType == SIZE_MAXIMIZED || nType == SIZE_MINIMIZED)
		{
			m_rectGrip.SetRectEmpty();
		}
		else if (nType == SIZE_RESTORED)
		{
			m_rectGrip.left   = cx - m_nGripSize;
			m_rectGrip.top    = cy - m_nGripSize;
			m_rectGrip.right  = cx;
			m_rectGrip.bottom = cy;
			InvalidateRect(m_rectGrip, TRUE);
		}
	}

    __super::OnSize(nType, cx, cy);

    if (m_bResizeAllow)
		RepositionControls(cx, cy);
}

BOOL CResizeDlg::OnEraseBkgnd(CDC* pDC) 
{
    BOOL bResult = __super::OnEraseBkgnd(pDC);
    if (m_bResizeAllow)
	{
		if (!m_rectGrip.IsRectEmpty())
			DrawSizeGrip(pDC);
	}
    return bResult;
}

void CResizeDlg::DrawSizeGrip(CDC* pDC)
{
    CPen penHighlight(PS_SOLID, 1, GetSysColor(COLOR_3DHIGHLIGHT));
    CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
    CPen* pPenOld = pDC->SelectObject(&penHighlight);
    int nLines = m_rectGrip.Width() / 4;
	int i = 0;
    for (; i < nLines; i++)
    {
        pDC->MoveTo(m_rectGrip.left + i * 4, m_rectGrip.bottom);
        pDC->LineTo(m_rectGrip.right, m_rectGrip.top + i * 4);
    }
    pDC->SelectObject(&penShadow);
    for (i = 0; i < nLines; i++)
    {
        pDC->MoveTo(m_rectGrip.left + i * 4 + 1, m_rectGrip.bottom);
        pDC->LineTo(m_rectGrip.right, m_rectGrip.top + i * 4 + 1);
        pDC->MoveTo(m_rectGrip.right, m_rectGrip.top + i * 4 + 2);
        pDC->LineTo(m_rectGrip.left + i * 4 + 2, m_rectGrip.bottom);
    }
    pDC->SelectObject(pPenOld);
}

LRESULT CResizeDlg::OnNcHitTest(CPoint point) 
{
	if (m_bResizeAllow)
	{
		if (!m_rectGrip.IsRectEmpty())
		{
			CRect rect(m_rectGrip);
			ClientToScreen(rect);
			if (rect.PtInRect(point))
				return HTBOTTOMRIGHT;
		}
	}
    return __super::OnNcHitTest(point);
}

int CResizeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;
	if (m_bResizeAllow && m_bModifyStyle)
		ModifyStyle(0, WS_THICKFRAME | WS_CLIPCHILDREN, SWP_DRAWFRAME);
    return 0;
}

BOOL CResizeDlg::OnInitDialog() 
{
    BOOL bResult = __super::OnInitDialog();

	if (m_bResizeAllow)
	{
		CRect rc;
		GetClientRect(rc);
		m_mapInitRects.SetAt(0, new CRect(rc));

		for (CWnd* pCtrl = GetWindow(GW_CHILD); pCtrl; pCtrl = pCtrl->GetWindow(GW_HWNDNEXT))
		{
			pCtrl->GetWindowRect(rc);
			ScreenToClient(rc);
			short idCtrl = pCtrl->GetDlgCtrlID();
			if (idCtrl != IDC_STATIC) 
				m_mapInitRects.SetAt(idCtrl, new CRect(rc));
		}
    }
    return bResult;
}

void CResizeDlg::OnDestroy() 
{
    __super::OnDestroy();

	if (m_bResizeAllow)
    {
		WORD wKey;
		CRect* pRect = NULL;
		POSITION pos = m_mapInitRects.GetStartPosition();
		while (pos != NULL)
		{
			m_mapInitRects.GetNextAssoc(pos, wKey, (void*&)pRect);
			m_mapInitRects[wKey] = NULL;
			if (pRect)
				delete pRect;
		}
		m_mapInitRects.RemoveAll();
	}
}

bool CResizeDlg::GetInitRect(int nID, LPRECT pRect)
{
    void* prc = NULL;
    if (!m_mapInitRects.Lookup(nID, prc))
        return false;
    ASSERT(pRect);
    CopyRect(pRect, (const RECT*)prc);
    return true;
}

bool CResizeDlg::GetInitRect(CWnd* pCtrl, LPRECT pRect)
{
    if (pCtrl == NULL)
        return GetInitRect(0, pRect);
    if (!IsChild(pCtrl))
        return false;
    return GetInitRect(pCtrl->GetDlgCtrlID(), pRect);
}

void CResizeDlg::RepositionControls(int cx, int cy)
{
    if (GetCtrlCount() <= 0)
        return;

    CRect rc;
    GetInitRect(0, rc);
    int cxInit = rc.Width();
    int cyInit = rc.Height();

    for (CWnd* pCtrl = GetWindow(GW_CHILD); pCtrl; pCtrl = pCtrl->GetWindow(GW_HWNDNEXT))
    {
        int nID = pCtrl->GetDlgCtrlID();
        CRect* pRectInit = NULL;
        if (m_mapInitRects.Lookup(nID, (void*&)pRectInit))
            if (QueryNewCtrlPos(nID, cxInit, cyInit, cx, cy, pRectInit, rc))
            {
                pCtrl->MoveWindow(rc);
                pCtrl->Invalidate(FALSE);
            }
    }
}
