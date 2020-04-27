#pragma once

#include "RecordPlay.h"	//���޸ĵĲ���ȫ���˺���룬�ָ�֮ǰ��BUCVֻ�轫��ͷ�ļ�ע�͵�

// CPannelRecordPlay �Ի���

class CPannelRecordPlay : public CDialogEx
{
	DECLARE_DYNAMIC(CPannelRecordPlay)

public:
	CPannelRecordPlay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPannelRecordPlay();

	virtual void OnFinalRelease();

// �Ի�������
	enum { IDD = IDD_PANNEL_RECORDPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnBnClickedRpOpen();
	afx_msg void OnBnClickedRpPlay();
	afx_msg void OnBnClickedRpClose();
	afx_msg void OnBnClickedRpSlow();
	afx_msg void OnBnClickedRpFast();
private:
public:
	BVCU_RecordPlay_Handler GetHandler();
	void SetHandler(BVCU_RecordPlay_Handler handler);
	afx_msg void OnBnClickedRpJumpto();
	afx_msg void OnBnClickedRpVoldown();
	afx_msg void OnBnClickedRpVolup();
	afx_msg void OnBnClickedRpStepforward();
};
static void gps_OnData(BVCU_RecordPlay_Handler hHandler, BVCU_RecordPlay_CallbackParam* pParam);