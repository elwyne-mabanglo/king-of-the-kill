/*	File			ClientDialog.h
	Purpose			SIMPLE TCP CLIENT (for demonstration)
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
	Updated			February 2009
	
	This is skeletal code with no frills.
	It is intended to provide a minimalistic TCP demonstration
	(and thus hopefully is quite easy to follow).
	Students following "Systems Programming" can use this code
	as a starting point for their coursework application
*/
#pragma once

#define MAX_ALIAS_LEN 30     

#include "afxwin.h"
#include "TCP_Demonstator_General.h"
#include "TCP_DemonstratorDlg.h"

struct Player {
	char PlayerName[10];
	int ClientID;
};

class CClientDialog : public CDialog
{
	DECLARE_DYNAMIC(CClientDialog)

public:
	//CClientDialog(CString username);
	CClientDialog(CWnd* pParent = NULL);   // constructor
	virtual ~CClientDialog();
	IN_ADDR GetConnectAddress();
	void InitiateTimer();
	void StopTimer();
	void OnTimer(UINT nIDEvent); 
	void DoReceive();
	void Close();
	void CloseAndQuit();
	void Disconnect();
	void AllChat();
	void DoSend(Message_PDU Message);
	IN_ADDR GetAddress();
	void AppendText(CEdit& edit, LPCTSTR pszText);
	void AppendLineText_LF(CEdit& edit, LPCTSTR pszText);

	enum { IDD = IDD_TCP_CLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	// Communications
	SOCKET m_iSocket;
	SOCKADDR_IN m_ConnectSockAddr;

	// Flags
	bool m_bTimerSet;

	// Timer
	UINT_PTR m_nTimer;

	// Controls
	CEdit m_ConnectAddress_1;
	CEdit m_ConnectAddress_2;
	CEdit m_ConnectAddress_3;
	CEdit m_ConnectAddress_4;
	CEdit m_SocketNumber;
	CEdit m_MessageText;
	CEdit m_MessageNumber;
	CEdit m_Message;
	CButton m_ConnectButton;
	CButton m_DisconnectButton;
	CButton m_CancelButton;
	CEdit m_Receive;

public:
	// Event handlers
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	CString vMessage;
	afx_msg void OnBnClickedSend();
	CString m_Nickname;
	CListBox vListPlayer;

	afx_msg void OnBnClickedButton2();
};

