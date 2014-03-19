
// VideoStreamDecoderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoStreamDecoder.h"
#include "VideoStreamDecoderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoStreamDecoderDlg dialog



CVideoStreamDecoderDlg::CVideoStreamDecoderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoStreamDecoderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoStreamDecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, edit_info);
	DDX_Control(pDX, IDC_RADIO_AUTO, radio_auto);
}

BEGIN_MESSAGE_MAP(CVideoStreamDecoderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CVideoStreamDecoderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVideoStreamDecoderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_H264, &CVideoStreamDecoderDlg::OnBnClickedRadioH264)
	ON_BN_CLICKED(IDC_RADIO_H263, &CVideoStreamDecoderDlg::OnBnClickedRadioH263)
	ON_BN_CLICKED(IDC_RADIO_MPEG4, &CVideoStreamDecoderDlg::OnBnClickedRadioMpeg4)
	ON_BN_CLICKED(IDC_RADIO_AVS, &CVideoStreamDecoderDlg::OnBnClickedRadioAvs)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO_AUTO, &CVideoStreamDecoderDlg::OnBnClickedRadioAuto)
END_MESSAGE_MAP()


// CVideoStreamDecoderDlg message handlers

BOOL CVideoStreamDecoderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	radio_auto.SetCheck(true);
	edit_info.SetWindowTextA("AUTO");
	stream_type = TYPE_AUTO;
	dec_thread = NULL;
	pre_dec_thread_num = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoStreamDecoderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoStreamDecoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoStreamDecoderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CVideoStreamDecoderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CVideoStreamDecoderDlg::OnBnClickedRadioH264()
{
	// TODO: Add your control notification handler code here
	edit_info.SetWindowTextA("H264");
	stream_type = TYPE_H264;
}


void CVideoStreamDecoderDlg::OnBnClickedRadioH263()
{
	// TODO: Add your control notification handler code here
	edit_info.SetWindowTextA("H263");
	stream_type = TYPE_H263;
}


void CVideoStreamDecoderDlg::OnBnClickedRadioMpeg4()
{
	// TODO: Add your control notification handler code here
	edit_info.SetWindowTextA("MPEG4");
	stream_type = TYPE_MPEG4;
}


void CVideoStreamDecoderDlg::OnBnClickedRadioAvs()
{
	// TODO: Add your control notification handler code here
	edit_info.SetWindowTextA("AVS");
	stream_type = TYPE_AVS;
}


void CVideoStreamDecoderDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	UINT count;
	char filePath[200];
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(count)
	{
		UINT i;
		if (NULL != dec_thread)
		{
			for (i=0; i<pre_dec_thread_num; ++i)
			{
				if (NULL != dec_thread[i])
				{
					dec_thread[i]->shutdown();
					dec_thread[i] = NULL;
				}
			}
			free(dec_thread);
			dec_thread = NULL;
		}
		dec_thread = (CDecodeThread **)calloc(count, sizeof(CDecodeThread *));
		pre_dec_thread_num = count;
		if (radio_auto.GetCheck())
		{
			stream_type = TYPE_AUTO;
		}
		for(i=0; i<count; ++i)
		{
			int pathLen = DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));//获取文件名列表
			if (TYPE_AUTO == stream_type)//根据后缀判断码流类型
			{
				CString temp = filePath;
				temp = temp.Right(temp.GetLength() - temp.ReverseFind('.'));
				if (".264" == temp)
				{
					stream_type = TYPE_H264;
				}
				else if (".263" == temp)
				{
					stream_type = TYPE_H263;
				}
				else if (".mpeg4" == temp || ".MPEG4" == temp)
				{
					stream_type = TYPE_MPEG4;
				}
				else if (".avs" == temp || ".AVS" == temp)
				{
					stream_type = TYPE_AVS;
				}
				else
				{
					AfxMessageBox("Unkown stream type.");
					edit_info.SetWindowTextA("Unkown");
					continue;
				}
				
			}
			dec_thread[i] = new CDecodeThread(stream_type, filePath);
			dec_thread[i]->start(this);
			if (radio_auto.GetCheck())
			{
				stream_type = TYPE_AUTO;
			}
		}

	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CVideoStreamDecoderDlg::OnBnClickedRadioAuto()
{
	// TODO: Add your control notification handler code here
	edit_info.SetWindowTextA("AUTO");
	stream_type = TYPE_AUTO;
}
