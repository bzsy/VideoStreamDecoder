
// VideoStreamDecoderDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "DecodeThread.h"

#define TYPE_H264 3
#define TYPE_H263 1
#define TYPE_MPEG4 2
#define TYPE_AVS 4
#define TYPE_AUTO 0

// CVideoStreamDecoderDlg dialog
class CVideoStreamDecoderDlg : public CDialogEx
{
// Construction
public:
	CVideoStreamDecoderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIDEOSTREAMDECODER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit edit_info;
	afx_msg void OnBnClickedRadioH264();
	afx_msg void OnBnClickedRadioH263();
	afx_msg void OnBnClickedRadioMpeg4();
	afx_msg void OnBnClickedRadioAvs();

	int stream_type;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CDecodeThread **dec_thread;
	unsigned int pre_dec_thread_num;//记录上次执行的线程数，以便销毁线程
	CButton radio_auto;
	afx_msg void OnBnClickedRadioAuto();
};
