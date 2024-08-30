/**
 * @file utils.cpp
 * @brief Implementation of utility functions for the Bassil language project.
 */

#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include <algorithm>
#include <stdexcept>
#include <strsafe.h> // Add this include for StringCchCopyW

// If StringCchCopyW is not available, provide a simple implementation
#ifndef StringCchCopyW
#define StringCchCopyW(dest, destSize, src) wcscpy_s(dest, destSize, src)
#endif

namespace Utils
{

/**
 * @brief Splits a string into a vector of substrings based on a specified delimiter.
 * @param s The input string to be split.
 * @param delimiter The string that serves as the delimiter.
 * @return std::vector<std::string> A vector containing the substrings.
 */
std::vector<std::string> split_string(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

/**
 * @brief Converts a standard string to a wide string.
 * @param _string The input string to be converted.
 * @param _isUtf8Enabled Flag to indicate if UTF-8 encoding should be used (default: true).
 * @return std::wstring The resulting wide string.
 */
std::wstring StringToWString(const std::string& _string, bool _isUtf8Enabled) {
    int len = MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), -1, NULL, 0);
    std::wstring wstr(len - 1, 0);
    MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), -1, &wstr[0], len);
    return wstr;
}

/**
 * @brief Converts a standard string to a wide character string (LPCWSTR).
 * @param _string The input string to be converted.
 * @return LPCWSTR The resulting wide character string.
 */
LPCWSTR StringToLPCWSTR(const std::string& _string) {
    static std::wstring wstr;
    wstr = StringToWString(_string);
    return wstr.c_str();
}

/**
 * @brief Creates a Windows API 32-bit message box.
 * @param _title The title of the message box.
 * @param _message The content of the message box.
 * @param _type The type of the message box (1-6).
 * @return int The ID of the button clicked by the user.
 * @throw std::runtime_error if an invalid message box type is provided.
 */
int CreateWinAPI32MessageBox(const std::string& _title, const std::string& _message, int _type) {
    UINT _popupType;
    switch (_type) {
        case 1: _popupType = MB_ABORTRETRYIGNORE; break;
        case 2: _popupType = MB_OKCANCEL; break;
        case 3: _popupType = MB_CANCELTRYCONTINUE; break;
        case 4: _popupType = MB_YESNOCANCEL; break;
        case 5: _popupType = MB_YESNO; break;
        case 6: _popupType = MB_OK; break;
        default: 
            throw std::runtime_error("Unknown Windows API 32-BIT-VERSION POPUP MESSAGE type");
    }
    return MessageBoxW(NULL, StringToLPCWSTR(_message), StringToLPCWSTR(_title), _popupType);
}

/**
 * @brief Creates a Windows API balloon notification.
 * @param _title The title of the notification.
 * @param _message The content of the notification.
 * @param _type The type of the notification (0-3, default: 0).
 * @throw std::runtime_error if an invalid notification type is provided.
 */
void CreateWinAPI32BallonNotification(const std::string& _title, const std::string& _message, int _type) {
    NOTIFYICONDATAW nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = NULL;
    nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;

    switch (_type) {
        case 0: nid.dwInfoFlags = NIIF_INFO; nid.uID = 1001; break;
        case 1: nid.dwInfoFlags = NIIF_ERROR; nid.uID = 1002; break;
        case 2: nid.dwInfoFlags = NIIF_WARNING; nid.uID = 1003; break;
        case 3: nid.dwInfoFlags = NIIF_NONE; nid.uID = 1004; break;
        default: throw std::runtime_error("Unknown Notification type");
    }

    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    StringCchCopyW(nid.szTip, ARRAYSIZE(nid.szTip), L"My App Notification");
    StringCchCopyW(nid.szInfo, ARRAYSIZE(nid.szInfo), StringToWString(_message).c_str());
    StringCchCopyW(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), StringToWString(_title).c_str());
    nid.uTimeout = 2000;

    Shell_NotifyIconW(NIM_ADD, &nid);
    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

/**
 * @brief Retrieves the maximum screen size for a given monitor.
 * @param monitorIndex The index of the monitor.
 * @return RECT The screen dimensions.
 * @throw std::runtime_error if the monitor index does not exist or if unable to get monitor info.
 */
