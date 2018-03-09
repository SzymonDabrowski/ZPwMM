#define _CRT_SECURE_NO_WARNINGS // deletes warning about sprintf - Visual wants me to use sprintf_s instead

#include <Windows.h>
#include <stdio.h>

void startGame();
void showMsgBox(char* appMsg, char* appName, int & min, int & max);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    int iRet = MessageBox(0, "Do you want to play?", "Start Window", MB_YESNO);

    while (iRet == IDYES)
    {
        startGame();
        iRet = MessageBox(0, "New game?", "Start Window", MB_YESNO);
    }

    iRet = MessageBox(0, "Thank you, bye!", "Exit Window", MB_OK);

    return 0;
}

void startGame()
{
    char appMsg[50] = "Choose number from 0 to 40";
    char appName[5] = "Game";
    int min = 0;
    int max = 40;

    MessageBox(0, appMsg, appName, MB_OK);
    showMsgBox(appMsg, appName, min, max);

    return;
}

void showMsgBox(char* appMsg, char* appName, int & min, int & max)
{
    int middle = 0;

    while (min < max)
    {
        middle = (min + max) / 2;
        sprintf(appMsg, "Is your number bigger than %d", middle);
        int iRetKey = MessageBox(0, appMsg, appName, MB_YESNO);

        if (iRetKey == IDYES)
            min = middle + 1;
        else
            max = middle;
    }

    sprintf(appMsg, "Your choice was %d", min);
    MessageBox(0, appMsg, appName, MB_OK);
    return;
}
