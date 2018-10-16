#pragma once
#include "../SDK/include/IAgoraMediaEngine.h"
#include "CBufferMgr.h"

class CExtendVideoFrameObserver :
	public agora::media::IVideoFrameObserver
{
public:
	CExtendVideoFrameObserver();
	~CExtendVideoFrameObserver();

	virtual bool onCaptureVideoFrame(VideoFrame& videoFrame);
	virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame);

private:
	LPBYTE				m_lpImageBuffer;
	LPBYTE				m_lpImageBufferTemp;
	LPBYTE				m_lpImageBufferRemote;
	LPBYTE				m_lpImageBufferRemoteTemp;
	LPBYTE				m_lpY;
	LPBYTE				m_lpU;
	LPBYTE				m_lpV;
};

