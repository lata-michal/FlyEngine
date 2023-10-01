#include "Scene.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <thread>
#include <memory>
#include <regex>
#include <vector>

#pragma comment(lib, "ComCtl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(linker, "/manifest:manifestFile=\"Launcher.exe.manifest\"")

#define ID_BUTTON1 501
#define ID_BUTTON2 502
#define ID_BUTTON3 503
#define ID_BUTTON4 504
#define ID_BUTTON5 505
#define ID_BUTTON6 506
#define ID_BUTTON7 507
#define ID_BUTTON8 508
#define ID_BUTTON9 509
#define ID_BUTTON10 510
#define ID_BUTTON11 511

static std::unique_ptr<std::thread> thScene;
static bool bDevLogToggle = false;
static bool bReadOnly = true;
static HWND hText;
static HWND hButtonExecution;
static uint32_t index = 0;
static std::vector<std::wstring> avCommands = {
    L"-msc sandbox",
    L"-ubo test",
    L"-gs test", 
    L"-phong shading test",
    L"-quad instancing test",
    L"-blinn phong shading test",
    L"-shadow map -dir light test",
    L"-shadow map -point light test",
    L"-normal & parallax mapping 1",
    L"-normal & parallax mapping 2",
    L"-normal & parallax mapping 3",
    L"-HDR tone mapping test",
    L"-HDR tone mapping autoexposure",
};

