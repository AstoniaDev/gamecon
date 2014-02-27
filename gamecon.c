/*
 * gamecon.c
 * A C library that allows students to write simple Windows GUI games
 * Created by J. Nan in August, 2008
 */
#include "gamecon.h"
#include <windowsx.h>
#include <windows.h>

static const char cst_szClassName[]  = "GameCon20090828";
static const int LEFT  = 20;
static const int TOP   = 20;
static const int PANEL = 150;

static int gc_initialized = 0;
static int gc_ready	  = 0;
static HINSTANCE gc_hInstance = NULL;
static HWND gc_hwnd	      = NULL;

static int gc_width;
static int gc_height;
static int gc_blockSize;
static COLORREF* gc_frontBuffer = 0;
static COLORREF* gc_backBuffer  = 0;

static int gc_buttonUp;
static int gc_buttonDown;
static int gc_buttonLeft;
static int gc_buttonRight;
static int gc_buttonA;
static int gc_buttonB;
static int gc_buttonC;
static int gc_buttonD;
static int gc_buttonReset;
static HANDLE gc_hButtonEvent = NULL;

BOOL Register();
DWORD WINAPI UI(LPVOID lpParam);
HWND Create(const char* title);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
void GameCon_OnDestroy(HWND hwnd);
void GameCon_OnPaint(HWND hwnd);
void GameCon_OnKey(HWND hwnd,UINT vk,BOOL fDown,int cRepeat,UINT flags);

int GC_Init(const char* title, int width, int height, int blockSize)
{
    if(gc_initialized)
	return E_ALREADY_INITIALIZED;

    if(!gc_ready)
    {
	gc_hInstance = GetModuleHandle(NULL);
	if(!Register())
	    return E_INIT;

	gc_ready = 1;
    }

    if(width <= 0 || height <= 0 || blockSize <= 0)
	return E_PARAMETER;

    gc_width = width;
    gc_height = height;
    gc_blockSize = blockSize;

    int bufSize = sizeof(COLORREF) * width * height;

    gc_frontBuffer = (COLORREF*)malloc(bufSize);
    gc_backBuffer = (COLORREF*)malloc(bufSize);
    if(gc_frontBuffer == 0 || gc_backBuffer == 0)
	return GC_Finalize(), E_INIT;

    memset(gc_frontBuffer, 0, bufSize);
    memset(gc_backBuffer, 0, bufSize);

    gc_buttonUp    = 0;
    gc_buttonDown  = 0;
    gc_buttonLeft  = 0;
    gc_buttonRight = 0;
    gc_buttonA     = 0;
    gc_buttonB     = 0;
    gc_buttonC     = 0;
    gc_buttonD     = 0;
    gc_buttonReset = 0;

    gc_hButtonEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!gc_hButtonEvent)
	return GC_Finalize(), E_INIT;

    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!hEvent)
	return GC_Finalize(), E_INIT;

    void* param[2];
    param[0] = (void*)title;
    param[1] = &hEvent;
    if(!CreateThread(NULL, 0, UI, param, 0, NULL))
    {
	CloseHandle(hEvent);
	return GC_Finalize(), E_INIT;
    }
    
    DWORD retWait = WaitForSingleObject(hEvent, INFINITE);
    CloseHandle(hEvent);

    if(retWait != WAIT_OBJECT_0)
	return GC_Finalize(), E_INIT;

    if(!gc_hwnd)
	return GC_Finalize(), E_INIT;

    gc_initialized = 1;
    return E_SUCCESS;
}

int GC_Finalize()
{
    if(gc_hwnd)
    {
	DestroyWindow(gc_hwnd);
	gc_hwnd = NULL;
    }

    if(gc_hButtonEvent)
    {
	CloseHandle(gc_hButtonEvent);
	gc_hButtonEvent = NULL;
    }

    if(gc_frontBuffer)
    {
	free(gc_frontBuffer);
	gc_frontBuffer = 0;
    }

    if(gc_backBuffer)
    {
	free(gc_backBuffer);
	gc_backBuffer = 0;
    }

    if(gc_initialized)
    {
	gc_initialized = 0;
	return E_SUCCESS;
    }
    else
	return E_NOT_INITIALIZED;
}

int GC_Sleep(int ms)
{
    if(!gc_initialized)
	return E_NOT_INITIALIZED;

    DWORD start = GetTickCount();
    WaitForSingleObject(gc_hButtonEvent, ms > 0 ? ms : INFINITE);
    DWORD end = GetTickCount();

    int elapsed = end - start;
    if(ms <= 0 || elapsed < 0 || elapsed + 10 >= ms)
	return 0;

    return ms - elapsed;
}

int GC_Set(int x, int y, int red, int green, int blue)
{
    if(!gc_initialized)
	return E_NOT_INITIALIZED;

    if(x < 0 || x >= gc_width || y < 0 || y >= gc_height)
	return E_PARAMETER;

    gc_backBuffer[x + y * gc_width] = RGB(red, green, blue);
    return E_SUCCESS;
}

int GC_Flip()
{
    if(!gc_initialized)
	return E_NOT_INITIALIZED;

    COLORREF* temp = gc_frontBuffer;
    gc_frontBuffer = gc_backBuffer;
    gc_backBuffer = temp;

    RECT rect;
    rect.left = LEFT;
    rect.top = TOP;
    rect.right = LEFT + gc_width * gc_blockSize;
    rect.bottom = TOP + gc_height * gc_blockSize;

    InvalidateRect(gc_hwnd, &rect, FALSE);
    return E_SUCCESS;
}

int GC_UpPressed()
{
    return gc_buttonUp;
}

