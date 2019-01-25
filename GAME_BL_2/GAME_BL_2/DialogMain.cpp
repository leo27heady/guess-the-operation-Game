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

HWND hEdit1, hCombo1; // дескрипторы окон
CHAR text_log[255]; // массив для считывания логина
CHAR text_pass[255]; // массив для считывания пароля

void quick(Gamer * s, int low, int high);
void SortGamer();
int quant_gamer();

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // функция обработки сообщений для диалогового окна "Main"
{
	switch (message) // обработка сообщений
	{

	case WM_INITDIALOG: // инициализация, здесь будут добавляться елементы
	{
		ifstream file; // создание переменной на считывание
		Gamer game; // создание переменной класса
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)); // загрузка иконки из ресурсов для диалогового окна
		SendMessage(hWnd, WM_SETICON, 1, (LPARAM)icon); // установка иконки

		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1); // получение дескриптора для edit текста
		hCombo1 = GetDlgItem(hWnd, IDC_COMBO2); // получение дескриптора для combobox

		file.open(FileName, ios::out); // открытие файла на считывание
		if (!file.is_open()) { // если файл не открыт
		ofstream file1(FileName); //открывается файл на запись(при открытии на запись файл автоматичести может быть создан, если небыл найден)
		file1.close(); // файл закрывается
		file.open(FileName); // и снова открывается на считывание
		}
		
		file.clear(); // выравниваем потоки
		file.seekg(0); // устанавливаем указатель на начало файла

		while (true)
		{
		file.read((char *)&game, sizeof(game)); // считываем классы из файла
		if (file.eof()) { break; } // пока не конец
		SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)game.get_login()); // и отсылаем в комбобокс наши логины
		if (game.get_remember()) // если игрок "запоминаем"
		{
		SetWindowText(hCombo1, game.get_login()); // установка логина в поле комбобокс
		SetWindowText(hEdit1, game.get_password()); // установка пароля в поле для пароля
		SendDlgItemMessage(hWnd, IDC_CHECK1, BM_SETCHECK, 1, 0); // установка галочки на "запоминание"
		}
		}
		file.close(); // закрытие файла

		SetFocus(hCombo1); // устанока указателя на поле в комбобокс

		break;
	}

	case WM_COMMAND: // обработа кнопок, полей ввода...
	{

		switch (wParam)
		{
		case IDC_OK: // при нажатии на кнопку "ОК"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал

			Gamer game; // создание переменной класса
			ifstream file; // создание переменной на считывание 

			file.open(FileName, ios::app); // открытие файла с указателем на начало

			BOOL CHECK = SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0); // присваиваем переменной значение "запоминания"
			BOOL f = FALSE; // переменная для проверки на наявность игрока в базе данных
			BOOL EXIT = FALSE; // переменная для присваивания прав на выход
			GetWindowText(hCombo1, text_log, 255); // получение логина из поля
			GetWindowText(hEdit1, text_pass, 255); // получение пароля из поля
			if (strlen(text_log) == 0 && strlen(text_pass) == 0) // если оба поля пусты
			{
				MessageBox(hWnd, "Fields are not filled!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке
				break;
			}
			else if (strlen(text_log) == 0) // если только поле логина пустое
			{
				MessageBox(hWnd, "Login field is empty!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке
				break;
			}
			else if (strlen(text_pass) == 0) // если только поле пароля пустое
			{
				MessageBox(hWnd, "Password field is empty!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке
				break;
			}

			ofstream file2(FORTIME); // создание и открытие файла на запись
			file.clear(); // выравнивание потоков
			file.seekg(0); // установка указателя на начало файла
			while (true)
			{
				file.read((char*)&game, sizeof(game)); // считываем классы
				if (file.eof()) { break; } // пока не конец
				if (strcmp(text_log, game.get_login()) == 0) { // если логин введенный и который находится в файле совпали
					if (strcmp(text_pass, game.get_password()) != 0) // если пароли не совпали
					{
						MessageBox(hWnd, "Wrong password!", "Error", MB_ICONERROR); // вывод окна с уведомлением об ошибке 
					}
					else EXIT = TRUE;
					f = TRUE; // отметил, что данный игрок есть в базе данных
					game.set_remember(CHECK); // отмечаем игроку "запоминание"
				}
				else // если логины не совпали
				{
					if (CHECK) game.set_remember(!CHECK); // отмечаем игроку "запоминание" 
				}
				file2.write((char*)&game, sizeof(game)); // записываем в временный файл
			}
			if (!f) // если игрока нет в базе данных
			{
				if (MessageBox(hWnd, "Create a new account?", "Registration", MB_YESNO | MB_ICONQUESTION) == IDYES) // если ответ "Да"
				{
					game.set_all(text_log, text_pass, CHECK, 0); // инициализируем все поля класса
					file2.write((char*)&game, sizeof(game)); // записываем у файл
					EXIT = TRUE;
				}
			}
			file.close();
			file2.close();
			remove(FileName); // удаляем основной файл
			rename(FORTIME, FileName); // переименовываем временный в основной файл

			if (EXIT) {
				EndDialog(hWnd, 1); // если выход - правда, тогда закрыть диалог
				SortGamer();
			}

			break;
		}
		case IDC_CANCEL: // при нажатии на кнопку "Cancel"
		{
			Beep(1000, 10); // издается приятный) звуковой сигнал
			if (MessageBox(hWnd, "Do you want to quit?", "Exit from the program", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
			{
				break; // диалоговое окно не закроется
			}
			EndDialog(hWnd, 0); // закрыть диалоговое окно
			//PostQuitMessage(0); // завершить роботу всей программы
			break;
		}
		default: break;
		}

		break;
	}

	case WM_CLOSE: // обработка закрытия диалогового окна
	{
		if (MessageBox(hWnd, "Do you want to quit?", "Exit from the program", MB_YESNO | MB_ICONQUESTION) == IDNO) // если ответ нет
		{
			break; // диалоговое окно не закроется
		}
		EndDialog(hWnd, 0); // закрыть диалоговое окно
		//PostQuitMessage(0); // завершить роботу всей программы
	}

	}
	return FALSE;
}

void quick(Gamer *s, int low, int high) {
	Gamer count, aux;

	int i, j;

	if (high>low) {
		i = low; j = high;
		count = s[i];
		while (i < j) {
			if (s[i + 1].get_record() >= count.get_record()) {
				s[i] = s[i + 1];
				s[i + 1] = count;
				i++;
			}
			else {
				if (s[j].get_record() >= count.get_record()) {
					aux = s[j];
					s[j] = s[i + 1];
					s[i + 1] = aux;
				}
				j--;
			}
		}
		quick(s, low, i - 1);
		quick(s, i + 1, high);
	}
}

void SortGamer() {
	
	int kilclass = quant_gamer();
	if (kilclass <= 1) return;
	Gamer *massGamer = new Gamer[kilclass];

	int i = 0;
	ifstream file1;
	file1.open(FileName, ios::out);
	file1.clear();
	file1.seekg(0);
	
	while (true)
	{
		file1.read((char*)&massGamer[i], sizeof(massGamer[i]));
		if (file1.eof()) { break; }
		//massGamer[i].set_record(0);
		i++;
	}
	file1.close();
	ofstream file2;
	file2.open(FileName, ios::in | ios::trunc);
	quick(massGamer, 0, kilclass - 1);
	for (int i = 0; i < kilclass; i++) file2.write((char*)&massGamer[i], sizeof(massGamer[i]));
	file2.close();
	delete[] massGamer;
}

int quant_gamer()
{
	ifstream file1;
	file1.open(FileName, ios::out);
	file1.clear();
	file1.seekg(0, ios::end);
	int m = (int)file1.tellg() / sizeof(Gamer);
	file1.close();

	return m;
}
