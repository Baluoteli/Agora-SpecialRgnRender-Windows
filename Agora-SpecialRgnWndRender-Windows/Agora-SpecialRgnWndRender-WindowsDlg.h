
// Agora-SpecialRgnWndRender-WindowsDlg.h : header file
//

#pragma once

#include <gdiplusgraphics.h> 
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")

// CAgoraSpecialRgnWndRenderWindowsDlg dialog
class CAgoraSpecialRgnWndRenderWindowsDlg : public CDialogEx
{
// Construction
public:
	CAgoraSpecialRgnWndRenderWindowsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AGORASPECIALRGNWNDRENDERWINDOWS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:

	void initCtrl();
	void uninitCtrl();

	void initAgoraMediaRtc();
	void uninitAgoraMediaRtc();

	void DrawClient(CDC *lpDC);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	CString m_strAppId;
	CString m_strAppCertificatId;
	UINT m_uId;
	CString m_strChannelName;

	CAgoraObject* m_lpAgoraObject;
	IRtcEngine* m_lpRtcEngine;

	HWND m_RngRenderWnd;

	ULONG_PTR m_gdiplusToken;
};
