#include <Windows.h>
#include "res.h"

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
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED: // Works. Commented, because not needed at the moment.
			/*switch (LOWORD(wParam))
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
			*/
			return TRUE;
		case EN_CHANGE: // STACK OVERFLOW. CHECK IF CHANGED. IF SO - CHANGE. IF NOT - MISS INSTRUCTIONS.
			HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
			int iTextLength = GetWindowTextLength(hwndEditBox);
			CHAR szText[50];
			GetWindowText(hwndEditBox, szText, iTextLength + 1);

			// ERROR WHILE USING THIS FUNCTION
			SetWindowText(HWND(lParam), szText);

			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
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