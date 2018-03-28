//=====2018 Szymon D¹browski========//
//=====Simple game - hit the fly.===//
//=====VI semester project==========//

#include <Windows.h>
#include <windowsx.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "res.h"


//TODO(deGrasso):
//		- refreshing
//		- complete the class and functions
//		- second image (fly size) - b&w, as a mask (fly form) - irl we use 1 file
//		- put class in another file
//		- make public and private sections in class


#define ID_TIMER_FLY 2137
#define ID_TIMER_BRAIN 2138
#define ID_TIMER_REBORN 2139

#define FLY_COUNT 10
#define PI 3.14159


HINSTANCE hInst;

int iWidth, iHeight;
double dt = 0.15; // to timer - 0.1s; timer of move
double dChangeDirection = 0.3;// to timer - 0.5s; timer of direction change
double dTimeToReborn = 10;


class sFly
{
public:
	int m_iFlyXPos;
	int m_iFlyYPos;
	int m_iFlyWidth;
	int m_iFlyHeight;
	int m_iFlyAngle; 
	int m_iFlySpeed;
	// use variables below when you want to move flies by vector (x,y) not (angle,speed)
	//int m_iFlySpeedX;
	//int m_iFlySpeedY;

	bool m_bIsAlive;

	sFly() 
		: m_bIsAlive(TRUE),
		m_iFlyHeight(100),
		m_iFlyWidth(100)
	{
		this->m_iFlyXPos = rand() % iWidth; //"this->" not needed but more clearly
		this->m_iFlyYPos = rand() % iHeight; 

		srand(time(NULL));
		
		this->m_iFlyAngle = rand() % 100;
		this->m_iFlySpeed = 35;
	}

	bool isAlive(int iMouseX, int iMouseY);
	void refresh(HDC hDC);
	void display(HDC &hDC, HDC &hDCBitmap, HBITMAP &hBitmap_alive, HBITMAP &hBitmap_dead);
	void move();
	void brain();
	void reborn();
	//~sFly() {};
};

bool sFly::isAlive(int iMouseX, int iMouseY)
{
	if (iMouseX > this->m_iFlyXPos && iMouseX < (this->m_iFlyXPos + this->m_iFlyWidth) && 
		iMouseY > this->m_iFlyYPos && iMouseY < (this->m_iFlyYPos + this->m_iFlyHeight))
		
		return this->m_bIsAlive = FALSE;
}

void sFly::refresh(HDC hDC)
{
	Rectangle(	hDC,
				this->m_iFlyXPos - this->m_iFlySpeed,
				this->m_iFlyYPos - this->m_iFlySpeed,
				this->m_iFlyXPos + this->m_iFlyWidth + this->m_iFlySpeed,
				this->m_iFlyYPos + this->m_iFlyHeight + this->m_iFlySpeed);
	Rectangle(	hDC, 0, 0, iWidth, iHeight);

	return;
}

void sFly::display(HDC &hDC, HDC &hDCBitmap, HBITMAP &hBitmap_alive, HBITMAP &hBitmap_dead)
{
	if (this->m_bIsAlive)
		SelectObject(hDCBitmap, hBitmap_alive);
	else
		SelectObject(hDCBitmap, hBitmap_dead);
	
	BitBlt(hDC, this->m_iFlyXPos, this->m_iFlyYPos, 100, 100, hDCBitmap, 0, 0, SRCCOPY);

	return;
}


/// <summary>
/// Makes flies move in the window.This is not changing their directions - vois sFly::brain() does.
/// </summary>
void sFly::move()
{
	if (this->m_bIsAlive)
	{
		this->m_iFlyXPos += this->m_iFlySpeed * cos(this->m_iFlyAngle*PI / 180)*dt;
		this->m_iFlyYPos += this->m_iFlySpeed * sin(this->m_iFlyAngle*PI / 180)*dt;


		if (this->m_iFlyXPos > iWidth - this->m_iFlyWidth)
			this->m_iFlyAngle = (this->m_iFlyAngle + 180) % 360; // add: iFlyXPos = iFlyWidth -i...; - muchy uciekaja za ekran
		if (this->m_iFlyXPos < 0)
			this->m_iFlyAngle = (this->m_iFlyAngle + 180) % 360;
		if (this->m_iFlyYPos > iHeight - this->m_iFlyHeight)
			this->m_iFlyAngle = (this->m_iFlyAngle + 180) % 360;
		if (this->m_iFlyYPos < 0)
			this->m_iFlyAngle = (this->m_iFlyAngle + 180) % 360;
	}
	return;
}

