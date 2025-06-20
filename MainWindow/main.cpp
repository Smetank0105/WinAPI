﻿#include<Windows.h>
#include<cstdio>
#include"resource.h"

#define IDC_STATIC	1000
#define IDC_EDIT	1001
#define IDC_BUTTON	1002

CONST CHAR g_sz_CLASS_NAME[] = "My First Window";

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);	//cb - Count Bytes
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	//Внешнее оформление окна
	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_ATOM));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_BEER));	//Sm - small
	//wClass.hIcon = (HICON)LoadImage(hInstance, "Atom.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hIconSm = (HICON)LoadImage(hInstance, "Beer.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
	wClass.hCursor = (HCURSOR)LoadImage
	(
		hInstance, "Cursors\\Working In Background.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_LOADFROMFILE
	);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;

	if (RegisterClassEx(&wClass) == NULL)
	{
		MessageBox(NULL, "Class registration error", "", MB_OK | MB_ICONERROR);
	}

	//2) Создание окна:
	INT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	INT window_width = screenWidth * 3 / 4;
	INT window_height = screenHeight * 3 / 4;
	INT window_start_x = screenWidth / 8;
	INT window_start_y = screenHeight / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL,									//ExStyle
		g_sz_CLASS_NAME,						//ClassName
		g_sz_CLASS_NAME,						//WindowName (Title)
		WS_OVERLAPPEDWINDOW,					//Такой стиль задается для всех главных окон. Это окно будет родительским для других окон приложения
		window_start_x, window_start_y,			//Position
		window_width, window_height,			//Size
		NULL,									//ParentWindow
		NULL,									//Строка меню для главного окна, или же ID_-ресурса для дочернего окна
		hInstance,								//Это экземпляр *.exe-файла нашей программы
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation failed", "", MB_OK | MB_ICONERROR);
	}
	ShowWindow(hwnd, nCmdShow);			//Задает режим отображения окна: Развернуто на весь экран, Свернуто в окно, Свернуто на панель задач
	UpdateWindow(hwnd);					//Прорисовывает рабочую область окна

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		CreateWindowEx
		(
			NULL,
			"Static",
			"Этот StaticText создан при помощи функции CreateWindowEx().",
			//WS_ - Window Style
			WS_CHILD | WS_VISIBLE,
			10, 10,
			550, 25,
			hwnd,
			(HMENU)IDC_STATIC,	//ResourceID
			GetModuleHandle(NULL), //hInstance
			NULL
		);
		CreateWindowEx
		(
			NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			//ES_ - Edit Style
			10, 50,
			550, 22,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "Применить",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			450, 75,
			110, 25,
			hwnd,
			(HMENU)IDC_BUTTON,
			GetModuleHandle(NULL),
			NULL
		);
		break;
	case WM_MOVE:
	case WM_SIZE:
	{
		RECT window_rect;
		GetWindowRect(hwnd, &window_rect);
		INT window_width = window_rect.right - window_rect.left;
		INT window_height = window_rect.bottom - window_rect.top;
		CONST INT size = 256;
		CHAR sz_title[size] = {};
		sprintf(
			sz_title,
			"%s - Position:%ix%i, Size:%ix%i",
			g_sz_CLASS_NAME,
			window_rect.left, window_rect.top,
			window_width, window_height
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			HWND hStatic = GetDlgItem(hwnd, IDC_STATIC);
			CONST INT SIZE = 1024;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);	//String Zero (NULL-Terminate Line)
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
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