int GC_DownPressed()
{
    return gc_buttonDown;
}

int GC_LeftPressed()
{
    return gc_buttonLeft;
}

int GC_RightPressed()
{
    return gc_buttonRight;
}

int GC_APressed()
{
    return gc_buttonA;
}

int GC_BPressed()
{
    return gc_buttonB;
}

int GC_CPressed()
{
    return gc_buttonC;
}

int GC_DPressed()
{
    return gc_buttonD;
}

int GC_ResetPressed()
{
    return gc_buttonReset;
}

BOOL Register()
{
    WNDCLASSEX WndClassEx;

    WndClassEx.cbSize	     = sizeof(WndClassEx);
    WndClassEx.style	     = 0;
    WndClassEx.lpfnWndProc   = WndProc;
    WndClassEx.cbClsExtra    = 0;
    WndClassEx.cbWndExtra    = 0;
    WndClassEx.hInstance     = gc_hInstance;
    WndClassEx.hIcon	     = NULL;
    WndClassEx.hCursor	     = LoadCursor(NULL, IDC_ARROW);
    WndClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WndClassEx.lpszMenuName  = NULL;
    WndClassEx.lpszClassName = cst_szClassName;
    WndClassEx.hIconSm	     = NULL;

    return RegisterClassEx(&WndClassEx) != 0;
}

DWORD WINAPI UI(LPVOID lpParam)
{
    void** param = (void**)lpParam;
    const char* title = (const char*)param[0];
    HANDLE hEvent = *(HANDLE*)param[1];

    gc_hwnd = Create(title);
    SetEvent(hEvent);
    if(!gc_hwnd)
	return 0;

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) != FALSE)
	DispatchMessage(&msg);

    return msg.wParam;
}

HWND Create(const char* title)
{
    HWND hwnd = CreateWindow(cst_szClassName, title, WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT,
	    LEFT * 2 + 5 + gc_width * gc_blockSize, TOP + PANEL + gc_height * gc_blockSize, NULL, NULL, gc_hInstance, NULL);

    if(!hwnd)
	return hwnd;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch(uMessage)
    {
    HANDLE_MSG(hwnd, WM_DESTROY,	GameCon_OnDestroy);
    HANDLE_MSG(hwnd, WM_PAINT,		GameCon_OnPaint);
    HANDLE_MSG(hwnd, WM_KEYDOWN,	GameCon_OnKey);
    HANDLE_MSG(hwnd, WM_KEYUP,		GameCon_OnKey);
    default:
	return DefWindowProc(hwnd, uMessage, wParam, lParam);
    }
}

void GameCon_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

void GameCon_OnPaint(HWND hwnd)
{
    PAINTSTRUCT paintStruct;
    HDC hdc = BeginPaint(hwnd, &paintStruct);

    HDC hdcBack = CreateCompatibleDC(hdc);
    HBITMAP bitmapBack = CreateCompatibleBitmap(hdc, gc_width * gc_blockSize, gc_height * gc_blockSize);
    HGDIOBJ bitmapOld = SelectObject(hdcBack, bitmapBack);
    HBRUSH brushOld = SelectBrush(hdcBack, CreateSolidBrush(RGB(0, 0, 0)));

    int x;
    for(x = 0; x < gc_width; x++)
    {
	int y;
	for(y = 0; y < gc_height; y++)
	{
	    HBRUSH brushPrev = SelectBrush(hdcBack, CreateSolidBrush(gc_frontBuffer[x + y * gc_width]));
	    DeleteBrush(brushPrev);

	    int px = x * gc_blockSize;
	    int py = y * gc_blockSize;
	    Rectangle(hdcBack, px, py, px + gc_blockSize, py + gc_blockSize);
	}
    }

    BitBlt(hdc, LEFT, TOP, LEFT + gc_width * gc_blockSize, TOP + gc_height * gc_blockSize, hdcBack, 0, 0, SRCCOPY);

    HBRUSH brushLast = SelectBrush(hdcBack, brushOld);
    DeleteBrush(brushLast);
    SelectObject(hdcBack, bitmapOld);
    DeleteObject(bitmapBack);
    DeleteObject(hdcBack);

    EndPaint(hwnd, &paintStruct);
}

void GameCon_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    int down = (fDown ? 1 : 0);
    int changed;

    switch(vk)
    {
    case VK_UP:
    case 'W':
	changed = (down != gc_buttonUp);
	gc_buttonUp = down;
	break;
    case VK_DOWN:
    case 'S':
	changed = (down != gc_buttonDown);
	gc_buttonDown = down;
	break;
    case VK_LEFT:
    case 'A':
	changed = (down != gc_buttonLeft);
	gc_buttonLeft = down;
	break;
    case VK_RIGHT:
    case 'D':
	changed = (down != gc_buttonRight);
	gc_buttonRight = down;
	break;
    case VK_NUMPAD1:
    case 'J':
	changed = (down != gc_buttonA);
	gc_buttonA = down;
	break;
    case VK_NUMPAD2:
    case 'K':
	changed = (down != gc_buttonB);
	gc_buttonB = down;
	break;
    case VK_NUMPAD4:
    case 'U':
	changed = (down != gc_buttonC);
	gc_buttonC = down;
	break;
    case VK_NUMPAD5:
    case 'I':
	changed = (down != gc_buttonD);
	gc_buttonD = down;
	break;
    case VK_ESCAPE:
	changed = (down != gc_buttonReset);
	gc_buttonReset = down;
	break;
    default:
	changed = 0;
    }

    if(changed)
	SetEvent(gc_hButtonEvent);
}
