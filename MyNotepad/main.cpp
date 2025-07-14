#include<Windows.h>
#include<Richedit.h>
#include"resource.h"

CONST CHAR g_sz_CLASS_NAME[] = "MyNotepad";

CONST CHAR* g_sz_MENU_NAME[] = { "FILE","EDIT","STYLE" };
CONST CHAR* g_sz_BUTTON_FILE_NAME[] = { "NEW", "OPEN FILE", "SAVE FILE", "SAVE AS ...", "EXIT" };
CONST CHAR* g_sz_BUTTON_EDIT_NAME[] = { "UNDO", "SELECT ALL", "CUT", "COPY", "PASTE", "DELETE" };
CONST CHAR* g_sz_BUTTON_STYLE_NAME[] = { "BOLD", "ITALIC", "UNDERLINE" };
CONST INT g_i_BUTTONS_COUNT[] = { 5, 6, 3 };
CONST CHAR** g_sz_BUTTONS_NAME[] = { g_sz_BUTTON_FILE_NAME, g_sz_BUTTON_EDIT_NAME, g_sz_BUTTON_STYLE_NAME };

CONST INT g_i_PANEL_VSIZE = 30;
CONST INT g_i_BUTTON_HSIZE = 66;
CONST INT g_i_BUTTON_VSIZE = 22;
CONST INT g_i_INTERVAL = 4;

CHAR sz_filepath[MAX_PATH] = {};

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID Open_File(HWND hwnd, CONST CHAR* path);
VOID Save_File(HWND hwnd, CONST CHAR* path);
VOID SaveAs_File(HWND hwnd, CONST CHAR* path);

HINSTANCE hLib;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	hLib = LoadLibrary("riched20.dll");

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
			NULL, RICHEDIT_CLASS, "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL |
			ES_NOHIDESEL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE,
			0, g_i_PANEL_VSIZE, rect.right - rect.left, rect.bottom - rect.top - g_i_PANEL_VSIZE,
			hwnd, (HMENU)IDC_RICHEDIT, GetModuleHandle(NULL), NULL
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
				AppendMenu(hFileMenu, MF_BYPOSITION | MF_STRING, (BM_FILE_NEW + i + index * 10), g_sz_BUTTONS_NAME[index][i]);
			}

			RECT rect;
			GetWindowRect(GetDlgItem(hwnd, LOWORD(wParam)), &rect);
			BOOL item = TrackPopupMenuEx(hFileMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, rect.left, rect.bottom, hwnd, NULL);

			if (item == BM_FILE_NEW) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_SETTEXT, 0, (LPARAM)"\0");
			if (item == BM_FILE_OPEN) Open_File(hwnd, sz_filepath);
			if (item == BM_FILE_SAVE) Save_File(hwnd, sz_filepath);
			if (item == BM_FILE_SAVEAS) SaveAs_File(hwnd, sz_filepath);
			if (item == BM_FILE_EXIT) SendMessage(hwnd, WM_DESTROY, 0, 0);
			if (item >= BM_STYLE_BOLD && item <= BM_STYLE_UNDERLINE)
			{
				CHARFORMAT cf;
				cf.cbSize = sizeof(cf);
				SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

				if (item == BM_STYLE_BOLD) { cf.dwMask = CFM_BOLD; cf.dwEffects ^= CFE_BOLD; }
				if (item == BM_STYLE_ITALIC) { cf.dwMask = CFM_ITALIC; cf.dwEffects ^= CFE_ITALIC; }
				if (item == BM_STYLE_UNDERLINE) { cf.dwMask = CFM_UNDERLINE; cf.dwEffects ^= CFE_UNDERLINE; }

				SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
			}
			if (item == BM_EDIT_UNDO) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_UNDO, 0, 0);
			if (item == BM_EDIT_SELECTALL)
			{
				CHARRANGE charR;
				charR.cpMin = 0;
				charR.cpMax = -1;
				SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_EXSETSEL, 0, (LPARAM)&charR);
			}
			if (item == BM_EDIT_CUT) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_CUT, 0, 0);
			if (item == BM_EDIT_COPY) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_COPY, 0, 0);
			if (item == BM_EDIT_PASTE) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_PASTE, 0, 0);
			if (item == BM_EDIT_DELETE) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_CLEAR, 0, 0);

			DestroyMenu(hFileMenu);
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam)), BM_SETSTATE, FALSE, 0);
			SetFocus(GetDlgItem(hwnd, IDC_RICHEDIT));
		}
	}
	break;
	case WM_CONTEXTMENU:
	{
		HMENU hCMenu = CreatePopupMenu();
		for (INT i = BM_EDIT_UNDO; i <= BM_EDIT_DELETE; i++)
		{
			AppendMenu(hCMenu, MF_BYPOSITION | MF_STRING, i, g_sz_BUTTON_EDIT_NAME[i - BM_EDIT_UNDO]);
		}

		BOOL item = TrackPopupMenuEx(hCMenu, TPM_RETURNCMD | TPM_RIGHTALIGN | TPM_BOTTOMALIGN, LOWORD(lParam), HIWORD(lParam), hwnd, NULL);
		if (item == BM_EDIT_UNDO) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_UNDO, 0, 0);
		if (item == BM_EDIT_SELECTALL)
		{
			CHARRANGE charR;
			charR.cpMin = 0;
			charR.cpMax = -1;
			SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), EM_EXSETSEL, 0, (LPARAM)&charR);
		}
		if (item == BM_EDIT_CUT) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_CUT, 0, 0);
		if (item == BM_EDIT_COPY) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_COPY, 0, 0);
		if (item == BM_EDIT_PASTE) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_PASTE, 0, 0);
		if (item == BM_EDIT_DELETE) SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_CLEAR, 0, 0);

		DestroyMenu(hCMenu);
	}
	break;
	case WM_SIZE:
		MoveWindow(GetDlgItem(hwnd, IDC_RICHEDIT), 0, g_i_PANEL_VSIZE, LOWORD(lParam), HIWORD(lParam) - g_i_PANEL_VSIZE, TRUE);
		break;
	case WM_SETFOCUS:
		SetFocus(GetDlgItem(hwnd, IDC_RICHEDIT));
		break;
	case WM_DESTROY:
		FreeConsole();
		FreeLibrary(hLib);
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

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, MAX_PATH, (LPARAM)sz_filepath);
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

