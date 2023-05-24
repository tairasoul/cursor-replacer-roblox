#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include <cstdlib>
#include <curl/curl.h>

using std::string;

bool isConsoleHidden = false;
bool isRcoEnabled = false;

std::string rootDir("C:\\RCOC");

char* buf = nullptr;
size_t sz = 0;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND consoleWindow = GetConsoleWindow();

//Tray icon code from stackoverflow and various other internet sources, I was unsure on how it worked.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LPCWSTR lpszClass = L"__hidden__";

int traySystem() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc;
    HWND hWnd;
    MSG msg;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = lpszClass;
    wc.lpszMenuName = nullptr;
    wc.style = 0;
    RegisterClass(&wc);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static NOTIFYICONDATA nid;
    std::ofstream isHiddenFile;

    switch (iMsg) {
    case WM_CREATE:
        std::memset(&nid, 0, sizeof(nid));
        nid.cbSize = sizeof(nid);
        nid.hWnd = hWnd;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
        nid.uCallbackMessage = WM_APP + 1;
        nid.hIcon = (HICON)LoadImageA(NULL, (rootDir + "\\animegirl.ico").c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
        memcpy_s(nid.szTip, sizeof(nid.szTip), L"RCR <3\nClick to toggle console", sizeof(wchar_t[31]));
        Shell_NotifyIcon(NIM_ADD, &nid);
        return 0;
    case WM_APP + 1:
        switch (lParam) {
        case WM_LBUTTONDBLCLK:
            if (isConsoleHidden) {
                ShowWindow(consoleWindow, SW_SHOW);
                isConsoleHidden = false;
                isHiddenFile.open(rootDir + "\\isHidden.rco");
                isHiddenFile << "f";
                isHiddenFile.close();
            }
            else {
                ShowWindow(consoleWindow, SW_HIDE);
                isConsoleHidden = true;
                isHiddenFile.open(rootDir + "\\isHidden.rco");
                isHiddenFile << "t";
                isHiddenFile.close();
            }
            break;
        case WM_LBUTTONDOWN:
            if (isConsoleHidden) {
                ShowWindow(consoleWindow, SW_SHOW);
                isConsoleHidden = false;
                isHiddenFile.open(rootDir + "\\isHidden.rco");
                isHiddenFile << "f";
                isHiddenFile.close();
            }
            else {
                ShowWindow(consoleWindow, SW_HIDE);
                isConsoleHidden = true;
                isHiddenFile.open(rootDir + "\\isHidden.rco");
                isHiddenFile << "t";
                isHiddenFile.close();
            }
            break;
        }
        break;
    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void printMainText() {
    system("cls");
    std::cout << "Roblox Client Optimizer original coded by Kaede | Modified by Tanki (fheahdythdr)\n\nRCR is currently: ";
    if (isRcoEnabled) {
        SetConsoleTextAttribute(hConsole, 10);
        std::cout << "Enabled";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "\nType d to ";
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Disable";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << " RCR.\n\n";
    }
    else {
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Disabled";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "\nType e to ";
        SetConsoleTextAttribute(hConsole, 10);
        std::cout << "Enable";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << " RCR.\n\n";
    }

    SetConsoleTextAttribute(hConsole, 6);
    std::cout << "This window can be hidden via the RCR tray icon!\nYou can ";
    SetConsoleTextAttribute(hConsole, 12);
    std::cout << "close";
    SetConsoleTextAttribute(hConsole, 6);
    std::cout << " RCR with ALT+F4 or any other similar method.\nYou can add cursors in the cursors folder located in C:\\RCOC.\nType c to change cursors, type u to remove RCR from registry and rerun to add RCR back to registry.\n";
    SetConsoleTextAttribute(hConsole, 7);
}

void NodeJSProcessThread() {
    while (true) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        const char* command = "node C:\\RCOC\\backend\\main.mjs";

        // Determine the required size of the wide string
        int wideSize = MultiByteToWideChar(CP_UTF8, 0, command, -1, NULL, 0);

        // Allocate memory for the wide string
        wchar_t* wideCommand = new wchar_t[wideSize];

        // Convert the multibyte string to a wide string
        MultiByteToWideChar(CP_UTF8, 0, command, -1, wideCommand, wideSize);

        // Create the child process
        if (!CreateProcessW(nullptr, wideCommand, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            std::cout << "Error creating process." << std::endl;
        }

        // Wait for the process to exit
        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int main(int argc, char** argv) {
    //Preinit
    SetConsoleTitle(L"Roblox Cursor Replacer");

    if (std::filesystem::exists(rootDir) == false) {
        std::cout << "Could not find proper RCR files, please reinstall RCR | 0x1\n";
        std::cin.get();
        return 1;
    }

    if (std::filesystem::exists(rootDir + "\\animegirl.ico") == false) {
        FILE* file;
        if (fopen_s(&file, (rootDir + "\\animegirl.ico").c_str(), "wb") != 0) {
            std::cout << "Could not open filepoint... | 0x2\n";
            std::cin.get();
            return 2;
        }

        CURL* req = curl_easy_init();
        CURLcode res;
        curl_easy_setopt(req, CURLOPT_URL, "https://raw.githubusercontent.com/fheahdythdr/random-shit-again/main/icon.ico");
        curl_easy_setopt(req, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS); // add HTTP/2 support for speed gains
        curl_easy_setopt(req, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2); // force TLSv1.2 support as HTTP/2 requires it
        curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(req, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(req);
        if (res != CURLE_OK) {
            std::cout << "NETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | 0x4 | ERROR DOWNLOADING ICON\n";
            std::cin.get();
            return 4;
        }
        curl_easy_cleanup(req);

        fclose(file);
    }

    if (std::filesystem::exists(rootDir + "\\isHidden.rco") == false) {
        std::ofstream isHiddenFile;
        isHiddenFile.open(rootDir + "\\isHidden.rco");
        isHiddenFile << "f";
        isHiddenFile.close();
    }

    if (std::filesystem::exists(rootDir + "\\isEnabled.rco") == false) {
        std::ofstream isEnabledFile;
        isEnabledFile.open(rootDir + "\\isEnabled.rco");
        isEnabledFile << "f";
        isEnabledFile.close();
    }

    HKEY regcheck;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &regcheck) == ERROR_SUCCESS) {
        // Query the value to check if it exists
        char valueData[MAX_PATH];
        DWORD valueSize = sizeof(valueData);
        DWORD valueType;

        if (RegQueryValueExA(regcheck, "RCOCEXE", nullptr, &valueType, reinterpret_cast<LPBYTE>(valueData), &valueSize) == ERROR_SUCCESS) {
            // Value exists
            std::cout << "Registry value exists: " << valueData << std::endl;
        }
        else {
            HKEY hKey;
            if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
                RegSetValueExA(hKey, "RCOCEXE", 0, REG_SZ, (const BYTE*)(rootDir + "\\RCOC.exe").c_str(), 36);
                RegCloseKey(hKey);
            }
            else {
                std::cout << "Registry error, RCO not added to startup...\n";
            }
        }

        // Close the registry key
        RegCloseKey(regcheck);
    }

    //Initialize the tray icon system
    std::thread t1(traySystem);

    //Set Hidden and Enabled based on saved file
    std::ifstream hiddenFile(rootDir + "\\isHidden.rco");
    hiddenFile.seekg(0, std::ios::end);
    size_t size = hiddenFile.tellg();
    string buffer = string(size, ' ');
    hiddenFile.seekg(0);
    hiddenFile.read(&buffer[0], size);
    if (buffer == "t") {
        isConsoleHidden = true;
    }
    hiddenFile.close();

    std::ifstream enabledFile(rootDir + "\\isEnabled.rco");
    enabledFile.seekg(0, std::ios::end);
    size = enabledFile.tellg();
    buffer = string(size, ' ');
    enabledFile.seekg(0);
    enabledFile.read(&buffer[0], size);
    if (buffer == "t") {
        isRcoEnabled = true;
    }
    enabledFile.close();

    //Handle Hidden Value
    if (isConsoleHidden) {
        ShowWindow(consoleWindow, SW_HIDE);
    }
    else {
        ShowWindow(consoleWindow, SW_SHOW);
    }

    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    EnableMenuItem(GetSystemMenu(consoleWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

    std::thread njsthread(NodeJSProcessThread);

    //Input loop
    while (true) {
        printMainText();
        string t;
        std::getline(std::cin, t);
        if (t == "e") {
            isRcoEnabled = true;
            std::ofstream isEnabledFile;
            isEnabledFile.open(rootDir + "\\isEnabled.rco");
            isEnabledFile << "t";
            isEnabledFile.close();
        }
        else if (t == "d") {
            isRcoEnabled = false;
            std::ofstream isEnabledFile;
            isEnabledFile.open(rootDir + "\\isEnabled.rco");
            isEnabledFile << "f";
            isEnabledFile.close();
        }
        else if (t == "c") {
            system("node C:\\RCOC\\backend\\select.mjs");
        }
        else if (t == "u") {
            HKEY hKey;

            // Open the registry key for reading
            if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
                // Check if the value exists
                DWORD valueType;
                if (RegQueryValueExA(hKey, "RCOCEXE", nullptr, &valueType, nullptr, nullptr) == ERROR_SUCCESS) {
                    // Delete the value
                    if (RegDeleteValueA(hKey, "RCOCEXE") == ERROR_SUCCESS) {
                        std::cout << "Registry value deleted." << std::endl;
                    }
                    else {
                        std::cerr << "Failed to delete registry value." << std::endl;
                    }
                }
                else {
                    std::cout << "Registry value does not exist." << std::endl;
                }

                // Close the registry key
                RegCloseKey(hKey);
            }
            else {
                std::cerr << "Failed to open registry key." << std::endl;
            }
        }
    }
}
