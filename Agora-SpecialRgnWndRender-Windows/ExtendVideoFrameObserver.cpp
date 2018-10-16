#include "stdafx.h"
#include "ExtendVideoFrameObserver.h"
#include <iostream>

#include "../LibYUV/include/libyuv.h"
#ifdef _M_IX86
#pragma comment(lib,"../LibYUV/lib/X86/libyuv.lib")
#elif defined _M_X64
#pragma comment(lib,"../LibYUV/lib/X64/libyuv.lib")
#endif

#include "CBufferMgr.h"

CExtendVideoFrameObserver::CExtendVideoFrameObserver()
{
	m_lpImageBuffer = new BYTE[0x800000];
	m_lpImageBufferTemp = new BYTE[0x800000];
	m_lpImageBufferRemote = new BYTE[0x800000];
	m_lpImageBufferRemoteTemp = new BYTE[0x800000];
}


CExtendVideoFrameObserver::~CExtendVideoFrameObserver()
{
	delete[] m_lpImageBuffer;
	delete[] m_lpImageBufferTemp;
	delete[] m_lpImageBufferRemote;
	delete[] m_lpImageBufferRemoteTemp;
}

bool CExtendVideoFrameObserver::onCaptureVideoFrame(VideoFrame& videoFrame)
{
	int nBufferSize = videoFrame.height * videoFrame.width * 3 /2;
	
	FILE* pFileRecord = fopen("..\\Extendaudio.txt", "ab+");
	if (pFileRecord) {

		std::string str = __FUNCTION__ + std::string("\n");
		fwrite(str.data(), 1, str.length(), pFileRecord);
		fclose(pFileRecord);
		pFileRecord = nullptr;
	}

	int nWidth = videoFrame.width;
	int nHeight = videoFrame.height;
	int nYStride = videoFrame.yStride;
	int nUStride = videoFrame.uStride;
	int nVStride = videoFrame.vStride;

	memcpy_s(m_lpImageBuffer, nYStride * nHeight, videoFrame.yBuffer, nYStride*nHeight);
	memcpy_s(m_lpImageBuffer + nYStride * nHeight, nUStride * nHeight / 2, videoFrame.uBuffer, nUStride * nHeight / 2);
	memcpy_s(m_lpImageBuffer + nYStride * nHeight + nUStride * nHeight / 2, nVStride * nHeight / 2, videoFrame.vBuffer, nVStride * nHeight / 2);
	int nBufferLen = nYStride * nHeight + nUStride *nHeight / 2 + nVStride *nHeight / 2;
	libyuv::ConvertToI420(m_lpImageBuffer, nBufferLen,
		m_lpImageBufferTemp, nYStride,
		m_lpImageBufferTemp + nYStride * nHeight, nUStride,
		m_lpImageBufferTemp + nYStride * nHeight + nUStride * nHeight / 2, nVStride,
		0, 0, nYStride, nHeight,
		nWidth, nHeight, libyuv::kRotate180, libyuv::FOURCC_I420);

	CBufferMgr::getInstance()->pushYUVBuffer(0, (uint8_t*)m_lpImageBufferTemp, nBufferLen, nWidth, nHeight);

	return true;
}

bool CExtendVideoFrameObserver::onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame)
{
	int nWidth = videoFrame.width;
	int nHeight = videoFrame.height;
	int nYStride = videoFrame.yStride;
	int nUStride = videoFrame.uStride;
	int nVStride = videoFrame.vStride;

	memcpy_s(m_lpImageBufferRemote, nYStride * nHeight, videoFrame.yBuffer, nYStride*nHeight);
	memcpy_s(m_lpImageBufferRemote + nYStride * nHeight, nUStride * nHeight / 2, videoFrame.uBuffer, nUStride * nHeight / 2);
	memcpy_s(m_lpImageBufferRemote + nYStride * nHeight + nUStride * nHeight / 2, nVStride * nHeight / 2, videoFrame.vBuffer, nVStride * nHeight / 2);
	int nBufferLen = nYStride * nHeight + nUStride *nHeight / 2 + nVStride *nHeight / 2;

	libyuv::ConvertToI420((uint8_t*)m_lpImageBufferRemote, nBufferLen,
		m_lpImageBufferRemoteTemp, nWidth,
		m_lpImageBufferRemoteTemp + nWidth * nHeight, nWidth / 2,
		m_lpImageBufferRemoteTemp + nWidth * nHeight * 5 / 4, nWidth / 2,
		0, 0, nYStride, nHeight,
		nWidth, nHeight, libyuv::kRotate180, libyuv::FOURCC_I420);
	nBufferLen = nWidth * nHeight * 3 / 2;

	CBufferMgr::getInstance()->pushYUVBuffer(uid, (uint8_t*)m_lpImageBufferRemoteTemp, nBufferLen, nWidth, nHeight);

	return true;
}
