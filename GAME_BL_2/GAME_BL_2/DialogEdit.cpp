#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "Class.h"
#include <fstream>
#define FileName "PersonalFile.txt" // файл где храняться данные пользователей
#define FORTIME "FORTIME.txt" // временный файл для перезаписи

using namespace std;

extern CHAR text_log[255];
extern CHAR text_pass[255];
extern HWND hWndMain, hlist1;
extern HINSTANCE hInst;

HWND hStaticPass, hStaticLog, hEditPass, hEditLog, hButtonOK, hButtonCancel, hButtonLog, hButtonPass; // дескрипторы окон

void list_highscore();

CHAR re_log[255]; // массив для считывания логина
CHAR re_pass[255]; // массив для считывания пароля

BOOL CALLBACK DlgEdit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // функция обработки сообщений для диалогового окна "Main"
{
	static int i = 0;

	switch (message) // обработка сообщений
	{

	case WM_INITDIALOG: // инициализация, здесь будут добавляться елементы
	{
		i = 0;
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // загрузка иконки из ресурсов для диалогового окна
		SendMessage(hWnd, WM_SETICON, 1, (LPARAM)icon); // установка иконки

		hEditPass = GetDlgItem(hWnd, IDC_WRIT_PASS); // получение дескриптора для edit текста
		hEditLog = GetDlgItem(hWnd, IDC_WRIT_LOG); // получение дескриптора для combobox
		hStaticPass = GetDlgItem(hWnd, IDC_ENT_PASS); // получение дескриптора для edit текста
		hStaticLog = GetDlgItem(hWnd, IDC_ENT_LOG); // получение дескриптора для combobox
		hButtonOK = GetDlgItem(hWnd, IDC_OK);
		hButtonCancel = GetDlgItem(hWnd, IDC_CANCEL);
		hButtonLog = GetDlgItem(hWnd, IDC_BUTTON_EDIT_LOG);
		hButtonPass = GetDlgItem(hWnd, IDC_BUTTON_EDIT_PASS);

		break;
	}
	
	case WM_COMMAND: // обработа кнопок, полей ввода...
	{

		switch (wParam)
		{
		case IDC_BUTTON_EDIT_LOG: i++;
		case IDC_BUTTON_EDIT_PASS:
		{
			i++;

			ShowWindow(hButtonLog, SW_HIDE);
			ShowWindow(hButtonPass, SW_HIDE);
			if (i == 1) 
			{
				ShowWindow(hStaticPass, SW_SHOW);
				ShowWindow(hEditPass, SW_SHOW);
			}
			else if (i == 2)
			{
				ShowWindow(hStaticLog, SW_SHOW);
				ShowWindow(hEditLog, SW_SHOW);
			}

			break;
		}
		case IDC_OK: // при нажатии на кнопку "ОК"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал

			if (MessageBox(hWnd, "Want to check the entered text?", "Saving the entered text", MB_YESNO | MB_ICONQUESTION) == IDYES) // если ответ Да
			{
				break; // диалоговое окно не закроется
			}

			if (i == 2)
			{
				GetWindowText(hEditLog, re_log, 255); // получение логина из поля
				if (strlen(re_log) == 0)
				{
					MessageBox(hWnd, "Login field is empty!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке
					break;
				}
				
			}
			else if (i == 1)
			{
				GetWindowText(hEditPass, re_pass, 255); // получение логина из поля
				if (strlen(re_pass) == 0)
				{
					MessageBox(hWnd, "Password field is empty!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке
					break;
				}
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
				if (strcmp(text_log, game.get_login()) == 0) { // если логин введенный и который находится в файле совпали
					
					if (i == 2)
					{
						game.set_login(re_log);
						strcpy(text_log, re_log);
					}
					else if (i == 1)
					{
						game.set_password(re_pass);
						strcpy(text_pass, re_pass);
					}

					file2.write((char*)&game, sizeof(game)); // записываем в временный файл
				}
				else file2.write((char*)&game, sizeof(game)); // записываем в временный файл
			}
			file.close();
			file2.close();
			remove(FileName); // удаляем основной файл
			rename(FORTIME, FileName); // переименовываем временный в основной файл

			ShowWindow(hlist1, SW_HIDE);
			DestroyWindow(hlist1); // удаление списка рекордов
			hlist1 = CreateWindow("listbox", NULL, WS_CHILD, 0, 0, 215, 300, hWndMain, (HMENU)LIST_SCORES, hInst, NULL); // создание списка рекордов
			SendMessage(hWndMain, WM_SIZE, NULL, NULL);
			list_highscore();

			EndDialog(hWnd, 1); // закрыть диалоговое окно
			DestroyWindow(hWnd); // удаление диалогового окна
			break;
		}
		case IDC_CANCEL: // при нажатии на кнопку "Cancel"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал
			if (MessageBox(hWnd, "Do you want to quit?", "Exit from \"Edit Profile\"", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
			{
				break; // диалоговое окно не закроется
			}
			EndDialog(hWnd, 0); // закрыть диалоговое окно
			DestroyWindow(hWnd); // удаление диалогового окна
			break;
		}
		default: break;
		}

		break;
	}

	case WM_CLOSE: // обработка закрытия диалогового окна
	{
		if (MessageBox(hWnd, "Do you want to quit?", "Exit from \"Edit Profile\"", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
		{
			break; // диалоговое окно не закроется
		}
		EndDialog(hWnd, 0); // закрыть диалоговое окно
		DestroyWindow(hWnd); // удаление диалогового окна
	}

	}
	return FALSE;
}