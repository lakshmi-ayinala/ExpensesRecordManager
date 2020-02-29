
// ExpensesRecordManagerDlg.h : header file
//

#pragma once

#include <vector>
#include <fstream>

typedef struct {
	int Day;
	int Month;
	int Year;
}Date;

typedef struct {
	int from;
	int to;
}FY;	// Financial Year

typedef struct {
	int _quater;
	int _rate;
	int _quantity;
	int _amount;
	Date _date;
	FY _fy;
	char _mainItem[256];
	char _subItem[256];

	///Reserved
	char reserved[1024];
}Record;

// CExpensesRecordManagerDlg dialog
class CExpensesRecordManagerDlg : public CDialogEx
{
// Construction
public:
	CExpensesRecordManagerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPENSESRECORDMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitExpensesListControl();
	void InsertRecord(int row, Record &rec);
	int Serialize(CArchive &ar, Record &rec);
	int UpdateDb();
	void InitControls();
	Date m_Today;
	std::vector<Record> m_RecordVec;
	afx_msg void OnBnClickedBtnStore();
	afx_msg void OnBnClickedBtnLoad();
	CListCtrl m_ExpensesList;
	CString m_MainItem;
	CString m_SubItem;
	UINT m_Quantity;
	UINT m_Rate;
	afx_msg void OnCbnSelchangeFySel();
	CComboBox m_FySelCtrl;
	CComboBox m_QuarterSelCtrl;
	CDateTimeCtrl m_DatePicker;
	CStatic m_StatusCtrl;
	CString m_sRupee;
	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnPopupDelete();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
