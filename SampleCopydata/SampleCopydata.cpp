// SampleCopydata.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SampleCopydata.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND OtherhWnd;
TCHAR *pSavedString;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAMPLECOPYDATA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMPLECOPYDATA));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMPLECOPYDATA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SAMPLECOPYDATA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   OtherhWnd = FindWindow(szWindowClass, NULL);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 250, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COPYDATA:
	{
		COPYDATASTRUCT *pMessageCDS = (PCOPYDATASTRUCT)lParam;
		// first save the string
		pSavedString = (TCHAR*)malloc(pMessageCDS->cbData);
		memcpy_s(pSavedString, pMessageCDS->cbData, pMessageCDS->lpData, pMessageCDS->cbData);
		// now use it now and later
		SetDlgItemTextW(hWnd, IDC_MESSAGE_EDIT, pSavedString);
	}
	break;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_SEND_BUTTON:
			{
				TCHAR timeString[30];
				time_t secs = time(0);
				struct tm local;
				localtime_s(&local, &secs);
				swprintf(timeString, sizeof(timeString), L"%02d:%02d:%02d", local.tm_hour, local.tm_min, local.tm_sec);
				COPYDATASTRUCT cds{};
				cds.dwData = 1;
				cds.cbData = (wcslen(timeString)+1)*sizeof(TCHAR);
				cds.lpData = (PVOID)timeString;
				SendMessage(OtherhWnd, WM_COPYDATA, NULL, (LPARAM)&cds);
			}
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_CREATE:
		{
		TCHAR DirectionReceive[] = L"I will receive";
		TCHAR DirectionSend[] = L"I will send";
		TCHAR *Direction;
		if (OtherhWnd == NULL)
			Direction = DirectionReceive;
		else
			Direction = DirectionSend;
		CreateWindowExW(
				0,
				L"STATIC",
				Direction,
				WS_CHILD | WS_VISIBLE,
				0, 0, 100, 16,
				hWnd,
				(HMENU)IDC_STATIC,
				hInst,
				NULL);
			HWND hwndEdit = CreateWindowExW(
				0,
				L"EDIT",
				L"Message!",
				WS_CHILD | WS_VISIBLE | ES_LEFT,
				10, 50, 100, 20,
				hWnd,
				(HMENU)IDC_MESSAGE_EDIT,
				hInst,
				NULL);
			if (OtherhWnd == NULL)
				return 0;
			CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"Send",      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				10,         // x position 
				100,         // y position 
				100,        // Button width
				20,        // Button height
				hWnd,     // Parent window
				(HMENU)IDC_SEND_BUTTON,
				hInst,
				NULL);      // Pointer not needed.
		}
		return 0;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
