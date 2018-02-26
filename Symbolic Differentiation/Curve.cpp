// Curve.cpp : implementation file

#include "stdafx.h"
#include "Symbolic Differentiation.h"
#include "Curve.h"
#include "OptionsDialog.h"

IMPLEMENT_DYNAMIC(CCurve, CWnd)
CCurve::CCurve()
{
	if (AfxGetApp()->GetProfileInt("Options\\Graph", "FirstTime", 1) == TRUE)
	{
		AfxGetApp()->WriteProfileString("Options\\Graph", "MinX", "-20");
		AfxGetApp()->WriteProfileString("Options\\Graph", "MaxX", "20");
		AfxGetApp()->WriteProfileInt("Options\\Graph", "DrawLines", 1);
		AfxGetApp()->WriteProfileInt("Options\\Graph", "FirstTime", 0);
	}
	m_bCalcLimits = false;
	m_bDrawLines = AfxGetApp()->GetProfileInt("Options\\Graph", "DrawLines", 1);
}

CCurve::~CCurve()
{
}

BEGIN_MESSAGE_MAP(CCurve, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(ID__SIN, ID__SIN + 100, OnMenuClick)
	ON_COMMAND(ID__OPTIONS, OnOptions)
END_MESSAGE_MAP()

// CCurve message handlers

double YScreenToMath(int y, CRect& rect, double dMaxY)
{
	return (double)dMaxY * (rect.bottom - 2 * y) / rect.bottom;
}

int YMathToScreen(double y, CRect& rect, double dMaxY)
{
	return (int)(rect.bottom * (1 - y / dMaxY) / 2);
}

double XScreenToMath(int x, CRect& rect, double dMinX, double dMaxX)
{
	return (double)x * (dMaxX - dMinX) / rect.right + dMinX;
}

int XMathToScreen(double x, CRect& rect, double dMinX, double dMaxX)
{
	return (int)((double)rect.right * (x - dMinX) / (dMaxX - dMinX));
}

void GetAccuracy(double x, CString& str)
{
	if ((x = ABS(x) - (int)ABS(x)) == 0)
	{
		str = "%.0f";
		return;
	}
	for (int n = 1, i = 10; n < 32; n++, i *= 10)
	{
		if (x * i > 1)
		{
			str.Format("%%.%df", n);
			return;
		}
		str = "%.32f";
	}
}

void CCurve::DrawAxes(CPaintDC& dc, CRect& rect)
{
	GetAccuracy((m_dMaxX - m_dMinX) / 10, m_sAccuracyX);
	GetAccuracy(2 * m_dMaxY / 10, m_sAccuracyY);

	dc.SetTextColor(COLOR_AXES);
	dc.SetBkMode(TRANSPARENT);

	CPen penAxesText(PS_SOLID, 1, COLOR_AXES_TEXT);
	CPen* pOldPen = dc.SelectObject(&penAxesText);
	CString sPoint;
	// draw vertical lines
	dc.SetTextAlign(TA_CENTER);

	double step = (m_dMaxX - m_dMinX) / 10, x;
	for (int n = 0; n < 10; n++)
	{
		x = m_dMinX + n * step;
		CPoint point(XMathToScreen(x, rect, m_dMinX, m_dMaxX), 0);
		dc.MoveTo(point);
		point.Offset(0, rect.bottom);
		dc.LineTo(point);
		sPoint.Format(m_sAccuracyX, x);
		dc.TextOut(point.x, rect.bottom / 2, sPoint);
	}

	int nXZero = XMathToScreen(0, rect, m_dMinX, m_dMaxX);
	// draw horizontal lines
	dc.SetTextAlign(TA_BASELINE);
	for (int n = -4; n <= 4; n++)
	{
		CPoint point(0, rect.bottom / 2 + rect.bottom * n / 10);
		double y = YScreenToMath(point.y, rect, m_dMaxY);
		if (n != 0)
		{
			dc.MoveTo(point);
			point.Offset(rect.right, 0);
			dc.LineTo(point);
			sPoint.Format(m_sAccuracyY, y);
			dc.TextOut(nXZero + 2, point.y, sPoint);
		}
	}

	CPen penAxes(PS_SOLID, 1, COLOR_AXES);
	dc.SelectObject(&penAxes);
	// draw vertical axis
	dc.MoveTo(nXZero, 0);
	dc.LineTo(nXZero, rect.bottom);
	// draw horizontal axis
	dc.MoveTo(0, rect.bottom / 2);
	dc.LineTo(rect.right, rect.bottom / 2);

	dc.SelectObject(pOldPen);
}

void CCurve::GetLimits(CRect& rect)
{
	double dMinX = m_dMinX, dMaxX = m_dMaxX, dMaxY = 0;
	bool bFirstX = false, bModifyX = false;
	double x, y;
	CString sPoint, sCurve;

	for (int nCurve = 0; nCurve < (int)m_vCurves.size(); nCurve++)
	{
		for (int n = 0; n < rect.right; n++)
		{
			sCurve = m_vCurves[nCurve];
			x = XScreenToMath(n, rect, m_dMinX, m_dMaxX);
			sPoint.Format("%f", x);
			sCurve.Replace("x", sPoint);
			if (Calculate(sCurve, y) < 0 || ABS(y) > m_dMaxY)
			{
				bModifyX = true;
				continue;
			}
			if (ABS(y) > dMaxY)
				dMaxY = ABS(y) * 4 / 3;
			if (bFirstX == false)
				dMinX = x, bFirstX = true;
			else
				dMaxX = x;
		}
	}
	if (bModifyX)
	{
		m_dMinX = dMinX - (dMaxX - dMinX) / 10;
		m_dMaxX = dMaxX - (dMaxX - dMinX) / 10;
	}
	m_dMaxY = dMaxY;
	m_bCalcLimits = true;
}

void CCurve::DrawCurves(CPaintDC& dc, CRect& rect)
{
	dc.SetTextAlign(TA_BOTTOM | TA_LEFT);
	dc.SetTextColor(COLOR_CALCULATE);
	if (m_vCurves.size() == 2)
	{
		CString str = "d(" + m_vCurves[0] + ")/dx = ";
		dc.TextOut(2, rect.bottom - 1, str);
		CSize size = dc.GetTextExtent(str);
		dc.SetTextColor(COLOR_DIFFERENTIATE);
		dc.TextOut(2 + size.cx, rect.bottom - 1, m_vCurves[1]);
	}
	else if (m_vCurves.size() == 1)
		dc.TextOut(2, rect.bottom - 1, CString(m_vCurves[0]));

	CRect rcPaint(dc.m_ps.rcPaint);
	double y;
	CString sPoint, sCurve;
	CPoint point;
	for (int nCurve = 0; nCurve < (int)m_vCurves.size(); nCurve++)
	{
		COLORREF color = nCurve ? COLOR_DIFFERENTIATE : COLOR_CALCULATE;
		CPen penCurve(PS_SOLID, 1, color);
		CPen* pOldPen = dc.SelectObject(&penCurve);
		bool bFirstPoint = true;
		for (int x = 0; x < rect.right; x++)
		{
			point.x = x;
			if (point.x < rcPaint.left || point.x > rcPaint.right)
			{
				bFirstPoint = true;
				continue;
			}

			sCurve = m_vCurves[nCurve];
			sPoint.Format("%.32f", XScreenToMath(x, rect, m_dMinX, m_dMaxX));
			sCurve.Replace("x", sPoint);
			if (Calculate(sCurve, y) < 0)
			{
				bFirstPoint = true;
				continue;
			}
			point.y = YMathToScreen(y, rect, m_dMaxY);
			if (point.y < 0 || point.y > rect.bottom)
			{
				bFirstPoint = true;
				continue;
			}
			if (point.y < rcPaint.top || point.y > rcPaint.bottom)
				break;
			if (m_bDrawLines)
			{
				if (bFirstPoint)
					dc.MoveTo(point), bFirstPoint = false;
				else
					dc.LineTo(point);
			}
			else
				dc.SetPixel(point, color);
		}
		dc.SelectObject(pOldPen);
	}
}

void CCurve::OnPaint()
{
	CRect rect;
	GetClientRect(&rect);
	if (m_bCalcLimits == false && m_vCurves.size() > 0)
		GetLimits(rect);

	CPaintDC dc(this);  // device context for painting

	CFont* pOldFont = (CFont*)dc.SelectObject(GetParent()->GetFont());
	DrawAxes(dc, rect);
	DrawCurves(dc, rect);
	dc.SelectObject(pOldFont);

	// Do not call CWnd::OnPaint() for painting messages
}

void CCurve::Draw(LPCSTR lpcsCurve)
{
	m_vCurves.push_back(lpcsCurve);
	Invalidate();
}

void CCurve::InitLimits()
{
	CString str = AfxGetApp()->GetProfileString("Options\\Graph", "MinX", "-20");
	m_dMinX = ATOF(str);
	str = AfxGetApp()->GetProfileString("Options\\Graph", "MaxX", "+20");
	m_dMaxX = ATOF(str);
	str = AfxGetApp()->GetProfileString("Options\\Graph", "MaxY", "32767");
	m_dMaxY = ATOF(str);

	m_bCalcLimits = false;
}

void CCurve::Clear()
{
	InitLimits();
	m_vCurves.clear();
	m_vCalc.clear();
	Invalidate();
}

void CCurve::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

int CCurve::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitLimits();

	return 0;
}

