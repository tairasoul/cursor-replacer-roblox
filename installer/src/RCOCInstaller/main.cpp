#include <iostream>
#include <filesystem>
#include <thread>
#include <curl/curl.h>
#include <windows.h>

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
	std::cout << "Welcome to the RCO Installer/Updater!\nRCO is made, owned, and developed by Kaede and L8X\n\nType \"install\" and press enter to continue!\nType \"uninstall\" and press enter to uninstall :(\n\n";
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

        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "RCOC", 0, REG_SZ, (const BYTE*)(rootDir + "\\RCOC.exe").c_str(),36);
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
            RegDeleteValueA(hKey, "RCOC");
        }
        else {
            std::cout << "Registry error, RCOC startup regkey not removed...\n";
        }
        exit(0);
	}
    exit(0);
}
