#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>

extern HINSTANCE hInst;

BOOL NewWindowClass(WNDPROC Proc, TCHAR szName[], UINT brBackground, UINT icon, UINT cursor, UINT menu)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); // ������ ��������� (� ������)
	wcex.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wcex.lpfnWndProc = Proc; // ��������� �� ���������������� �������
	wcex.cbClsExtra = 0; // ����� ������������� ������ ��� �������� ���������� ����������
	wcex.cbWndExtra = 0; // ����� ������������� ������ � ����� ���������
	wcex.hInstance = hInst; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	wcex.hIcon = LoadIcon(hInst, (LPCSTR)icon); // ��������� �����������
	wcex.hCursor = LoadCursor(hInst, (LPCSTR)cursor); // ���������� �������
	wcex.hbrBackground = (HBRUSH)(brBackground + 1); // ���������� ����� ��� �������� ���� ����
	wcex.lpszMenuName = (LPCSTR)menu; // ��������� �� ��� ����
	wcex.lpszClassName = szName; // ��������� �� ��� ������
	wcex.hIconSm = LoadIcon(hInst, (LPCSTR)icon); // ���������� ��������� ����������� (� ����)

	if (!RegisterClassEx(&wcex)) // �������� �� ���������� �������� ����
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", szName, MB_OK); // ����� ����-���� ��� ������
		return 0;
	}
	else return 1;
}