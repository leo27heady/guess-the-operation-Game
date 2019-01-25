#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>

extern HINSTANCE hInst;

BOOL NewWindowClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); // размер структуры (в байтах)
	wcex.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wcex.lpfnWndProc = Proc; // указатель на пользовательскую функцию
	wcex.cbClsExtra = 0; // число освобождаемых байтов при создании экземпляра приложения
	wcex.cbWndExtra = 0; // число освобождаемых байтов в конце структуры
	wcex.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
	wcex.hIcon = LoadIcon(hInst, (LPCSTR)icon); // декриптор пиктограммы
	wcex.hCursor = LoadCursor(hInst, (LPCSTR)cursor); // дескриптор курсора
	wcex.hbrBackground = (HBRUSH)(brBackground + 1); // дескриптор кисти для закраски фона окна
	wcex.lpszMenuName = (LPCSTR)menu; // указатель на имя меню
	wcex.lpszClassName = szName; // указатель на имя класса
	wcex.hIconSm = LoadIcon(hInst, (LPCSTR)icon); // дескриптор маленькой пиктограммы (в трэе)

	if (!RegisterClassEx(&wcex)) // проверка на успешность создания окна
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", szName, MB_OK); // Вывод мини-окна при ошибке
		return 0;
	}
	else return 1;
}