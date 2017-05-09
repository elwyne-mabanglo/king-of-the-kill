/*	File			TCP_Demonstrator.cpp
	Purpose			Top-level application clsss
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
*/

#include "stdafx.h"
#include "TCP_Demonstrator.h"
#include "TCP_DemonstratorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCP_DemonstratorApp

BEGIN_MESSAGE_MAP(CTCP_DemonstratorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTCP_DemonstratorApp construction

CTCP_DemonstratorApp::CTCP_DemonstratorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


CTCP_DemonstratorApp theApp;

BOOL CTCP_DemonstratorApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();


	CTCP_DemonstratorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
