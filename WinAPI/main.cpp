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
		"Hello WinAPI\n��� ����� ������� ���� - ���� ��������� (MessageBox)",
		"������!",
		MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_HELP
		| MB_DEFBUTTON3
		| MB_SYSTEMMODAL
	);
#endif // MESSAGE_BOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	//����������� ���� ��� ��� ������� ����
		break;
	case WM_COMMAND:	//������������ ������� ������, ����������� ���� � ��
		switch (LOWORD(wParam)) {
		case IDOK:
			MessageBox(hwnd, "���� ������ ������ ��", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:		//������������ ��� ������� �� ������ "�������" �
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
