/*	File			ServerDialog.cpp
	Purpose			SIMPLE TCP SERVER (for demonstration)
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
	Updated			February 2009

	Outline operation:
	The server, once enabled, waits for TCP connection requests from clients
	Multiple client connections are supported
	When a client connects, the server sends a message to ALL connected clients
	The message states the total number of connected clients
*/

#include "stdafx.h"
#include "ServerDialog.h"
#include <sys/types.h>
#include <sys/timeb.h>

IMPLEMENT_DYNAMIC(CServerDialog, CDialog)
CServerDialog::CServerDialog(CWnd* pParent /*=NULL*/) // Class Constructor
	: CDialog(CServerDialog::IDD, pParent)	
	, str_Server_Port(_T("8000"))
	, vReceiveMessage(_T(""))
	, m_SendMessage(_T(""))
{
	m_bTimerSet = false;

	for(int i = 0; i < MAX_CONNECTIONS; i++) // Initialise connections array
	{	
		m_ConnectionArray[i].bInUse = false;
	}	
	m_iMessageNumber = 0;
}

CServerDialog::~CServerDialog() // Class Destructor
{
	CloseAll();
}

void CServerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Map controls onto variables in the code
	DDX_Control(pDX, IDC_ADDRESS, m_LocalAddress);
	DDX_Control(pDX, IDC_CONNECTED_COUNT, m_ConnectedCount);
	DDX_Control(pDX, IDC_ENABLE_SERVER, m_EnableServerButton);
	DDX_Control(pDX, ID_CLOSE, m_ShutdownServerButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_ReceiveMessage, m_Message);
	DDX_Control(pDX, IDC_SENDMSG, m_Send);

	// Initialisation of User Interface controls
	m_EnableServerButton.EnableWindow(true);
	m_ShutdownServerButton.EnableWindow(false);
	m_CancelButton.EnableWindow(true);
	SetUpLocalAddressStruct();	// Also displays address
	CountActiveConnections();	// Updates control
	DDX_Text(pDX, IDC_Server_Port, str_Server_Port);
	DDV_MaxChars(pDX, str_Server_Port, 4);
	DDX_Text(pDX, IDC_ReceiveMessage, vReceiveMessage);
	DDX_Text(pDX, IDC_SENDMSG, m_SendMessage);
}

