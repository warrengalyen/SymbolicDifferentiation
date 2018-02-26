#pragma once

#include "Resource.h"

class COptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(COptionsDialog)

public:
	COptionsDialog(CWnd* pParent = NULL);	// standard constructor
	virtual ~COptionsDialog();

// Dialog data
	enum { IDD = IDD_DIALOG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	double m_dMinX;
	double m_dMaxX;
	double m_dMaxY;
	BOOL m_bDrawLines;
	afx_msg void OnButtonClickedOk();
};

