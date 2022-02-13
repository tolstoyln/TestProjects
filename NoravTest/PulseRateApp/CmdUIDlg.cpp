#include "pch.h"
#include "framework.h"
#include "CmdUIDlg.h"

class CCmdUITarget
{
public:
    CCmdUITarget(CCmdTarget* pTarget) : m_pTarget(pTarget) {}

    void InitMenu(CMenu* pMenu)
    {
		if (pMenu == NULL || m_pTarget == NULL)
		{
			return;
		}

		ASSERT_VALID(pMenu);
		ASSERT_VALID(m_pTarget);

		CCmdUI state;
		state.m_pMenu = pMenu;
		state.m_nIndexMax = pMenu->GetMenuItemCount();

		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; ++state.m_nIndex)
		{
			state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
			state.m_pSubMenu = NULL;

			if (state.m_nID == ID_SEPARATOR)
			{
				continue;
			}

			static const UINT ID_SUBMENU = 0xFFFFFFFF;

			if (state.m_nID == ID_SUBMENU)
			{
				state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);

				if (state.m_pSubMenu == NULL ||
					(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == ID_SEPARATOR ||
					state.m_nID == ID_SUBMENU)
				{
					continue;
				}

				state.DoUpdate(m_pTarget, false);
			}
			else
			{
				state.DoUpdate(m_pTarget, false);
			}
		}
    }

private:
    CCmdTarget* m_pTarget;
};

IMPLEMENT_DYNAMIC(CCmdUIDlg, CDialog)

BEGIN_MESSAGE_MAP(CCmdUIDlg, CDialog)
    ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
    ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

void CCmdUIDlg::OnKickIdle()
{
	OnUpdateCmdUINow();
    UpdateDialogControls(this, false);
}

void CCmdUIDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
    if (pMenu == NULL || bSysMenu)
    {
        return;
    }
   
    CCmdUITarget(this).InitMenu(pMenu);
}
