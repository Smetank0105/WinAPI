#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_CLASS_NAME[] = "MyNotepad";

CONST CHAR* g_sz_MENU_NAME[] = { "FILE","EDIT","STYLE" };
CONST CHAR* g_sz_BUTTON_FILE_NAME[] = { "OPEN FILE", "SAVE FILE", "SAVE AS ...", "EXIT" };
CONST CHAR* g_sz_BUTTON_EDIT_NAME[] = { "UNDO", "CUT", "COPY", "PASTE", "DELETE" };
CONST CHAR* g_sz_BUTTON_STYLE_NAME[] = { "BOLD", "ITALIC", "UNDERLINE" };
CONST INT g_i_BUTTONS_COUNT[] = { 4, 5, 3 };
CONST CHAR** g_sz_BUTTONS_NAME[] = { g_sz_BUTTON_FILE_NAME, g_sz_BUTTON_EDIT_NAME, g_sz_BUTTON_STYLE_NAME };

CONST INT g_i_PANEL_VSIZE = 30;
CONST INT g_i_BUTTON_HSIZE = 66;
CONST INT g_i_BUTTON_VSIZE = 22;
CONST INT g_i_INTERVAL = 4;

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
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

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class Registraion Error", "", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Create Window Error", "", MB_OK | MB_ICONERROR);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

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
		for (INT i = 0; i < 3; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_sz_MENU_NAME[i],
				WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				g_i_INTERVAL + (g_i_INTERVAL + g_i_BUTTON_HSIZE) * i, g_i_INTERVAL, g_i_BUTTON_HSIZE, g_i_BUTTON_VSIZE,
				hwnd, (HMENU)(IDC_BUTTON_FILE + i), GetModuleHandle(NULL), NULL
			);
		}

		RECT rect;
		GetClientRect(hwnd, &rect);
		HWND hRichEdit = CreateWindowEx
		(
			NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL |
			ES_NOHIDESEL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE,
			0, g_i_PANEL_VSIZE, rect.right - rect.left, rect.bottom - rect.top - g_i_PANEL_VSIZE,
			hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL
		);
		if (hRichEdit == NULL) return FALSE;
		SetFocus(hRichEdit);
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) >= IDC_BUTTON_FILE && LOWORD(wParam) <= IDC_BUTTON_STYLE)
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam)), BM_SETSTATE, TRUE, 0);

			HMENU hFileMenu = CreatePopupMenu();
			INT index = LOWORD(wParam) - IDC_BUTTON_FILE;

			for (INT i = 0; i < g_i_BUTTONS_COUNT[index]; i++)
			{
				AppendMenu(hFileMenu, MF_BYPOSITION | MF_STRING, (BM_FILE_OPEN + i + index * 10), g_sz_BUTTONS_NAME[index][i]);
			}

			RECT rect;
			GetWindowRect(GetDlgItem(hwnd,LOWORD(wParam)), &rect);
			BOOL item = TrackPopupMenuEx(hFileMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, rect.left, rect.bottom, hwnd, NULL);
			if (item == BM_FILE_EXIT) SendMessage(hwnd, WM_DESTROY, 0, 0);

			DestroyMenu(hFileMenu);
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam)), BM_SETSTATE, FALSE, 0);
		}
	}
	break;
	case WM_SIZE:
	{
		MoveWindow(GetDlgItem(hwnd,IDC_EDIT), 0, g_i_PANEL_VSIZE, LOWORD(lParam), HIWORD(lParam) - g_i_PANEL_VSIZE, TRUE);
	}
	break;
	case WM_DESTROY:
		FreeConsole();
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