
// ExpensesRecordManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpensesRecordManager.h"
#include "ExpensesRecordManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FY_START 2000
#define DB_DIRECTORY L"Database_v101"
#define FILE_NAME_FORMAT  L"Database\\ERM_%d-%d_Q%d.edb"

const std::string months[12] = { "Jan", "Feb", "Mar",
                                 "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep",
                                 "Oct", "Nov", "Dec"
                               };

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
    , m_VendorName(_T(""))
{
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_BILL);
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
    DDX_Control(pDX, IDC_FY_SEL, m_FySelCtrl);
    DDX_Control(pDX, IDC_QUATER_SEL, m_QuarterSelCtrl);
    DDX_Control(pDX, IDC_DATE_PICKER, m_DatePicker);
    DDX_Control(pDX, IDC_STATUS, m_StatusCtrl);
    DDX_Text(pDX, IDC_VENDOR, m_VendorName);
}

BEGIN_MESSAGE_MAP(CExpensesRecordManagerDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_STORE, &CExpensesRecordManagerDlg::OnBnClickedBtnStore)
    ON_BN_CLICKED(IDC_BTN_LOAD, &CExpensesRecordManagerDlg::OnBnClickedBtnLoad)
    ON_CBN_SELCHANGE(IDC_FY_SEL, &CExpensesRecordManagerDlg::OnCbnSelchangeFySel)
    ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CExpensesRecordManagerDlg::OnBnClickedBtnDownload)
    ON_COMMAND(ID_POPUP_DELETE, &CExpensesRecordManagerDlg::OnPopupDelete)
    ON_WM_CONTEXTMENU()
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

    m_sRupee.Format(L"%lc", 0x20B9);

    ShowWindow(SW_SHOWMAXIMIZED);

    m_ExpensesList.SetExtendedStyle(m_ExpensesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
    InitControls();
	
    _wmkdir(DB_DIRECTORY);

    SetDlgItemText(IDC_STATUS, L" Ready");
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
        0,                      // Rank/order of item 
        L"SNo",                 // Caption for this header 
        LVCFMT_CENTER,          // Relative position of items under header 
        50);                    // Width of items under header

    m_ExpensesList.InsertColumn(1, L"Date", LVCFMT_CENTER, 80);
    m_ExpensesList.InsertColumn(2, L"Financial Year", LVCFMT_CENTER, 100);
    m_ExpensesList.InsertColumn(3, L"Quarter", LVCFMT_CENTER, 50);
    m_ExpensesList.InsertColumn(4, L"Month", LVCFMT_CENTER, 150);
    m_ExpensesList.InsertColumn(5, L"Year", LVCFMT_CENTER, 150);
    m_ExpensesList.InsertColumn(6, L"Vendor", LVCFMT_CENTER, 150);
    m_ExpensesList.InsertColumn(7, L"Main Item", LVCFMT_CENTER, 150);
    m_ExpensesList.InsertColumn(8, L"Sub-Item", LVCFMT_CENTER, 150);
    m_ExpensesList.InsertColumn(9, L"Rate (" + m_sRupee + L")", LVCFMT_CENTER, 70);
    m_ExpensesList.InsertColumn(10, L"Quantity", LVCFMT_CENTER, 70);
    m_ExpensesList.InsertColumn(11, L"Amount (" + m_sRupee + L")", LVCFMT_CENTER, 100);
}

void CExpensesRecordManagerDlg::InsertRecord(int row, Record & rec)
{
    int nItem;
    CString _format;

    _format.Format(L"%d", row + 1);
    nItem = m_ExpensesList.InsertItem(0, _format);

    _format.Format(L"%d/%d/%d", rec._date.Day, rec._date.Month, rec._date.Year);
    m_ExpensesList.SetItemText(nItem, 1, _format);

    _format.Format(L"%d-%d", rec._fy.from, rec._fy.to);
    m_ExpensesList.SetItemText(nItem, 2, _format);

    _format.Format(L"Q%d", rec._quater);
    m_ExpensesList.SetItemText(nItem, 3, _format);

    CString _month(months[rec._date.Month - 1].c_str());
    m_ExpensesList.SetItemText(nItem, 4, _month);

    _format.Format(L"%d", rec._date.Year);
    m_ExpensesList.SetItemText(nItem, 5, _format);

    CString _strVendorName(rec._vendorName);
    m_ExpensesList.SetItemText(nItem, 6, _strVendorName);

    CString _strMainItem(rec._mainItem);
    m_ExpensesList.SetItemText(nItem, 7, _strMainItem);

    CString _strSubItem(rec._subItem);
    m_ExpensesList.SetItemText(nItem, 8, _strSubItem);

    _format.Format(L"%d", rec._rate);
    m_ExpensesList.SetItemText(nItem, 9, _format);

    _format.Format(L"%d", rec._quantity);
    m_ExpensesList.SetItemText(nItem, 10, _format);

    _format.Format(L"%d", rec._amount);
    m_ExpensesList.SetItemText(nItem, 11, _format);
}