BEGIN_MESSAGE_MAP(CServerDialog, CDialog)
	ON_WM_TIMER()	// Needed if using a timer
	
	// User Interface Event handlers
	ON_BN_CLICKED(IDC_ENABLE_SERVER, OnBnClickedEnableServer)
	ON_BN_CLICKED(ID_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

void CServerDialog::SetUpLocalAddressStruct()
{	

	if (m_FirstRun) {

		m_LocalSockAddr.sin_addr = GetLocalHostAddressDetails();

		CString csStr;
		csStr.Format("%d.%d.%d.%d",
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b1,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b2,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b3,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b4);
		m_LocalAddress.SetWindowText(csStr.GetString());

		m_FirstRun = false;
	}
	else {

		int x = atoi(str_Server_Port);

		m_LocalSockAddr.sin_addr = GetLocalHostAddressDetails();
		m_LocalSockAddr.sin_family = AF_INET;
		//m_LocalSockAddr.sin_port = htons(iPORT);
		m_LocalSockAddr.sin_port = htons(x);

		// Display the local address value
		CString csStr;
		csStr.Format("%d.%d.%d.%d",
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b1,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b2,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b3,
			m_LocalSockAddr.sin_addr.S_un.S_un_b.s_b4);
		m_LocalAddress.SetWindowText(csStr.GetString());
	}
}

void CServerDialog::OnBnClickedEnableServer()
{
	m_iSocket = socket(AF_INET, SOCK_STREAM, PF_UNSPEC); // 1. Create the (listen) socket

	if(INVALID_SOCKET == m_iSocket)
	{
		MessageBox("Socket create failed","Server");
		exit(1);
	}

	u_long lNonBlocking = 1;
	int iError = ioctlsocket(m_iSocket, FIONBIO, &lNonBlocking); // 2. Set the socket NonBlocking
	if(SOCKET_ERROR == iError)
	{
		MessageBox("Could not set Non-Blocking behaviour for listen socket","Server");
		exit(1);
	}

	SetUpLocalAddressStruct(); // 3. Set up the address structure

	int iResult = bind(m_iSocket, (const SOCKADDR FAR*)&m_LocalSockAddr, sizeof(m_LocalSockAddr));	// 4. Bind
	if(SOCKET_ERROR == iResult)
	{
		MessageBox("Bind failed !\nPort is probably in use","Server");
		exit(1);
	}
	
	iResult = listen(m_iSocket,SOMAXCONN); // 5. Listen - for TCP connection requests from clients
	if(SOCKET_ERROR == iResult)
	{
		MessageBox("Listen failed");
		exit(1);
	}
	
	m_EnableServerButton.EnableWindow(false); // 6. Update controls on success
	m_ShutdownServerButton.EnableWindow(true);
	m_CancelButton.EnableWindow(false);
	Accept();
}

int CServerDialog::GetUnusedConnection()
{
	for(int i = 0; i < MAX_CONNECTIONS; i++)
	{	
		if(m_ConnectionArray[i].bInUse == false)
		{
			return i;
		}
	}
	return -1;	// Signal that all connections are in use
}

void CServerDialog::Accept()
{
	int iIndex = GetUnusedConnection();
	if(iIndex == -1)
	{
		return;
	}
	
	int iRemoteAddrLen = sizeof(SOCKADDR_IN);
	bool bResult = false;
	m_ConnectionArray[iIndex].iConnectSocket = accept(m_iSocket, 
		(SOCKADDR FAR*)&m_ConnectionArray[iIndex].ConnectSockAddr, &iRemoteAddrLen);
	if(INVALID_SOCKET == m_ConnectionArray[iIndex].iConnectSocket)
	{
		// If no connection is queued for the socket the operation would block.
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{	
			MessageBox("Accept failed","Server");
		}
	}
	else
	{
		// Set the newly-created connection socket to Non-Blocking
		u_long lNonBlocking = 1;
		int iError = ioctlsocket(m_ConnectionArray[iIndex].iConnectSocket, FIONBIO, &lNonBlocking);
		if(SOCKET_ERROR == iError)
		{
			MessageBox("Could not set Non-Blocking behaviour for connection socket","Server");
			OnCancel();	// Exit the application
		}

		m_ConnectionArray[iIndex].bInUse = true;

		Message_PDU SendClientID;
		SendClientID.mType = 0;
		SendClientID.intValue = iIndex;
		Send(SendClientID, iIndex);

		Message_PDU RefreshPlayerList;
		RefreshPlayerList.mType = 1;
		DoSend(RefreshPlayerList);

		UpdateConnectionDisplayCount();
	}
	InitiateTimer();	// Needed because socket is Non-Blocking 
						// The timer facilitates periodically checking 
						// for connection requests
}

int CServerDialog::CountActiveConnections()
{
	int iCount = 0;
	for(int i = 0; i < MAX_CONNECTIONS; i++)
	{	
		if(m_ConnectionArray[i].bInUse == true)
		{
			iCount++;;
		}
	}
	CString csStr;
	csStr.Format("%d",iCount);
	m_ConnectedCount.SetWindowText(csStr.GetString());	// Update No. connections control
	return iCount;
}

void CServerDialog::InitiateTimer()
{	
	m_nTimer = SetTimer(1, 100, 0);
	m_bTimerSet = true;
}

void CServerDialog::StopTimer()
{
	if(true == m_bTimerSet)
	{
		KillTimer(m_nTimer);
		m_bTimerSet = false;
	}
}

void CServerDialog::OnTimer(UINT nIDEvent) 
{	// Every time the timer 'times out' this code runs
	StopTimer();
	DoReceive();
	Accept();	// Accept thus gets called periodically
				// Needs to be periodic because a client may request to connect at any time
}

void CServerDialog::UpdateConnectionDisplayCount() // Send a simple message to all connected clients
{
	int iConnections;
	iConnections = CountActiveConnections();
	char szTemp[15];
	_itoa_s(iConnections,szTemp,10);
}

void CServerDialog::SendMessage2() // Send a simple message to all connected clients
{
	Message_PDU Message;
	m_Send.GetWindowTextA(Message.szText, MAX_MESSAGE_LENGTH);
	DoSend(Message);
}

void CServerDialog::DoSend(Message_PDU Message) // The actual send
{	
	int iBytesSent;
	int iIndex;
	for(iIndex = 0; iIndex < MAX_CONNECTIONS; iIndex++)
	{
		if(true == m_ConnectionArray[iIndex].bInUse)
		{
			iBytesSent = send(m_ConnectionArray[iIndex].iConnectSocket,
					(char *) &Message, sizeof(Message_PDU), 0);
			if(SOCKET_ERROR == iBytesSent)
			{
				MessageBox("Failed to send","Server");
				CloseConnection(iIndex);	// Simplistic - assumes connection has been lost
			}
		}
	}
}

void CServerDialog::Send(Message_PDU Message, int ClientID) // The actual send
{
	int iBytesSent;
	if (true == m_ConnectionArray[ClientID].bInUse)
	{
		iBytesSent = send(m_ConnectionArray[ClientID].iConnectSocket,
			(char *)&Message, sizeof(Message_PDU), 0);
		if (SOCKET_ERROR == iBytesSent)
		{
			MessageBox("Failed to send", "Server");
			CloseConnection(ClientID);	// Simplistic - assumes connection has been lost
		}
	}
}

void CServerDialog::CloseConnection(int iConnection)
{	
	if(m_ConnectionArray[iConnection].bInUse == true)
	{	
		shutdown(m_ConnectionArray[iConnection].iConnectSocket,2);	// Close the connection
		closesocket(m_ConnectionArray[iConnection].iConnectSocket);	// Close the socket
		m_ConnectionArray[iConnection].bInUse = false;
		memset(m_ConnectionArray[iConnection].userName, 0, sizeof m_ConnectionArray[iConnection].userName);
		UpdateConnectionDisplayCount();

		Message_PDU RefreshPlayerList;
		RefreshPlayerList.mType = 1;
		DoSend(RefreshPlayerList);

		//SendPlayerList();
	}
}

void CServerDialog::CloseAll()
{
	for(int i = 0; i < MAX_CONNECTIONS; i++)	// Close All active connections
	{	
		CloseConnection(i);
	}
	closesocket(m_iSocket); // Close the listen socket
}

void CServerDialog::OnBnClickedClose() // Shutdown Button handler
{
	CloseAll();
	OnCancel();	// Exit the application
}

void CServerDialog::OnBnClickedCancel() // Cancel Button handler
{
	CloseAll();
	OnCancel();	// Exit the application
}

IN_ADDR CServerDialog::GetLocalHostAddressDetails()
{	
	// This is the most complicated aspect of addressing.
	// You probably don't need to understand this method,
	// just use it as is.
	char szStr[80];
	DWORD lLen = sizeof(szStr);
    GetComputerNameA(szStr,&lLen);
	hostent* pHost;
	pHost = gethostbyname(szStr);
		
	IN_ADDR addr;
	char*  pChar;
	char** ppChar = pHost->h_addr_list;

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
		MessageBox("Failed to get local address!","Server");
		this->OnCancel();
	}
	return addr;
}