bool regexTextFind(const std::wstring& srcText, const std::wstring& text);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Launcher App Class";

    InitCommonControls();

    WNDCLASSEX wcex = { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return -1;

    HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW & ~WS_THICKFRAME, CLASS_NAME,
        L"Launcher", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), 
        1700, 400, 455, 720, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return -1;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HWND hButton1 = CreateWindowEx(0, L"BUTTON", L"Scene Flying Plane and Sunset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 15, 400, 50, hWnd, (HMENU)ID_BUTTON1, hInstance, NULL);

    HWND hButton2 = CreateWindowEx(0, L"BUTTON", L"Scene Planet and Asteroids", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 80, 400, 50, hWnd, (HMENU)ID_BUTTON2, hInstance, NULL);

    HWND hButton3 = CreateWindowEx(0, L"BUTTON", L"Scene Titan Implosion Simulation", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 145, 400, 50, hWnd, (HMENU)ID_BUTTON3, hInstance, NULL);

    HWND hButton4 = CreateWindowEx(0, L"BUTTON", L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 210, 400, 50, hWnd, (HMENU)ID_BUTTON4, hInstance, NULL);

    HWND hButton5 = CreateWindowEx(0, L"BUTTON", L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 275, 400, 50, hWnd, (HMENU)ID_BUTTON5, hInstance, NULL);

    HWND hButton6 = CreateWindowEx(0, L"BUTTON", L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 340, 400, 50, hWnd, (HMENU)ID_BUTTON6, hInstance, NULL);

    HWND hButton7 = CreateWindowEx(0, L"BUTTON", L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 405, 400, 50, hWnd, (HMENU)ID_BUTTON7, hInstance, NULL);

    HWND hButton8 = CreateWindowEx(0, L"BUTTON", L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 470, 400, 50, hWnd, (HMENU)ID_BUTTON8, hInstance, NULL);

    HWND hButton9 = CreateWindowEx(0, L"BUTTON", L"Dbg - Toggle Developer Panel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 535, 400, 50, hWnd, (HMENU)ID_BUTTON9, hInstance, NULL);

    HWND hButton10 = CreateWindowEx(0, L"BUTTON", L"Dbg - Toggle Console Mode", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 600, 400, 50, hWnd, (HMENU)ID_BUTTON10, hInstance, NULL);

    hButtonExecution = CreateWindowEx(0, L"BUTTON", L"Execute Console Command", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        430, 600, 355, 50, hWnd, (HMENU)ID_BUTTON11, hInstance, NULL);


    hText = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL
        | ES_MULTILINE | ES_AUTOVSCROLL, 430, 15, 380, 570, hWnd, NULL, hInstance, NULL);

    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    SendMessage(hButton1, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton2, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton3, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton4, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton5, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton6, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton7, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton8, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton9, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButton10, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hButtonExecution, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hText, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    SendMessage(hText, EM_SETREADONLY, TRUE, 0);


    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case ID_BUTTON1:
        {
            if (!thScene)
            {
                thScene.reset(new std::thread(Scene::RenderScene5, hText));
            }
            else
            {
                Scene::ForceEnd();
                thScene.get()->join();
                thScene.reset(new std::thread(Scene::RenderScene5, hText));

            }
            break;
        }
        case ID_BUTTON2:
        {
            if (!thScene)
            {
                thScene.reset(new std::thread(Scene::RenderScene7, hText));
            }
            else
            {
                Scene::ForceEnd();
                thScene.get()->join();
                thScene.reset(new std::thread(Scene::RenderScene7, hText));

            }
            break;
        }
        case ID_BUTTON3:
        {
            if (!thScene)
            {
                thScene.reset(new std::thread(Scene::RenderScene8, hText));
            }
            else
            {
                Scene::ForceEnd();
                thScene.get()->join();
                thScene.reset(new std::thread(Scene::RenderScene8, hText));

            }
            break;
        }
        case ID_BUTTON4:
        {
            break;
        }
        case ID_BUTTON5:
        {
            break;
        }
        case ID_BUTTON6:
        {
            break;
        }
        case ID_BUTTON7:
        {
            break;
        }
        case ID_BUTTON8:
        {
            break;
        }
        case ID_BUTTON9:
        {
            if (!bDevLogToggle)
            {
                SetWindowPos(hWnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, 840, 720, SWP_NOMOVE);
                bDevLogToggle = !bDevLogToggle;
            }
            else
            {
                SetWindowPos(hWnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, 455, 720, SWP_NOMOVE);
                SetWindowText(hText, L"");
                bDevLogToggle = !bDevLogToggle;
            }
            break;
        case ID_BUTTON10:
            if (!bReadOnly)
            {
                SendMessage(hText, EM_SETREADONLY, TRUE, 0);
                bReadOnly = !bReadOnly;
            }
            else
            {
                SendMessage(hText, EM_SETREADONLY, FALSE, 0);
                bReadOnly = !bReadOnly;

                DWORD lengthPrevText = GetWindowTextLength(hText);

                std::wstring prevText = L"";
                if (lengthPrevText > 0)
                {
                    LPWSTR buffer = new WCHAR[lengthPrevText + 1];
                    GetWindowText(hText, buffer, lengthPrevText + 1);

                    prevText += buffer;
                    delete[] buffer;
                }

                std::wstring wmessage = prevText +
                    L"\r\nCommand line mode.\r\nType your command between two slashes. Example: /any command/.\r\nAvailable commands : \r\nres\r\n";

                for (auto& command : avCommands)
                    wmessage += (L"launch " + command + L"\r\n");

                SetWindowText(hText, wmessage.c_str());
                SendMessage(hText, EM_LINESCROLL, 0, 3000000);
            }
            break;
        }
        case ID_BUTTON11:
        {
            DWORD lengthPrevText = GetWindowTextLength(hText);

            std::wstring prevText = L"";
            if (lengthPrevText > 0)
            {
                LPWSTR buffer = new WCHAR[lengthPrevText + 1];
                GetWindowText(hText, buffer, lengthPrevText + 1);

                prevText += buffer;
                delete[] buffer;
            }

            if (regexTextFind(prevText, L"/launch"))
            {
                if (regexTextFind(prevText, avCommands[0] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene1, hText));
                        index = 0;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene1, hText));
                        index = 0;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[1] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene2, hText));
                        index = 1;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene2, hText));
                        index = 1;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[2] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene3, hText));
                        index = 2;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene3, hText));
                        index = 2;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[3] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene4, hText));
                        index = 3;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene4, hText));
                        index = 3;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[4] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene6, hText));
                        index = 4;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene6, hText));
                        index = 4;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[5] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene9, hText));
                        index = 5;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene9, hText));
                        index = 5;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[6] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene10, hText));
                        index = 6;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene10, hText));
                        index = 6;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[7] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene11, hText));
                        index = 7;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene11, hText));
                        index = 7;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[8] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene12, hText));
                        index = 8;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene12, hText));
                        index = 8;
                    }
                    SetWindowText(hText, L"");
                    }
                else if (regexTextFind(prevText, avCommands[9] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene13, hText));
                        index = 9;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene13, hText));
                        index = 9;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[10] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene14, hText));
                        index = 10;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene14, hText));
                        index = 10;
                    }
                    SetWindowText(hText, L"");
                    }
                else if (regexTextFind(prevText, avCommands[11] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene15, hText));
                        index = 11;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene15, hText));
                        index = 11;
                    }
                    SetWindowText(hText, L"");
                }
                else if (regexTextFind(prevText, avCommands[12] + L"/"))
                {
                    if (!thScene)
                    {
                        thScene.reset(new std::thread(Scene::RenderScene16, hText));
                        index = 12;
                    }
                    else
                    {
                        Scene::ForceEnd();
                        thScene.get()->join();
                        thScene.reset(new std::thread(Scene::RenderScene16, hText));
                        index = 12;
                    }
                    SetWindowText(hText, L"");
                }
                else
                {
                    std::wstring wmessage = prevText + L"\r\nCommand Execution Failed! Command not recognized. Try again!\r\n";
                    SetWindowText(hText, wmessage.c_str());
                    SendMessage(hText, EM_LINESCROLL, 0, 3000000);
                }
            }
            else if (regexTextFind(prevText, L"/res/"))
            {
                SetWindowText(hText, std::wstring(L"/launch " + avCommands[index] + L"/").c_str());
                SendMessage(hButtonExecution, BM_CLICK, 0, 0);
            }
            else
            {
                std::wstring wmessage = prevText + L"\r\nCommand Execution Failed! Command not recognized. Try again!\r\n";
                SetWindowText(hText, wmessage.c_str());
                SendMessage(hText, EM_LINESCROLL, 0, 3000000);
            }
            break;
        }
        }
        }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
    
        hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
    
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        if (thScene)
            if (thScene.get()->joinable())
                thScene.get()->join();
        break;
    }
    default:
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    }

    return 0;
}

bool regexTextFind(const std::wstring& srcText, const std::wstring& text)
{
    std::wregex pattern(text);
    return std::regex_search(srcText, pattern);
}