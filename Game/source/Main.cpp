#include <Windows.h>
#include <stdio.h>
#include "res.h"

void Play(); // set parameters

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		PostQuitMessage(0);
		return TRUE;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_PLAYBUTTON:
				//
				// Create YES and NO buttons
				// or
				// IDC_PLAYBUTTON visible = false
				// IDC_PLAYBUTTON disable = true (needed?)
				// IDC_YESBUTTON visible = true (or create new button in function)
				// IDC_NOBUTTON as YESBUTTON
				//
				return TRUE;
			case IDC_YESBUTTON:
				//
				// can it be in other function?
				//
				return TRUE;
			case IDC_NOBUTTON:
				//
				// can it be in other function?
				//
				return TRUE;
			}
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

	//
	// handlers for buttons needed?
	//


	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void Play()
{
	return;
}