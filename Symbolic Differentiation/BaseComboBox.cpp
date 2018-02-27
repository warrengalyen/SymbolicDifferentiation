#include "stdafx.h"
#include "Resource.h"
#include "BaseComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBaseComboBox
CBaseComboBox::CBaseComboBox()
{
}

CBaseComboBox::~CBaseComboBox()
{
}

BEGIN_MESSAGE_MAP(CBaseComboBox, CComboBox)

END_MESSAGE_MAP()

// CBaseComboBox message handlers

void CBaseComboBox::AddItem(LPCSTR lpcs)
{
	int nItem = FindStringExact(-1, lpcs);
	if (nItem != -1)
		DeleteString(nItem);
	InsertString(0, lpcs);
	SetWindowText(lpcs);
}

void CBaseComboBox::InitStatus(LPCSTR lpcsSection)
{
	DWORD dwStyle = GetStyle();
	if ((dwStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
	{
		int nCurSel = AfxGetApp()->GetProfileInt(lpcsSection, "Current selection", 0);
		SetCurSel(nCurSel);
	}
	else
	{
		int nRegCount = AfxGetApp()->GetProfileInt(lpcsSection, "Count", 0);
		CString sValue, sCount;
		for (int nCount = 0; nCount < nRegCount; nCount++)
		{
			sCount.Format("Value%d", nCount + 1);
			sValue = AfxGetApp()->GetProfileString(lpcsSection, sCount);
			AddString(sValue);
		}
		SetCurSel(0);
	}
}

void CBaseComboBox::SaveStatus(LPCSTR lpcsSection, int nSaveCount)
{
	DWORD dwStyle = GetStyle();
	if ((dwStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
		AfxGetApp()->WriteProfileInt(lpcsSection, "Current selection", GetCurSel());
	else
	{
		int nComboCount = GetCount();
		if (nSaveCount == -1)
			nSaveCount = nComboCount;
		else
			nSaveCount = min(nSaveCount, nComboCount);
		CString sValue, sCount;
		int nIndex = 1;
		GetWindowText(sValue);
		if (sValue == "")
		{
			AfxGetApp()->WriteProfileString(lpcsSection, "Value1", "");
			nIndex++;
		}
		int nCount = 0;
		for (; nCount < nSaveCount; nCount++)
		{
			GetLBText(nCount, sValue);
			sCount.Format("Value%d", nIndex++);
			AfxGetApp()->WriteProfileString(lpcsSection, sCount, sValue);
		}
		AfxGetApp()->WriteProfileInt(lpcsSection, "Count", nCount);
	}
}


