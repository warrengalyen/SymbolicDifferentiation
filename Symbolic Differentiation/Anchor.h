// Anchor.h : implementation file

#ifndef __ANCHOR_H__
#define __ANCHOR_H__

#define A_LEFT		1
#define A_TOP		2
#define A_BOTTOM	4
#define A_RIGHT		8

void Anchor(CWnd *pWndParent, UINT ID, DWORD dwAnchor, CSize sizeOld, CSize sizeNew);

#endif // __ANCHOR_H__