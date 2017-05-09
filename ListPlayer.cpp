// ListPlayer.cpp : implementation file
//

#include "stdafx.h"
#include "ListPlayer.h"
#include "afxdialogex.h"


// ListPlayer dialog

IMPLEMENT_DYNAMIC(ListPlayer, CDialogEx)

ListPlayer::ListPlayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(ListPlayer::IDD, pParent)
{

}

ListPlayer::~ListPlayer()
{
}

void ListPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ListPlayer, CDialogEx)
END_MESSAGE_MAP()


// ListPlayer message handlers
