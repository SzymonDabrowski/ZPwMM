#include <Windows.h>
#include <random>
#include <chrono>
#include <cstdio>
#include "res.h"

int SetNumber(); 
void Check(int &computer_number, int &number, char *message); // first argument not needed when global

//
// TODO: replace global with local variable
//
int g_computer_number; 

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
			{
				g_computer_number = SetNumber();
				return TRUE;
			}
			case IDC_CHECKBUTTON:
				HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDITBOX);
				HWND hwndStaticText = GetDlgItem(hwndDlg, IDC_STATIC_GAMESTATUS);
				int iTextLength = GetWindowTextLength(hwndEditBox);
				char users_number[3];
				char message[50];
				GetWindowText(hwndEditBox, users_number, iTextLength + 1);

				int number = atoi(users_number);
				Check(g_computer_number, number, message);

				SetWindowText(hwndStaticText, message);

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

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

int SetNumber()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution(1, 40);
	
	return distribution(generator);
}


//
// this function has to be rewritten
// TODO: 
//		- check if there are any letters, other signs or too much/less signs (atoi(), strlen()) 
//		- or number bigger than 40
//		- count iters!
//
void Check(int &computer_number, int &number, char *message)
{
	if (computer_number == number)
	{
		sprintf_s(message, 50, "Correct! Press Play/Resume to start restart.");
		return;
	}
	else if (computer_number > number)
	{
		sprintf_s(message, 50, "My number is greater. Try again!");
		return;
	}
	else
	{
		sprintf_s(message, 50, "My number is lesser. Try again!");
		return;
	}
}