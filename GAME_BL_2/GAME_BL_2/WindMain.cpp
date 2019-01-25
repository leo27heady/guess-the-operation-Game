#pragma once
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "time.h"
#include "Class.h"
#include <fstream>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include <commctrl.h>
#pragma comment (lib,"comctl32.lib")

#define FileName "PersonalFile.txt" // файл где храняться данные пользователей
#define FORTIME "FORTIME.txt" // временный файл для перезаписи

using namespace std;

extern HINSTANCE hInst;
extern CHAR text_log[255];

HWND hDlgRules, hDlgAuthor, hDlgAboutProgram, hDlgEdit, hbutton1, hbutton2, hbutton3, hbutton_plus, hbutton_minus, hbutton_muliply, hbutton_divide,
hlist1, hStaticImage1, hStaticTime, hStaticNum, hStaticLife, hStatic_game_over, hStatic_game_pause, hbuttonPlay, hStaticPoint, hbuttonVolume_on, hbuttonVolume_off; // дескрипторы окошек

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Main"
BOOL CALLBACK DlgRules(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Settings"
BOOL CALLBACK DlgAuthor(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Author"
BOOL CALLBACK DlgAboutProgram(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Program"
BOOL CALLBACK DlgEdit(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Edit"

void game_ov(int x0, int y0, unsigned long long &points, int delta, int x, int yl, HWND hWnd);
void set_text_number(CHAR*, unsigned long long);
int point_in_word(CHAR*);
int numlen(int);
void list_highscore();
void strcpy(WCHAR * lf, const char * arr) { for (size_t i = 0; i < strlen(arr) + 1; i++) lf[i] = (WCHAR)arr[i]; } // функция д копирования из чар в вчар
int move_but = 25;
const int x = 215, y = 50, yl = 300, xb3 = 148, yb3 = 54; // размер кнопок "PLAY" и "HIGHSCORES"

int button;
BOOL PLUS = FALSE, MINUS = FALSE, MULTIPLY = FALSE, DIVIDE = FALSE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // функция обработки сообщений для основного окна "Main"
{
	static LOGFONT lf; // создаем текст(который будет сохраняться)
	lf.lfHeight = 35; // висота шрифта в логічних одиницях(за замовч. - 14);
	lf.lfWidth = 0; // ширина символів в логічних одиницях(за замовч. - 0);
	lf.lfWeight = FW_DONTCARE; // жирність
	strcpy((char *)lf.lfFaceName, "Dubai"); // шрифт
	lf.lfItalic = 0; // без нахилу(не курсив), ≠0 – з нахилом;
	lf.lfUnderline = 0; // без підкреслення, ≠0 – з підкресленням;

	static BOOL TIMER1 = FALSE, TIMER2 = FALSE, TIMER3 = FALSE, TIMER4 = FALSE, TIMER5 = FALSE, TIMER6 = FALSE, PLAY = FALSE;
	static int x0 = (1100 - x) / 2, y0 = (650 - y) / 2 - 200; // координаты начального местоположения кнопок на окне
	static int ix, ex, m, b_p;
	static int width = 0, height = 0; // ширина и высота окна
	const int delta = 70; // расстояние между кнопками
	RECT rect; // стр-ра, определяющая размер клиентской области

	static BOOL PAINT_SWAP1 = FALSE;

	HDC hCompDC, hdc; // хендли контекстів пристрою 
	HANDLE hBitmap;	  // хендли зображення
	BITMAP bitmap;    // для зберігання розмірів зображення
	RECT r;
	static CHAR TIME[6];
	static CHAR LIFES[4];
	static CHAR NUMBERS[255];
	static CHAR POINTS[255];
	static int t = 31;
	static int l = 3;
	static unsigned long long points = 0;
	static BOOL GAMEO = FALSE;
	static BOOL VOLUME = TRUE;


	
	static HWND hTB;
	static TBBUTTON but[2];
	
	but[0].idCommand = ID_HELP;
	but[0].iBitmap = STD_HELP;
	but[0].fsState = TBSTATE_ENABLED;
	but[0].fsStyle = TBSTYLE_BUTTON;

	but[1].idCommand = ID_ANSWER;
	but[1].iBitmap = STD_REPLACE;
	but[1].fsState = TBSTATE_ENABLED;
	but[1].fsStyle = TBSTYLE_BUTTON;

	

	switch (message)
	{
	case WM_DESTROY: // если нажали на крестик, то прогрмма прекращает роботу
		PostQuitMessage(0);
		break;
	case WM_CREATE: // выполняется при создании окна
	{
		
		hbutton1 = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, x0, y0, x, y, hWnd, (HMENU)BUTTON_PLAY, hInst, NULL); // создание кнопки "PLAY"
		HBITMAP image = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 215, 54, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image); // установка изображения

		hlist1 = CreateWindow("listbox", NULL, WS_CHILD, x0, y0 + delta, x, yl, hWnd, (HMENU)LIST_SCORES, hInst, NULL); // создание списка рекордов
		list_highscore();

		hStaticImage1 = CreateWindow("static", NULL, WS_CHILD | SS_BITMAP, width / 4, (3 * height) / 8, (width / 2), (height / 4), hWnd, (HMENU)STATIC_IMAGE1, hInst, NULL); // создание кнопки "PLAY"
		HBITMAP staticImage = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 600, 163, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hStaticImage1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)staticImage); // установка изображения

		hStatic_game_over = CreateWindow("static", NULL, WS_CHILD | SS_BITMAP, (width - width / 1.7) / 2, (height - height / 2.1) / 2, (width / 1.7), (height / 2.1), hWnd, (HMENU)STATIC_GAME_OVER, hInst, NULL); // создание кнопки "PLAY"
		HBITMAP staticGame_over = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 841, 541, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hStatic_game_over, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)staticGame_over); // установка изображения

		hStaticTime = CreateWindow("static", NULL, WS_CHILD | WS_BORDER | SS_CENTER | SS_CENTERIMAGE, (width / 2) - 50, (3 * height) / 14, 100, 34, hWnd, (HMENU)STATIC_NUMB, hInst, NULL); // создание кнопки "PLAY"
		hStaticNum = CreateWindow("static", NULL, WS_CHILD | SS_CENTER | SS_CENTERIMAGE, width / 4, (3 * height) / 8, (width / 2), (height / 4), hWnd, (HMENU)STATIC_NUM, hInst, NULL);
		hStaticLife = CreateWindow("static", NULL, WS_CHILD | SS_CENTER | SS_CENTERIMAGE, width / 4, (3 * height) / 9.9, (width / 2), (height / 16), hWnd, (HMENU)STATIC_NUM, hInst, NULL);
		hStaticPoint = CreateWindow("static", NULL, WS_CHILD | SS_CENTER | SS_CENTERIMAGE, (width / 2) - 100, (height / 7), 200, 36, hWnd, (HMENU)STATIC_POINT, hInst, NULL);

		hbutton2 = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, x0, y0 + delta, x, y, hWnd, (HMENU)BUTTON_SCORES, hInst, NULL); // создание кнопки "HIGHSCORES"
		HBITMAP image2 = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 215, 54, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image2); // установка изображения

		hbutton3 = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, x0, y0 + delta, xb3, yb3, hWnd, (HMENU)BUTTON_BACK, hInst, NULL); // создание кнопки "HIGHSCORES"
		HBITMAP image3 = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, xb3, yb3, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image3); // установка изображения

		hbuttonVolume_on = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, (width - 60) / 1.3, (height - 60) / 2, 40, 40, hWnd, (HMENU)VOLUME_ON, hInst, NULL); // создание кнопки "DIVIDE"
		HBITMAP image_v_on = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP15), IMAGE_BITMAP, 40, 40, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbuttonVolume_on, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_v_on); // установка изображения

		hbuttonVolume_off = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, (width - 60) / 1.3, (height - 60) / 2, 40, 40, hWnd, (HMENU)VOLUME_OFF, hInst, NULL); // создание кнопки "DIVIDE"
		HBITMAP image_v_off = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP16), IMAGE_BITMAP, 40, 40, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbuttonVolume_off, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_v_off); // установка изображения

		hbuttonPlay = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, (width - 60) / 2, (height - 60) / 2, 60, 60, hWnd, (HMENU)BUT_PL, hInst, NULL); // создание кнопки "DIVIDE"
		HBITMAP image_pl = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP13), IMAGE_BITMAP, 60, 60, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbuttonPlay, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_pl); // установка изображения

		hbutton_plus = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, width / 4, (3 * height) / 4, 50, 50, hWnd, (HMENU)BUTTON_PLUS, hInst, NULL); // создание кнопки "PLUS"
		HBITMAP image_plus = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 60, 60, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton_plus, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_plus); // установка изображения

		hbutton_minus = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, 0 + 85, 0, 50, 50, hWnd, (HMENU)BUTTON_MINUS, hInst, NULL); // создание кнопки "MINUS"
		HBITMAP image_minus = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP11), IMAGE_BITMAP, 60, 60, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton_minus, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_minus); // установка изображения

		hbutton_muliply = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, 0 + 170, 0, 50, 50, hWnd, (HMENU)BUTTON_MILTIPLY, hInst, NULL); // создание кнопки "MULTIPLY"
		HBITMAP image_muliply = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 60, 60, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton_muliply, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_muliply); // установка изображения

		hbutton_divide = CreateWindow("button", NULL, WS_CHILD | BS_BITMAP, 0 + 255, 0, 50, 50, hWnd, (HMENU)BUTTON_DIVIDE, hInst, NULL); // создание кнопки "DIVIDE"
		HBITMAP image_divide = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP12), IMAGE_BITMAP, 60, 60, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		SendMessage(hbutton_divide, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image_divide); // установка изображения

		
		hTB = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_DLGFRAME, ID_TOOLBAR, 0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, but, 2, 0, 0, 0, 0, sizeof(TBBUTTON));

		

		SendMessage(hWnd, WM_SIZE, NULL, NULL);

		break;
	}
	case WM_LBUTTONDOWN:
	{

		SetFocus(hWnd);


		break;
	}
	case WM_TIMER:
	{
		if (TIMER1)
		{
			if (y0 > height)
			{
				TIMER1 = FALSE;
				KillTimer(hWnd, 1);
				ShowWindow(hbutton1, SW_HIDE);
				ShowWindow(hlist1, SW_HIDE);
				ShowWindow(hbutton2, SW_HIDE);

				PLAY = TRUE;

				SendMessage(hWnd, WM_SIZE, NULL, NULL);
				MoveWindow(hbutton3, -x, 0, xb3, yb3, true); // двигаем кнопку к приблизительному центру
				ShowWindow(hbutton3, SW_SHOW);
				SetTimer(hWnd, 2, 1, NULL);
				TIMER2 = TRUE;
				ex = -x;
				break;
			}

			MoveWindow(hbutton1, x0, y0 += move_but, x, y, true); // двигаем кнопку вниз
			MoveWindow(hbutton2, x0, y0 + delta, x, y, true); // двигаем кнопку вниз
			MoveWindow(hlist1, x0, y0 + delta + y, x, yl, true); // двигаем кнопку к приблизительному центру
		}
		else if (TIMER2)
		{

			if (ex >= x0)
			{
				TIMER2 = FALSE;
				KillTimer(hWnd, 2);
				MoveWindow(hbutton3, ((width - xb3) * 0.1) / 2, ((height - yb3) * 0.1) / 2, xb3, yb3, true);
				TIMER4 = TRUE;
				SetTimer(hWnd, 4, 1000, NULL);
				SendMessage(hWnd, GAME, NULL, NULL);
				break;
			}
			MoveWindow(hbutton3, ex += move_but, ((height - yb3) * 0.1) / 2, xb3, yb3, true); // двигаем кнопку к приблизительному центру
		}
		else if (TIMER3)
		{
			if (!(ix + x < 0))
			{
				MoveWindow(hbutton3, ix -= move_but, ((height - yb3) * 0.1) / 2, xb3, yb3, true); // двигаем кнопку к приблизительному центру
			}
			else {
				if (m <= y0)
				{
					TIMER3 = FALSE;
					ShowWindow(hbutton3, SW_HIDE);

					MoveWindow(hbutton1, x0, y0, x, y, true); // двигаем кнопку вниз
					MoveWindow(hbutton2, x0, y0 + delta, x, y, true); // двигаем кнопку вниз
					MoveWindow(hlist1, x0, y0 + delta + y, x, yl, true); // двигаем кнопку к приблизительному центру

					KillTimer(hWnd, 3);
					break;
				}
				MoveWindow(hbutton1, x0, m -= move_but, x, y, true); // двигаем кнопку вниз
				MoveWindow(hbutton2, x0, m + delta, x, y, true); // двигаем кнопку вниз
				MoveWindow(hlist1, x0, m + delta + y, x, yl, true); // двигаем кнопку к приблизительному центру
			}
		}

		else if (TIMER4)
		{
			t--;

			if (t == -61)
			{
				TIMER4 = FALSE;

				KillTimer(hWnd, 4);
				SendMessage(hWnd, GAME_OVER, NULL, NULL);
				break;
			}
			ShowWindow(hStaticTime, SW_HIDE);
			if (t > 9) _stprintf(TIME, "01:%d", t);
			else if (t > -1) _stprintf(TIME, "01:0%d", t);
			else if (t > -51) _stprintf(TIME, "00:%d", t + 60);
			else _stprintf(TIME, "00:0%d", t + 60);
			SetWindowText(hStaticTime, TIME);
			ShowWindow(hStaticTime, SW_SHOW);
		}

		else if (TIMER5)
		{
			if (b_p > (height * 0.02) / 2)
			{
				TIMER5 = FALSE;
				KillTimer(hWnd, 5);
				break;
			}
			MoveWindow(hbuttonPlay, (width - 60) / 2, b_p += 6, 60, 60, true);
		}
		if (TIMER6)
		{

			if (b_p < -60)
			{
				TIMER6 = FALSE;
				KillTimer(hWnd, 6);
				TIMER4 = TRUE;
				SetTimer(hWnd, 4, 1000, NULL);
				ShowWindow(hbuttonPlay, SW_HIDE);
				ShowWindow(hStatic_game_over, SW_HIDE);
				ShowWindow(hStaticPoint, SW_HIDE);
				ShowWindow(hStatic_game_over, SW_HIDE);
				SendMessage(hWnd, GAME, NULL, NULL);
				break;
			}
			MoveWindow(hbuttonPlay, (width - 60) / 2, b_p -= 6, 60, 60, true);

		}

		break;
	}

	case WM_MOUSEMOVE:
	{

		static int yPos;

		yPos = HIWORD(lParam);

		if (yPos <= 32)
		{
		if(!GAMEO)ShowWindow(hTB, SW_SHOW);
		}
		else if(yPos <= 1) ShowWindow(hTB, SW_HIDE);
		else ShowWindow(hTB, SW_HIDE);

		break;
	}
	case WM_SIZE: // выполняется после изменения размера окна хотя бы на один пиксель
	{
		GetClientRect(hWnd, &rect); // получение координат окна
		width = rect.right - rect.left; // вычисляем ширину
		height = rect.bottom - rect.top; // вычисляем высоту

		MoveWindow(hTB, 0, 0, 0, 0, true);

		if (!PLAY)
		{
			if (!TIMER1)
			{
				x0 = (width - x) / 2;
				y0 = ((height - y) * 0.6) / 2;
			}

			MoveWindow(hbutton1, x0, y0, x, y, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hbutton2, x0, y0 + delta, x, y, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hlist1, x0, y0 + delta + y, x, yl, true); // двигаем кнопку к приблизительному центру
		}
		else
		{
			if (!TIMER2)
			{
				x0 = ((width - 148) * 0.1) / 2;
				y0 = ((height - 54) * 0.1) / 2;
			}
			MoveWindow(hbuttonPlay, (width - 60) / 2, (height * 0.02) / 2, 60, 60, true);
			MoveWindow(hbuttonVolume_on, (width - 60) / 1.063, (height * 0.1) / 2, 40, 40, true);
			MoveWindow(hbuttonVolume_off, (width - 60) / 1.063, (height * 0.1) / 2, 40, 40, true);
			MoveWindow(hbutton3, x0, y0, xb3, yb3, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hbutton_plus, (width / 2) - 152, (3 * height) / 4.45, 60, 60, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hbutton_minus, (width / 2) - 152 + 85, (3 * height) / 4.45, 60, 60, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hbutton_muliply, (width / 2) - 152 + 170, (3 * height) / 4.45, 60, 60, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hbutton_divide, (width / 2) - 152 + 255, (3 * height) / 4.45, 60, 60, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hStaticImage1, width / 4, (3 * height) / 8, (width / 2), (height / 4), true); // двигаем кнопку к приблизительному центру
			MoveWindow(hStaticTime, (width / 2) - 50, (3 * height) / 13, 100, 34, true); // двигаем кнопку к приблизительному центру
			MoveWindow(hStaticLife, width / 4, (3 * height) / 9.9, (width / 2), (height / 16), true); // двигаем кнопку к приблизительному центру
			MoveWindow(hStaticPoint, (width / 2) - 100, (height / 7), 200, 36, true);
			MoveWindow(hStaticNum, width / 4, (3 * height) / 8, (width / 2), (height / 4), true); // двигаем кнопку к приблизительному центру
			MoveWindow(hStatic_game_over, (width - width / 1.7) / 2, (height - height / 1.7) / 2, (width / 1.7), (height / 1.7), true);
		}

		break;
	}
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 'm':
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (PLAY)
				{
					if (VOLUME) SendMessage(hWnd, WM_COMMAND, VOLUME_ON, NULL);
					else SendMessage(hWnd, WM_COMMAND, VOLUME_OFF, NULL);
				}
			}

			break;
		}
		case 0x08: // BACKSPACE
		{

			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (PLAY) SendMessage(hWnd, WM_COMMAND, BUTTON_BACK, NULL);
			}

			break;
		}
		case 0x0D: // ENTER
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!PLAY) SendMessage(hWnd, WM_COMMAND, BUTTON_PLAY, NULL);
				else if(GAMEO) SendMessage(hWnd, WM_COMMAND, BUT_PL, NULL);
			}
			break;
		}
		case 49: // 1
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!GAMEO) SendMessage(hWnd, WM_COMMAND, BUTTON_PLUS, NULL);
				
			}
			break;
		}
		case 50: // 2
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!GAMEO) SendMessage(hWnd, WM_COMMAND, BUTTON_MINUS, NULL);

			}
			break;
		}
		case 51: // 3
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!GAMEO) SendMessage(hWnd, WM_COMMAND, BUTTON_MILTIPLY, NULL);

			}
			break;
		}
		
		case 52: // 4
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!GAMEO) SendMessage(hWnd, WM_COMMAND, BUTTON_DIVIDE, NULL);

			}
			break;
		}
		}
		break;
	}
	case GAME:
	{
		ShowWindow(hStaticTime, SW_SHOW);
		ShowWindow(hStaticImage1, SW_SHOW);
		ShowWindow(hStaticNum, SW_SHOW);
		ShowWindow(hbutton_plus, SW_SHOW);
		ShowWindow(hbutton_minus, SW_SHOW);
		ShowWindow(hbutton_muliply, SW_SHOW);
		ShowWindow(hbutton_divide, SW_SHOW);
		ShowWindow(hStaticLife, SW_SHOW);
		if(VOLUME) ShowWindow(hbuttonVolume_on, SW_SHOW);
		else ShowWindow(hbuttonVolume_off, SW_SHOW);

		SetWindowText(hStaticPoint, "Points: 0");
		ShowWindow(hStaticPoint, SW_SHOW);
		SetWindowText(hStaticTime, "01:30");
		SetWindowText(hStaticLife, "©©©");

		set_text_number(NUMBERS, points);



		GAMEO = FALSE;
		

		break;
	}
	case GAME_OVER:
	{
		if (PLAY && ((l == 0) || (t == -61)))
		{
			GAMEO = TRUE;
			PlaySound(0, NULL, 0);
			ShowWindow(hStaticTime, SW_HIDE);
			ShowWindow(hStaticImage1, SW_HIDE);
			ShowWindow(hStaticNum, SW_HIDE);
			ShowWindow(hbutton_plus, SW_HIDE);
			ShowWindow(hbutton_minus, SW_HIDE);
			ShowWindow(hbutton_muliply, SW_HIDE);
			ShowWindow(hbutton_divide, SW_HIDE);
			ShowWindow(hStaticLife, SW_HIDE);

			KillTimer(hWnd, 4);
			TIMER4 = FALSE;
			t = 31;
			l = 3;

			ShowWindow(hStatic_game_over, SW_SHOW);

			SendMessage(hWnd, WM_SIZE, NULL, NULL);
			b_p = -60;
			MoveWindow(hbuttonPlay, (width - 60) / 2, b_p, 60, 60, true);
			SetTimer(hWnd, 5, 5, NULL);
			TIMER5 = TRUE;
			ShowWindow(hbuttonPlay, SW_SHOW);

			game_ov(x0, y0, points, delta, x, yl, hWnd);
		}
		break;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (((HWND)lParam == hStaticTime))
		{
			lf.lfHeight = 42; // висота шрифта в логічних одиницях(за замовч. - 14);
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(60, 160, 160));
			return(int)GetStockObject(NULL_BRUSH);

		}

		if (((HWND)lParam == hStaticPoint))
		{
			lf.lfHeight = 33; // висота шрифта в логічних одиницях(за замовч. - 14);
			lf.lfUnderline = 3; // без підкреслення, ≠0 – з підкресленням;
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(40, 40, 140));
			return(int)GetStockObject(NULL_BRUSH);

		}

		if ((HWND)lParam == hStaticNum)
		{
			lf.lfHeight = 50; // висота шрифта в логічних одиницях(за замовч. - 14);
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 110));
			return(int)GetStockObject(NULL_BRUSH);

		}

		if ((HWND)lParam == hStaticLife)
		{
			lf.lfHeight = 40; // висота шрифта в логічних одиницях(за замовч. - 14);
			strcpy((char *)lf.lfFaceName, "Symbol"); // шрифт
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(200, 0, 50));
			return(int)GetStockObject(NULL_BRUSH);

		}

		break;
	}
	case WM_COMMAND:		// если "попали" на какой-то оконный елемент
	{
		switch (wParam)		// на какой именно?
		{
		
		case ID_HELP:
		{

			Beep(1000, 10);



			break;
		}
		case ID_ANSWER:
		{

			Beep(1000, 10);

			SendMessage(hWnd, WM_COMMAND, ID_PROGRAM_SETTINGS, NULL);


			break;
		}
		case BUTTON_PLAY: // на кнопку "PLAY"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (VOLUME) PlaySound(MAKEINTRESOURCE(IDR_WAVE1), 0, SND_RESOURCE | SND_ASYNC | SND_MEMORY);

				SetTimer(hWnd, 1, 1, NULL);
				TIMER1 = TRUE;
			}

			break;
		}
		case BUTTON_SCORES:
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!ShowWindow(hlist1, SW_SHOW));
				else ShowWindow(hlist1, SW_HIDE);
			}

			break;
		}
		case BUTTON_BACK:
		{

			Beep(1000, 10); // издается приятный) звуковой сигнал
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (!GAMEO)
				{
					PlaySound(0, NULL, 0);
					ShowWindow(hStaticTime, SW_HIDE);
					ShowWindow(hStaticImage1, SW_HIDE);
					ShowWindow(hStaticNum, SW_HIDE);
					ShowWindow(hbutton_plus, SW_HIDE);
					ShowWindow(hbutton_minus, SW_HIDE);
					ShowWindow(hbutton_muliply, SW_HIDE);
					ShowWindow(hbutton_divide, SW_HIDE);
					ShowWindow(hStaticLife, SW_HIDE);
					game_ov(x0, y0, points, delta, x, yl, hWnd);
					KillTimer(hWnd, 4);
					t = 31;
					l = 3;
					points = 0;

				}
				ShowWindow(hbuttonVolume_on, SW_HIDE);
				ShowWindow(hbuttonVolume_off, SW_HIDE);
				ShowWindow(hStaticPoint, SW_HIDE);
				ShowWindow(hbuttonPlay, SW_HIDE);
				ShowWindow(hStatic_game_over, SW_HIDE);
				SetTimer(hWnd, 3, 1, NULL);
				TIMER3 = TRUE;
				m = height + (y + delta);
				PLAY = FALSE;
				ix = x0;
				SendMessage(hWnd, WM_SIZE, NULL, NULL);

				MoveWindow(hbutton1, x0, height + y, x, y, false); // двигаем кнопку к приблизительному центру
				MoveWindow(hbutton2, x0, height + (y + delta), x, y, false); // двигаем кнопку к приблизительному центру
				MoveWindow(hlist1, x0, height + (y + delta + y + yl), x, yl, false); // двигаем кнопку к приблизительному центру

				ShowWindow(hbutton1, SW_SHOW);
				ShowWindow(hbutton2, SW_SHOW);
				GAMEO = FALSE;
			}

			break;
		}
		case BUT_PL:
		{
			if (!TIMER1 && !TIMER2 && !TIMER3 && !TIMER5 && !TIMER6)
			{
				if (VOLUME) PlaySound(MAKEINTRESOURCE(IDR_WAVE1), 0, SND_RESOURCE | SND_ASYNC | SND_MEMORY);
				SendMessage(hWnd, WM_SIZE, NULL, NULL);
				b_p = (height * 0.02) / 2;
				SetTimer(hWnd, 6, 5, NULL);
				TIMER6 = TRUE;
			}

			break;
		}
		case VOLUME_ON:
		{
			PlaySound(0, NULL, 0);
			VOLUME = FALSE;
			ShowWindow(hbuttonVolume_on, SW_HIDE);
			ShowWindow(hbuttonVolume_off, SW_SHOW);

			break;
		}
		case VOLUME_OFF:
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), 0, SND_RESOURCE | SND_ASYNC | SND_MEMORY);
			VOLUME = TRUE;
			ShowWindow(hbuttonVolume_off, SW_HIDE);
			ShowWindow(hbuttonVolume_on, SW_SHOW);

			break;
		}
		case BUTTON_PLUS:
		{

			if (PLUS)
			{
				PLUS = FALSE;
				points += 100;
				_stprintf(POINTS, "Points: %d", points);
				ShowWindow(hStaticPoint, SW_HIDE);
				SetWindowText(hStaticPoint, POINTS);
				ShowWindow(hStaticPoint, SW_SHOW);
				set_text_number(NUMBERS, points);
			}
			else
			{
				l--;
				PLUS = FALSE;
				MINUS = FALSE;
				MULTIPLY = FALSE;
				DIVIDE = FALSE;
				if (l == 0)
				{
					SendMessage(hWnd, GAME_OVER, NULL, NULL);
					break;
				}
				int i = 0;
				for (; i < l; i++)
				{
					LIFES[i] = '©';
				}
				LIFES[i] = '\0';
				set_text_number(NUMBERS, points);
				ShowWindow(hStaticLife, SW_HIDE);
				SetWindowText(hStaticLife, LIFES);
				ShowWindow(hStaticLife, SW_SHOW);
			}


			break;
		}
		case BUTTON_MINUS:
		{

			if (MINUS)
			{
				MINUS = FALSE;
				points += 100;
				_stprintf(POINTS, "Points: %d", points);
				ShowWindow(hStaticPoint, SW_HIDE);
				SetWindowText(hStaticPoint, POINTS);
				ShowWindow(hStaticPoint, SW_SHOW);
				set_text_number(NUMBERS, points);
			}
			else
			{
				l--;
				PLUS = FALSE;
				MINUS = FALSE;
				MULTIPLY = FALSE;
				DIVIDE = FALSE;
				if (l == 0)
				{
					SendMessage(hWnd, GAME_OVER, NULL, NULL);
					break;
				}
				int i = 0;
				for (; i < l; i++)
				{
					LIFES[i] = '©';
				}
				LIFES[i] = '\0';
				set_text_number(NUMBERS, points);
				ShowWindow(hStaticLife, SW_HIDE);
				SetWindowText(hStaticLife, LIFES);
				ShowWindow(hStaticLife, SW_SHOW);
			}

			break;
		}
		case BUTTON_MILTIPLY:
		{

			if (MULTIPLY)
			{
				MULTIPLY = FALSE;
				points += 100;
				_stprintf(POINTS, "Points: %d", points);
				ShowWindow(hStaticPoint, SW_HIDE);
				SetWindowText(hStaticPoint, POINTS);
				ShowWindow(hStaticPoint, SW_SHOW);
				set_text_number(NUMBERS, points);
			}
			else
			{
				l--;
				PLUS = FALSE;
				MINUS = FALSE;
				MULTIPLY = FALSE;
				DIVIDE = FALSE;
				if (l == 0)
				{
					SendMessage(hWnd, GAME_OVER, NULL, NULL);
					break;
				}
				int i = 0;
				for (; i < l; i++)
				{
					LIFES[i] = '©';
				}
				LIFES[i] = '\0';
				set_text_number(NUMBERS, points);
				ShowWindow(hStaticLife, SW_HIDE);
				SetWindowText(hStaticLife, LIFES);
				ShowWindow(hStaticLife, SW_SHOW);
			}

			break;
		}
		case BUTTON_DIVIDE:
		{

			if (DIVIDE)
			{
				DIVIDE = FALSE;
				points += 100;
				_stprintf(POINTS, "Points: %d", points);
				ShowWindow(hStaticPoint, SW_HIDE);
				SetWindowText(hStaticPoint, POINTS);
				ShowWindow(hStaticPoint, SW_SHOW);
				set_text_number(NUMBERS, points);
			}
			else
			{
				l--;
				PLUS = FALSE;
				MINUS = FALSE;
				MULTIPLY = FALSE;
				DIVIDE = FALSE;
				if (l == 0)
				{
					SendMessage(hWnd, GAME_OVER, NULL, NULL);
					break;
				}
				int i = 0;
				for (; i < l; i++)
				{
					LIFES[i] = '©';
				}
				LIFES[i] = '\0';
				set_text_number(NUMBERS, points);
				ShowWindow(hStaticLife, SW_HIDE);
				SetWindowText(hStaticLife, LIFES);
				ShowWindow(hStaticLife, SW_SHOW);
			}

			break;
		}
		case ID_PROGRAM_QUIT: // на пункт меню "Quit"
		{
			if (MessageBox(hWnd, "Do you want to quit?", "Exit from the program", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
			{
				break; // диалоговое окно не закроется
			}
			PostQuitMessage(0); // завершить роботу всей программы
			break;
		}
		case ID_PROGRAM_SETTINGS: // на пункт меню "Settings"
		{

			hDlgRules = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_RULES), hWnd, (DLGPROC)DlgRules); // создание диалогового окна "Settings"
			ShowWindow(hDlgRules, SW_SHOW); // показать диалоговое окно

			break;
		}
		case ID_ABOUT_AUTHOR: // на пункт меню "Author"
		{

			hDlgAuthor = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_AUTHOR), hWnd, (DLGPROC)DlgAuthor); // создание диалогового окна "Author"
			ShowWindow(hDlgAuthor, SW_SHOW); // показать диалоговое окно

			break;
		}
		case ID_ABOUT_PROGRAM: // на пункт меню "Program"
		{

			hDlgAboutProgram = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_VERSION), hWnd, (DLGPROC)DlgAboutProgram); // создание диалогового окна "Author"
			ShowWindow(hDlgAboutProgram, SW_SHOW); // показать диалоговое окно

			break;
		}
		case ID_PROFILE_DELETE:
		{

			if (MessageBox(hWnd, "Are you sure?", "Delete profile", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
			{
				break; // профиль не удалится
			}

			Gamer game; // создание переменной класса
			ifstream file; // создание переменной на считывание 

			file.open(FileName, ios::app); // открытие файла с указателем на начало

			ofstream file2(FORTIME); // создание и открытие файла на запись
			file.clear(); // выравнивание потоков
			file.seekg(0); // установка указателя на начало файла
			while (true)
			{
				file.read((char*)&game, sizeof(game)); // считываем классы
				if (file.eof()) { break; } // пока не конец
				if (strcmp(text_log, game.get_login()) != 0) { // если логин введенный и который находится в файле не совпали
					file2.write((char*)&game, sizeof(game)); // записываем в временный файл
				}
			}
			file.close();
			file2.close();
			remove(FileName); // удаляем основной файл
			rename(FORTIME, FileName); // переименовываем временный в основной файл

			ShowWindow(hWnd, SW_HIDE);

			if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0)) // создание диалогового окна "Main"
			{
				DestroyWindow(hlist1); // удаление списка рекордов
				hlist1 = CreateWindow("listbox", NULL, WS_CHILD, x0, y0 + delta, x, yl, hWnd, (HMENU)LIST_SCORES, hInst, NULL); // создание списка рекордов
				SendMessage(hWnd, WM_SIZE, NULL, NULL);
				list_highscore();
				ShowWindow(hWnd, SW_SHOW);
			}
			else
			{
				DestroyWindow(hWnd);
				PostQuitMessage(0);
				return 0;
			}
			break;
		}
		case ID_PROFILE_EDIT:
		{

			hDlgEdit = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_EDIT), hWnd, (DLGPROC)DlgEdit); // создание диалогового окна "Author"
			ShowWindow(hDlgEdit, SW_SHOW); // показать диалоговое окно

			break;
		}
		case ID_PROFILE_LOGOUT:
		{

			if (MessageBox(hWnd, "Are you sure?", "Exit from the profile", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
			{
				break; // профиль не удалится
			}

			ShowWindow(hWnd, SW_HIDE);

			if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0)) // создание диалогового окна "Main"
			{
				DestroyWindow(hlist1); // удаление списка рекордов
				hlist1 = CreateWindow("listbox", NULL, WS_CHILD, x0, y0 + delta, x, yl, hWnd, (HMENU)LIST_SCORES, hInst, NULL); // создание списка рекордов
				SendMessage(hWnd, WM_SIZE, NULL, NULL);
				list_highscore();
				ShowWindow(hWnd, SW_SHOW);
			}
			else
			{
				DestroyWindow(hWnd);
				PostQuitMessage(0);
				return 0;
			}

			break;
		}
		break;
		}

	}
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 1500, 923, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов

		hCompDC = CreateCompatibleDC(hdc); // створили сумісний контекст у пам’яті
		SelectObject(hCompDC, hBitmap); // ввели наше зображення у контекст в пам’яті і запам’ятали те, що витіснили звідти

		GetObject(hBitmap, sizeof(bitmap), &bitmap); // отримали розміри зображення
		GetClientRect(hWnd, &r); // отримали розміри екрану

		StretchBlt(hdc, r.left, r.top, r.right, r.bottom,     // куди
			hCompDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight,   // звідки
			SRCCOPY);					                      // як
															  // “натягнули” зображення в пам’яті на екран
		DeleteObject(hBitmap); // знищили об’єкт
		DeleteDC(hCompDC);	   // знищили сумісний контекст
		EndPaint(hWnd, &ps);   // знищили контекст пристрою

		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