void CCurve::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	dc.SetTextColor(COLOR_AXES);
	dc.SetBkColor(TRANSPARENT);
	CRect rect;
	GetClientRect(&rect);
	double x = XScreenToMath(point.x, rect, m_dMinX, m_dMaxX);
	double y = YScreenToMath(point.y, rect, m_dMaxY);

	CString str;
	str.Format(m_sAccuracyX + ", " + m_sAccuracyY, x, y);

	CFont* pOldFont = dc.SelectObject(GetParent()->GetFont());
	CRect rectText = rect;
	dc.DrawText(str, rectText, DT_CALCRECT);
	rectText.right = rectText.right * 3 / 2;
	InvalidateRect(rectText);
	UpdateWindow();
	dc.DrawText(str, rectText, DT_LEFT);
	dc.SelectObject(pOldFont);

	if (GetCapture() != this)
		return;
	dc.SetROP2(R2_XORPEN);
	dc.Rectangle(m_rectSelection);

	CRect rectSelection(m_rectSelection.TopLeft(), point);
	rectSelection.top = 0;
	rectSelection.bottom = rect.bottom;
	dc.Rectangle(rectSelection);
	m_rectSelection = rectSelection;

	CWnd::OnMouseMove(nFlags, point);
}

void CCurve::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CRect rect;
	GetClientRect(&rect);
	m_rectSelection.SetRect(point.x, 0, point.x, rect.bottom);

	CWnd::OnLButtonDown(nFlags, point);
}

