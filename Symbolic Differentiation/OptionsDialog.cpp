// OptionsDialog.cpp : implementation file

#include "stdafx.h"
#include "Symbolic Differentiation.h"
#include "OptionsDialog.h"

IMPLEMENT_DYNAMIC(COptionsDialog, CDialog)
COptionsDialog::COptionsDialog(CWnd* pParent)
	: CDialog(COptionsDialog::IDD, pParent)
{
	m_dMinX = atof(AfxGetApp()->GetProfileString("Options\\Graph", "MinX", "-20"));
	m_dMaxX = atof(AfxGetApp()->GetProfileString("Options\\Graph", "MaxX", "+20"));
	m_dMaxY = atof(AfxGetApp()->GetProfileString("Options\\Graph", "MaxY", "32767"));
	m_bDrawLines = AfxGetApp()->GetProfileInt("Options\\Graph", "DrawLines", 1);
}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dMinX);
	DDX_Text(pDX, IDC_EDIT2, m_dMaxX);
	DDX_Text(pDX, IDC_CHECK1, m_bDrawLines);
	DDX_Text(pDX, IDC_EDIT3, m_dMaxY);
}

BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnButtonClickedOk)
END_MESSAGE_MAP()

// COptionsDialog message handlers

void COptionsDialog::OnButtonClickedOk()
{
	UpdateData(true);

	OnOK();
}