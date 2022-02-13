
#pragma once

class CCmdUIDlg : public CDialog
{
DECLARE_DYNAMIC(CCmdUIDlg)
public:
    CCmdUIDlg() {}
    CCmdUIDlg(UINT idTemplate, CWnd* pParent = NULL) : CDialog(idTemplate, pParent) {}
DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKickIdle();
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
protected:
	virtual void OnUpdateCmdUINow() {}
};
