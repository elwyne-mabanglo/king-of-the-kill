/*	File			TCP_DemonstratorDlg.h
	Purpose			Top-level dialog clsss
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
*/

#pragma once

#include <string>

#include "afxwin.h"
#include "TCP_Demonstator_General.h"

class CTCP_DemonstratorDlg : public CDialog
{
public:
	CTCP_DemonstratorDlg(CWnd* pParent = NULL);	// constructor

	enum { IDD = IDD_TCP_DEMONSTRATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	// Message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CEdit mUsername;
	CEdit mPassword;

public:

	// Event handlers
	afx_msg void OnBnClickedClient();
	afx_msg void OnBnClickedServer();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedLoginDlgRegisterBtn();
	afx_msg void OnBnClickedLoginDlgLoginBtn();
	CString vUsername;
	CString vPassword;

};