VOID Open_File(HWND hwnd, CONST CHAR* path)
{
	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_FILE), BM_SETSTATE, FALSE, 0);
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgProc, 0);
	HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		MessageBox(hwnd, "File not found", "", MB_OK | MB_ICONERROR);
		return;
	}
	DWORD dwRead;
	DWORD size;
	size = GetFileSize(file, NULL);
	LPSTR sz_buffer = (LPSTR)malloc(size);
	ReadFile(file, sz_buffer, size, &dwRead, NULL);
	SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_SETTEXT, 0, (LPARAM)sz_buffer);
	sz_buffer = {};
	free(sz_buffer);
	CloseHandle(file);
}

VOID Save_File(HWND hwnd, CONST CHAR* path)
{
	HANDLE file = CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		SaveAs_File(hwnd, sz_filepath);
		return;
	}
	DWORD dwWrite;
	DWORD size = 1 + GetWindowTextLength(GetDlgItem(hwnd, IDC_RICHEDIT));
	LPSTR sz_buffer = (LPSTR)malloc(size);
	SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_GETTEXT, size, (LPARAM)sz_buffer);
	WriteFile(file, sz_buffer, size, &dwWrite, NULL);
	sz_buffer = {};
	free(sz_buffer);
	CloseHandle(file);
}

VOID SaveAs_File(HWND hwnd, CONST CHAR* path)
{
	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_FILE), BM_SETSTATE, FALSE, 0);
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)DlgProc, 0);
	HANDLE file = CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwWrite;
	DWORD size = 1 + GetWindowTextLength(GetDlgItem(hwnd, IDC_RICHEDIT));
	LPSTR sz_buffer = (LPSTR)malloc(size);
	SendMessage(GetDlgItem(hwnd, IDC_RICHEDIT), WM_GETTEXT, size, (LPARAM)sz_buffer);
	WriteFile(file, sz_buffer, size, &dwWrite, NULL);
	sz_buffer = {};
	free(sz_buffer);
	CloseHandle(file);
}