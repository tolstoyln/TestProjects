#pragma once

class CSampleAutomationObject : public CCmdTarget
{
public:
	CSampleAutomationObject();
	~CSampleAutomationObject();

	virtual void OnFinalRelease();

	DECLARE_DYNCREATE(CSampleAutomationObject)
	DECLARE_OLECREATE(CSampleAutomationObject)
	DECLARE_INTERFACE_MAP()
	DECLARE_DISPATCH_MAP()

protected:
	afx_msg BOOL ShowMessage(LPCTSTR message);
};

