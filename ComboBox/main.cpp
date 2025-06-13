﻿#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* G_SZ_VALUES[] = { "This","is","my","first","combo","box","Хорошо","живет","на","свете","Вини-пух"};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdline, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		//GetModulHandle(NULL) - возвращает hInstance нашего *.exe - файла
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		for (int i = 0; i < sizeof(G_SZ_VALUES) / sizeof(G_SZ_VALUES[0]); i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)G_SZ_VALUES[i]);
		}
	}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			CHAR sz_message[SIZE] = {};
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);	//CB_GETCURSEL - ComboBox get current selection
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_buffer);
			if (i == -1)strcpy(sz_message, "Выберите ваш вариант");
			//strcat(dst,src); dst - строка получсатель, src - строка источник
			else
				sprintf(sz_message, "Вы выбрали элемент № %i со значением '%s'", i, sz_buffer);
			//sz_message - это строка будет содержать результат форматирования
			//"Вы выбрали..." - это строка содержит формат, в ней указывается 
			//куда именно нужно вставить значения, идущие после этой строки, 
			//в данном случае это 'i' и 'sz_buffer'
			//sprintf() - это функция с произвольным числом параметров
			//спецификатор '%i' заменяется целочисленным значением 
			//спецификатор '%s' заменяется строкой
			//'%e' - дробное число и тд (подробнее в документации sprintf/printf)
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}