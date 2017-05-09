/*	File			TCP_DemonstratorDlg.cpp
	Purpose			Top-level dialog clsss
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
*/

#include <string>
#include <iostream>

#include "stdafx.h"
#include "TCP_Demonstrator.h"
#include "TCP_DemonstratorDlg.h"
#include "ClientDialog.h"
#include "ServerDialog.h"
#include "RegisterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char name[10];

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CTCP_DemonstratorDlg::CTCP_DemonstratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCP_DemonstratorDlg::IDD, pParent)
	, vUsername(_T("Test"))
	, vPassword(_T("Test"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCP_DemonstratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LOGIN_DLG_USERNAME_TXT, mUsername);
	DDX_Control(pDX, IDC_LOGIN_DLG_PASSWORD_TXT, mPassword);
	DDX_Text(pDX, IDC_LOGIN_DLG_USERNAME_TXT, vUsername);
	DDX_Text(pDX, IDC_LOGIN_DLG_PASSWORD_TXT, vPassword);
}

BEGIN_MESSAGE_MAP(CTCP_DemonstratorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CLIENT, OnBnClickedClient)
	ON_BN_CLICKED(IDC_SERVER, OnBnClickedServer)
	ON_BN_CLICKED(IDC_About, OnBnClickedAbout)
	ON_BN_CLICKED(IDC_LOGIN_DLG_REGISTER_BTN, &CTCP_DemonstratorDlg::OnBnClickedLoginDlgRegisterBtn)
	ON_BN_CLICKED(IDC_LOGIN_DLG_LOGIN_BTN, &CTCP_DemonstratorDlg::OnBnClickedLoginDlgLoginBtn)
END_MESSAGE_MAP()

BOOL CTCP_DemonstratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;
}

void CTCP_DemonstratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void CTCP_DemonstratorDlg::OnPaint() 
{
	CDialog::OnPaint();
}

void CTCP_DemonstratorDlg::OnBnClickedClient()
{
	CClientDialog dlg;
	dlg.DoModal();
}

void CTCP_DemonstratorDlg::OnBnClickedServer()
{
	CServerDialog dlg;
	dlg.DoModal();
}

void CTCP_DemonstratorDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CTCP_DemonstratorDlg::OnBnClickedLoginDlgRegisterBtn()
{
	CRegisterDlg dlg;
	dlg.DoModal();
}


void CAboutDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

Player p;

void CTCP_DemonstratorDlg::OnBnClickedLoginDlgLoginBtn()
{
	//char name[10];

	//mUsername.GetWindowTextA(p.PlayerName, 10);

	//strcpy(name, p.PlayerName);

	//vUsername.SetString( mUsername.GetWindowTextA);

	//returnUsername(mUsername.GetWindowTextA);

	mUsername.GetWindowTextA(name,10);

	//mUsername.GetWindowTextA(p.PlayerName, 10);
	CClientDialog dlg; //= new CClientDialog(mUsername.GetWindowTextA);
	dlg.DoModal();
}







