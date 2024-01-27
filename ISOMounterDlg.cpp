
// ISOMounterDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ISOMounter.h"
#include "ISOMounterDlg.h"
#include "afxdialogex.h"
#include <initguid.h>
#include <virtdisk.h>

#pragma comment(lib, "virtdisk.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CISOMounterDlg dialog

CISOMounterDlg::CISOMounterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ISOMOUNTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CISOMounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CISOMounterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_MOUNT, &CISOMounterDlg::OnBnClickedMount)
	ON_BN_CLICKED(IDC_UNMOUNT, &CISOMounterDlg::OnBnClickedUnmount)
END_MESSAGE_MAP()


// CISOMounterDlg message handlers

BOOL CISOMounterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	pEditBrowse = (CMFCEditBrowseCtrl*)this->GetDlgItem(IDC_FILE_EDITBROWSE);
	pEditBrowse->EnableFileBrowseButton(NULL, _T("ISO Image (*.iso)|*.iso|All Files (*.*)|*.*||"));
	btnMount.Attach(GetDlgItem(IDC_MOUNT)->m_hWnd);
	btnUnmount.Attach(GetDlgItem(IDC_UNMOUNT)->m_hWnd);

	btnMount.EnableWindow(TRUE);  // Enable the first button
	btnUnmount.EnableWindow(FALSE); // Disable the second button

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CISOMounterDlg::OnPaint()
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
HCURSOR CISOMounterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CISOMounterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
		// Get the number of dropped files
	UINT nFilesDropped = DragQueryFile(hDropInfo, -1, nullptr, 0);

	if (nFilesDropped == 1) // Ensure only one file is dropped
	{
		TCHAR szFilePath[MAX_PATH];
		// Get the path of the dropped file
		DragQueryFile(hDropInfo, 0, szFilePath, MAX_PATH);
		CString strFilePath = szFilePath;
		if (strFilePath.Right(4).CompareNoCase(_T(".iso")) == 0)
		{
			pEditBrowse->SetWindowText(szFilePath);
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CISOMounterDlg::OnBnClickedMount()
{
	// TODO: Add your control notification handler code here
	VIRTUAL_STORAGE_TYPE vst;
	TCHAR szFilePath[MAX_PATH];
	DWORD ret;
	vst.DeviceId = 1;
	vst.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;
	pEditBrowse->GetWindowText(szFilePath, MAX_PATH);
	ret = OpenVirtualDisk(&vst, szFilePath,
		VIRTUAL_DISK_ACCESS_ATTACH_RO | VIRTUAL_DISK_ACCESS_DETACH,
		OPEN_VIRTUAL_DISK_FLAG_NONE, NULL, &isoHandle);
	if (ret != ERROR_SUCCESS) {
		AfxMessageBox(L"Mount failed!");
		return;
	}
	ret = AttachVirtualDisk(isoHandle, NULL, ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY, 0, NULL, NULL);
	if (ret != ERROR_SUCCESS) {
		AfxMessageBox(L"Attach failed!");
		CloseHandle(isoHandle);
		return;
	}
	btnMount.EnableWindow(FALSE);
	btnUnmount.EnableWindow(TRUE);
}

void CISOMounterDlg::OnBnClickedUnmount()
{
	// TODO: Add your control notification handler code here
	DWORD ret = DetachVirtualDisk(isoHandle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	if (ret != ERROR_SUCCESS) {
		AfxMessageBox(L"Detach failed!");
	}
	CloseHandle(isoHandle);
	btnMount.EnableWindow(TRUE);
	btnUnmount.EnableWindow(FALSE);
}

