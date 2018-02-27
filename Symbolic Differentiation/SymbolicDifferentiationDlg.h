// SymbolicDifferentiationDlg.h : header file
#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "BaseComboBox.h"
#include "Curve.h"
#include "resource.h"

class CSymbolicDifferentiationDlg : public CDialog
{
public:
	CSymbolicDifferentiationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog data
	enum { IDD = IDD_SYMBOLICDIFFERENTIATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
	void ShowTabs();

	HICON m_hIcon;
	CSize m_size;
	CCurve m_curve;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabOut;
	CString m_sInput;
	CString m_sDifferentiation;
	CString m_sDifferentiationStack;
	CString m_sCalculation;
	CString m_sCalculationStack;
	CString m_sHelp;
	CBaseComboBox m_comboInput;
	afx_msg void OnButtonClickedOk();
	afx_msg void OnTcnSelChangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonClickedDraw();
};

