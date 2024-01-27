
// ISOMounterDlg.h : header file
//

#pragma once


// CISOMounterDlg dialog
class CISOMounterDlg : public CDialogEx
{
// Construction
public:
	CISOMounterDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ISOMOUNTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CMFCEditBrowseCtrl* pEditBrowse;
	CButton btnMount, btnUnmount;
	HANDLE isoHandle;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedMount();
	afx_msg void OnBnClickedUnmount();
};