int numlen(int num)
{
	float len = 0.0;

	while (num) {
		num /= 10;
		len++;
	}

	return len;
}

int point_in_word(CHAR * buf)
{
	float sum = 0;

	for (int i = 0; i < strlen(buf); i++)
	{

		switch (buf[i])
		{
		case 'a':
		case 'A':
		case 'b':
		case 'C':
		case 'd':
		case 'e':
		case 'E':
		case 'F':
		case 'g':
		case 'h':
		case 'K':
		case 'L':
		case 'n':
		case 'o':
		case 'p':
		case 'P':
		case 'q':
		case 's':
		case 'S':
		case 'T':
		case 'u':
		case 'v':
		case 'V':
		case 'x':
		case 'X':
		case 'y':
		case 'z':
		case 'Z':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '@':
		case '#':
		case '$':
		case '%':
		case '&':
			sum += 2;
			break;
		case 'B':
		case 'D':
		case 'G':
		case 'H':
		case 'N':
		case 'O':
		case 'Q':
		case 'R':
		case 'U':
		case 'w':
		case 'Y':
			sum += 2.5;
			break;
		case 'f':
		case 'i':
		case 'I':
		case 'j':
		case 'l':
		case 'r':
		case '!':
		case 't':
		case '"':
		case '\'':
		case '^':
		case '*':
		case '+':
		case '-':
		case '/':
		case '=':
		case '.':
		case ',':
		case ':':
		case '(':
		case ')':
		case '<':
		case '>':
		case '[':
		case ']':
		case ';':
		case '|':
		case '{':
		case '}':
		case ' ':
		case '_':
		case '`':
		case '~':
			sum += 1;
			break;
		case 'c':
		case 'k':
		case 'J':
			sum += 1.5;
			break;
		case 'm':
		case 'M':
			sum += 3;
			break;
		case 'W':
			sum += 3.5;
			break;
		default:
			break;
		}

	}

	return sum;
}