RECT GetMaximizedScreenSize(int monitorIndex) {
    DISPLAY_DEVICE dd = { sizeof(dd) };
    RECT screenSize = {0};
    int deviceIndex = 0;

    while (EnumDisplayDevices(NULL, deviceIndex, &dd, 0)) {
        if (deviceIndex == monitorIndex) {
            HMONITOR hMonitor = MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);
            MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
            if (GetMonitorInfo(hMonitor, &monitorInfo)) {
                return monitorInfo.rcMonitor;
            } else {
                throw std::runtime_error("Failed to get monitor info");
            }
        }
        deviceIndex++;
    }
    throw std::runtime_error("Monitor index does not exist");
}

/**
 * @brief Trims leading whitespace from a string.
 * @param s The string to be trimmed.
 * @return std::string& Reference to the trimmed string.
 */
std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

/**
 * @brief Trims trailing whitespace from a string.
 * @param s The string to be trimmed.
 * @return std::string& Reference to the trimmed string.
 */
std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

/**
 * @brief Trims both leading and trailing whitespace from a string.
 * @param s The string to be trimmed.
 * @return std::string& Reference to the trimmed string.
 */
std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

/**
 * @brief Logs a message to a file and optionally prints it.
 * @param str The message to be logged.
 * @param isPrintTrue Flag to indicate if the message should be printed (default: true).
 * @return int 0 on success, 1 on failure.
 */
int general_log(const std::string& str, bool isPrintTrue) {
    if (!isPrintTrue) return 0;

    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/logs.txt", std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "[general_log] Failed to open file." << std::endl;
        return 1;
    }
    outputFile << str << "\n";
    outputFile.close();
    return 0;
}

/**
 * @brief Clears the content of the lexical analysis output file.
 * @return int 0 on success, 1 on failure.
 */
int clear_lex_out() {
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt", std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "[clear_lex_out] Failed to open file." << std::endl;
        return 1;
    }
    outputFile.close();
    return 0;
}

/**
 * @brief Clears the content of the log file.
 * @return int 0 on success, 1 on failure.
 */
int clear_logs() {
    std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/logs.txt", std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "[clear_logs] Failed to open file." << std::endl;
        return 1;
    }
    outputFile.close();
    return 0;
}

/**
 * @brief Reads the entire content of a file into a string.
 * @param filename The path to the file to be read.
 * @return std::string The content of the file.
 * @throw std::runtime_error if unable to open the file.
 */
std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("[readFileToString] Unable to open file");
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

/**
 * @brief Enables ANSI escape sequences for console output.
 * @throw std::runtime_error if unable to get or set console mode.
 */
void enableAnsiInConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (!GetConsoleMode(hConsole, &mode)) {
        throw std::runtime_error("Failed to get console mode");
    }
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, mode)) {
        throw std::runtime_error("Failed to set console mode");
    }
}

/**
 * @brief Checks if ANSI escape sequences are enabled in the console.
 * @return bool True if ANSI is enabled, false otherwise.
 */
bool isAnsiEnabledInConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (!GetConsoleMode(hConsole, &mode)) {
        return false;
    }
    return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

/**
 * @brief Validates a hex color code.
 * @param colorCode The hex color code to validate.
 * @return bool True if the color code is valid, false otherwise.
 */
bool isValidHexColor(const std::string& colorCode) {
    static const std::regex hexPattern("^#([A-Fa-f0-9]{6})$");
    return std::regex_match(colorCode, hexPattern);
}

/**
 * @brief Applies a hex color code to a given text string using ANSI escape sequences.
 * @param text The text to be colored.
 * @param colorCode The hex color code to apply.
 * @return std::string The text formatted with the specified color.
 */
std::string colorText(const std::string& text, const std::string& colorCode) {
    if (!isValidHexColor(colorCode)) {
        return "Invalid color code!";
    }

    int r = std::stoi(colorCode.substr(1, 2), nullptr, 16);
    int g = std::stoi(colorCode.substr(3, 2), nullptr, 16);
    int b = std::stoi(colorCode.substr(5, 2), nullptr, 16);

    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + text + "\033[0m";
}

/**
 * @brief Formats text as bold using ANSI escape sequences.
 * @param text The text to be formatted as bold.
 * @return std::string The bold-formatted text.
 * @throw std::runtime_error if ANSI is not enabled in the console.
 */
std::string boldText(const std::string& text) {
    if (!isAnsiEnabledInConsole()) {
        throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
    }
    return "\033[1m" + text + "\033[0m";
}

/**
 * @brief Formats text as italic using ANSI escape sequences.
 * @param text The text to be formatted as italic.
 * @return std::string The italic-formatted text.
 * @throw std::runtime_error if ANSI is not enabled in the console.
 */
