#pragma once

#include "Calculate.h"

#define COLOR_AXES			RGB(0,0,0)
#define COLOR_AXES_TEXT		RGB(224,224,224)
#define COLOR_CALCULATE		RGB(0,192,0)
#define COLOR_DIFFERENTIATE	RGB(255,0,0)

#define ABS(a) ((a)<0 ? (-a) : (a))

class CCurve : public CWnd
{
	DECLARE_DYNAMIC(CCurve)

public:
	CCurve();
	virtual ~CCurve();

	std::vector<CString> m_vCurves;
	std::vector<double> m_vCalc;
	bool m_bCalcLimits;
	double m_dMaxY;
	double m_dMinX, m_dMaxX;
	CRect m_rectSelection;
	BOOL m_bDrawLines;
	CString m_sAccuracyX;
	CString m_sAccuracyY;

	void Clear();
	void InitLimits();
	void GetLimits(CRect& rect);
	void Draw(LPCSTR lpcsCurve);
	void DrawAxes(CPaintDC& dc, CRect& rect);
	void DrawCurves(CPaintDC& dc, CRect& rect);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg	void OnMenuClick(UINT nID);
	afx_msg void OnOptions();
};

