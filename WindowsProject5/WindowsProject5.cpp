// WindowsProject5.cpp: Определяет точку входа для приложения.
//1fgrgfg

#include "stdafx.h"
#include "WindowsProject5.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK contactdlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK addcontactdlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK newport(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadFunc(LPVOID lpv);
HWND mainw;
contact contacts[99];
int avablecontact = 0;
int selectedaddress = 0;
int myport = 5556;
SOCKET seller;
std::wstring history;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT5));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT5);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND button;
	static HWND hEditControl;
	static HANDLE hreadA;
	RECT clrect;
    switch (message)
    {
	case WM_CREATE:
		mainw = hWnd;
		GetClientRect(hWnd, &clrect);
		button = CreateWindow(L"button", L"Отправить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, clrect.right - 130, clrect.bottom - 130, 120, 30, hWnd, (HMENU)10000, hInst, NULL);
		hEditControl = CreateWindowEx(NULL, _T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, clrect.left + 10, clrect.bottom - 160, clrect.right-clrect.left-160, 90, hWnd, (HMENU)10000, hInst, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case IDM_CONECT:
				WSADATA wsaData;
				if (WSAStartup(0x202, &wsaData) != 0) {
					exit(WSAGetLastError());
				}
				sockaddr_in myAddr;
				memset(myAddr.sin_zero, 0, sizeof(myAddr.sin_zero));
				myAddr.sin_family = AF_INET;
				myAddr.sin_port = htons(myport);
				myAddr.sin_addr.s_addr = INADDR_ANY;
				seller = socket(AF_INET, SOCK_DGRAM, 0);
				if (bind(seller, (SOCKADDR *)&myAddr, sizeof(myAddr)) != 0) {
					exit(WSAGetLastError());
				}
				hreadA = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
				break;
			case IDM_CONTACT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, contactdlg);
				break;
			case IDM_PORT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, newport);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
			if (lParam == (LPARAM)button) {
				wchar_t wstnum[1024];
				//for (int r = 0;r < 1024;r++) {
				//	wstnum[r] = '№';
				//}
				GetWindowText(hEditControl, wstnum, 1024);
				SetWindowText(hEditControl, L"");
				//GetDlgItemText(hEditControl, IDC_EDIT3, stnum, 18);
				//std::cin.getline(stnum, sizeof(stnum));
				char stnum[1024];
				WideCharToMultiByte(CP_ACP, 0, wstnum, 1024, stnum, 1024, 0, 0);
				sendto(seller, stnum, sizeof(stnum), 0, (SOCKADDR *)&contacts[selectedaddress].getaddr(), sizeof(contacts[selectedaddress].getaddr()));
				//history = history + "я:" + wstnum;
				history.append(L"я:");
				history.append(wstnum);
				history.append(L"\x0A");
				RECT rect;
				GetClientRect(hWnd, &rect);
				InvalidateRect(hWnd, &rect, true);
				//UpdateWindow(hWnd);
				}
				else {
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
            }
        }
        break;
	case WM_SIZE:
		DestroyWindow(button);
		DestroyWindow(hEditControl);
		GetClientRect(hWnd, &clrect);
		button = CreateWindow(L"button", L"Отправить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, clrect.right - 130, clrect.bottom - 130, 120, 30, hWnd, (HMENU)10000, hInst, NULL);
		hEditControl = CreateWindowEx(NULL, _T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, clrect.left + 10, clrect.bottom - 160, clrect.right - clrect.left - 160, 90, hWnd, (HMENU)10000, hInst, NULL);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC..
			RECT rect;
			GetClientRect(hWnd, &rect);
			DrawText(hdc, (LPWSTR)history.c_str(), -1, &rect, DT_LEFT);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
BOOL CALLBACK contactdlg (HWND hDlg, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		for (int p = 0;p < avablecontact;p++) {
			SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)contacts[p].name);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, addcontactdlg);
				SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)contacts[avablecontact-1].name);
				break;
		case IDC_BUTTON2:
			for (int mn = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);mn < avablecontact-1;mn++) {
				if (mn == -1) {
					exit(GetLastError());
				}
				contacts[mn] = contacts[mn + 1];
			}
			avablecontact--;
			SendDlgItemMessage(hDlg, IDC_LIST2, LB_RESETCONTENT, 0, 0);
			for (int p = 0;p < avablecontact;p++) {
				SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)contacts[p].name);
			}
			break;
		case IDOK:
			selectedaddress=SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		}
	}
	return FALSE;
}
BOOL CALLBACK addcontactdlg(HWND hDlg, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			wchar_t ipp[18];
			char ippp[18];
			GetDlgItemText(hDlg, IDC_EDIT3, ipp, 18);
			WideCharToMultiByte(CP_ACP, 0, ipp, 18,ippp,18, 0, 0);
			contacts[avablecontact].setip(ippp);
			//GetDlgItemInt(hDlg, IDC_EDIT2,NULL,false);
			contacts[avablecontact].setport(GetDlgItemInt(hDlg, IDC_EDIT1, NULL, false));
			wchar_t nme[99];
			GetDlgItemText(hDlg, IDC_EDIT2, nme, 18);
			contacts[avablecontact].setname(nme);
			avablecontact++;
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		}
	}
	return FALSE;
}
DWORD WINAPI ThreadFunc(LPVOID lpv) {
	DWORD dwResult = 0;
	sockaddr_in wtfAddr;
	while (1) {
		char getstnum[1024];
		int youAddrSize = sizeof(wtfAddr);
		ioctlsocket(seller, FIONBIO, 0);
		recvfrom(seller, getstnum, sizeof(getstnum), 0, (SOCKADDR *)&wtfAddr, &youAddrSize);
		history.append(contacts[selectedaddress].name);
		history.append(L": ");
		wchar_t wgetstnum[1024];
		MultiByteToWideChar(CP_ACP,0, getstnum, 1024, wgetstnum, 1024);
		history.append((LPWSTR)wgetstnum);
		history.append(L"\x0A");
		RECT rect;
		GetClientRect(mainw, &rect);
		InvalidateRect(mainw, &rect, true);
		/*
		for (int i = 0;i < sizeof(getstnum);i++) {
			if (getstnum[i] != '№') {
				std::cout << getstnum[i];
			}
		}
		std::cout << std::endl;
		*/
	}
	return dwResult;
}
BOOL CALLBACK newport(HWND hDlg, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			myport = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, false);
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		}
	}
	return FALSE;
}