void list_highscore()
{
	ifstream file; // создание переменной на считывание
	Gamer game; // создание переменной класса

	file.open(FileName, ios::out); // открытие файла на считывание

	file.clear(); // выравниваем потоки
	file.seekg(0); // устанавливаем указатель на начало файла

	CHAR buf[55];
	CHAR b[55];
	int point;
	int abc = 0;

	while (true)
	{
		file.read((char *)&game, sizeof(game)); // считываем классы из файла
		if (file.eof()) { break; } // пока не конец

		_stprintf(buf, "%s%d", game.get_login(), game.get_record());

		point = 53 - point_in_word(buf);

		int i = 0;
		for (; i < point - 1; i++) buf[i] = '.';
		buf[0] = ' ';
		buf[i - 1] = ' ';
		buf[i] = '\0';

		_stprintf(b, "%s%s%d", game.get_login(), buf, game.get_record());

		SendMessage(hlist1, LB_ADDSTRING, 0, (LPARAM)b); // и отсылаем в комбобокс наши логины
		if (strcmp(text_log, game.get_login()) == 0) // если логины совпали
		{
			SendMessage(hlist1, LB_SETCURSEL, abc, 0); // выделяем игрока
		}
		abc++;
	}
	file.close(); // закрытие файла
}

void set_text_number(CHAR *NUMBERS, unsigned long long points)
{

	srand(time(NULL));

	int a = points / 500;
	if (a < 3) a = 2;
	else if (a >= 3) a = 3;

	int b = points / 100;
	if (b < 2) b = 2;
	else if (b > 200) b = 200;

	static int u = 0;
	static int test = 0;
	static int test_p = -1;
	for ( ; ; ) 
	{
		test_p = test;
		button = rand() % 4;
		test = button;

		if (u == 0) u++;

		else if (test_p == test) u++;
		else if (test_p != test)
		{
			test = 0;
			test_p = -1;
			u = 0;
		}

		if (u < 3)
		{
			if (button == 0) PLUS = TRUE;
			else if (button == 1) MINUS = TRUE;
			else if (button == 2) MULTIPLY = TRUE;
			else if (button == 3) DIVIDE = TRUE;
			break;
		}
		else
		{
			if (button == 0) PLUS = FALSE;
			else if (button == 1) MINUS = FALSE;
			else if (button == 2) MULTIPLY = FALSE;
			else if (button == 3) DIVIDE = FALSE;
		}
		
	}

	int answer;
	
	if (a == 2)
	{
		int num1 = rand() % (b * 5) + 2;
		int num2 = rand() % (b * 5) + 2;

		if (PLUS)
		{

			if ((num1+num2) == (num1*num2))
			{
				num1 = rand() % (b * 5) + 2;
			}

			answer = num1 + num2;
		}
		else if (MINUS)
		{
			if ((num1 - num2) == (num1/num2))
			{
				num1 = rand() % (b * 5) + 2;
			}
			if (num2 > num1) swap(num2, num1);
			answer = num1 - num2;
		}
		else if (MULTIPLY)
		{
			if ((num1 + num2) == (num1*num2))
			{
				num1 = rand() % (b * 5) + 2;
			}
			answer = num1 * num2;
		}
		else if (DIVIDE)
		{

			if ((num1 - num2) == (num1/num2))
			{
				num1 = rand() % (b * 5) + 2;
			}

			if (num2 > num1) swap(num2, num1);
			if (!(num1 % num2 == 0) || num2 == 1)
			{
				num2 = 2;
				while (!(num1 % num2 == 0))
				{
					num2++;
				}
			}

			answer = num1 / num2;
		}

		_stprintf(NUMBERS, "%d ? %d = %d", num1, num2, answer);
	}
	else if (a == 3)
	{
		int num1 = rand() % (b * 5) + 2;
		int num2 = rand() % (b * 5) + 2;
		int num3 = rand() % (b * 5) + 2;

		if (PLUS) answer = num1 + num2 + num3;
		else if (MINUS)
		{
			
			if ((num2 + num3) > num1)
			{
				swap(num2, num1);
				swap(num2, num3);
				if ((num2 + num3) > num1) swap(num2, num3);
				
				int k = num1 - num2 - num3;
				if (k < 1) num1 += abs(k) * 1.2;

			}
			answer = num1 - num2 - num3;
		}
		else if (MULTIPLY) answer = num1 * num2 * num3;
		else if (DIVIDE)
		{

			if (num2 > num1) swap(num2, num1);
			else if (num3 > num1) swap(num3, num1);
			int i = 2;
			bool a = true;
			for (; i < num1; i++)
			{

				if (num1 % i == 0)
				{
					num2 = i;
					a = false;
					break;
				}

			}
			if (a)
			{
				num2 = 1;
			}
			i++;
			for (; i < num1; i++)
			{
				if (num1 % i == 0)
				{
					num3 = i;
					a = false;
					break;
				}
			}
			if (a)
			{
				num3 = 1;
			}
			if (num2 == num3 == 1)
			{
				num3 = num1;
			}
			answer = num1 / num2 / num3;
		}

		_stprintf(NUMBERS, "%d ? %d ? %d = %d", num1, num2, num3, answer);
	}
	ShowWindow(hStaticNum, SW_HIDE);
	SetWindowText(hStaticNum, NUMBERS);
	ShowWindow(hStaticNum, SW_SHOW);

}

