#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_CLASS_NAME[] = "MyCalc";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wClass)) {
		MessageBox(NULL, "Class Registration Error", "", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		530, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		MessageBox(NULL, "Create Window Error", "", MB_OK | MB_ICONERROR);
	}
	ShowWindow(hwnd, nCmdShow);		//https://learn.microsoft.com/ru-ru/windows/win32/api/winuser/nf-winuser-showwindow
	UpdateWindow(hwnd);				// taskkill /f /im calc.exe

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		CreateWindowEx
		(
			NULL, "Edit", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			500, 22,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "1",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 160,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_1,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "2",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			110, 160,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_2,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "3",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			210, 160,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_3,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "4",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 110,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_4,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "5",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			110, 110,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_5,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "6",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			210, 110,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_6,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "7",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 60,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_7,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "8",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			110, 60,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_8,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "9",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			210, 60,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_9,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			10, 210,
			200, 50,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			210, 210,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "+",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			310, 60,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_PLUS,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "-",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			310, 110,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_MINUS,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "*",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			310, 160,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_ASTER,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "/",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			310, 210,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_SLASH,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "BSP",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			410, 60,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "CLR",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			410, 110,
			100, 50,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			410, 160,
			100, 100,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_0:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "0");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_1:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "1");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_2:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "2");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_3:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "3");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_4:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "4");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_5:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "5");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_6:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "6");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_7:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "7");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_8:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "8");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_9:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, "9");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_POINT:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			strcat(sz_buffer, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_CLR:
		{
			HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditDisplay, WM_SETTEXT, 0, 0);
		}
		break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}