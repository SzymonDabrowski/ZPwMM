#include <Windows.h>
#include "res.h"

// TODO(deGrasso)
//		- odœwie¿anie kontrolek
//		- poprawa jakoœci t³a
//		- odœwie¿anie - jak usun¹æ mruganie?
//		- pêtla obs³ugi komunikatów - WM_PAINT, WM_SIZING -> zmienne globalne (rozmiar okna) oraz funkcje (mniej kodu)
//		- œcie¿ka dostêpu do LPSTR, wy³¹cz escape sequences
//		- œcie¿ka dostêpu poœrednia, nie ca³kowita

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
		CHAR szText[200];
		wsprintf(szText, "Mouse clicked in x = %d, y = %d", LOWORD(lParam), HIWORD(lParam));
		MessageBox(hwndDlg, szText, TEXT("Click Position"), MB_OK);
		return TRUE;
	// przed WM_COMMAND!
	case WM_PAINT:
	{
		HDC hDC; // uchwyt do kontekstu urz¹dzenia
		hDC = GetDC(hwndDlg);

		HBITMAP hBitmap_background; // z zasobów
		hBitmap_background = (HBITMAP) LoadImage(NULL, "D:\\repos\\ZPwMM\\lab02\\source\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND)); // z zasobów

		HDC hDCBitmap_background;
		hDCBitmap_background = CreateCompatibleDC(hDC);  // utworzenie kompatybilnego kontekstu

		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		int main_window_width = rect.right - rect.left;
		int main_window_height = rect.bottom - rect.top;

		SelectObject(hDCBitmap_background, hBitmap_background); //wybranie bitmapy z kontekstu
		StretchBlt(hDC, 0, 0, main_window_width, main_window_height, hDCBitmap_background, 0, 0, 1600, 900, SRCCOPY); // 1600,900 - size of desktop?
		DeleteDC(hDCBitmap_background);
		DeleteObject(hBitmap_background);

		SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
		SetTextColor(hDC, RGB(150, 40, 50));

		const char text_output[] = "TEXT.";
		int text_size = (int)strlen(text_output);//sizeof(text_output) / sizeof(text_output[0]);

		TextOut(hDC, 100, 10, text_output, text_size);
		ReleaseDC(hwndDlg, hDC);
		break;
	}
	case WM_SIZING:
	{
		HDC hDC; // uchwyt do kontekstu urz¹dzenia
		hDC = GetDC(hwndDlg);

		HDC hDCBitmap_background;
		hDCBitmap_background = CreateCompatibleDC(hDC);

		HBITMAP hBitmap_background; 
		hBitmap_background = (HBITMAP)LoadImage(NULL, "D:\\repos\\ZPwMM\\lab02\\source\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//hBitmap_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));

		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		int main_window_width = rect.right - rect.left;
		int main_window_height = rect.bottom - rect.top;

		SelectObject(hDCBitmap_background, hBitmap_background); //wybranie bitmapy z kontekstu
		StretchBlt(hDC, 0, 0, main_window_width, main_window_height, hDCBitmap_background, 0, 0, 1600, 900, SRCCOPY); // 1600,900 - size of desktop?
		DeleteDC(hDCBitmap_background);
		DeleteObject(hBitmap_background);

		SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
		SetTextColor(hDC, RGB(150, 40, 50));

		const char text_output[] = "TEXT.";
		int text_size = (int)strlen(text_output);//sizeof(text_output) / sizeof(text_output[0]);

		TextOut(hDC, 100, 10, text_output, text_size);
		ReleaseDC(hwndDlg, hDC);
		break;
	}
	//WM_SIZE - ended
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED: // Works. Commented, because not needed at the moment.
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1:
				MessageBox(hwndDlg, TEXT("Test"), TEXT("Clicked Test Button"), MB_OK);
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