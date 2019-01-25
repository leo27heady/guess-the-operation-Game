#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "Class.h"

using namespace std;

HWND hstatimage, htext1, htext2;
HBITMAP staticAuth;
HBRUSH hBrush;

BOOL CALLBACK DlgAuthor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // функция обработки сообщений для диалогового окна "Main"
{
	static LOGFONT lf; // создаем текст(который будет сохраняться)
	lf.lfHeight = 35; // висота шрифта в логічних одиницях(за замовч. - 14);
	lf.lfWidth = 0; // ширина символів в логічних одиницях(за замовч. - 0);
	lf.lfWeight = FW_DONTCARE; // жирність
	strcpy((char *)lf.lfFaceName, "Dubai"); // шрифт
	lf.lfItalic = 0; // без нахилу(не курсив), ≠0 – з нахилом;
	lf.lfUnderline = 0; // без підкреслення, ≠0 – з підкресленням;

	switch (message) // обработка сообщений
	{

	case WM_INITDIALOG: // инициализация, здесь будут добавляться елементы
	{


		staticAuth = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP14), IMAGE_BITMAP, 250, 187, LR_COPYFROMRESOURCE); // загрузка изображения из ресурсов
		hstatimage = GetDlgItem(hWnd, IDC_STATIC_IM);
		htext1 = GetDlgItem(hWnd, STATIC_1);
		htext2 = GetDlgItem(hWnd, STATIC_2);

		SetWindowText(htext1, "Автор этой программы - Леонід Кулик\nКонтактные номера:\n38-(067)-164-76-XX\n38-(093)-648-12-XX.\nЭлектронная почта:\nleonidkulik2707@gmail.com");
		SetWindowText(htext2, "Я студент Винницкого Национального Технического Университета, кафедра защиты информации.");


		LOGBRUSH lb;
		memset(&lb, 0, sizeof(LOGBRUSH));
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(255, 255, 255); // цвет окна
		hBrush = CreateBrushIndirect(&lb);
		break;
	}
	case WM_CTLCOLORDLG:
	{
		if (hBrush != NULL)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			return (BOOL)hBrush;
		}

	}

	case WM_COMMAND: // обработа кнопок, полей ввода...
	{

		switch (wParam)
		{
		case IDC_OK: // если нажали на кнопку "ОК"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал



			EndDialog(hWnd, 0);
			DestroyWindow(hWnd);
			return TRUE;
		}
		default: break;
		}

		break;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (((HWND)lParam == htext1))
		{
			lf.lfHeight = 28; // висота шрифта в логічних одиницях(за замовч. - 14);
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 30, 60));
			return(int)GetStockObject(NULL_BRUSH);

		}

		if (((HWND)lParam == htext2))
		{
			lf.lfHeight = 33; // висота шрифта в логічних одиницях(за замовч. - 14);
			lf.lfUnderline = 3; // без підкреслення, ≠0 – з підкресленням;
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 30, 60));
			return(int)GetStockObject(NULL_BRUSH);

		}

		break;
	}
	case WM_PAINT:
	{

		BITMAP bm;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hstatimage, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HGDIOBJ hBmOld = SelectObject(hdcMem, staticAuth);
		GetObject(staticAuth, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hBmOld);
		DeleteDC(hdcMem);
		EndPaint(hstatimage, &ps);

		break;
	}
	case WM_CLOSE: // обработка закрытия диалогового окна
	{
		EndDialog(hWnd, 0); // прекращение работы диалогового окна 
		DestroyWindow(hWnd); // удаление диалогового окна
		return TRUE;
	}

	}
	return FALSE;
}