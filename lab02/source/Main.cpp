#include <Windows.h>
#include <GL\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "res.h"
#include "Main.h"
#include <math.h>
#include <stdio.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")

//glew na stronie na pi¹tek

//TODO(deGrasso)
//kontekst graficzny (wykorzystuje GPU) - CreateOpenGlContext
//przejmuje to obs³ugê komunikatu WM_PAINT - nie ma go w naszym programie
//
//rzutowanie perspektywiczne (równanie soczewkowe, podobieñsto trójk¹tów)
//	dla dwóch sk³adowych
//
//przes³anianie! 
//bufor g³êbi (zbufer, ztest)
//
//figury - prymitywy
//idea figur geometrycznych
//punkt ma domyœlnie 1 punk obrazu (pixel)
//glPoint(x,y,z,kolor)
//
//rasteryzacja - zamiana geometrii na grafikê
//
//tekstury
//ka¿dy wierzcho³ek ma wspó³rzêdne tekstury
//texture mapping
//
//model œwiat³a
//zaokr¹glanie obiektów - pochylanie wektora normalnego
//
//==============================================================================
//
//przekszta³cenia 
//translacja - np. przesuwanie sztachet,zamiast implementowania ka¿dej z nich
//rotacja
//skalowanie
//...
//"szczebelek. To siê jakoœ ³adnie nazywa. Sztacheta! Element strukturalny p³otu.
//pamiêtaj: macierze i przestrzeñ homogeniczna
//
//podwójne buforowanie!
//backbuffer, w którym rysujemy
//nastêpnie zamieniamy backbuffer z ekranem g³ównym 
//ekran g³ówny staje siê backbufferem
//itd. (swapbuffers() na samym koñcu)
//
//jest te¿ potrójne buforowanie
//
//==============================================================================
//
//problem skali
//liczby ³atwe do zapisania (10,20), intuicyjne (10 = 10mm)

HINSTANCE hInst;
int iStartTime;
//handlers (to PrepareTextures);
GLuint iTextureEyes;
GLuint iTextureBody;
GLuint iTextureLegs;
GLuint iTextureWings;

void Axes(float a)
{
	glBegin(GL_LINES);

	//static float fangle = 0.0;
	//fangle += 0.01;

	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(+a, 0, 0);
	glVertex3f(-a, 0, 0);

	glColor3f(50.0, 10.0, 0.0);
	glVertex3f(0, +a, 0);
	glVertex3f(0, -a, 0);

	glColor3f(10.0, 50.0, 20.0);
	glVertex3f(0, 0, +a);
	glVertex3f(0, 0, -a);

	glEnd();
}

void Box(float a, float c, float b)
{
	glBegin(GL_QUADS);

	//glColor3f(0.50, 0.99, 0.99);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-a / 2, -c / 2, -b / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(+a / 2, -c / 2, -b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(+a / 2, +c / 2, -b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, +c / 2, -b / 2);

	//glColor3f(0.95, 0.30, 0.95);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(+a / 2, +c / 2, -b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(+a / 2, +c / 2, +b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(+a / 2, -c / 2, +b / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(+a / 2, -c / 2, -b / 2);

	//glColor3f(0.91, 0.91, 0.10);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, +c / 2, -b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, +c / 2, +b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(+a / 2, +c / 2, +b / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(+a / 2, +c / 2, -b / 2);

	//glColor3f(0.87, 0.87, 0.87);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -c / 2, +b / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(+a / 2, -c / 2, +b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(+a / 2, +c / 2, +b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, +c / 2, +b / 2);

	//glColor3f(0.83, 0.10, 0.83);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, +c / 2, -b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-a / 2, +c / 2, +b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -c / 2, +b / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -c / 2, -b / 2);

	//glColor3f(0.68, 0.79, 0.69);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -c / 2, -b / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -c / 2, +b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(+a / 2, -c / 2, +b / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(+a / 2, -c / 2, -b / 2);

	glEnd();
}