/// <summary>
/// This function changes the angle of movement (m_iFlyAngle) of sFly object.
/// </summary>
void sFly::brain()
{
	this->m_iFlyAngle += rand() % 17 - 5;

	return;
}

void sFly::reborn()
{
	if (this->m_bIsAlive == FALSE)
		this->m_bIsAlive = TRUE;

	return;
}

sFly *flies; //m_iFlyXPos and YPos must be hard coded while constructor is called as global object!


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		iWidth = abs(rect.right - rect.left) - 100;
		iHeight = abs(rect.bottom - rect.top) - 100;

		flies = new sFly[FLY_COUNT]; //std::vector instead - no problems with destructor

		return TRUE;
	}
	case WM_CLOSE:
	{
		delete[]flies; // remind
		DestroyWindow(hwndDlg);
		PostQuitMessage(0);

		return TRUE;
	}
	case WM_LBUTTONDOWN:
	{
		int iMouseX = GET_X_LPARAM(lParam);
		int iMouseY = GET_Y_LPARAM(lParam);

		for (int i = 0; i < 10; ++i)
			flies[i].isAlive(iMouseX, iMouseY);

		RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);

		return TRUE;
	}
	case WM_PAINT:// before WM_COMMAND!
	{
		HDC hDC; // uchwyt do kontekstu urz¹dzenia
		hDC = GetDC(hwndDlg);

		SelectObject(hDC, GetStockObject(WHITE_BRUSH));
		SelectObject(hDC, GetStockObject(NULL_PEN)); //without frame

		for (int i = 0; i < 10; ++i)
			flies[i].refresh(hDC);

		HBITMAP hBitmap_alive, hBitmap_dead; // z zasobów
		hBitmap_alive = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FLY));
		hBitmap_dead = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEADFLY));
		//hBitmap_alive = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_dead = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\!mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		HDC hDCBitmap = CreateCompatibleDC(hDC);

		for (int i = 0; i < 10; ++i)
			flies[i].display(hDC, hDCBitmap, hBitmap_alive, hBitmap_dead);

		DeleteObject(hBitmap_dead);
		DeleteObject(hBitmap_alive);

		DeleteDC(hDCBitmap);
		ReleaseDC(hwndDlg, hDC);

		return FALSE;
	}
	//case WM_SIZING: - during
	case WM_SIZE:
	{
		iWidth = LOWORD(lParam);
		iHeight = HIWORD(lParam);

		return FALSE;
	}
	case WM_TIMER:
	{
		switch (wParam)
		{
		case ID_TIMER_FLY:
		{
			for (int i = 0; i < 10; ++i)
				flies[i].move();

			RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);

			return TRUE;
		}
		case ID_TIMER_BRAIN:
		{
			for (int i = 0; i < 10; ++i)
				flies[i].brain();
			return TRUE;
		}
		case ID_TIMER_REBORN:
		{
			for (int i = 0; i < 10; ++i)
				flies[i].reborn();
			return TRUE;
		}
		}
	}
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;

	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	
	//refresh flies
	SetTimer(hwndMainWindow, ID_TIMER_FLY, dt*10, NULL);
	//change direction
	SetTimer(hwndMainWindow, ID_TIMER_BRAIN, dChangeDirection*150, NULL);
	//make flies alive
	SetTimer(hwndMainWindow, ID_TIMER_REBORN, dTimeToReborn * 1000, NULL);

	//background colour
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	SetClassLongPtr(hwndMainWindow,GCLP_HBRBACKGROUND,(LONG)brush);

	ShowWindow(hwndMainWindow, iCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}