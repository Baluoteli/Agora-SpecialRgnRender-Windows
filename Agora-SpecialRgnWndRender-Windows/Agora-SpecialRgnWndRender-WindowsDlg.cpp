
// Agora-SpecialRgnWndRender-WindowsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Agora-SpecialRgnWndRender-Windows.h"
#include "Agora-SpecialRgnWndRender-WindowsDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <fcntl.h>
#include <WinBase.h>

#include "../LibYUV/include/libyuv.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void ContructBhh(int nWidth, int nHeight, BITMAPFILEHEADER& bhh) //add 2010-9-04  
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）  
	bhh.bfType = ((WORD)('M' << 8) | 'B');  //'BM'  
	bhh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + widthStep * nHeight;
	bhh.bfReserved1 = 0;
	bhh.bfReserved2 = 0;
	bhh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
}

void ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih)
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8;


	bih.biSize = 40;       // header size  
	bih.biWidth = nWidth;
	bih.biHeight = nHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;     // RGB encoded, 24 bit  
	bih.biCompression = BI_RGB;   // no compression 非压缩  
	bih.biSizeImage = widthStep*nHeight * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
}

int RGB24ToBMP(BYTE* pRGB24Src, BYTE* pBmpDest, int nWidth, int nHeight) {

	int nLen = 0;

	BITMAPINFOHEADER bih;
	ConstructBih(nWidth, nHeight, bih);
	BITMAPFILEHEADER bhh;
	ContructBhh(nWidth, nHeight, bhh);

	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）  
	int DIBSize = widthStep * nHeight;  //buffer的大小 （字节为单位）  

	memcpy(pBmpDest, (LPSTR)&bhh,sizeof(BITMAPFILEHEADER));
	memcpy(pBmpDest + sizeof(BITMAPFILEHEADER), (LPSTR)&bih, sizeof(BITMAPINFOHEADER));
	memcpy(pBmpDest, pRGB24Src, DIBSize);

	nLen += sizeof(BITMAPINFOHEADER);
	nLen += sizeof(BITMAPFILEHEADER);
	nLen += DIBSize;

	return nLen;

	FILE* pFile = fopen("..\\bmp.bmp", "ab+");
	if (pFile) {
		fwrite(pBmpDest, 1, nLen, pFile);
		fclose(pFile);
	}
}

#define MAX_LEN (3*1280*720)
#define POSITIVE_HEIGHT (1)

#if 0
/*12Bytes*/
typedef struct                       /**** BMP file header structure ****/
{
	unsigned int   bfSize;           /* Size of file */
	unsigned short bfReserved1;      /* Reserved */
	unsigned short bfReserved2;      /* ... */
	unsigned int   bfOffBits;        /* Offset to bitmap data */
}BITMAPFILEHEADER;

/*40Bytes*/
typedef struct                       /**** BMP file info structure ****/
{
	unsigned int   biSize;           /* Size of info header */
	int            biWidth;          /* Width of image */
	int            biHeight;         /* Height of image */
	unsigned short biPlanes;         /* Number of color planes */
	unsigned short biBitCount;       /* Number of bits per pixel */
	unsigned int   biCompression;    /* Type of compression to use */
	unsigned int   biSizeImage;      /* Size of image data */
	int            biXPelsPerMeter;  /* X pixels per meter */
	int            biYPelsPerMeter;  /* Y pixels per meter */
	unsigned int   biClrUsed;        /* Number of colors used */
	unsigned int   biClrImportant;   /* Number of important colors */
}BITMAPINFOHEADER;
#endif

