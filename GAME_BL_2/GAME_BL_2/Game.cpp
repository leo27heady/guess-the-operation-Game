#include "resource.h"
#include "stdafx.h"
#include "NewWind.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

HINSTANCE hInst;
HWND hWndMain, hWndA; // дескрипторы будущих окошек

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Прототип функции обработки сообщений для "WindMain"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений диалогового окна "Main"

BOOL NewWindowClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu); // прототип функции для задания параметров окна

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) // ГЛАВНАЯ ФУНКЦИЯ ПРОГРАММЫ
{
	hInst = hInstance; // хранит значение hInstance в глобальной переменной

	if (!DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0)) // если закрыли диалоговое окно
	{
		PostQuitMessage(0);
		return 0;
	}

	if (!NewWindowClass(WndProc, (TCHAR*)("CLMain"), 3, IDI_ICON1, IDC_CURSOR, IDR_MENU1)) return FALSE; // задание параметров окна "WindMain"

	hWndMain = CreateWindow("CLMain", "Guess the operation", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - 1100) / 2, (GetSystemMetrics(SM_CYSCREEN) - 650) / 2, 1100, 650, NULL, NULL, hInst, NULL); // создание окна "WindMain"

	MSG msg;

	while (GetMessage(&msg, NULL, NULL, NULL)) // обработка поступающих сообщений
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}