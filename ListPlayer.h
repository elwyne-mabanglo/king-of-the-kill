#pragma once

#include "afxwin.h"
#include "TCP_Demonstator_General.h"
#include "TCP_Demonstrator.h"

// ListPlayer dialog

class ListPlayer : public CDialogEx
{
	DECLARE_DYNAMIC(ListPlayer)

public:
	ListPlayer(CWnd* pParent = NULL);   // standard constructor
	virtual ~ListPlayer();

// Dialog Data
	enum { IDD = IDD_TCP_CLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
