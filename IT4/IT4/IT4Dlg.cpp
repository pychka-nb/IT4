
// IT4Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "IT4.h"
#include "IT4Dlg.h"
#include "afxdialogex.h"
#include <thread>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Pi 3.1415926535


// Диалоговое окно CIT4Dlg
bool killtimer = false; // флаг убийства таймера
int timer; // переменная для таймера
bool lflag = false; // флаг для масива лямбд

// функция потока
DWORD dwThread; HANDLE hThread;
DWORD WINAPI func1(PVOID pv)
{
	CIT4Dlg* p = (CIT4Dlg*)pv;
	p->MHJ(p->N, p->lambda);
	return 0;
}


CIT4Dlg::CIT4Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IT4_DIALOG, pParent)
	, A1(1)
	, A2(0.7)
	, A3(0.9)
	, Ah(1)
	, t01(1)
	, t02(3)
	, t03(4)
	, S1(0.25)
	, S2(0.25)
	, S3(0.25)
	, Sh(0.1)
	, N(50)
	, fd(10)
	,ya1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIT4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, A2);
	DDX_Text(pDX, IDC_EDIT3, A3);
	DDX_Text(pDX, IDC_EDIT5, ya1);
	DDX_Text(pDX, IDC_EDIT16, Ah);
	DDX_Text(pDX, IDC_EDIT10, t01);
	DDX_Text(pDX, IDC_EDIT11, t02);
	DDX_Text(pDX, IDC_EDIT12, t03);
	DDX_Text(pDX, IDC_EDIT13, S1);
	DDX_Text(pDX, IDC_EDIT14, S2);
	DDX_Text(pDX, IDC_EDIT15, S3);
	DDX_Text(pDX, IDC_EDIT20, Sh);
	DDX_Text(pDX, IDC_EDIT17, N);
	DDX_Text(pDX, IDC_EDIT18, fd);
}