void CExpensesRecordManagerDlg::InitControls()
{
    // Date Picker Control
    CTime *fromTime = new CTime(2000, 4, 1, 0, 0, 0);
    CTime *toTime = new CTime(CTime::GetCurrentTime());
    m_DatePicker.SetRange(fromTime, toTime);

    // Combo Boxes
    m_FySelCtrl.InsertString(0, L"-- Select --");
    m_QuarterSelCtrl.InsertString(0, L"-- Select --");

    m_FySelCtrl.SetCueBanner(L"-- Select --");
    m_QuarterSelCtrl.SetCueBanner(L"-- Select --");

    m_FySelCtrl.SetMinVisibleItems(5);

    CTime time = CTime::GetTickCount();
    m_Today.Day = time.GetDay();
    m_Today.Month = time.GetMonth();
    m_Today.Year = time.GetYear();

    int year_from = FY_START;
    int year_to = m_Today.Year;

    if (m_Today.Month > 3) {
        year_to += 1;
    }

    int i = 1; 
    int year = year_from;
    for ( ; year < year_to; ++i, year++) {
        CString _format;
        _format.Format(L"%d-%d", year, year + 1);
        m_FySelCtrl.InsertString(i, _format);
    }

    m_FySelCtrl.SetCurSel(m_FySelCtrl.GetCount()  - 1);

    OnCbnSelchangeFySel();
    m_QuarterSelCtrl.SetCurSel(0);

    // CLiistControl
    InitExpensesListControl();
}

int CExpensesRecordManagerDlg::UpdateDb()
{
    CString filename;
    filename.Format(FILE_NAME_FORMAT, m_RecordVec[0]._fy.from, m_RecordVec[0]._fy.to,
        m_RecordVec[0]._quater);

    CFile _file;
    if (!_file.Open(filename, CFile::modeCreate | CFile::modeWrite)) {
        AfxMessageBox(L"Store Error: Failed to open the Database file");
        SetDlgItemText(IDC_STATUS, L" Error");
        return -1;
    }
    //_file.SeekToBegin();

    CArchive ar(&_file, CArchive::store);

    for (auto it = m_RecordVec.begin(); it != m_RecordVec.end(); ++it) {

        Serialize(ar, *it);
    }

    m_ExpensesList.DeleteAllItems();
    for (int i = m_RecordVec.size() - 1; i >= 0; --i) {

        InsertRecord(i, m_RecordVec[i]);
    }

    ar.Close();

    return 0;
}

void CExpensesRecordManagerDlg::OnBnClickedBtnStore()
{
    SetDlgItemText(IDC_STATUS, L"");

    if (!UpdateData())
        return;

    SetDlgItemInt(IDC_AMOUNT, m_Quantity * m_Rate);

    if (m_MainItem.IsEmpty())
    {
        AfxMessageBox(L"Please provide the Main Item.");
        return;
    }

    if (m_SubItem.IsEmpty())
    {
        AfxMessageBox(L"Please provide the Sub-Item.");
        return;
    }

    SetDlgItemText(IDC_STATUS, L" Saving...");

    Record record;

    CT2A asci_vendor(m_VendorName, CP_UTF8);
    strcpy_s(record._vendorName, asci_vendor.m_psz);

    CT2A asci1(m_MainItem, CP_UTF8);
    strcpy_s(record._mainItem, asci1.m_psz);

    CT2A asci2(m_SubItem, CP_UTF8);
    strcpy_s(record._subItem, asci2.m_psz);

    record._quantity = m_Quantity;
    record._rate = m_Rate;
    record._amount = m_Quantity * m_Rate;

    CTime time;
    m_DatePicker.GetTime(time);
    record._date.Day = time.GetDay();
    record._date.Month = time.GetMonth();
    record._date.Year = time.GetYear();

    if (record._date.Month <= 3)
        record._quater = 4;
    else if (record._date.Month > 3 && record._date.Month <= 6)
        record._quater = 1;
    else if (record._date.Month > 6 && record._date.Month <= 9)
        record._quater = 2;
    else
        record._quater = 3;

    if (record._quater == 4)
    {
        record._fy.from = record._date.Year - 1;
        record._fy.to = record._date.Year;
    }
    else
    {
        record._fy.from = record._date.Year;
        record._fy.to = record._date.Year + 1;
    }

    CString filename;
    filename.Format(FILE_NAME_FORMAT, record._fy.from, record._fy.to, record._quater);

    CFile _file;
    if (!_file.Open(filename, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite)) {
        AfxMessageBox(L"Store Error: Failed to open the Database file");
        SetDlgItemText(IDC_STATUS, L" Error");
        return;
    }
    _file.SeekToEnd();

    CArchive ar(&_file, CArchive::store);
    Serialize(ar, record);
    ar.Close();
 
    SetDlgItemText(IDC_STATUS, L" Saved");
}

