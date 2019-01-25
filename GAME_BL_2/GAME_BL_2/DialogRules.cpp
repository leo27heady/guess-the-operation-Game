#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "Class.h"

using namespace std;

extern HINSTANCE hInst;

BOOL CALLBACK DlgRules(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // обработка сообщений диалогового окна "Settings"
{
	static HWND hstatic_text;
	static HBRUSH hBrush;

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

		hstatic_text = GetDlgItem(hWnd, IDC_STATIC_TEXT);
		SetWindowText(hstatic_text, "Игроку предлагается арифметическое равенство, в котором отсутствуют знаки математических операций. В тренажере «Guess the operation» необходимо правильно указать как можно больше количество подходящих операций, для соблюдения поставленных условий.\nЧем больше правильных ответов вы даете – тем выше будет сложность. Неправильный ответ не даёт баллов и забирает одно из 3-х жизней, правильный - приводит к начислению 100 баллов, по которым вы сможете оценить свой уровень.");


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



			EndDialog(hWnd, 0); // прекращение работы диалогового окна 
			DestroyWindow(hWnd); // удаление диалогового окна
			return TRUE; // выход из функции обработки сообщений
		}

		default: break;
		}

		break;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (((HWND)lParam == hstatic_text))
		{
			lf.lfHeight = 28; // висота шрифта в логічних одиницях(за замовч. - 14);
			lf.lfUnderline = 3;
			HDC hdc = (HDC)wParam;
			HFONT myFont = CreateFontIndirect(&lf);
			SelectObject(hdc, myFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 30, 60));
			return(int)GetStockObject(NULL_BRUSH);

		}


		break;
	}
	case WM_CLOSE: // обработка закрытия диалогового окна
	{
		EndDialog(hWnd, 0); // прекращение работы диалогового окна 
		DestroyWindow(hWnd); // удаление диалогового окна
		return TRUE; // выход из функции обработки сообщений
	}

	}
	return FALSE;
}