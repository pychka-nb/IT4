
// IT4Dlg.h: файл заголовка
//

#pragma once

#include <vector>
#include <iostream>
#include <time.h>
#include "Drawer.h"
typedef struct cmplx { float real; float image; };
// Диалоговое окно CIT4Dlg
class CIT4Dlg : public CDialogEx
{
// Создание
public:
	CIT4Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IT4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double A2;
	double A3;
	double Ah;
	double t01;
	double t02;
	double t03;
	double S1;
	double S2;
	double S3;
	double Sh;
	int N;
	double fd;
	double sum;
	double ya1;
	void CreateSignal();
	void CreateSignalH();
	void CreateSignalY();
	vector <double> Sign;
	vector <double> SignH;
	vector <double> SignY;
	vector <double> keys;
	vector <double> keysH;
	vector <double> keysY;
	vector <double> SignNew;
	//double *lambda;
	float lambda[200];
	float lambda1[200];
	//double lambda[1000];
	//double SignNew[1000];
	double FindMin(vector<double> data, int N);
	double FindMax(vector<double> data, int N);
	//UINT MyThread(LPVOID pParam);
	//LPVOID pParam;
	//DWORD  WINAPI MyThread(PVOID);
	//HANDLE t1;
	float function(float* x);
	//float MHJ();
	float MHJ(int kk, float* x);
	Drawer
		graph1,
		graph2,
		graph3;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//UINT timer1;
	
	afx_msg void OnBnClickedButton2();
};
