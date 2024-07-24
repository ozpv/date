#include <Windows.h>

int random(void) {
	int r = 0;
	HCRYPTPROV c;

	CryptAcquireContext(&c, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	CryptGenRandom(c, sizeof(int), (BYTE*)&r);
	CryptReleaseContext(c, 0);

	return r & INT_MAX;
}

inline int random_range(int greaterthan, int lessthan) {
	return ((random() % (lessthan - greaterthan + 1)) + greaterthan);
}

LRESULT CALLBACK date_proc(
	HWND window_handle, 
	UINT message, 
	WPARAM message_info, 
	LPARAM param
) {
	switch (message) {
		case WM_CREATE: {
			HWND yes_button = CreateWindowExW(
				0,
				L"BUTTON",
				L"YES",
				WS_VISIBLE | WS_CHILD,
				52,
				120,
				60,
				20,
				window_handle,
				(HMENU)100,
				GetModuleHandleW(NULL),
				NULL
			);

			HWND no_button = CreateWindowExW(
				0,
				L"BUTTON",
				L"NO",
				WS_VISIBLE | WS_CHILD,
				266,
				120,
				60,
				20,
				window_handle,
				(HMENU)101,
				GetModuleHandleW(NULL),
				NULL
			);

			SendMessageW(yes_button, WM_SETFONT, GetStockObject(DEFAULT_GUI_FONT), TRUE);
			SendMessageW(no_button, WM_SETFONT, GetStockObject(DEFAULT_GUI_FONT), TRUE);

			break;
		} case WM_PAINT: {
			COLORREF color = RGB(255, 255, 255);
			PAINTSTRUCT paint;
			RECT rect;
			HDC hdc = BeginPaint(window_handle, &paint);
			GetClientRect(window_handle, &rect);
			SetDCBrushColor(hdc, color);
			
			FillRect(hdc, &rect, GetStockObject(DC_BRUSH));

			EndPaint(window_handle, &paint);

			hdc = GetWindowDC(window_handle);

			SetTextColor(hdc, 0x00000000);

			SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));

			SetBkMode(hdc, TRANSPARENT);

			rect.left = 30;
			rect.top = 110;

			DrawTextW(hdc, L"Would you like to go on a date with me?", 40, &rect, DT_CENTER);

			ReleaseDC(window_handle, hdc);
			break;
		} case WM_COMMAND: {
			if (LOWORD(message_info) == 101) {
				HWND button_handle = FindWindowExW(window_handle, NULL, L"BUTTON", L"NO");

				SetWindowPos(button_handle, NULL, random_range(0, 340), random_range(0, 480), 0, 0, SWP_NOSIZE);
				InvalidateRect(window_handle, NULL, TRUE);
			} else if (LOWORD(message_info) == 100) {
				SendMessageW(window_handle, WM_CLOSE, NULL, NULL);
				break;
			}
			break;
		} case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return DefWindowProcW(window_handle, message, message_info, param);
}

LRESULT CALLBACK thank_you_proc(
	HWND window_handle,
	UINT message,
	WPARAM message_info,
	LPARAM param
) {
	switch (message) {
		case WM_CREATE: {
			break;
		} case WM_PAINT: {
			COLORREF color = RGB(255, 255, 255);
			PAINTSTRUCT paint;
			RECT rect;
			HDC hdc = BeginPaint(window_handle, &paint);
			GetClientRect(window_handle, &rect);
			SetDCBrushColor(hdc, color);

			FillRect(hdc, &rect, GetStockObject(DC_BRUSH));

			EndPaint(window_handle, &paint);

			hdc = GetWindowDC(window_handle);

			SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));

			SetTextColor(hdc, 0x00000000);

			SetBkMode(hdc, TRANSPARENT);

			rect.left = 28;
			rect.top = 110;

			DrawTextW(hdc, L"I knew you would say yes, meet me tmrw at 8\nLispenard street", 61, &rect, DT_CENTER);

			ReleaseDC(window_handle, hdc);
			break;
		} case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return DefWindowProcW(window_handle, message, message_info, param);
}


/* remove redundancy */
int setup_window(HWND window_handle, WNDCLASSW wc) {
	SetWindowLongW(window_handle, GWL_STYLE, GetWindowLongW(window_handle, GWL_STYLE) & ~WS_MINIMIZEBOX);
	SetWindowLongW(window_handle, GWL_STYLE, GetWindowLongW(window_handle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	ShowWindow(window_handle, SW_SHOW);

	MSG message;

	while (GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	DestroyWindow(window_handle);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

int main(void) {
	WNDCLASSW window_class = {
		0,
		date_proc,
		0,
		0,
		GetModuleHandleW(NULL),
		NULL,
		LoadCursorW(NULL,IDC_ARROW),
		NULL,
		NULL,
		L"Date"
	};

	RegisterClassW(&window_class);

	HWND window_handle = CreateWindowExW(
		0, 
		window_class.lpszClassName, 
		L"Date?", 
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		400, 
		500, 
		HWND_DESKTOP, 
		NULL, 
		window_class.hInstance, 
		NULL
	);

	EnableMenuItem(GetSystemMenu(window_handle, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	
	setup_window(window_handle, window_class);

	window_class.lpfnWndProc = thank_you_proc;

	RegisterClassW(&window_class);

	window_handle = CreateWindowExW(
		0, 
		window_class.lpszClassName, 
		L"Date", 
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		400, 
		250, 
		HWND_DESKTOP, 
		NULL, 
		window_class.hInstance, 
		NULL
	);

	setup_window(window_handle, window_class);

	return 0;
}