#include <Windows.h>
#include "res.h"

HINSTANCE hInst;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		PostQuitMessage(0);
		return TRUE;
	case WM_LBUTTONDOWN:
		return TRUE;
	// before WM_COMMAND!
	case WM_PAINT:
	{
		HDC hDC; // uchwyt do kontekstu urz¹dzenia
		hDC = GetDC(hwndDlg);

		HBITMAP hBitmap_alive, hBitmap_dead, hBitmap_background; // z zasobów
		hBitmap_alive = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_FLY));
		hBitmap_dead = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEADFLY));
		hBitmap_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
		//hBitmap_alive = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_dead = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\!mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_background = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		HDC hDCBitmap_alive, hDCBitmap_dead, hDCBitmap_background;
		hDCBitmap_alive = CreateCompatibleDC(hDC); // utworzenie kompatybilnego kontekstu
		hDCBitmap_dead = CreateCompatibleDC(hDC);
		hDCBitmap_background = CreateCompatibleDC(hDC);

		// needed while flies are moving
		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		int main_window_width = rect.right - rect.left;
		int main_window_height = rect.bottom - rect.top;

		SelectObject(hDCBitmap_background, hBitmap_background); //wybranie bitmapy z kontekstu
		StretchBlt(hDC, 0, 0, main_window_width, main_window_height, hDCBitmap_background, 0, 0, 1600, 900, SRCCOPY); // 1600,900 - size of desktop?
		DeleteDC(hDCBitmap_background);
		DeleteObject(hBitmap_background);

		SelectObject(hDCBitmap_alive, hBitmap_alive); //wybranie bitmapy z kontekstu
		BitBlt(hDC, 0, 0, 150, 100, hDCBitmap_alive, 0, 0, SRCCOPY); //skopiowanie obrazu
		DeleteDC(hDCBitmap_alive);
		DeleteObject(hBitmap_alive);

		SelectObject(hDCBitmap_dead, hBitmap_dead); //wybranie bitmapy z kontekstu
		BitBlt(hDC, 150, 0, 150, 100, hDCBitmap_dead, 0, 0, SRCCOPY); //skopiowanie obrazu
		DeleteDC(hDCBitmap_dead);
		DeleteObject(hBitmap_dead);

		SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
		SetTextColor(hDC, RGB(150, 40, 50));

		//char text_output[] = "Some Flies.";
		//int text_size = (int)strlen(text_output);
		//
		//TextOut(hDC, 100, 10, text_output, text_size);

		ReleaseDC(hwndDlg, hDC);
		break;
	}
	case WM_SIZING:
	{
		HDC hDC; // uchwyt do kontekstu urz¹dzenia
		hDC = GetDC(hwndDlg);

		HDC hDCBitmap_alive, hDCBitmap_dead, hDCBitmap_background;
		hDCBitmap_alive = CreateCompatibleDC(hDC); // utworzenie kompatybilnego kontekstu
		hDCBitmap_dead = CreateCompatibleDC(hDC);
		hDCBitmap_background = CreateCompatibleDC(hDC);

		HBITMAP hBitmap_alive, hBitmap_dead, hBitmap_background; // z zasobów
		hBitmap_alive = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FLY));
		hBitmap_dead = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEADFLY));
		hBitmap_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
		//hBitmap_alive = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_dead = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\!mucha.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_background = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\HitTheFly\\source\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		int main_window_width = rect.right - rect.left;
		int main_window_height = rect.bottom - rect.top;

		SelectObject(hDCBitmap_background, hBitmap_background); //wybranie bitmapy z kontekstu
		StretchBlt(hDC, 0, 0, main_window_width, main_window_height, hDCBitmap_background, 0, 0, 1600, 900, SRCCOPY); // 1600,900 - size of desktop?
		DeleteDC(hDCBitmap_background);
		DeleteObject(hBitmap_background);

		SelectObject(hDCBitmap_alive, hBitmap_alive); //wybranie bitmapy z kontekstu
		BitBlt(hDC, 0, 0, 150, 100, hDCBitmap_alive, 0, 0, SRCCOPY); //skopiowanie obrazu
		DeleteDC(hDCBitmap_alive);
		DeleteObject(hBitmap_alive);

		SelectObject(hDCBitmap_dead, hBitmap_dead); //wybranie bitmapy z kontekstu
		BitBlt(hDC, 150, 0, 150, 100, hDCBitmap_dead, 0, 0, SRCCOPY); //skopiowanie obrazu
		DeleteDC(hDCBitmap_dead);
		DeleteObject(hBitmap_dead);

		SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
		SetTextColor(hDC, RGB(150, 40, 50));

		//char text_output[] = "Some Flies.";
		//int text_size = (int)strlen(text_output);
		//
		//TextOut(hDC, 100, 10, text_output, text_size);

		ReleaseDC(hwndDlg, hDC);
		break;
	}
	//WM_SIZE - ended
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED: 
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1:
				HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
				int iTextLength = GetWindowTextLength(hwndEditBox);
				CHAR szText[50];
				GetWindowText(hwndEditBox, szText, iTextLength + 1);

				SetWindowText((HWND)lParam, szText);

				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;

	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	ShowWindow(hwndMainWindow, iCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}