int simplest_rgb24_to_bmp(BYTE* pRGB24, int w, int h, BYTE* pBmpBuffer)
{
	int s32Ret = 0;
	char bfType[2] = { 'B', 'M' };
	BITMAPFILEHEADER myHead;
	BITMAPINFOHEADER myHeadInfo;
	memset(&myHead, 0, sizeof(myHead));
	memset(&myHeadInfo, 0, sizeof(myHeadInfo));
	BYTE* readBuff4Ph = new BYTE[MAX_LEN];
	ZeroMemory(readBuff4Ph, MAX_LEN);

	/*myHead*/
	int headerSize = sizeof(bfType) + sizeof(myHead) + sizeof(myHeadInfo);
	myHead.bfSize = headerSize + w*h * 3;
	myHead.bfOffBits = headerSize;

	/*myHeadInfo*/
	myHeadInfo.biSize = sizeof(myHeadInfo);
	myHeadInfo.biWidth = w;

#ifndef POSITIVE_HEIGHT
	myHeadInfo.biHeight = -1 * h;
#else
	myHeadInfo.biHeight = h;
#endif

	myHeadInfo.biPlanes = 1;
	myHeadInfo.biBitCount = 24;
	myHeadInfo.biSizeImage = w*h * 3;

	/*change R-G-B to B-G-R*/
	int temp = 0;
	for (int i = 0; i < (w*h); i++)
	{
		temp = *(pRGB24 + i * 3);
		*(pRGB24 + i * 3) = *(pRGB24 + i * 3 + 2);
		*(pRGB24 + i * 3 + 2) = temp;
	}

#ifdef POSITIVE_HEIGHT
	for (int i = (h - 1), j = 0; i >= 0; i--, j++)
	{
		memcpy(readBuff4Ph + j*w * 3, pRGB24 + i*w * 3, w * 3);
	}
#endif

	/*write-4 parts*/
	int nLen = 0;
	FILE* pFile = fopen("..\\bmp.bmp", "ab+");
	s32Ret = fwrite(bfType, 1, sizeof(bfType), pFile); nLen += s32Ret;
	s32Ret = fwrite(&myHead, 1, sizeof(myHead), pFile); nLen += s32Ret;
	s32Ret = fwrite(&myHeadInfo, 1, sizeof(myHeadInfo), pFile); nLen += s32Ret;
#ifdef POSITIVE_HEIGHT
	s32Ret = fwrite(readBuff4Ph, 1, w*h * 3, pFile); nLen += s32Ret;
#else
	s32Ret = write(fd_bmp, pRGB24, w*h * 3);
	if (s32Ret < 0)
	{
		printf("write pRGB24 failed!\n");
		close(fd_bmp);
		close(fd_ori);
		return -1;
	}
	printf("write pRGB24 success!\n");
#endif
	fclose(pFile);
	return nLen;
}

