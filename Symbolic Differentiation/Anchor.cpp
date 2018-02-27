// Anchor.cpp : implementation file

#include "stdafx.h"

#define A_LEFT		1
#define A_TOP		2
#define A_BOTTOM	4
#define A_RIGHT		8

void Anchor(CWnd *pWndParent, UINT ID, DWORD dwAnchor, CSize sizeOld, CSize sizeNew)
{
	CWnd *pWnd = pWndParent->GetDlgItem(ID);
	CRect rect;
	pWnd->GetWindowRect(rect);
	pWndParent->ScreenToClient(rect);

	int nCx = sizeNew.cx - sizeOld.cx;
	int nCy = sizeNew.cy - sizeOld.cy;
	if ((dwAnchor & A_LEFT) == 0)
		rect.left += nCx;
	if ((dwAnchor & A_RIGHT) != 0)
		rect.right += nCx;
	if ((dwAnchor & A_TOP) == 0)
		rect.top += nCy;
	if ((dwAnchor & A_BOTTOM) != 0)
		rect.bottom += nCy;
	pWnd->MoveWindow(rect);
}