void CCurve::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCapture() != this)
		return;
	ReleaseCapture();

	CClientDC dc(this);
	dc.SetROP2(R2_XORPEN);
	dc.Rectangle(m_rectSelection);

	CRect rect;
	GetClientRect(&rect);
	double dMinX = XScreenToMath(m_rectSelection.left, rect, m_dMinX, m_dMaxX);
	double dMaxX = XScreenToMath(m_rectSelection.right, rect, m_dMinX, m_dMaxX);
	if (dMinX == dMaxX)
		return;

	m_dMinX = min(dMinX, dMaxX);
	m_dMaxX = max(dMinX, dMaxX);
	m_bCalcLimits = false;
	Invalidate();
	UpdateWindow();

	CWnd::OnLButtonUp(nFlags, point);
}

void CCurve::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu, *pMenu;
	if (menu.LoadMenu(IDR_MENU1) == FALSE)
		return;
	pMenu = menu.GetSubMenu(0);

	CMenu* pMenuFunction = pMenu->GetSubMenu(0);
	pMenuFunction->DeleteMenu(ID__SIN, MF_BYCOMMAND);

	int nIndex = 0;
	int nFunction = 1;
	while (pFunctions[nFunction])
	{
		pMenuFunction->InsertMenu(nIndex++, MF_BYPOSITION, ID__SIN + nFunction - 1, pFunctions[nFunction] + CString("x)"));
		nFunction++;
	}

	MSG *pMsg = (MSG *)CWnd::GetCurrentMessage();
	pMenu->TrackPopupMenu(
		TPM_RIGHTALIGN,
		pMsg->pt.x,
		pMsg->pt.y,
		this);

	CWnd::OnRButtonDown(nFlags, point);
}

void CCurve::OnMenuClick(UINT nID)
{
	Clear();
	InitLimits();

	Draw(pFunctions[nID - ID__SIN + 1] + CString("x)"));
}

void CCurve::OnOptions()
{
	COptionsDialog dlg;
	dlg.m_dMinX = m_dMinX;
	dlg.m_dMaxX = m_dMaxX;
	dlg.m_dMaxY = m_dMaxY;
	dlg.m_bDrawLines = m_bDrawLines;
	if (dlg.DoModal() == IDOK)
	{
		m_dMinX = dlg.m_dMinX;
		m_dMaxX = dlg.m_dMaxX;
		m_dMaxY = dlg.m_dMaxY;
		m_bDrawLines = dlg.m_bDrawLines;
		Invalidate();
	}

}