int CExpensesRecordManagerDlg::Serialize(CArchive & ar, Record & rec)
{
    if (ar.IsStoring())
    {
        size_t length = sizeof(Record);

        ar.Write(&rec, length);
    }
    else 
    {
        size_t length = sizeof(Record);

        size_t ret = ar.Read(&rec, length);

        if (ret < length)
            return -1;
    }

    return 0;
}

void CExpensesRecordManagerDlg::OnBnClickedBtnLoad()
{
    SetDlgItemText(IDC_STATUS, L"");

    int ret = 0;

    int index = m_FySelCtrl.GetCurSel();
    if (index == 0)
    {
        AfxMessageBox(L"Please select the Financial Year");
        SetDlgItemText(IDC_STATUS, L" Please select the Financial Year");
        return;
    }
    int year = index + FY_START;

    int quarter = m_QuarterSelCtrl.GetCurSel();
    if (quarter == 0)
    {
        AfxMessageBox(L"Please select the Quarter for selected Financial Year");
        SetDlgItemText(IDC_STATUS, L" Please select the Quarter for selected Financial Year");
        return;
    }

    SetDlgItemText(IDC_STATUS, L" Loading...");

    m_ExpensesList.DeleteAllItems();

    CString filename;
    filename.Format(FILE_NAME_FORMAT, year - 1, year, quarter);

    CFile _file;
    if (!_file.Open(filename, CFile::modeRead))
    {
        AfxMessageBox(L"No database for selected Financial Year and Quarter");
        SetDlgItemText(IDC_STATUS, L" No database for selected Financial Year and Quarter");
        return;
    }

    // Create a CArchive
    CArchive ar(&_file, CArchive::load);

    m_RecordVec.clear();

    for ( ; !ret ; )
    {
        Record rec;

        // Serialize
        if (Serialize(ar, rec) == -1)
        {
            ret = 1;
            break;
        }

        m_RecordVec.push_back(rec);

    }

    ar.Close();

    for (int i = m_RecordVec.size() - 1; i >= 0; --i) {

        InsertRecord(i, m_RecordVec[i]);
    }

    SetDlgItemText(IDC_STATUS, L" Load Complete");
}


void CExpensesRecordManagerDlg::OnCbnSelchangeFySel()
{
    int index = m_FySelCtrl.GetCurSel();

    int selYear = index + FY_START;
    int Quarter_to = 1;

    if (selYear > m_Today.Year)
    {
        if (m_Today.Month > 6 && m_Today.Month <= 9)
        {
            Quarter_to = 2;
        }
        if (m_Today.Month > 9 && m_Today.Month <= 12)
        {
            Quarter_to = 3;
        }
    }
    else 
    {
        Quarter_to = 4;
    }

    int prevSelection = m_QuarterSelCtrl.GetCurSel();
    m_QuarterSelCtrl.ResetContent();

    m_QuarterSelCtrl.InsertString(0, L"-- Select --");
    m_QuarterSelCtrl.InsertString(1, L"Q1");
    m_QuarterSelCtrl.SetCurSel(1);

    for (int i = 2; i <= Quarter_to; i++)
    {
        CString _format;
        _format.Format(L"Q%d", i);

        m_QuarterSelCtrl.InsertString(i, _format);
    }

    if (prevSelection < m_QuarterSelCtrl.GetCount())
    {
        m_QuarterSelCtrl.SetCurSel(prevSelection);
    }
}