int simplest_rgb24_to_bmp(const char* rgb24Path, int w, int h, const char* bmpPath)
{
	int s32Ret = 0;
	int fd_ori = -1;
	int fd_bmp = -1;
	int headerSize = 0;
	int i = 0;//for circle
	int j = 0;//for circle
	unsigned char temp = 0;

	unsigned char *readBuff = new unsigned char[MAX_LEN];
	memset(readBuff, 0, sizeof(readBuff));

#ifdef POSITIVE_HEIGHT
	unsigned char *readBuff4Ph = new unsigned char[MAX_LEN];
	memset(readBuff4Ph, 0, sizeof(readBuff4Ph));
#endif

	char bfType[2] = { 'B', 'M' };

	BITMAPFILEHEADER myHead;
	BITMAPINFOHEADER myHeadInfo;
	memset(&myHead, 0, sizeof(myHead));
	memset(&myHeadInfo, 0, sizeof(myHeadInfo));
	printf("sizeof(myHead) = %d\n", sizeof(myHead));
	printf("sizeof(myHeadInfo) = %d\n", sizeof(myHeadInfo));

	/*myHead*/
	headerSize = sizeof(bfType) + sizeof(myHead) + sizeof(myHeadInfo);
	myHead.bfSize = headerSize + w*h * 3;
	myHead.bfOffBits = headerSize;

	/*myHeadInfo*/
	myHeadInfo.biSize = sizeof(myHeadInfo);
	myHeadInfo.biWidth = w;

#ifndef POSITIVE_HEIGHT
	myHeadInfo.biHeight = -1 * h;
#else
	myHeadInfo.biHeight = h;
#endif

	myHeadInfo.biPlanes = 1;
	myHeadInfo.biBitCount = 24;
	myHeadInfo.biSizeImage = w*h * 3;

	/*open files*/
	fd_ori = _open(rgb24Path, O_RDONLY);
	if (fd_ori < 0)
	{
		printf("open rgb24 failed!\n");
		return -1;
	}
	printf("open rgb24 success!\n");

	fd_bmp = _open(bmpPath, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 777);
	if (fd_bmp < 0)
	{
		printf("open bmp failed!\n");
		_close(fd_ori);
		return -1;
	}
	printf("open bmp success!\n");

	/*read*/
	memset(readBuff, 0, sizeof(readBuff));
	s32Ret = _read(fd_ori, readBuff, sizeof(readBuff));
	if ((s32Ret < 0) || (s32Ret != w*h * 3))
	{
		printf("read RGB file failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
	printf("read RGB file success!\n");

	/*change R-G-B to B-G-R*/
	for (i = 0; i < (w*h); i++)
	{
		temp = *(readBuff + i * 3);
		*(readBuff + i * 3) = *(readBuff + i * 3 + 2);
		*(readBuff + i * 3 + 2) = temp;
	}

	/*positive height storage sequence:left-right down-up*/
#ifdef POSITIVE_HEIGHT
	for (i = (h - 1), j = 0; i >= 0; i--, j++)
	{
		memcpy(readBuff4Ph + j*w * 3, readBuff + i*w * 3, w * 3);
	}
#endif

	/*write-4 parts*/
	s32Ret = _write(fd_bmp, bfType, sizeof(bfType));
	if (s32Ret < 0)
	{
		printf("write bfType failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
	s32Ret = _write(fd_bmp, &myHead, sizeof(myHead));
	if (s32Ret < 0)
	{
		printf("write myHead failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
	s32Ret = _write(fd_bmp, &myHeadInfo, sizeof(myHeadInfo));
	if (s32Ret < 0)
	{
		printf("write myHeadInfo failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
#ifdef POSITIVE_HEIGHT
	s32Ret = _write(fd_bmp, readBuff4Ph, w*h * 3);
	if (s32Ret < 0)
	{
		printf("write readBuff4Ph failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
	printf("write readBuff4Ph success!\n");
#else
	s32Ret = _write(fd_bmp, readBuff, w*h * 3);
	if (s32Ret < 0)
	{
		printf("write readBuff failed!\n");
		_close(fd_bmp);
		_close(fd_ori);
		return -1;
	}
	printf("write readBuff success!\n");
#endif

	delete[] readBuff4Ph; readBuff4Ph = NULL;
	_close(fd_bmp);
	_close(fd_ori);
	return 0;
}


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
	: CDialogEx(CAgoraSpecialRgnWndRenderWindowsDlg::IDD, pParent),
	m_gdiplusToken(0)
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
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
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
		CRect rt;
		GetClientRect(&rt);
		InvalidateRect(&rt, TRUE);
		//	CDialogEx::OnPaint();
	}
}

void CAgoraSpecialRgnWndRenderWindowsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent) {

		ShowWindow(SW_SHOW);
		//DrawClient(GetDC());
	}
}

BOOL CAgoraSpecialRgnWndRenderWindowsDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAgoraSpecialRgnWndRenderWindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAgoraSpecialRgnWndRenderWindowsDlg::initCtrl()
{
	SetBackgroundColor(RGB(255,0,0));
	Gdiplus::GdiplusStartupInput startupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &startupInput,nullptr);

	SetTimer(1, 40, NULL);
	CenterWindow();
	m_RngRenderWnd = ::CreateWindow(L"Static", L"", WS_CHILD, 0,0,100,100,m_hWnd,NULL,NULL,NULL);
}

void CAgoraSpecialRgnWndRenderWindowsDlg::uninitCtrl()
{
	KillTimer(1);

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	m_gdiplusToken = 0;
}

void CAgoraSpecialRgnWndRenderWindowsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CRect rt;
	GetWindowRect(&rt);
	//::SetWindowPos(m_hWnd, wndTopMost, rt.left, rt.top, rt.Width(), rt.Height(), SWP_SHOWWINDOW);

	CRect rt1;
	GetClientRect(&rt1);

	int nXPos = rt.left;
	int nYpos = rt.top;
	int nWidth = rt1.Height();
	int nHeight = rt.Height();
	nWidth = 360;
	nHeight = 360;
	MoveWindow(nXPos,nYpos,nWidth,nHeight, TRUE);
	GetClientRect(&rt);
}

void CAgoraSpecialRgnWndRenderWindowsDlg::initAgoraMediaRtc()
{
	m_strAppId = L"0c0b4b61adf94de1befd7cdd78a50444"; //please iniput appId;
	if (m_strAppId.IsEmpty())
		return;
	m_lpAgoraObject = CAgoraObject::GetAgoraObject((m_strAppId));
	ASSERT(m_lpAgoraObject);
	m_lpAgoraObject->SetMsgHandlerWnd(m_hWnd);

	m_lpRtcEngine = CAgoraObject::GetEngine();
	ASSERT(m_lpRtcEngine);

	m_lpAgoraObject->SetLogFilePath();
	m_lpAgoraObject->EnableVideo(TRUE);
	m_lpAgoraObject->SetVideoRenderType(1);

	m_lpRtcEngine->setChannelProfile(CHANNEL_PROFILE_TYPE::CHANNEL_PROFILE_LIVE_BROADCASTING);
	m_lpRtcEngine->setClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER);

	m_lpAgoraObject->EnableExtendVideoCapture(TRUE, &m_ExtendVideoFrame);

  //	m_lpAgoraObject->LocalVideoPreview(m_hWnd, TRUE);
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
	int nBufferWidth = 1280; int nBufferHeight = 720; UINT uIdLocal = 0;
	int nBufferOutLen = nBufferWidth * nBufferHeight * 3 / 2;
	BYTE *pBufferOutYUV = new BYTE[nBufferOutLen];
	ZeroMemory(pBufferOutYUV, nBufferOutLen);
	if (!CBufferMgr::getInstance()->popYUVBuffer(uIdLocal, pBufferOutYUV, nBufferOutLen, nBufferWidth, nBufferHeight)) {
		delete[] pBufferOutYUV; pBufferOutYUV = nullptr;
		return;
	}

	int nYStride = nBufferWidth;
	int nUStride = nBufferWidth / 2;
	int nVStride = nBufferWidth / 2;
	uint8_t* pBufferY = (uint8_t*)pBufferOutYUV;
	uint8_t* pBufferU = (uint8_t*)pBufferOutYUV + nYStride * nBufferHeight;
	uint8_t* pBufferV = pBufferU + nUStride * nBufferHeight / 2;
	nBufferOutLen = nBufferWidth * nBufferHeight * 3 / 2;
	
	BYTE *pBufferOutRGB24 = new BYTE[nBufferOutLen * 2];
	ZeroMemory(pBufferOutRGB24, nBufferOutLen * 2);
	CONVERT_YUV420PtoRGB24(pBufferOutYUV, pBufferOutRGB24, nBufferWidth, nBufferHeight);
	CHANGE_ENDIAN_PIC(pBufferOutRGB24,nBufferWidth,nBufferHeight,24);
	
	BITMAPINFO m_bmphdr = { 0 };
	DWORD dwBmpHdr = sizeof(BITMAPINFO);
	m_bmphdr.bmiHeader.biBitCount = 24;
	m_bmphdr.bmiHeader.biClrImportant = 0;
	m_bmphdr.bmiHeader.biSize = dwBmpHdr;
	m_bmphdr.bmiHeader.biSizeImage = 0;
	m_bmphdr.bmiHeader.biWidth = nBufferWidth;
	m_bmphdr.bmiHeader.biHeight = nBufferHeight;
	m_bmphdr.bmiHeader.biXPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biYPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biClrUsed = 0;
	m_bmphdr.bmiHeader.biPlanes = 1;
	m_bmphdr.bmiHeader.biCompression = BI_RGB;

	CRect rt;
	GetClientRect(&rt);
	HDC hdc = ::GetWindowDC(m_hWnd);
	HDC hMemDc = CreateCompatibleDC(hdc);
	HBITMAP hMemBitMap = CreateCompatibleBitmap(hdc, rt.Width(), rt.Height());
	HGLOBAL hOldObj = SelectObject(hMemDc, hMemBitMap);

	int nDcX = 0; int nDcY = 0; int nDcWidth = rt.Width(); int nDcHeight = rt.Height();
	int nResult = StretchDIBits(hMemDc,
		nDcX, nDcY,
		nDcWidth, nDcHeight,
		(nBufferWidth - nDcWidth ) /2, (nBufferHeight - nDcHeight ) /	2,
		nBufferHeight, nBufferHeight,
		pBufferOutRGB24,
		&m_bmphdr,
		DIB_RGB_COLORS,
		SRCCOPY);

	Graphics *graphics = Graphics::FromHDC(hMemDc);
	Image *pImage = Image::FromFile(_T("Combined Shape.png"));
	if (Gdiplus::Ok != pImage->GetLastStatus())
		return;

	Gdiplus::Pen pen(Color(153, 124, 3),3);

	POINT imRotateCenterPos = { rt.Width() / 2, rt.Height() / 2 };
	CRect rcShow(rt);
	PointF centerPos(imRotateCenterPos.x + rcShow.left, imRotateCenterPos.y + rcShow.top);
	graphics->TranslateTransform(centerPos.X, centerPos.Y);
	graphics->RotateTransform(0);
	graphics->TranslateTransform(-centerPos.X, -centerPos.Y);
	graphics->DrawImage(pImage, rt.left, rt.top, rt.Width(), rt.Height());

	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics->DrawEllipse(&pen, rt.left, rt.top, rt.Width(), rt.Height());

	BitBlt(hdc, rt.left, rt.top, rt.Width(), rt.Height(), hMemDc, 0, 0, SRCCOPY);

	graphics->ReleaseHDC(hMemDc);
	::ReleaseDC(m_hWnd, hdc);
	::DeleteObject(hMemDc);
	::DeleteObject(hMemBitMap);

	delete[] pBufferOutYUV;
	pBufferOutYUV = NULL;
	delete[] pBufferOutRGB24;
	pBufferOutRGB24 = NULL;

	Sleep(40);
	InvalidateRect(&rt, TRUE);
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