std::string italicText(const std::string& text) {
    if (!isAnsiEnabledInConsole()) {
        throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
    }
    return "\033[3m" + text + "\033[0m";
}

/**
 * @brief Formats text with an underline using ANSI escape sequences.
 * @param text The text to be underlined.
 * @return std::string The underlined text.
 * @throw std::runtime_error if ANSI is not enabled in the console.
 */
std::string underlineText(const std::string& text) {
    if (!isAnsiEnabledInConsole()) {
        throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
    }
    return "\033[4m" + text + "\033[0m";
}

/**
 * @brief Applies multiple text formatting options using ANSI escape sequences.
 * @param text The text to be formatted.
 * @param bold Apply bold formatting if true.
 * @param italic Apply italic formatting if true.
 * @param underline Apply underline formatting if true.
 * @param colorCode Hex color code to apply (e.g., "#FF0000" for red).
 * @return std::string The formatted text.
 * @throw std::runtime_error if ANSI is not enabled in the console.
 */
std::string formatText(const std::string& text, bool bold, bool italic, bool underline, const std::string& colorCode) {
    if (!isAnsiEnabledInConsole()) {
        throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
    }

    std::string result;
    if (!colorCode.empty()) {
        result += colorText("", colorCode); // Apply color
    }
    if (bold) result += "\033[1m";
    if (italic) result += "\033[3m";
    if (underline) result += "\033[4m";

    result += text + "\033[0m"; // Add text and reset formatting
    return result;
}

/**
 * @brief Removes all ANSI escape sequences from a string.
 * @param text The text containing ANSI escape sequences.
 * @return std::string The text with all ANSI escape sequences removed.
 */
std::string stripAnsiEscapeCodes(const std::string& text) {
    static const std::regex ansiEscapePattern("\033\\[[0-9;]*[A-Za-z]");
    return std::regex_replace(text, ansiEscapePattern, "");
}

/**
 * @brief Truncates a string to a specified length, adding an ellipsis if truncated.
 * @param text The input text to truncate.
 * @param maxLength The maximum length of the truncated string, including the ellipsis.
 * @return std::string The truncated string.
 */
std::string truncateString(const std::string& text, size_t maxLength) {
    if (text.length() <= maxLength) {
        return text;
    }
    return text.substr(0, maxLength - 3) + "...";
}

/**
 * @brief Centers a string within a field of a given width.
 * @param text The text to be centered.
 * @param width The width of the field.
 * @param fillChar The character to use for padding (default is space).
 * @return std::string The centered string.
 */
std::string centerString(const std::string& text, size_t width, char fillChar = ' ') {
    if (text.length() >= width) {
        return text;
    }
    size_t padding = width - text.length();
    size_t leftPadding = padding / 2;
    size_t rightPadding = padding - leftPadding;
    return std::string(leftPadding, fillChar) + text + std::string(rightPadding, fillChar);
}

/**
 * @brief Wraps text to a specified line length.
 * @param text The input text to wrap.
 * @param lineLength The maximum length of each line.
 * @return std::string The wrapped text.
 */
std::string wrapText(const std::string& text, size_t lineLength) {
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;

    size_t currentLineLength = 0;
    while (words >> word) {
        if (currentLineLength + word.length() > lineLength) {
            wrapped << '\n';
            currentLineLength = 0;
        }
        if (currentLineLength > 0) {
            wrapped << ' ';
            currentLineLength++;
        }
        wrapped << word;
        currentLineLength += word.length();
    }

    return wrapped.str();
}

bool SetFileAssociationIcon(const std::wstring& fileExtension, const std::wstring& iconPath) {
    HKEY hKey;
    std::wstring regPath = L"SOFTWARE\\Classes\\" + fileExtension;
    
    // Create or open the key for the file extension
    LONG result = RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 
                                  REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to create/open registry key for file extension" << std::endl;
        return false;
    }
    RegCloseKey(hKey);

    // Create DefaultIcon subkey
    regPath += L"\\DefaultIcon";
    result = RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 
                             REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to create DefaultIcon subkey" << std::endl;
        return false;
    }

    // Set the default value of DefaultIcon to the icon path
    result = RegSetValueExW(hKey, NULL, 0, REG_SZ, 
                            reinterpret_cast<const BYTE*>(iconPath.c_str()), 
                            (iconPath.length() + 1) * sizeof(wchar_t));
    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Failed to set icon path in registry" << std::endl;
        return false;
    }

    // Notify the system about the change
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return true;
}

} // namespace Utils