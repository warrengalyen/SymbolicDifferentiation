// Symbolic Differentiation.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Symbolic Differentiation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSymbolicDifferentiationApp

BEGIN_MESSAGE_MAP(CSymbolicDifferentiationApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// CSymbolicDifferentiationApp construction

CSymbolicDifferentiationApp::CSymbolicDifferentiationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSymbolicDifferentiationApp object

CSymbolicDifferentiationApp theApp;


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL CSymbolicDifferentiationApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("MechanikaDesign"));

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

