
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "../../renderers/win32_gdi/clay_renderer_gdi.c"

#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "../shared-layouts/clay-video-demo.c"

ClayVideoDemo_Data demo_data;

#define APPNAME "Clay GDI Example"
char szAppName[] = APPNAME; // The name of this application
char szTitle[] = APPNAME;   // The title bar text

void CenterWindow(HWND hWnd);

long lastMsgTime = 0;
bool ui_debug_mode;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {

    // ----------------------- first and last
    case WM_CREATE:
        CenterWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_MOUSEWHEEL: // scrolling data
    {
        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        // todo: i think GetMessageTime can roll over, so something like if(lastmsgtime > now) ... may be needed
        long now = GetMessageTime();
        float dt = (now - lastMsgTime) / 1000.00;

        lastMsgTime = now;

        // little hacky hack to make scrolling *feel* right
        if (abs(zDelta) > 100)
        {
            zDelta = zDelta * .012;
        }

        Clay_UpdateScrollContainers(true, (Clay_Vector2){.x = 0, .y = zDelta}, dt);

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event
        break;
    }
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MOUSEMOVE: // mouse events
    {
        short mouseX = GET_X_LPARAM(lParam);
        short mouseY = GET_Y_LPARAM(lParam);
        short mouseButtons = LOWORD(wParam);

        Clay_SetPointerState((Clay_Vector2){mouseX, mouseY}, mouseButtons & 0b01);

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event
        break;
    }

    case WM_SIZE: // resize events
    {

        RECT r = {0};
        if (GetClientRect(hwnd, &r))
        {
            Clay_Dimensions dim = (Clay_Dimensions){.height = r.bottom - r.top, .width = r.right - r.left};
            Clay_SetLayoutDimensions(dim);
        }

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event

        break;
    }

    case WM_KEYDOWN:
        if (VK_ESCAPE == wParam)
        {
            DestroyWindow(hwnd);
            break;
        }

        if (wParam == VK_F12)
        {
            Clay_SetDebugModeEnabled(ui_debug_mode = !ui_debug_mode);
            break;
        }

        printf("Key Pressed: %d\r\n", wParam);
        InvalidateRect(hwnd, NULL, false); // force a wm_paint event
        break;

    // ----------------------- render
    case WM_PAINT:
    {
        Clay_RenderCommandArray renderCommands = ClayVideoDemo_CreateLayout(&demo_data);
        Clay_Win32_Render(hwnd, renderCommands);
        break;
    }

    // ----------------------- let windows do all other stuff
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

bool didAllocConsole = false;

void HandleClayErrors(Clay_ErrorData errorData)
{
    if (!didAllocConsole)
    {
        didAllocConsole = AllocConsole();
    }

    printf("Handle Clay Errors: %s\r\n", errorData.errorText.chars);
}

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;
    WNDCLASS wc;
    HWND hwnd;

    demo_data = ClayVideoDemo_Initialize();

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory));
    Clay_Initialize(clayMemory, (Clay_Dimensions){.width = 800, .height = 600}, (Clay_ErrorHandler){HandleClayErrors}); // This final argument is new since the video was published
    Clay_SetMeasureTextFunction(Clay_Win32_MeasureText, NULL); 

    ZeroMemory(&wc, sizeof wc);
    wc.hInstance = hInstance;
    wc.lpszClassName = szAppName;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (FALSE == RegisterClass(&wc))
        return 0;


    hwnd = CreateWindow(
        szAppName,
        szTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800, // CW_USEDEFAULT,
        600, // CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);

    if (hwnd == NULL)
        return 0;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void CenterWindow(HWND hwnd_self)
{
    HWND hwnd_parent;
    RECT rw_self, rc_parent, rw_parent;
    int xpos, ypos;

    hwnd_parent = GetParent(hwnd_self);
    if (NULL == hwnd_parent)
        hwnd_parent = GetDesktopWindow();

    GetWindowRect(hwnd_parent, &rw_parent);
    GetClientRect(hwnd_parent, &rc_parent);
    GetWindowRect(hwnd_self, &rw_self);

    xpos = rw_parent.left + (rc_parent.right + rw_self.left - rw_self.right) / 2;
    ypos = rw_parent.top + (rc_parent.bottom + rw_self.top - rw_self.bottom) / 2;

    SetWindowPos(
        hwnd_self, NULL,
        xpos, ypos, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

//+---------------------------------------------------------------------------