BEGIN_MESSAGE_MAP(CIT4Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CIT4Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CIT4Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений CIT4Dlg

BOOL CIT4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	//CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)MyThread, NULL);
	graph1.Create(GetDlgItem(IDC_SIGNAL)->GetSafeHwnd());
	graph2.Create(GetDlgItem(IDC_IMP)->GetSafeHwnd());
	graph3.Create(GetDlgItem(IDC_OUTPUT)->GetSafeHwnd());
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CIT4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CIT4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*Ф-ция, вычисляющая предел графика*/
double CIT4Dlg::FindMin(vector <double> data, int N)
{
	double min = data[0];
	for (int i = 0; i < N; i++)
	{
		if (min > data[i])
			min = data[i];
	}
	return min;
}
double CIT4Dlg::FindMax(vector <double> data, int N)
{
	double max = data[0];
	for (int i = 0; i < N; i++)
	{
		if (max < data[i])
			max = data[i];
	}
	return max;
}
/*Ф-ция, генерирующая рандомное число*/
double Random()
{
	double a = 0;
	for (int i = 0; i < 12; i++)
		a += -1. + 2 * (double)rand() / RAND_MAX;
	return a;
}
/*Ф-ция, считающая сумму квадратов отсчётов сигнала*/
double Sum(vector <double> mas)
{
	double a = 0;
	for (int i = 0; i < mas.size(); i++)
		a += mas[i] * mas[i];
	return a;
}
/*Ф-ция, создающая полигармонический сигнал*/
void CIT4Dlg::CreateSignal()
{
	for (int i = 0; i < N; i++)
	{
		Sign.push_back(A1*exp(-pow(i/fd-t01,2)/(2*pow(S1,2)))+ A2 * exp(-pow(i / fd - t02, 2) / (2 * pow(S2, 2)))+ A3 * exp(-pow(i / fd - t03, 2) / (2 * pow(S3, 2))));
	}
	vector <double> n0;
	double Es;
	double En;
	double B;
	double Al = ya1 / 100;
	for (int i = 0; i < N; i++)
	{
		n0.push_back(Random());
	}
	Es = Sum(Sign);
	En = Sum(n0);
	B = sqrt(Al * Es / En);
	for (int i = 0; i < N; i++)
	{
		Sign[i] += B * n0[i];
	}
	n0.clear();
}
void CIT4Dlg::CreateSignalH()
{
	for (int i = 0; i < N; i++)
	{
		if (i < N / 2)
			SignH.push_back(Ah * exp(-pow(i / fd, 2) / (2 * pow(Sh, 2))));
		else SignH.push_back(Ah * exp(-pow((N - i - 1) / fd, 2) / (2 * pow(Sh, 2))));
	}
}
void CIT4Dlg::CreateSignalY()
{
	for (int i = 0; i < N; i++)
	{
		sum = 0;
		for (int k = 0; k < N; k++)
		{
			sum += SignH[abs(k-i)] * Sign[k];
		}
		SignY.push_back(sum);
		
	}
}
float CIT4Dlg::function(float* x)
{
	
	double func = 0;
	double sum1;
	//SignNew.clear();
	double sign_new[200];
	for (int m = 0; m < N; m++)
	{
		sum1 = 0;
		for (int j = 0; j < N; j++)
		{
			sum1 += SignH[abs(j - m)] * x[j];

		}
		//SignNew.push_back(exp(-1 - sum1));
		sign_new[m] = exp(-1 - sum1);
	}

	for (int m = 0; m < N; m++)
	{
		sum1 = 0;
		for (int j = 0; j < N; j++)
		{
			//sum1 += SignNew[j] * SignH[abs(m - j)];
			sum1 += sign_new[j] * SignH[abs(m - j)];
		}
		func += (SignY[m] - sum1) * (SignY[m] - sum1);
	}
	
	//func += (2 - x[0]) * (2 - x[0]);
	//	func += (3 - x[1]) * (3 - x[1]);
	//SignNew.clear();
	return func;
}
float CIT4Dlg::MHJ(int kk, float* x)
{
	// kk - количество параметров; x - массив параметров
	float  TAU = 1.e-15f; // Точность вычислений
	int i, j, bs, ps;
	float z, h, k, fi, fb;
	float* b = new float[kk];
	float* y = new float[kk];
	float* p = new float[kk];
	killtimer = false;

	h = 1.;
	//x[0] = 1.;
	for (i = 0; i < kk; i++) {
		//x[i] = (float)rand() / RAND_MAX; // Задается начальное приближение
		lambda1[i] = x[i];
	}
	k = h;
	for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
	fi = function(x);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	while (1)
	{
		//calc++; // Счетчик итераций. Можно игнорировать

		x[j] = y[j] + k;
		z = function(x);
		if (z >= fi) {
			x[j] = y[j] - k;
			z = function(x);
			if (z < fi)   y[j] = x[j];
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = function(x);

		if (j < kk - 1) { j++;  continue; }
		if (fi + 1e-15 >= fb) {
			if (ps == 1 && bs == 0) {
				for (i = 0; i < kk; i++) {
					p[i] = y[i] = x[i] = b[i];
				}
				z = function(x);
				bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
				continue;
			}
			k /= 10.;
			if (k < TAU) break;
			j = 0;
			continue; 
		}

		for (i = 0; i < kk; i++) {
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = function(x);
		fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;
	} //  end of while(1)

	for (i = 0; i < kk; i++) {
		x[i] = p[i];
		/*if (lambda1[i] == x[i]) {
			killtimer = TRUE;
		}*/
	}
	/*if (fb < 0.5) {
		killtimer = true;
	}*/
	delete b;
	delete y;
	delete p;
	//killtimer = TRUE;
	killtimer = true;
	return fb;
}
/*UINT CIT4Dlg::MyThread(LPVOID pParam) {
	timer1 = SetTimer(1, 1000, NULL);
	double fb1 = MHJ(N, lambda);
	Sleep(1000);
	KillTimer(timer1);

}*/
/*DWORD WINAPI CIT4Dlg::MyThread(PVOID p)
{
	timer1 = SetTimer(1, 1000, NULL);
	double fb1 = MHJ(N, lambda);
	Sleep(1000);
	KillTimer(timer1);
}*/
void CIT4Dlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	Sign.clear();
	SignH.clear();
	SignY.clear();
	keys.clear();
	SignNew.clear();
	srand(time(NULL));
	graph1.DrawW();
	graph2.DrawW();
	graph3.DrawW();
	UpdateData(TRUE);
	//lambda = new double[N];
	CreateSignal();
	CreateSignalH();
	CreateSignalY();
	/*
	double fb1 = MHJ(N, lambda);
	SignNew.clear();
	double sum2;
	for (int j = 0; j < N; j++)
	{
		sum2 = 0;
		for (int k = 0; k < N; k++)
		{
			sum2 += SignH[abs(k - j)] * lambda[k];

		}
		sum2 = exp(-1 - sum2);
		SignNew.push_back(sum2);

	}
	*/

	for (int i = 0; i < N; i++)
	{
		keys.push_back(i / fd);
	}

	double xmax = (N - 1) / fd,
		ymax = FindMax(Sign, N),
		ymin = FindMin(Sign, N);
	//graph1.DrawTwo(Sign, SignNew, 0, xmax, ymin, ymax, keys, keys);// была постройка начального сигнала
	graph1.DrawOne(Sign,0, xmax, ymin, ymax, keys);// была постройка начального сигнала

	xmax = (N - 1) / fd;
	ymax = FindMax(SignH, N),
		ymin = FindMin(SignH, N);
	graph2.DrawOne(SignH, 0, xmax, ymin, ymax, keys);

	xmax = (N - 1) / fd;
	ymax = FindMax(SignY, N),
		ymin = FindMin(SignY, N);
	graph3.DrawOne(SignY, 0, xmax, ymin, ymax, keys);

	//Sign.clear();
	//SignH.clear();
	//SignY.clear();
	//keys.clear();
	//SignNew.clear();
	
}

void CIT4Dlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	srand(time(NULL));
	//UpdateData();
	SignNew.clear();
    
	for (int i = 0; i < N; i++)
	{
		lambda[i]=(double)rand() / RAND_MAX;
	}

	timer = SetTimer(1, 100, NULL);

	hThread = CreateThread(NULL, 0, func1, this, 0, &dwThread);

}

void CIT4Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	//graph1.DrawW();
	//MessageBeep(0xFFFFFFFF);
	keys.clear();
	SignNew.clear();
	double sum2;
	for (int j = 0; j < N; j++)
	{
		sum2 = 0;
		for (int k = 0; k < N; k++)
		{
			sum2 += SignH[abs(k - j)] * lambda[k];

		}
		sum2 = exp(-1 - sum2);
		SignNew.push_back(sum2);

	}
	for (int i = 0; i < N; i++)
	{
		keys.push_back(i / fd);
	}

	double xmax = (N - 1) / fd,
		ymax = FindMax(Sign, N),
		ymin = FindMin(Sign, N);
	//graph1.DrawW();
	graph1.DrawTwo(Sign, SignNew, 0, xmax, ymin, ymax, keys, keys);// была постройка начального сигнала
	


	UpdateData(FALSE);
	if (killtimer)
	{
		KillTimer(timer);
	}
	CDialogEx::OnTimer(nIDEvent);
}