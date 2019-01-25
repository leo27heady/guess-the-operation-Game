#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "Class.h"

using namespace std;
extern HINSTANCE hInst;
BOOL CALLBACK DlgAboutProgram(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // функция обработки сообщений для диалогового окна "Main"
{
	static HWND hStatImage;
	static HICON hICon;
	static HBRUSH hBrusH;
	static HWND htext1, htext2;

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

		hICon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // загрузка изображения из ресурсов
		
		htext1 = GetDlgItem(hWnd, IDC_STATIC_HTEXT1);
		htext2 = GetDlgItem(hWnd, IDC_STATIC_HTEXT2);
		LOGBRUSH lb;
		memset(&lb, 0, sizeof(LOGBRUSH));
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(255, 255, 255); // цвет окна
		hBrusH = CreateBrushIndirect(&lb);
		break;
	}
	case WM_CTLCOLORDLG:
	{
		if (hBrusH != NULL)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			return (BOOL)hBrusH;
		}

	}
	case WM_CTLCOLORSTATIC:
	{

		if (((HWND)lParam == htext1))
		{
			lf.lfHeight = 14; // висота шрифта в логічних одиницях(за замовч. - 14);
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			return(int)GetStockObject(NULL_BRUSH);

		}

		if (((HWND)lParam == htext2))
		{
			lf.lfHeight = 14; // висота шрифта в логічних одиницях(за замовч. - 14);
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			return(int)GetStockObject(NULL_BRUSH);

		}

		break;
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

	/*case WM_PAINT:
	{

		BITMAP bm;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hstatimage, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HGDIOBJ hBmOld = SelectObject(hdcMem, hicon);
		GetObject(hicon, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hBmOld);
		DeleteDC(hdcMem);
		EndPaint(hstatimage, &ps);

		break;
	}*/
	case WM_CLOSE: // обработка закрытия диалогового окна
	{
		EndDialog(hWnd, 0); // прекращение работы диалогового окна 
		DestroyWindow(hWnd); // удаление диалогового окна
		return TRUE;
	}

	}
	return FALSE;
}