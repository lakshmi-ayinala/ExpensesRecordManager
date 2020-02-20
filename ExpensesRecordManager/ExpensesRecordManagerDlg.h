
// ExpensesRecordManagerDlg.h : header file
//

#pragma once


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
	void Serialize(CArchive &ar);
	void InitControls();
	afx_msg void OnBnClickedBtnStore();
	afx_msg void OnBnClickedBtnLoad();
	CListCtrl m_ExpensesList;
	CString m_MainItem;
	CString m_SubItem;
	UINT m_Quantity;
	UINT m_Rate;
};
