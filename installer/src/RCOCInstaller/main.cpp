#include <iostream>
#include <filesystem>
#include <thread>
#include <curl/curl.h>
#include <windows.h>
#include <cstdlib>

// thanks chatgpt

std::wstring stringToWideString(const std::string& str) {
    int wideStrLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (wideStrLength == 0) {
        throw std::runtime_error("Failed to convert string to wide string.");
    }
    std::wstring wideStr(wideStrLength, L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], wideStrLength) == 0) {
        throw std::runtime_error("Failed to convert string to wide string.");
    }
    return wideStr;
}

std::string wideStringToString(const std::wstring& wideStr) {
    int strLength = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (strLength == 0) {
        throw std::runtime_error("Failed to convert wide string to string.");
    }
    std::string str(strLength, '\0');
    if (WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &str[0], strLength, nullptr, nullptr) == 0) {
        throw std::runtime_error("Failed to convert wide string to string.");
    }
    return str;
}


std::string rootDir("C:\\RCOC");

int writefile(std::string url, std::string path) {
    FILE* file;
    if (fopen_s(&file, (path).c_str(), "wb") != 0) {
        std::cout << "Could not open filepoint... | 0x1\n";
        std::cin.get();
        return 1;
    }

    CURL* req = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(req, CURLOPT_URL, url);
    curl_easy_setopt(req, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(req, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
    curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(req, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform(req);
    if (res != CURLE_OK) {
        std::cout << "NETWORK ERROR | PLEASE CHECK YOUR INTERNET CONNECTION | 0x2\n";
        std::cin.get();
        return 2;
    }
    curl_easy_cleanup(req);

    fclose(file);
}

int main(int argc, char* argv[]) {
	std::string userIn;
	std::cout << "Welcome to the RCOC Installer/Updater!\nRCOC is made by fheahdythdr, parts of the source are from Kaede and L8X's RCO (Roblox Client Optimizer)\n\nType \"install\" and press enter to continue!\nType \"uninstall\" and press enter to uninstall :(\n\n";
    std::cin >> userIn;

    if (std::filesystem::exists(rootDir) == false) {
        std::filesystem::create_directory(rootDir);
    }

    if (userIn == std::string("install")) { // Install
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/RCOC.exe", rootDir + "\\RCOC.exe");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/libcurl.dll", rootDir + "\\libcurl.dll");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/zlib1.dll", rootDir + "\\zlib1.dll");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/selected.json", rootDir + "\\selected.json");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/animegirl.ico", rootDir + "\\animegirl.ico");
        std::filesystem::create_directory(rootDir + "\\backend");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/backend/main.mjs", rootDir + "\\backend\\main.mjs");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/backend/package.json", rootDir + "\\backend\\package.json");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/backend/select.mjs", rootDir + "\\backend\\select.mjs");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/backend/utils.mjs", rootDir + "\\backend\\utils.mjs");
        std::filesystem::create_directory(rootDir + "\\default_cursors");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/default_cursors/arrow.png", rootDir + "\\default_cursors\\arrow.png");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/default_cursors/arrowfar.png", rootDir + "\\default_cursors\\arrowfar.png");
        writefile("https://raw.githubusercontent.com/fheahdythdr/cursor-replacer-roblox/main/default_cursors/ibeam.png", rootDir + "\\default_cursors\\ibeam.png");
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = nullptr;
        sa.bInheritHandle = TRUE;

        HANDLE hRead, hWrite;
        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
            std::cout << "Error creating pipe." << std::endl;
            return 1;
        }

        STARTUPINFOA si1;
        PROCESS_INFORMATION pi1;
        ZeroMemory(&si1, sizeof(STARTUPINFOA));
        si1.cb = sizeof(STARTUPINFOA);
        si1.hStdError = hWrite;
        si1.hStdOutput = hWrite;
        si1.dwFlags |= STARTF_USESTDHANDLES;

        char commandLine[] = "node --version";

        if (!CreateProcessA(nullptr, commandLine, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si1, &pi1)) {
            std::cout << "Error creating process." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            return 1;
        }

        CloseHandle(hWrite);

        char buffer[128];
        DWORD bytesRead;
        std::string output;

        while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
            if (bytesRead == 0) {
                break;
            }
            buffer[bytesRead] = '\0';
            output += buffer;
        }

        CloseHandle(hRead);

        DWORD exitCode;
        GetExitCodeProcess(pi1.hProcess, &exitCode);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);

        if (exitCode == 0 && output == "v20.2.0\n") {
            std::cout << "Node.js is installed, skipping NodeJS install.\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        else {
            std::cout << "Node.js is not installed or is on a lower version. Installing NodeJS v20.2.0 in 5 seconds." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::filesystem::path temp = std::filesystem::temp_directory_path();
            temp /= "RCOCINSTALL";
            std::filesystem::create_directory(temp);
            writefile("https://nodejs.org/dist/v20.2.0/node-v20.2.0-x64.msi", (temp / "node-v20.2.0-x64.msi").string());
            std::wstring tempWide = stringToWideString((temp / "node-v20.2.0-x64.msi").string());

            std::wstring commandLine = L"msiexec /passive /i " + tempWide;

            std::this_thread::sleep_for(std::chrono::seconds(5));

            STARTUPINFOW si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(STARTUPINFOW));
            si.cb = sizeof(STARTUPINFOW);

            if (!CreateProcessW(nullptr, const_cast<LPWSTR>(commandLine.c_str()), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
                std::cout << "Error creating process msiexec." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                return 1;
            }

            WaitForSingleObject(pi.hProcess, INFINITE);

            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            if (exitCode == 0) {
                std::cout << "Installation completed successfully." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            else {
                std::cout << "Installation failed." << "\nExit code:" << exitCode << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }

            std::filesystem::remove_all(temp);
        }
        std::filesystem::path directory = "C:\\RCOC\\backend";
        std::filesystem::current_path(directory);
        std::cout << "installing required packages in 5 seconds\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "installing.\n";
        system("npm i");

        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "RCOCEXE", 0, REG_SZ, (const BYTE*)(rootDir + "\\RCOC.exe").c_str(),36);
            RegCloseKey(hKey);
        }
        else {
            std::cout << "Registry error, RCOC not added to startup...\n";
        }

        //CreateProcess code from https://stackoverflow.com/a/15440094
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        CreateProcessA((rootDir + "\\RCOC.exe").c_str(), argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        exit(0);
	}
    else if (userIn == std::string("uninstall")) { // Uninstall
        if (argv[0] == (rootDir+"\\RCO2Installer.exe").c_str() || argv[0] == "C:\\RCOC\\RCOC.exe") {
            std::cout << "To uninstall, please run the installer outside of this updater, you can redownload this installer on the github.\nClosing in 5 seconds.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            return 0;
        }

        if (std::filesystem::exists("C:\\RCO2") == true) {
            std::filesystem::remove_all("C:\\RCO2");
        }

        if (std::filesystem::exists(rootDir) == true) {
            std::filesystem::remove_all(rootDir);
        }

        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
            RegDeleteValueA(hKey, "RCOCEXE");
        }
        else {
            std::cout << "Registry error, RCOC startup regkey not removed...\n";
        }
        exit(0);
	}
    exit(0);
}