void game_ov(int x0, int y0, unsigned long long &points, int delta, int x, int yl, HWND hWnd)
{
	Gamer game; // создание переменной класса
	ifstream file; // создание переменной на считывание 

	file.open(FileName, ios::app); // открытие файла с указателем на начало
	bool b = false;
	ofstream file2(FORTIME); // создание и открытие файла на запись
	file.clear(); // выравнивание потоков
	file.seekg(0); // установка указателя на начало файла
	while (true)
	{
		file.read((char*)&game, sizeof(game)); // считываем классы
		if (file.eof()) { break; } // пока не конец
		if (strcmp(text_log, game.get_login()) == 0) { // если логин введенный и который находится в файле совпали

			if (game.get_record() < points)
			{
				game.set_record(points);
				b = true;
			}

			file2.write((char*)&game, sizeof(game)); // записываем в временный файл
		}
		else file2.write((char*)&game, sizeof(game));
	}

	file.close();
	file2.close();
	remove(FileName); // удаляем основной файл
	rename(FORTIME, FileName);
	if (b)
	{
		DestroyWindow(hlist1); // удаление списка рекордов
		hlist1 = CreateWindow("listbox", NULL, WS_CHILD, x0, y0 + delta, x, yl, hWnd, (HMENU)LIST_SCORES, hInst, NULL); // создание списка рекордов
		list_highscore();
	}
	points = 0;
}