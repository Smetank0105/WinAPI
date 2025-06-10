#include<Windows.h>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//#define MESSAGE_BOX

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdline, INT nCmdShow)
{
#ifdef MESSAGE_BOX
	MessageBox
	(
		NULL,
		"Hello WinAPI\nЭто самое простое окно - окно сообщения (MessageBox)",
		"Привет!",
		MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_HELP
		| MB_DEFBUTTON3
		| MB_SYSTEMMODAL
	);
#endif // MESSAGE_BOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	//выполняется один раз при запуске окна
	{
		SendDlgItemMessage(hwnd, IDC_EDIT_LOGIN, WM_SETTEXT, 0, (LPARAM)"Введите имя пользователя");
	}
	break;
	case WM_COMMAND:	//обрабатывает нажатие кнопок, перемещения мыши и тд
		switch (LOWORD(wParam)) {
		case IDC_EDIT_LOGIN:
			if (HIWORD(wParam) == EN_SETFOCUS)
				SendDlgItemMessage(hwnd, IDC_EDIT_LOGIN, WM_SETTEXT, 0, 0);
			if (!SendDlgItemMessage(hwnd,IDC_EDIT_LOGIN,EM_LINELENGTH,0,0))
			{
				if (HIWORD(wParam) == EN_KILLFOCUS)
					SendDlgItemMessage(hwnd, IDC_EDIT_LOGIN, WM_SETTEXT, 0, (LPARAM)"Введите имя пользователя");
			}
			break;
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDOK:
			MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:		//отрабатывает при нажатии на кнопку "закрыть" Х
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
