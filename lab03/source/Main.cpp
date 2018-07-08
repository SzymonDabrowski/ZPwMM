#include <Windows.h>
#include <random>
#include <chrono>
#include <cstdio>
#include "res.h"

#pragma comment(lib,"winmm.lib")

#define PI 3.1415926

int SetNumber(); 
void Check(int &computer_number, int &number, char *message); // first argument not needed when global

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

	// may be in button as well
	WAVEFORMATEX pcmWaveFormat;
	pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	pcmWaveFormat.nChannels = 1;
	pcmWaveFormat.nSamplesPerSec = 44100L;
	pcmWaveFormat.wBitsPerSample = 8;
	pcmWaveFormat.nAvgBytesPerSec = 44100L;
	pcmWaveFormat.nBlockAlign = 1;
	pcmWaveFormat.cbSize = 0;

	MMRESULT mmResult;
	HWAVEOUT hwo = 0;
	UINT devID;

	for (devID = 0; devID < waveOutGetNumDevs(); devID++)
	{
		mmResult = waveOutOpen(&hwo, devID, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
		if (mmResult == MMSYSERR_NOERROR)
			break;
	}
	if (mmResult != MMSYSERR_NOERROR)
	{
		MessageBox(hwndMainWindow, "ERROR", TEXT("Nie znalaziono karty dzwiêkwej"), MB_OK);
		return mmResult;
	}

	WAVEHDR whdr;
	ZeroMemory(&whdr, sizeof(WAVEHDR));
	whdr.lpData = new char[pcmWaveFormat.nAvgBytesPerSec * 1];
	whdr.dwBufferLength = pcmWaveFormat.nAvgBytesPerSec * 1;
	whdr.dwUser = 0;
	whdr.dwFlags = 0;
	whdr.dwLoops = 0;
	whdr.dwBytesRecorded = 0;
	whdr.lpNext = 0;
	whdr.reserved = 0;

	for (int i = 0; i < whdr.dwBufferLength; ++i)
	{
		whdr.lpData[i] = (128 * sin((i * 880 * 2 * PI) / (double)pcmWaveFormat.nSamplesPerSec) + 128);
	}

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	waveOutSetVolume(hwo, 0xFFFFFFFF);
	mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
	if (mmResult != MMSYSERR_NOERROR)
	{
		MessageBox(hwndMainWindow, TEXT("Nie mo¿na zainicjowaæ karty"), TEXT("ERROR"), MB_OK);
		return mmResult;
	}

	mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
	if (mmResult != MMSYSERR_NOERROR)
	{
		MessageBox(hwndMainWindow, TEXT("Nie uda³o siê za³adowaæ próbek"), TEXT("ERROR"), MB_OK);
		return mmResult;
	}

	while ((whdr.dwFlags & WHDR_DONE) != WHDR_DONE)
	{
		Sleep(100);
	}

	mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
	mmResult = waveOutClose(hwo);
	delete[] whdr.lpData;

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