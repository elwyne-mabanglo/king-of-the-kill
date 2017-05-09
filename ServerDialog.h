/*	File			ServerDialog.h
	Purpose			SIMPLE TCP SERVER (for demonstration)
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
	Updated			February 2009
*/

#pragma once

#include "afxwin.h"
#include "TCP_Demonstator_General.h"
#include "TCP_Demonstrator.h"

#define MAX_CONNECTIONS 50

struct Connection {
	SOCKET iConnectSocket;
	SOCKADDR_IN ConnectSockAddr;
	char userName[MAX_USER_LENGTH + 1];
	bool bInUse = false;
};

class CServerDialog : public CDialog
{
	DECLARE_DYNAMIC(CServerDialog)

public:
	CServerDialog(CWnd* pParent = NULL);   // constructor
	virtual ~CServerDialog();
	void InitialiseControls();
	void SetUpLocalAddressStruct();
	IN_ADDR GetLocalHostAddressDetails();
	void InitiateTimer();					// REPEAT for non-blocking recv's
	void StopTimer();
	void OnTimer(UINT nIDEvent); 
	int CountActiveConnections();
	int GetUnusedConnection();
	void CloseAll();
	void CloseConnection(int iConnection);
	void Accept();
	void UpdateConnectionDisplayCount();
	void DoSend(Message_PDU Message);
	void DoReceive();
	void AppendText(CEdit& edit, LPCTSTR pszText);
	void AppendLineText_LF(CEdit& edit, LPCTSTR pszText);
	void SendPlayerList();
	void SendMessage2();
	void Send(Message_PDU Message, int ClientID);

	void ReplaceEditLineText(int nLine);

	enum { IDD = IDD_TCP_SERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	// Controls
	CEdit m_LocalAddress;
	CEdit m_ConnectedCount;
	CEdit m_Message;
	CEdit m_Send;
	CButton m_EnableServerButton;
	CButton m_ShutdownServerButton;
	CButton m_CancelButton;
	
	// Communications
	int m_iMessageNumber;
	int counter = 0;
	SOCKET m_iSocket;
	SOCKADDR_IN m_LocalSockAddr;
	Connection m_ConnectionArray[MAX_CONNECTIONS];

	// Flags
	bool m_bTimerSet;
	bool m_FirstRun = true;

	// Timer
	UINT_PTR m_nTimer;



public:
	// Event handlers
	afx_msg void OnBnClickedEnableServer();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedCancel();
	CString str_Server_Port;
	CString vReceiveMessage;
	afx_msg void OnBnClickedButton1();
	CString m_SendMessage;
};