void DrawFly(float fSize)
{
	glPushMatrix();
	glScalef(fSize, fSize, fSize);

	//zamiast ustawienia koloru, ustaw teksturê
	glColor3f(0.2, 0.15, 0.1);
	Box(1.5, 1, 2);

	glColor3f(0.3, 0.18, 0.13);
	glPushMatrix();
	glTranslatef(-1.5 / 2 + 1.5 / 3 / 2,
				-1.0 / 2 - 1.0 / 3 / 2,
				-2.0 / 2 + 2.0 / 5 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
	//zmieniliœmy ju¿ nasz uk³ad wspó³rzêdnych 
	//przywracamy go
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
				-1.0 / 2 - 1.0 / 3 / 2,
				-2.0 / 2 + 2.0 / 5 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5 / 2 + 1.5 / 3 / 2,
		-1.0 / 2 - 1.0 / 3 / 2,
		+2.0 / 2 - 2.0 / 5 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
								
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
		-1.0 / 2 - 1.0 / 3 / 2,
		+2.0 / 2 - 2.0 / 5 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
	glPopMatrix();


	//MIDDLE LEGS
	glPushMatrix();
	glTranslatef(-1.5 / 2 + 1.5 / 3 / 2,
		-1.0 / 2 - 1.0 / 3 / 2,
		0);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
								
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+1.5 / 2 - 1.5 / 3 / 2,
		-1.0 / 2 - 1.0 / 3 / 2,
		0);// przesuñ o pó³ w ka¿dym wymiarze
	Box(2.0 / 5, 1.5 / 3, 1.0 / 3); //nó¿ka
	glPopMatrix();


	//EYES HERE!
	glColor3f(0.9, 0.9, 0.9);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, iTextureEyes);
	glPushMatrix();
	glTranslatef(-1.5 / 2,
				+2.0 / 2,
				+2.0 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(1.0, 1.0, 1.0); //oko
									
	glPopMatrix();

	glPushMatrix();
	glTranslatef(+1.5 / 2,
				+2.0 / 2,
				+2.0 / 2);// przesuñ o pó³ w ka¿dym wymiarze
	Box(1.0, 1.0, 1.0); //oko
	glPopMatrix();

	//WINGS HERE
	float fAngleFly = 0.0;

	fAngleFly = 10.0 * sin((GetTickCount() - iStartTime) * 2 * 3 * 3.1415926 / 1000);

	glColor3f(0.84, 0.84, 0.84);
	glPushMatrix();
	glTranslatef(-1.5 / 2 + 1.5, +2.0 / 2 - 0.4, - 1.0);
	glRotatef(30.0 + fAngleFly, 0.0, 1.0, 0.0);
	glRotatef(5.0 + fAngleFly/5.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, 1.0);
	Box(2.5, 1.0/10, 1.3); //skrzyd³o

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5 / 2, +2.0 / 2 - 0.4, -1.0);
	glRotatef(-30.0 - fAngleFly, 0.0, 1.0, 0.0);
	glRotatef(-5.0 - fAngleFly / 5.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, 1.0);
	Box(2.5, 1.0/10, 1.3); //skrzyd³o
	glPopMatrix();

}

