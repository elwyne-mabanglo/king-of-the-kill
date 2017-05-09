// RegisterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterDialog.h"
#include "afxdialogex.h"


// CRegisterDlg dialog

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialog)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDlg::IDD, pParent)
	, vUsername(_T("Test"))
	, vPassword(_T("Test"))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REGISTER_DLG_USERNAME_TXT, vUsername);
	DDX_Text(pDX, IDC_REGISTER_DLG_PASSWORD_TXT, vPassword);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialog)
	ON_BN_CLICKED(IDC_REGISTER_DLG_BACK_BTN, &CRegisterDlg::OnBnClickedRegisterDlgBackBtn)
	ON_BN_CLICKED(IDC_REGISTER_DLG_REGISTER_BTN, &CRegisterDlg::OnBnClickedRegisterDlgRegisterBtn)
END_MESSAGE_MAP()


// CRegisterDlg message handlers


void CRegisterDlg::OnBnClickedRegisterDlgBackBtn()
{
	CDialog::OnOK();
}


void CRegisterDlg::OnBnClickedRegisterDlgRegisterBtn()
{

	mUsername.GetWindowTextA(vUsername);
	mPassword.GetWindowTextA(vPassword);

	CDialog::OnOK();


}

