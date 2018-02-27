#pragma once

class CBaseComboBox : public CComboBox
{
public:
	CBaseComboBox();	
	virtual ~CBaseComboBox();

	void AddItem(LPCSTR lpcs);
	void InitStatus(LPCSTR lpcsSection);
	void SaveStatus(LPCSTR lpcsSection, int nSaveCount);

protected:
	DECLARE_MESSAGE_MAP()
};

