/*	File			TCP_Demonstrator.h
	Purpose			Top-level application clsss
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
*/

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CTCP_DemonstratorApp : public CWinApp
{
public:
	CTCP_DemonstratorApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CTCP_DemonstratorApp theApp;