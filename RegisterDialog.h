#pragma once

#include "afxwin.h"
#include "TCP_Demonstator_General.h"
#include "TCP_Demonstrator.h"

// CRegisterDlg dialog

class CRegisterDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterDlg();

// Dialog Data
	enum { IDD = IDD_REGISTER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:

	CEdit mUsername;
	CEdit mPassword;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRegisterDlgBackBtn();
	afx_msg void OnBnClickedRegisterDlgRegisterBtn();
	CString vUsername;
	CString vPassword;
};
