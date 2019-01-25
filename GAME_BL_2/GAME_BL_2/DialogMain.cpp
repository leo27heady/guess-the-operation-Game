#pragma once
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "stdafx.h"
#include <stdio.h>
#include "Class.h"
#include <fstream>
#define FileName "PersonalFile.txt" // ���� ��� ��������� ������ �������������
#define FORTIME "FORTIME.txt" // ��������� ���� ��� ����������

using namespace std;

HWND hEdit1, hCombo1; // ����������� ����
CHAR text_log[255]; // ������ ��� ���������� ������
CHAR text_pass[255]; // ������ ��� ���������� ������

void quick(Gamer * s, int low, int high);
void SortGamer();
int quant_gamer();

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // ������� ��������� ��������� ��� ����������� ���� "Main"
{
	switch (message) // ��������� ���������
	{

	case WM_INITDIALOG: // �������������, ����� ����� ����������� ��������
	{
		ifstream file; // �������� ���������� �� ����������
		Gamer game; // �������� ���������� ������
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)); // �������� ������ �� �������� ��� ����������� ����
		SendMessage(hWnd, WM_SETICON, 1, (LPARAM)icon); // ��������� ������

		hEdit1 = GetDlgItem(hWnd, IDC_EDIT1); // ��������� ����������� ��� edit ������
		hCombo1 = GetDlgItem(hWnd, IDC_COMBO2); // ��������� ����������� ��� combobox

		file.open(FileName, ios::out); // �������� ����� �� ����������
		if (!file.is_open()) { // ���� ���� �� ������
		ofstream file1(FileName); //����������� ���� �� ������(��� �������� �� ������ ���� ������������� ����� ���� ������, ���� ����� ������)
		file1.close(); // ���� �����������
		file.open(FileName); // � ����� ����������� �� ����������
		}
		
		file.clear(); // ����������� ������
		file.seekg(0); // ������������� ��������� �� ������ �����

		while (true)
		{
		file.read((char *)&game, sizeof(game)); // ��������� ������ �� �����
		if (file.eof()) { break; } // ���� �� �����
		SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)game.get_login()); // � �������� � ��������� ���� ������
		if (game.get_remember()) // ���� ����� "����������"
		{
		SetWindowText(hCombo1, game.get_login()); // ��������� ������ � ���� ���������
		SetWindowText(hEdit1, game.get_password()); // ��������� ������ � ���� ��� ������
		SendDlgItemMessage(hWnd, IDC_CHECK1, BM_SETCHECK, 1, 0); // ��������� ������� �� "�����������"
		}
		}
		file.close(); // �������� �����

		SetFocus(hCombo1); // �������� ��������� �� ���� � ���������

		break;
	}

	case WM_COMMAND: // �������� ������, ����� �����...
	{

		switch (wParam)
		{
		case IDC_OK: // ��� ������� �� ������ "��"
		{
			Beep(1000, 10); // �������� ��������) �������� ������

			Gamer game; // �������� ���������� ������
			ifstream file; // �������� ���������� �� ���������� 

			file.open(FileName, ios::app); // �������� ����� � ���������� �� ������

			BOOL CHECK = SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0); // ����������� ���������� �������� "�����������"
			BOOL f = FALSE; // ���������� ��� �������� �� ��������� ������ � ���� ������
			BOOL EXIT = FALSE; // ���������� ��� ������������ ���� �� �����
			GetWindowText(hCombo1, text_log, 255); // ��������� ������ �� ����
			GetWindowText(hEdit1, text_pass, 255); // ��������� ������ �� ����
			if (strlen(text_log) == 0 && strlen(text_pass) == 0) // ���� ��� ���� �����
			{
				MessageBox(hWnd, "Fields are not filled!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������
				break;
			}
			else if (strlen(text_log) == 0) // ���� ������ ���� ������ ������
			{
				MessageBox(hWnd, "Login field is empty!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������
				break;
			}
			else if (strlen(text_pass) == 0) // ���� ������ ���� ������ ������
			{
				MessageBox(hWnd, "Password field is empty!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������
				break;
			}

			ofstream file2(FORTIME); // �������� � �������� ����� �� ������
			file.clear(); // ������������ �������
			file.seekg(0); // ��������� ��������� �� ������ �����
			while (true)
			{
				file.read((char*)&game, sizeof(game)); // ��������� ������
				if (file.eof()) { break; } // ���� �� �����
				if (strcmp(text_log, game.get_login()) == 0) { // ���� ����� ��������� � ������� ��������� � ����� �������
					if (strcmp(text_pass, game.get_password()) != 0) // ���� ������ �� �������
					{
						MessageBox(hWnd, "Wrong password!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������ 
					}
					else EXIT = TRUE;
					f = TRUE; // �������, ��� ������ ����� ���� � ���� ������
					game.set_remember(CHECK); // �������� ������ "�����������"
				}
				else // ���� ������ �� �������
				{
					if (CHECK) game.set_remember(!CHECK); // �������� ������ "�����������" 
				}
				file2.write((char*)&game, sizeof(game)); // ���������� � ��������� ����
			}
			if (!f) // ���� ������ ��� � ���� ������
			{
				if (MessageBox(hWnd, "Create a new account?", "Registration", MB_YESNO | MB_ICONQUESTION) == IDYES) // ���� ����� "��"
				{
					game.set_all(text_log, text_pass, CHECK, 0); // �������������� ��� ���� ������
					file2.write((char*)&game, sizeof(game)); // ���������� � ����
					EXIT = TRUE;
				}
			}
			file.close();
			file2.close();
			remove(FileName); // ������� �������� ����
			rename(FORTIME, FileName); // ��������������� ��������� � �������� ����

			if (EXIT) {
				EndDialog(hWnd, 1); // ���� ����� - ������, ����� ������� ������
				SortGamer();
			}

			break;
		}
		case IDC_CANCEL: // ��� ������� �� ������ "Cancel"
		{
			Beep(1000, 10); // �������� ��������) �������� ������
			if (MessageBox(hWnd, "Do you want to quit?", "Exit from the program", MB_YESNO | MB_ICONQUESTION) == IDNO) // ���� ����� ���
			{
				break; // ���������� ���� �� ���������
			}
			EndDialog(hWnd, 0); // ������� ���������� ����
			//PostQuitMessage(0); // ��������� ������ ���� ���������
			break;
		}
		default: break;
		}

		break;
	}

	case WM_CLOSE: // ��������� �������� ����������� ����
	{
		if (MessageBox(hWnd, "Do you want to quit?", "Exit from the program", MB_YESNO | MB_ICONQUESTION) == IDNO) // ���� ����� ���
		{
			break; // ���������� ���� �� ���������
		}
		EndDialog(hWnd, 0); // ������� ���������� ����
		//PostQuitMessage(0); // ��������� ������ ���� ���������
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