void CExpensesRecordManagerDlg::OnBnClickedBtnDownload()
{
    SetDlgItemText(IDC_STATUS, L"");

    size_t count = m_RecordVec.size();
    if (count < 1) {
        AfxMessageBox(L"No data to download");
        return;
    }

    CFileDialog fileDlg(FALSE, // TRUE for FileOpen, FALSE for FileSaveAs
        L"csv",
        L"record",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        NULL,
        NULL,
        0,
        TRUE);

    if (fileDlg.DoModal() == IDOK)
    {
        CString _pathName = fileDlg.GetPathName();
        CT2A _pathAsci(_pathName);

        FILE *_file = 0;
        fopen_s(&_file, _pathAsci.m_psz, "w");
        if (!_file) {
	        AfxMessageBox(L"Download: File open error.");
	        SetDlgItemText(IDC_STATUS, L" Download error");
	        return;
        }

        SetDlgItemText(IDC_STATUS, L" Downloading...");

        fprintf_s(_file, "SNo,Date,Finacial Year,Quarter,Main Item,Sub Item,\
            Rate (Rs),Quantity,Amount (Rs)\n");

        unsigned long grandTotal = 0;

        for (size_t i = 0; i < count; ++i) {

            fprintf_s(_file, "%d,%d/%d/%d,%d-%d,Q%d,%s,%s,%s,%d,%d,%d\n",
                i + 1,
                m_RecordVec[i]._date.Day,
                m_RecordVec[i]._date.Month,
                m_RecordVec[i]._date.Year,
                m_RecordVec[i]._fy.from,
                m_RecordVec[i]._fy.to,
                m_RecordVec[i]._quater,
                m_RecordVec[i]._vendorName,
                m_RecordVec[i]._mainItem,
                m_RecordVec[i]._subItem,
                m_RecordVec[i]._rate,
                m_RecordVec[i]._quantity,
                m_RecordVec[i]._amount);

            grandTotal += m_RecordVec[i]._amount;
        }

        fprintf_s(_file, "\n,,,,,,,,%d\n", grandTotal);

        fclose(_file);		

        CString _fileName = fileDlg.GetFileTitle();

        SetDlgItemText(IDC_STATUS, L" Downloaded to " + _fileName + L".csv");

        _fileName += L" saved successfully";
        MessageBox(_fileName, L"Information");
    }
}


void CExpensesRecordManagerDlg::OnPopupDelete()
{
    SetDlgItemText(IDC_STATUS, L"");

    int row = 0;
    row = m_ExpensesList.GetSelectionMark();

    if (row < 0)
    {
        SetDlgItemText(IDC_STATUS, L" Nothing selected to delete");
        return;
    }
    else
    {
       SetDlgItemText(IDC_STATUS, L" Deleting...");

        m_RecordVec.erase(m_RecordVec.begin() + row, m_RecordVec.begin() + row + 1);

        m_ExpensesList.DeleteItem(row);

        if (UpdateDb() == 0)
        {
            SetDlgItemText(IDC_STATUS, L" Selected record deleted successfully");
        }
        else
        {
            AfxMessageBox(L"Error occured while updatig the record deletion updation to database");
            SetDlgItemText(IDC_STATUS, L" Error occured while updatig the deleted record");
        }
    }
}


void CExpensesRecordManagerDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
    // Load the desired menu
    CMenu mnuPopupSubmit;
    mnuPopupSubmit.LoadMenu(IDR_CONTEXT_MENU);

    // Get a pointer to the button
    CListCtrl *pList;
    pList = reinterpret_cast<CListCtrl *>(GetDlgItem(IDC_EXPENSES_LIST));

    // Find the rectangle around the button
    CRect rectList;
    pList->GetWindowRect(&rectList);

    // Get a pointer to the first item of the menu
    CMenu *mnuPopupMenu = mnuPopupSubmit.GetSubMenu(0);
    ASSERT(mnuPopupMenu);

    // Find out if the user right-clicked the button
    // because we are interested only in the button
    if (rectList.PtInRect(point)) // Since the user right-clicked the button, display the context menu
        mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