void CServerDialog::OnBnClickedButton1()
{
	//send message
	SendMessage2();
}

void CServerDialog::DoReceive()
{
	int iBytesRecd;
	Message_PDU Message;
	int iIndex;

	for (iIndex = 0; iIndex < MAX_CONNECTIONS; iIndex++)
	{
			iBytesRecd = recv(m_ConnectionArray[iIndex].iConnectSocket, (char *)&Message, sizeof(Message_PDU), 0);
			if (SOCKET_ERROR == iBytesRecd)
			{
				int iError = WSAGetLastError();			
			}
			else
			{
				// Check Message Type
				switch (Message.mType)
				{
				case 0:
					if (0 == strcmp(Message.szText, "_Disconnect"))
					{	// The message text was "Disconnect"
						CloseConnection(iIndex);			
					}
					break;
				case 1:
					strcpy(m_ConnectionArray[iIndex].userName, Message.szText);
					//AppendLineText_LF(m_Message, _T(m_ConnectionArray[iIndex].userName));

					SendPlayerList();

					//for (int x = 0; x < MAX_CONNECTIONS; x++)
					//{
					//	Message_PDU SendPlayerList;
					//	if (true == m_ConnectionArray[x].bInUse)
					//	{			
					//		SendPlayerList.mType = 2;
					//		strcpy(SendPlayerList.szText, m_ConnectionArray[x].userName);
					//		DoSend(SendPlayerList);

					//		//char tempChar[2];
					//		//_itoa_s(x, tempChar, 10);
					//		//AppendLineText_LF(m_Message, tempChar);

					//		//AppendLineText_LF(m_Message, _T(m_ConnectionArray[x].userName));
					//	}
					//}
					break;
				}
		}
	}
}

void CServerDialog::SendPlayerList(){
	for (int x = 0; x < MAX_CONNECTIONS; x++)
	{
		Message_PDU SendPlayerList;
		if (true == m_ConnectionArray[x].bInUse)
		{
			SendPlayerList.mType = 2;
			strcpy(SendPlayerList.szText, m_ConnectionArray[x].userName);
			DoSend(SendPlayerList);

			//char tempChar[2];
			//_itoa_s(x, tempChar, 10);
			//AppendLineText_LF(m_Message, tempChar);

			//AppendLineText_LF(m_Message, _T(m_ConnectionArray[x].userName));
		}
	}
}

void CServerDialog::AppendText(CEdit& edit, LPCTSTR pszText)
{
	// get the initial text length
	int nLength = edit.GetWindowTextLength();
	// put the selection at the end of text
	edit.SetSel(nLength, nLength);
	// replace the selection
	edit.ReplaceSel(pszText);
}

void CServerDialog::AppendLineText_LF(CEdit& edit, LPCTSTR pszText)
{
	CString strLine;
	// add CR/LF to text
	strLine.Format(_T("\r\n%s"), pszText);
	AppendText(edit, strLine);
}