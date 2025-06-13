#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* G_SZ_VALUES[] = { "This","is","my","first","combo","box","Хорошо","живет","на","свете","Вини-пух" };
CONST INT size = 256;
CHAR sz_buffer[size] = {};
CHAR sz_message[size] = {};


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgRedact(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		HWND hList = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(G_SZ_VALUES) / sizeof(G_SZ_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)G_SZ_VALUES[i]);
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			if (i == -1)strcpy(sz_message, "Выберите ваше вариант");
			else
				sprintf(sz_message, "Вы выбрали элемент № %i со значением %s", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		case IDC_BUTTON_ADD:
		{
			DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_REDACT), NULL, DlgRedact, 0);
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			if (SendMessage(hList, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_DEL:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			int lbItem = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_DELETESTRING, lbItem, 0);
		}
		break;
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				HWND hList = GetDlgItem(hwnd, IDC_LIST);
				int lbItem = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
				DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_REDACT), NULL, DlgRedact, 0);
				//SendMessage(hList, LB_SETITEMDATA, lbItem, (LPARAM)sz_buffer);
				SendMessage(hList, LB_DELETESTRING, lbItem, 0);
				SendMessage(hList, LB_INSERTSTRING, lbItem, (LPARAM)sz_buffer);
			}
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK DlgRedact(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		SetFocus(hEdit);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SendMessage(hEdit, WM_GETTEXT, size, (LPARAM)sz_buffer);
		}
		EndDialog(hwnd, 0);
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}