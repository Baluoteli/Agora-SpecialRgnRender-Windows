
// Agora-SpecialRgnWndRender-WindowsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Agora-SpecialRgnWndRender-Windows.h"
#include "Agora-SpecialRgnWndRender-WindowsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAgoraSpecialRgnWndRenderWindowsDlg dialog



CAgoraSpecialRgnWndRenderWindowsDlg::CAgoraSpecialRgnWndRenderWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAgoraSpecialRgnWndRenderWindowsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAgoraSpecialRgnWndRenderWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAgoraSpecialRgnWndRenderWindowsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CAgoraSpecialRgnWndRenderWindowsDlg message handlers

BOOL CAgoraSpecialRgnWndRenderWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	initCtrl();
	initAgoraMediaRtc();

	//
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAgoraSpecialRgnWndRenderWindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAgoraSpecialRgnWndRenderWindowsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawClient(&dc);
	//	CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAgoraSpecialRgnWndRenderWindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAgoraSpecialRgnWndRenderWindowsDlg::initCtrl()
{
	m_RngRenderWnd = ::CreateWindow(L"Static", L"", WS_VISIBLE | WS_CHILD, 0,0,100,100,m_hWnd,NULL,NULL,NULL);
}

void CAgoraSpecialRgnWndRenderWindowsDlg::uninitCtrl()
{

}

void CAgoraSpecialRgnWndRenderWindowsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CRect rt;
	GetClientRect(&rt);

	CRect rt1;
	GetWindowRect(&rt1);
	MoveWindow(rt1.left, rt1.top, rt.bottom, rt1.bottom - rt1.top, TRUE);
	::MoveWindow(m_RngRenderWnd, 0, 0, rt.bottom, rt.bottom, TRUE);
	HRGN  hRgn = CreateEllipticRgn(rt.top, rt.top, rt.bottom, rt.bottom);
	int nres = ::SetWindowRgn(m_RngRenderWnd, hRgn, TRUE);
	m_lpAgoraObject->LocalVideoPreview(m_RngRenderWnd, TRUE);
}

void CAgoraSpecialRgnWndRenderWindowsDlg::initAgoraMediaRtc()
{
	m_strAppId = L""; //please iniput appId;
	if (m_strAppId.IsEmpty())
		return;
	m_lpAgoraObject = CAgoraObject::GetAgoraObject((m_strAppId));
	ASSERT(m_lpAgoraObject);
	m_lpAgoraObject->SetMsgHandlerWnd(m_hWnd);

	m_lpRtcEngine = CAgoraObject::GetEngine();
	ASSERT(m_lpRtcEngine);

	m_lpAgoraObject->SetLogFilePath();
	m_lpAgoraObject->EnableVideo(TRUE);

	m_lpRtcEngine->setChannelProfile(CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_LIVE_BROADCASTING);
	m_lpRtcEngine->setClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER);

	m_lpAgoraObject->LocalVideoPreview(m_RngRenderWnd, TRUE);
	m_lpRtcEngine->startPreview();
	m_lpAgoraObject->JoinChannel(L"test");
}

void CAgoraSpecialRgnWndRenderWindowsDlg::uninitAgoraMediaRtc()
{
	if (nullptr == m_lpAgoraObject){
		return;
	}

	m_lpAgoraObject->EnableVideo(FALSE);
	if (m_lpAgoraObject){
		CAgoraObject::CloseAgoraObject();
		m_lpAgoraObject = nullptr;
		m_lpRtcEngine = nullptr;
	}
}

void CAgoraSpecialRgnWndRenderWindowsDlg::DrawClient(CDC *lpDC)
{

}

void CAgoraSpecialRgnWndRenderWindowsDlg::OnClose()
{
	if (m_lpAgoraObject && m_lpRtcEngine) {

		m_lpAgoraObject->LeaveCahnnel();
		m_lpRtcEngine->stopPreview();

		uninitAgoraMediaRtc();
		uninitCtrl();
	}

	CDialogEx::OnClose();
}
