/*	File			ClientDialog.h
	Purpose			SIMPLE TCP CLIENT (for demonstration)
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
	Updated			February 2009

	Outline operation:
	The client sends a TCP connection request to the server
	Once connected, the client displays messages sent by the server
*/

#include "stdafx.h"
#include "TCP_Demonstrator.h"
#include "ClientDialog.h"
#include <sys/types.h>
#include <sys/timeb.h>
#include "TCP_DemonstratorDlg.h"
#include <string>

Player player;
CTCP_DemonstratorDlg dlg = new CTCP_DemonstratorDlg();

IMPLEMENT_DYNAMIC(CClientDialog, CDialog)
CClientDialog::CClientDialog(CWnd* pParent /*=NULL*/) // Class Constructor
	: CDialog(CClientDialog::IDD, pParent)
	, vMessage(_T(""))
	, m_Nickname(_T("Bob"))
{

	m_bTimerSet = false;
}

CClientDialog::~CClientDialog() // Class Destructor
{
	Close();
}

BEGIN_MESSAGE_MAP(CClientDialog, CDialog)
	ON_WM_TIMER()	// Needed if using a timer
	
	// User Interface Event handlers
	ON_BN_CLICKED(IDC_CONNECT, OnBnClickedConnect)
	ON_BN_CLICKED(IDC_Disconnect, OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_Send, &CClientDialog::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDialog::OnBnClickedButton2)
END_MESSAGE_MAP()

void CClientDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Map controls onto variables in the code
	DDX_Control(pDX, IDC_MESSAGE_TEXT, m_MessageText);
	DDX_Control(pDX, IDC_MESSAGE_NUMBER, m_MessageNumber);
	DDX_Control(pDX, IDC_ADD_1, m_ConnectAddress_1);
	DDX_Control(pDX, IDC_ADD_2, m_ConnectAddress_2);
	DDX_Control(pDX, IDC_ADD_3, m_ConnectAddress_3);
	DDX_Control(pDX, IDC_ADD_4, m_ConnectAddress_4);
	DDX_Control(pDX, IDC_CONNECT, m_ConnectButton);
	DDX_Control(pDX, IDC_Disconnect, m_DisconnectButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_SendMessage, m_Message);
	DDX_Control(pDX, IDC_RECEIVE, m_Receive);

	// Initialisation of User Interface controls
	m_ConnectButton.EnableWindow(true);
	m_CancelButton.EnableWindow(true);
	m_DisconnectButton.EnableWindow(false);
	DDX_Text(pDX, IDC_SendMessage, vMessage);
	DDX_Text(pDX, IDC_EDIT2, m_Nickname);
	DDX_Control(pDX, IDC_LIST4, vListPlayer);

	char sPlayerAlias[MAX_ALIAS_LEN + 1];
	strcpy(sPlayerAlias, "Champion");
	vListPlayer.AddString(sPlayerAlias);
	strcpy(sPlayerAlias, "Bob");
	vListPlayer.AddString(sPlayerAlias);

	extern char name[10];
	char temp[10];
	strcpy(temp, (char*)name);
	strcpy(player.PlayerName, temp);
}

IN_ADDR CClientDialog::GetConnectAddress()
{	
	IN_ADDR addr;
	CString csStr;
	m_ConnectAddress_1.GetWindowText(csStr);
//	addr.S_un.S_un_b.s_b1 = (unsigned  char) atol(csStr.GetString());
	addr.S_un.S_un_b.s_b1 = (unsigned  char)atol("193");
	m_ConnectAddress_2.GetWindowText(csStr);
//	addr.S_un.S_un_b.s_b2 = (unsigned  char) atol(csStr.GetString());
	addr.S_un.S_un_b.s_b2 = (unsigned  char)atol("60");
	m_ConnectAddress_3.GetWindowText(csStr);
//	addr.S_un.S_un_b.s_b3 = (unsigned  char) atol(csStr.GetString());
	addr.S_un.S_un_b.s_b3 = (unsigned  char)atol("73");
	m_ConnectAddress_4.GetWindowText(csStr);
//	addr.S_un.S_un_b.s_b4 = (unsigned  char) atol(csStr.GetString());
	addr.S_un.S_un_b.s_b4 = (unsigned  char)atol("102");
	return addr;
}

void CClientDialog::OnBnClickedConnect() // Connect Button handler
{	
	int x = atoi("8000");
	m_iSocket = socket(AF_INET, SOCK_STREAM, PF_UNSPEC); // Create the (listen) socket
	if(INVALID_SOCKET == m_iSocket)
	{
		MessageBox("Socket create failed","Client");
		CloseAndQuit();
	}
	
	u_long lBlocking = 0;
	int iError = ioctlsocket(m_iSocket, FIONBIO, &lBlocking); // Set blocking socket behaviour
	if(SOCKET_ERROR == iError)
	{
		MessageBox("Could not set Blocking behaviour for socket","Client");
		CloseAndQuit();
	}

	m_ConnectSockAddr.sin_addr = GetConnectAddress();	// Setup Connect Address Structure
	m_ConnectSockAddr.sin_family = AF_INET;
	//m_ConnectSockAddr.sin_port = htons(iPORT);
	m_ConnectSockAddr.sin_port = htons(x);
	
	// Connect
	iError = connect(m_iSocket, (const SOCKADDR FAR*)&m_ConnectSockAddr, sizeof(m_ConnectSockAddr));
	if(SOCKET_ERROR == iError)
	{
		MessageBox("Connect failed !\n\nIs server address correct ?\nIs server running ?","Client");
		closesocket(m_iSocket);		// Close the socket
		return;
	}

	// Set socket non-blocking (now that connection established) for receiving
	u_long lNonBlocking = 1;
	iError = ioctlsocket(m_iSocket, FIONBIO, &lNonBlocking);
	if(SOCKET_ERROR == iError)
	{
		MessageBox("Could not set Non Blocking behaviour for socket","Client");
		closesocket(m_iSocket);		// Close the socket
		return;
	}
	
	DoReceive();	// 7. Enable receiving 
	m_ConnectButton.EnableWindow(false); // Update controls
	m_CancelButton.EnableWindow(false);
	m_DisconnectButton.EnableWindow(true);
}

