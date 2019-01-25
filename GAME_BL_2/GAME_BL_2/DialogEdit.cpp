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

extern CHAR text_log[255];
extern CHAR text_pass[255];
extern HWND hWndMain, hlist1;
extern HINSTANCE hInst;

HWND hStaticPass, hStaticLog, hEditPass, hEditLog, hButtonOK, hButtonCancel, hButtonLog, hButtonPass; // ����������� ����

void list_highscore();

CHAR re_log[255]; // ������ ��� ���������� ������
CHAR re_pass[255]; // ������ ��� ���������� ������

BOOL CALLBACK DlgEdit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // ������� ��������� ��������� ��� ����������� ���� "Main"
{
	static int i = 0;

	switch (message) // ��������� ���������
	{

	case WM_INITDIALOG: // �������������, ����� ����� ����������� ��������
	{
		i = 0;
		HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // �������� ������ �� �������� ��� ����������� ����
		SendMessage(hWnd, WM_SETICON, 1, (LPARAM)icon); // ��������� ������

		hEditPass = GetDlgItem(hWnd, IDC_WRIT_PASS); // ��������� ����������� ��� edit ������
		hEditLog = GetDlgItem(hWnd, IDC_WRIT_LOG); // ��������� ����������� ��� combobox
		hStaticPass = GetDlgItem(hWnd, IDC_ENT_PASS); // ��������� ����������� ��� edit ������
		hStaticLog = GetDlgItem(hWnd, IDC_ENT_LOG); // ��������� ����������� ��� combobox
		hButtonOK = GetDlgItem(hWnd, IDC_OK);
		hButtonCancel = GetDlgItem(hWnd, IDC_CANCEL);
		hButtonLog = GetDlgItem(hWnd, IDC_BUTTON_EDIT_LOG);
		hButtonPass = GetDlgItem(hWnd, IDC_BUTTON_EDIT_PASS);

		break;
	}
	
	case WM_COMMAND: // �������� ������, ����� �����...
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
		case IDC_OK: // ��� ������� �� ������ "��"
		{
			Beep(1000, 10); // �������� ��������) �������� ������

			if (MessageBox(hWnd, "Want to check the entered text?", "Saving the entered text", MB_YESNO | MB_ICONQUESTION) == IDYES) // ���� ����� ��
			{
				break; // ���������� ���� �� ���������
			}

			if (i == 2)
			{
				GetWindowText(hEditLog, re_log, 255); // ��������� ������ �� ����
				if (strlen(re_log) == 0)
				{
					MessageBox(hWnd, "Login field is empty!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������
					break;
				}
				
			}
			else if (i == 1)
			{
				GetWindowText(hEditPass, re_pass, 255); // ��������� ������ �� ����
				if (strlen(re_pass) == 0)
				{
					MessageBox(hWnd, "Password field is empty!", "Error", MB_ICONERROR); // ����� ���� � ������������ �� ������
					break;
				}
			}

			Gamer game; // �������� ���������� ������
			ifstream file; // �������� ���������� �� ���������� 

			file.open(FileName, ios::app); // �������� ����� � ���������� �� ������

			ofstream file2(FORTIME); // �������� � �������� ����� �� ������
			file.clear(); // ������������ �������
			file.seekg(0); // ��������� ��������� �� ������ �����
			while (true)
			{
				file.read((char*)&game, sizeof(game)); // ��������� ������
				if (file.eof()) { break; } // ���� �� �����
				if (strcmp(text_log, game.get_login()) == 0) { // ���� ����� ��������� � ������� ��������� � ����� �������
					
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

					file2.write((char*)&game, sizeof(game)); // ���������� � ��������� ����
				}
				else file2.write((char*)&game, sizeof(game)); // ���������� � ��������� ����
			}
			file.close();
			file2.close();
			remove(FileName); // ������� �������� ����
			rename(FORTIME, FileName); // ��������������� ��������� � �������� ����

			ShowWindow(hlist1, SW_HIDE);
			DestroyWindow(hlist1); // �������� ������ ��������
			hlist1 = CreateWindow("listbox", NULL, WS_CHILD, 0, 0, 215, 300, hWndMain, (HMENU)LIST_SCORES, hInst, NULL); // �������� ������ ��������
			SendMessage(hWndMain, WM_SIZE, NULL, NULL);
			list_highscore();

			EndDialog(hWnd, 1); // ������� ���������� ����
			DestroyWindow(hWnd); // �������� ����������� ����
			break;
		}
		case IDC_CANCEL: // ��� ������� �� ������ "Cancel"
		{
			Beep(1000, 10); // �������� ��������) �������� ������
			if (MessageBox(hWnd, "Do you want to quit?", "Exit from \"Edit Profile\"", MB_YESNO | MB_ICONQUESTION) == IDNO) // ���� ����� ���
			{
				break; // ���������� ���� �� ���������
			}
			EndDialog(hWnd, 0); // ������� ���������� ����
			DestroyWindow(hWnd); // �������� ����������� ����
			break;
		}
		default: break;
		}

		break;
	}

	case WM_CLOSE: // ��������� �������� ����������� ����
	{
		if (MessageBox(hWnd, "Do you want to quit?", "Exit from \"Edit Profile\"", MB_YESNO | MB_ICONQUESTION) == IDNO) // ���� ����� ���
		{
			break; // ���������� ���� �� ���������
		}
		EndDialog(hWnd, 0); // ������� ���������� ����
		DestroyWindow(hWnd); // �������� ����������� ����
	}

	}
	return FALSE;
}