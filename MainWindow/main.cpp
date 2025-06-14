#include<Windows.h>
#include<cstdio>

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
	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);	//Sm - small
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
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
	CONST INT size = 256;
	CHAR sz_title[size] = {};
	sprintf(sz_title, "Размер окна %ix%i пикселей, положение левого верхнего угла: X {%i}, Y {%i}", screenWidth*3/4, screenHeight * 3 / 4, screenWidth/8, screenHeight/8);

	HWND hwnd = CreateWindowEx
	(
		NULL,	//ExStyle
		g_sz_CLASS_NAME,						//ClassName
		sz_title,						//WindowName (Title)
		WS_OVERLAPPEDWINDOW,					//Такой стиль задается для всех главных окон. Это окно будет родительским для других окон приложения
		screenWidth/8, screenHeight/8,			//Position
		screenWidth*3/4, screenHeight*3/4,		//Size
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

	return msg.message;
}

INT WINAPI WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
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