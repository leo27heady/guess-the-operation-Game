#include "resource.h"
#include "stdafx.h"
#include "NewWind.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

HINSTANCE hInst;
HWND hWndMain, hWndA; // ����������� ������� ������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // �������� ������� ��������� ��������� ��� "WindMain"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // �������� ������� ��������� ��������� ����������� ���� "Main"

BOOL NewWindowClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu); // �������� ������� ��� ������� ���������� ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) // ������� ������� ���������
{
	hInst = hInstance; // ������ �������� hInstance � ���������� ����������

	if (!DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0)) // ���� ������� ���������� ����
	{
		PostQuitMessage(0);
		return 0;
	}

	if (!NewWindowClass(WndProc, (TCHAR*)("CLMain"), 3, IDI_ICON1, IDC_CURSOR, IDR_MENU1)) return FALSE; // ������� ���������� ���� "WindMain"

	hWndMain = CreateWindow("CLMain", "Guess the operation", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - 1100) / 2, (GetSystemMetrics(SM_CYSCREEN) - 650) / 2, 1100, 650, NULL, NULL, hInst, NULL); // �������� ���� "WindMain"

	MSG msg;

	while (GetMessage(&msg, NULL, NULL, NULL)) // ��������� ����������� ���������
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}