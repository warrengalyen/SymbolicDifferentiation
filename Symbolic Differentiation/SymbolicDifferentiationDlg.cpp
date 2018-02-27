// SymbolicDifferentiationDlg.cpp : implementation file

#include "stdafx.h"
#include "SymbolicDifferentiation.h"
#include "SymbolicDifferentiationDlg.h"
#include "Differentiate.h"
#include "Calculate.h"
#include "Anchor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog data
	enum { IDD = IDD_ABOUTBOX };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);		// DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CSymbolicDifferentiationDlg dialog

CSymbolicDifferentiationDlg::CSymbolicDifferentiationDlg(CWnd* pParent)
	: CDialog(CSymbolicDifferentiationDlg::IDD, pParent)
	, m_sDifferentiation(_T(""))
	, m_sDifferentiationStack(_T(""))
	, m_sCalculation(_T(""))
	, m_sCalculationStack(_T(""))
	, m_sHelp(_T(""))
{
	m_sHelp = "Supported constants:\r\n"
		"e = 2.7182818284590452353602874713527\r\n"
		"pi = 3.1415926535897932384626433832795\r\n";
	m_sHelp += "\r\nSupported functions:\r\n";
	for (int n = 1; pFunctions[n]; n++)
		m_sHelp += pFunctions[n] + CString("x)\r\n");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_size = CSize(0, 0);
}

void CSymbolicDifferentiationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabOut);
	DDX_Text(pDX, IDC_COMBO1, m_sInput);
	DDX_Text(pDX, IDC_EDIT2, m_sDifferentiation);
	DDX_Text(pDX, IDC_EDIT3, m_sDifferentiationStack);
	DDX_Text(pDX, IDC_EDIT4, m_sCalculation);
	DDX_Text(pDX, IDC_EDIT5, m_sCalculationStack);
	DDX_Text(pDX, IDC_EDIT6, m_sHelp);
	DDX_Control(pDX, IDC_COMBO1, m_comboInput);
}

BEGIN_MESSAGE_MAP(CSymbolicDifferentiationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnButtonClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelChangeTab1)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DRAW, OnButtonClickedDraw)
END_MESSAGE_MAP()

// CSymbolicDifferentiationDlg message handlers

BOOL CSymbolicDifferentiationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About...' menu item to system menu

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString sAboutMenu;
		sAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!sAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, sAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rect;
	GetDlgItem(IDC_EDIT2)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CString str = AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_CROSS),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH));
	m_curve.CreateEx(WS_EX_CLIENTEDGE, str, NULL, WS_CHILD | WS_TABSTOP, rect, this, IDC_CURVE);

	m_comboInput.InitStatus("Options\\Input");
	if (m_comboInput.GetCount() == 0)
	{
		m_comboInput.AddItem("sin(10*x)");
		m_comboInput.AddItem("sign(cos(x))");
		m_comboInput.AddItem("log(x)");
		m_comboInput.AddItem("x^sin(x)");
		m_comboInput.AddItem("sin(x)/x");
	}

	m_tabOut.InsertItem(0, "Differentiation");
	m_tabOut.InsertItem(1, "Differentiation Stack");
	m_tabOut.InsertItem(2, "Calculation");
	m_tabOut.InsertItem(3, "Calculation Stack");
	m_tabOut.InsertItem(4, "Draw");
	m_tabOut.InsertItem(5, "Help");
	m_tabOut.SetCurSel(AfxGetApp()->GetProfileInt("Options", "Tab", 0));

	ShowTabs();

	return TRUE;   // return TRUE  unless you set the focus to a control
} 

void CSymbolicDifferentiationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSymbolicDifferentiationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSymbolicDifferentiationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSymbolicDifferentiationDlg::OnButtonClickedOk()
{
	UpdateData(TRUE);
	m_comboInput.AddItem(m_sInput);

	m_sCalculationStack = "";
	m_sCalculation = Calculate(m_sInput, m_sCalculationStack, true);

	m_sDifferentiationStack = "";
	m_sDifferentiation = Differentiate(m_sCalculation, m_sDifferentiationStack, true);

	m_curve.Clear();
	m_curve.Draw(m_sInput);
	m_curve.Draw(m_sDifferentiation);

	UpdateData(FALSE);
}

void CSymbolicDifferentiationDlg::OnButtonClickedDraw()
{
	UpdateData(TRUE);
	m_comboInput.AddItem(m_sInput);

	m_sCalculationStack = "";
	m_sCalculation = Calculate(m_sInput, m_sCalculationStack, true);

	m_curve.Clear();
	m_curve.Draw(m_sInput);

	m_tabOut.SetCurSel(4);
	ShowTabs();

	UpdateData(FALSE);
}

void CSymbolicDifferentiationDlg::ShowTabs()
{
	int nCurSel = m_tabOut.GetCurSel();
	for (int n = 0; n < 6; n++)
		if (nCurSel == n)
			GetDlgItem(IDC_EDIT2 + n)->ShowWindow(SW_SHOW);
		else
			GetDlgItem(IDC_EDIT2 + n)->ShowWindow(SW_HIDE);
}

void CSymbolicDifferentiationDlg::OnTcnSelChangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowTabs();

	*pResult = 0;
}

void CSymbolicDifferentiationDlg::OnDestroy()
{
	UpdateData(TRUE);

	m_comboInput.SaveStatus("Options\\Input", 100);
	AfxGetApp()->WriteProfileInt("Options", "Tab", m_tabOut.GetCurSel());

	CDialog::OnDestroy();
}

void CSymbolicDifferentiationDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_size.cx > 0 && nType != SIZE_MINIMIZED)
	{
		Anchor(this, IDC_COMBO1, A_LEFT | A_RIGHT | A_TOP, m_size, CSize(cx, cy));
		Anchor(this, IDC_TAB1, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_EDIT2, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_EDIT3, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_EDIT4, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_EDIT5, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_EDIT6, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_CURVE, A_LEFT | A_RIGHT | A_TOP | A_BOTTOM, m_size, CSize(cx, cy));
		Anchor(this, IDC_DRAW, A_RIGHT | A_TOP, m_size, CSize(cx, cy));
		Anchor(this, IDOK, A_RIGHT | A_TOP, m_size, CSize(cx, cy));

		GetDlgItem(IDC_CURVE)->BringWindowToTop();
	}
	m_size = CSize(cx, cy);
}