void CClientDialog::InitiateTimer()
{	
	m_nTimer = SetTimer(1, 100, 0);
	m_bTimerSet = true;
}

void CClientDialog::StopTimer()
{
	if(true == m_bTimerSet)
	{
		KillTimer(m_nTimer);
		m_bTimerSet = false;
	}
}

void CClientDialog::OnTimer(UINT nIDEvent) 
{	
	DoReceive();
}

void CClientDialog::DoReceive()
{		
	StopTimer();
	int iBytesRecd;
	Message_PDU Message;

	iBytesRecd = recv(m_iSocket, (char *) &Message, sizeof(Message_PDU), 0);
	if(SOCKET_ERROR == iBytesRecd)
	{
		int iError = WSAGetLastError();
		// If no packet is queued for the socket the operation would block.
		if(iError != WSAEWOULDBLOCK)
		{	
			MessageBox("Receive failed","Client");
			CloseAndQuit();	// Simplistic - assumes connection has been lost
		}
	}
	else {

		// Check Message Type
		switch (Message.mType)
		{
		case 0:
			// Set Client ID
			player.ClientID = Message.intValue;

			char tempChar[2];
			_itoa_s(player.ClientID, tempChar, 10);
			AppendLineText_LF(m_Receive, tempChar);

			// Send Client Name
			Message_PDU SendAlias;
			SendAlias.mType = 1;
			strcpy(SendAlias.szText, player.PlayerName);
			DoSend(SendAlias);
			break;
		case 1:
			// Clear Player List
			while (vListPlayer.GetCount() > 0)
			{
				vListPlayer.DeleteString(0);
			}

			//// Request Player List After Clearing Current List
			//Message_PDU RequestPlayerList;
			//RequestPlayerList.mType = 2;
			//DoSend(RequestPlayerList);
			//break;
		case 2:
			// Add Player To List
			char sPlayerAlias[MAX_ALIAS_LEN + 1];
			strcpy(sPlayerAlias, Message.szText);
			vListPlayer.AddString(sPlayerAlias);
			break;
		}
	}
	InitiateTimer();
}

void CClientDialog::OnBnClickedDisconnect() // Disconnect Button handler
{
	Disconnect();
	CloseAndQuit();

}

void CClientDialog::Close()
{
	shutdown(m_iSocket,2);	// Close the connection
	closesocket(m_iSocket);	// Close the socket
}

void CClientDialog::CloseAndQuit()
{
	Close();
	StopTimer();
	OnCancel();	// Exit the application
}

IN_ADDR CClientDialog::GetAddress()
{
	char szStr[200];
	DWORD lLen = sizeof(szStr);
    GetComputerNameA(szStr,&lLen);
	hostent * m_pHostent;
	m_pHostent = gethostbyname(szStr); 

	IN_ADDR addr;
	char **ppChar = m_pHostent->h_addr_list;
	char * pChar;
	if(ppChar != NULL && *ppChar != NULL)  
	{
		pChar = *ppChar; 
		addr.S_un.S_un_b.s_b1 = (unsigned  char) *pChar++;
		addr.S_un.S_un_b.s_b2 = (unsigned  char) *pChar++;
		addr.S_un.S_un_b.s_b3 = (unsigned  char) *pChar++;
		addr.S_un.S_un_b.s_b4 = (unsigned  char) *pChar;
	}
	else
	{
		addr.S_un.S_addr = -1;	// signal error
	}
	return addr;
}


void CClientDialog::OnBnClickedSend()
{
	AllChat();
}

void CClientDialog::AllChat() // Send a simple message to all connected clients
{
	Message_PDU Message;
	m_Message.GetWindowTextA(Message.szText, MAX_MESSAGE_LENGTH);
	Message.mType = 0;
	DoSend(Message);
}

void CClientDialog::DoSend(Message_PDU Message) // The actual send
{
	int iBytesSent;
	iBytesSent = send(m_iSocket,(char *)&Message, sizeof(Message_PDU), 0);
	if (SOCKET_ERROR == iBytesSent)
	{
		MessageBox("Failed to send", "Client");
	}
}

void CClientDialog::Disconnect() // Send a simple message to all connected clients
{
	Message_PDU Disconnect;
	Disconnect.mType = 0;
	strcpy_s(Disconnect.szText, "_Disconnect");
	DoSend(Disconnect);
}

void CClientDialog::AppendText(CEdit& edit, LPCTSTR pszText)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(pszText);
}

void CClientDialog::AppendLineText_LF(CEdit& edit, LPCTSTR pszText)
{
	CString strLine;
	// add CR/LF to text
	strLine.Format(_T("\r\n%s"), pszText);
	AppendText(edit, strLine);
}



void CClientDialog::OnBnClickedButton2()
{
	Message_PDU Message;
	Message.mType = 9;
	DoSend(Message);
}
