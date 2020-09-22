#define OEMRESOURCE
#include <windows.h>

HINSTANCE   g_instance, g_instance_arrow;
HCURSOR     g_hc_ibeam, g_hc_arrow;
UINT_PTR    g_timer = NULL;
DWORD       g_layout = 0;

void CALLBACK UpdateTimer(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    int layout = (int)GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)) & 0xFFFF;
    int caps = GetKeyState(VK_CAPITAL) & 0xFFFF;
   
    if (caps) {
        HCURSOR hc_new = LoadCursor(g_instance, MAKEINTRESOURCE((layout * 10) + 2));
        HCURSOR hc_new_arrow = LoadCursor(g_instance_arrow, MAKEINTRESOURCE((layout * 10) + 1));

        if (hc_new)
        {
            SetSystemCursor(hc_new, OCR_IBEAM);
        }
        else
        {
            SetSystemCursor(CopyCursor(g_hc_ibeam), OCR_IBEAM);
        }
        if (hc_new_arrow)
        { 
            SetSystemCursor(hc_new_arrow, OCR_NORMAL);
        }
        else
        {
            SetSystemCursor(CopyCursor(g_hc_arrow), OCR_NORMAL);
        }
    }
    
    if (g_layout != layout && !caps)
    {
        HCURSOR hc_new = LoadCursor(g_instance, MAKEINTRESOURCE(layout));
        HCURSOR hc_new_arrow = LoadCursor(g_instance_arrow, MAKEINTRESOURCE(layout * 10));

        if (hc_new)
        {
            SetSystemCursor(hc_new, OCR_IBEAM);
        }
        else
        {
            SetSystemCursor(CopyCursor(g_hc_ibeam), OCR_IBEAM);
        }
        if (hc_new_arrow)
        {
            SetSystemCursor(hc_new_arrow, OCR_NORMAL);
        }
        else
        {
            SetSystemCursor(CopyCursor(g_hc_arrow), OCR_NORMAL);
        }
    }
}

int Main()
{
    HANDLE mutex = CreateMutex(NULL, FALSE, "LangCursor");
    if (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED) return 1;

    g_hc_ibeam = CopyCursor(LoadCursor(NULL, IDC_IBEAM));
    if (!g_hc_ibeam) return 1;

    //g_hc_arrow
    g_hc_arrow = CopyCursor(LoadCursor(NULL, IDC_ARROW));
    if (!g_hc_arrow) return 1;


    g_instance = GetModuleHandle(NULL);
    g_instance_arrow = GetModuleHandle(NULL);
    g_timer = SetTimer(NULL, g_timer, 200, UpdateTimer);
    if (!g_timer) return 1;

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyCursor(g_hc_ibeam);
    DestroyCursor(g_hc_arrow);
    return 0;
}

EXTERN_C void WINAPI WinMainCRTStartup()
{
    ExitProcess(Main());
}