unsigned char* ReadBmpFromFile(const char* szFileName, int &riWidth, int &riHeight)
{
	BITMAPFILEHEADER     bfh;
	BITMAPINFOHEADER     bih;

	int                i, j, h, v, lev, l, ls;
	unsigned char*     buff = NULL;

	unsigned char* p_palette = NULL;
	unsigned short n_colors = 0;

	unsigned char* pRGBBuffer = NULL;

	FILE* hfile;
	fopen_s(&hfile, szFileName, "rb");

	if (hfile != NULL)
	{
		fread(&bfh, sizeof(bfh), 1, hfile);
		if (!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))))
		{
			fread(&bih, sizeof(bih), 1, hfile);
			v = bih.biWidth;
			h = bih.biHeight;
			lev = bih.biBitCount;

			riWidth = v;
			riHeight = h;
			pRGBBuffer = new unsigned char[riWidth*riHeight * 3]; //Zaalokowanie odpowiedniego bufora obrazu

																  //Za³aduj Palete barw jesli jest
			if ((lev == 1) || (lev == 4) || (lev == 8))
			{
				n_colors = 1 << lev;
				p_palette = new unsigned char[4 * n_colors];
				fread(p_palette, 4 * n_colors, 1, hfile);
			}

			fseek(hfile, bfh.bfOffBits, SEEK_SET);

			buff = new unsigned char[v * 4];

			switch (lev)
			{
			case 1:
				//Nie obs³ugiwane
				break;
			case 4:
				//nie Obs³ugiwane
				break;
			case 8: //Skala szaroœci
				ls = (v + 3) & 0xFFFFFFFC;
				for (j = (h - 1); j >= 0; j--)
				{
					fread(buff, ls, 1, hfile);
					for (i = 0, l = 0; i < v; i++)
					{
						pRGBBuffer[((j*riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
						pRGBBuffer[((j*riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
						pRGBBuffer[((j*riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
					}
				};
				break;
			case 24:
				//bitmapa RGB
				ls = (v * 3 + 3) & 0xFFFFFFFC;
				for (j = (h - 1); j >= 0; j--)
				{
					//x_fread(hfile,buff,ls);
					fread(buff, ls, 1, hfile);
					for (i = 0, l = 0; i < v; i++, l += 3)
					{
						pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 0];
						pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
						pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 2];
					};
				};
				break;
			case 32:
				// RGBA bitmap 
				for (j = (h - 1); j >= 0; j--)
				{
					fread(buff, v * 4, 1, hfile);
					for (i = 0, l = 0; i < v; i++, l += 4)
					{
						pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 0];
						pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
						pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 2];
					}
				};
				break;
			};
			delete buff;
			if (p_palette) delete p_palette;

		}
	}
	return pRGBBuffer;
}

void PrepareTextures()
{
	glEnable(GL_TEXTURE_2D);

	//tworzenie tekstury
	glGenTextures(1, &iTextureBody);
	glGenTextures(1, &iTextureEyes);
	glGenTextures(1, &iTextureLegs);
	glGenTextures(1, &iTextureWings);

	int iWidth;
	int iHeight;
	//za³adowanie z pliku (rozmiar tekstury powinien byæ potêg¹ liczby 2)
	unsigned char* pBMPEye = ReadBmpFromFile("C:\\Users\\ASUS\\Desktop\\Eyes.bmp", iWidth, iHeight);
	glBindTexture(GL_TEXTURE_2D, iTextureEyes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBMPEye);
}

void DrawScene()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.97, 0.80, 0.92, 1);//R,G,B,A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(-1.0, -2.0, -10.0);

	static float fangle = 0.0;
	fangle += 0.01;

	glRotatef(fangle, 0, 1, 0);

	Axes(5.0);
	//Box(2, 2, 2);

	DrawFly(1.0);	//przesuniêto punkt rysowania 
					//trzeba przesun¹æ nogê o pó³ rozmiaru nogi
	//glTranslatef(-1.0, -2.0, -10.0);
	
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwndDlg);
		PostQuitMessage(0);
		return TRUE;
	}
	case WM_LBUTTONDOWN:
		return TRUE;
	case WM_PAINT:
	{
		//HDC hDC; // uchwyt do kontekstu urz¹dzenia
		//hDC = GetDC(hwndDlg);

		//HBITMAP hBitmap_background; // z zasobów
		//hBitmap_background = (HBITMAP) LoadImage(NULL, "D:\\repos\\ZPwMM\\lab02\\source\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		////hBitmap_background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND)); // z zasobów

		//HDC hDCBitmap_background;
		//hDCBitmap_background = CreateCompatibleDC(hDC);  // utworzenie kompatybilnego kontekstu

		//RECT rect;
		//GetWindowRect(hwndDlg, &rect);
		//int main_window_width = rect.right - rect.left;
		//int main_window_height = rect.bottom - rect.top;

		//SelectObject(hDCBitmap_background, hBitmap_background); //wybranie bitmapy z kontekstu
		//StretchBlt(hDC, 0, 0, main_window_width, main_window_height, hDCBitmap_background, 0, 0, 1600, 900, SRCCOPY); // 1600,900 - size of desktop?
		//DeleteDC(hDCBitmap_background);
		//DeleteObject(hBitmap_background);

		//SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
		//SetTextColor(hDC, RGB(150, 40, 50));

		//const char text_output[] = "TEXT.";
		//int text_size = (int)strlen(text_output);//sizeof(text_output) / sizeof(text_output[0]);

		//TextOut(hDC, 100, 10, text_output, text_size);
		//ReleaseDC(hwndDlg, hDC);
		break;
	}
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		glViewport(0, 0, width, height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, static_cast<double>(width / height), 0.1, 1000.0);	//k¹t widzenia kamery, aspect ratio, min i max odleg³oœæ,
																				//z której obserwujemy (renderujemy) obiekty

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		return TRUE;
	}
	case WM_SIZING:
	{
		break;
	}
	//WM_SIZE - ended
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	iStartTime = GetTickCount64();

	hInst = hInstance;

	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	
	//init openGL context
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW || PFD_SUPPORT_OPENGL || PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16, //16 or 24 bits
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0, //alpha channel
		16,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	HDC hDC = GetDC(hwndMainWindow);
	GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, PixelFormat, &pfd);

	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hwndMainWindow, iCmdShow);

	PrepareTextures();

	MSG msg = {};
	bool done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	//obs³u¿ komunikat
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	//rysuj
			DrawScene();
			SwapBuffers(hDC);
		}
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwndMainWindow, hDC);

	return 0;
}