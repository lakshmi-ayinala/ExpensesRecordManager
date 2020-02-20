
// ExpensesRecordManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpensesRecordManager.h"
#include "ExpensesRecordManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExpensesRecordManagerDlg dialog



CExpensesRecordManagerDlg::CExpensesRecordManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXPENSESRECORDMANAGER_DIALOG, pParent)
	, m_MainItem(_T(""))
	, m_SubItem(_T(""))
	, m_Quantity(1)
	, m_Rate(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExpensesRecordManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPENSES_LIST, m_ExpensesList);
	DDX_Text(pDX, IDC_MAIN_ITEM, m_MainItem);
	DDV_MaxChars(pDX, m_MainItem, 256);
	DDX_Text(pDX, IDC_SUB_ITEM, m_SubItem);
	DDV_MaxChars(pDX, m_SubItem, 256);
	DDX_Text(pDX, IDC_QUANTITY, m_Quantity);
	DDV_MinMaxUInt(pDX, m_Quantity, 1, UINT_MAX);
	DDX_Text(pDX, IDC_RATE, m_Rate);
	DDV_MinMaxUInt(pDX, m_Rate, 1, UINT_MAX);
}

BEGIN_MESSAGE_MAP(CExpensesRecordManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_STORE, &CExpensesRecordManagerDlg::OnBnClickedBtnStore)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CExpensesRecordManagerDlg::OnBnClickedBtnLoad)
END_MESSAGE_MAP()


// CExpensesRecordManagerDlg message handlers

BOOL CExpensesRecordManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitExpensesListControl();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExpensesRecordManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExpensesRecordManagerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExpensesRecordManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExpensesRecordManagerDlg::InitExpensesListControl()
{
	m_ExpensesList.InsertColumn(
		0,              // Rank/order of item 
		L"SNo",          // Caption for this header 
		LVCFMT_LEFT,    // Relative position of items under header 
		100);           // Width of items under header

	m_ExpensesList.InsertColumn(1, L"Date", LVCFMT_CENTER, 80);
	m_ExpensesList.InsertColumn(2, L"Financial Year", LVCFMT_LEFT, 80);
	m_ExpensesList.InsertColumn(3, L"Quater", LVCFMT_LEFT, 80);
	m_ExpensesList.InsertColumn(4, L"Main Item", LVCFMT_LEFT, 100);
	m_ExpensesList.InsertColumn(5, L"Sub-Item", LVCFMT_LEFT, 80);
	m_ExpensesList.InsertColumn(6, L"Rate", LVCFMT_LEFT, 80);
	m_ExpensesList.InsertColumn(7, L"Quantity", LVCFMT_LEFT, 80);
	m_ExpensesList.InsertColumn(8, L"Amount", LVCFMT_LEFT, 80);

	int nItem;

	nItem = m_ExpensesList.InsertItem(0, L"1");
	m_ExpensesList.SetItemText(nItem, 1, L"Mark");
	m_ExpensesList.SetItemText(nItem, 2, L"45");
	m_ExpensesList.SetItemText(nItem, 3, L"Address 1");
}

void CExpensesRecordManagerDlg::Serialize(CArchive & ar)
{
}

void CExpensesRecordManagerDlg::InitControls()
{
}


void CExpensesRecordManagerDlg::OnBnClickedBtnStore()
{
	
}


void CExpensesRecordManagerDlg::OnBnClickedBtnLoad()
{
	
}
