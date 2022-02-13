#pragma once

#include <unordered_map>

namespace GdiUtil
{
	inline void SetFontWeight(CWnd* pWnd, long lfWeight = FW_BOLD)
	{
		ASSERT(pWnd);
		CFont* pFont = pWnd->GetFont();
		LOGFONT lf = { 0 };
		pFont->GetLogFont(&lf);
		lf.lfWeight = lfWeight;
		CFont font;
		font.CreateFontIndirect(&lf);
		pWnd->SetFont(&font);
		font.Detach();
	}

	inline void FitFont(CWnd* pWnd)
	{
		ASSERT(pWnd);
		CFont* pFont = pWnd->GetFont();
		LOGFONT lf = { 0 };
		pFont->GetLogFont(&lf);
		CRect rc;
		pWnd->GetClientRect(&rc);
		rc.DeflateRect(2, 4);
		lf.lfHeight = -rc.Height();
		CFont font;
		font.CreateFontIndirect(&lf);
		pWnd->SetFont(&font);
		font.Detach();
	}

	inline HBRUSH GetBrush(COLORREF clr)
	{
		static std::unordered_map<COLORREF, HBRUSH> brushes;
		if (brushes.find(clr) == brushes.end())
		{
			brushes[clr] = CreateSolidBrush(clr);
		}
		return brushes[clr];
	}
}
