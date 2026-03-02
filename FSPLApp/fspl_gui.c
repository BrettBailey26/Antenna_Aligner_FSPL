#include <windows.h>
#include <stdlib.h>
#include <math.h>

/*
 * Simple Win32 GUI for FSPL calculation.
 * Compile with: gcc -municode -o fspl_gui.exe fspl_gui.c -lgdi32 -luser32 -lkernel32 -lm
 * (or use cl.exe on MSVC)
 */

#define ID_DISTANCE 101
#define ID_FREQUENCY 102
#define ID_CALCULATE 103
#define ID_RESULT 104

static HWND hDistance, hFrequency, hResult;

static double calculate_fspl(double distance_m, double frequency_hz) {
    const double c = 299792458.0;
    double ratio = 4.0 * M_PI * distance_m * frequency_hz / c;
    return 20.0 * log10(ratio);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"Distance (m):", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwnd, NULL, NULL, NULL);
        hDistance = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 120, 10, 150, 20, hwnd, (HMENU)ID_DISTANCE, NULL, NULL);
        CreateWindowW(L"STATIC", L"Frequency (Hz):", WS_VISIBLE | WS_CHILD, 10, 40, 100, 20, hwnd, NULL, NULL, NULL);
        hFrequency = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 120, 40, 150, 20, hwnd, (HMENU)ID_FREQUENCY, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Calculate", WS_VISIBLE | WS_CHILD, 10, 70, 80, 25, hwnd, (HMENU)ID_CALCULATE, NULL, NULL);
        hResult = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 105, 260, 20, hwnd, (HMENU)ID_RESULT, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_CALCULATE) {
            wchar_t buf[64];
            GetWindowTextW(hDistance, buf, 64);
            double d = _wtof(buf);
            GetWindowTextW(hFrequency, buf, 64);
            double f = _wtof(buf);
            if (d <= 0 || f <= 0) {
                SetWindowTextW(hResult, L"Enter values > 0.");
            } else {
                double loss = calculate_fspl(d, f);
                wchar_t out[128];
                swprintf(out, 128, L"FSPL = %.2f dB", loss);
                SetWindowTextW(hResult, out);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[]  = L"FSPLWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"FSPL Calculator",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 180,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
