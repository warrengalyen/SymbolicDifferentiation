// SymbolicDifferentiation.h : main header file for application

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"	// main symbols

class CSymbolicDifferentiationApp : public CWinApp
{
public:
	CSymbolicDifferentiationApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSymbolicDifferentiationApp theApp;