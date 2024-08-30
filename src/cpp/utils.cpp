#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <iostream>
#include <vector>
#include <shellapi.h>
#include <strsafe.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>

/**
 * @namespace Utils
 * @brief A collection of utility functions for string manipulation, Windows API interactions, and more.
 */
namespace Utils
{
    /**
     * @brief Splits a string into a vector of substrings based on a specified delimiter.
     *
     * This function takes an input string `s` and a delimiter string `delimiter` as parameters.
     * It splits the input string `s` into multiple substrings wherever the delimiter is found,
     * and stores these substrings in a `std::vector<std::string>`. The function returns this 
     * vector of substrings.
     *
     * The function works by iteratively searching for the delimiter within the input string.
     * Each time the delimiter is found, the substring from the beginning of the string up to the
     * delimiter is extracted and added to the vector. The function continues to search through
     * the remainder of the string until no more delimiters are found. The final substring, which 
     * follows the last delimiter or is the entire string if no delimiter is found, is also added 
     * to the vector.
     *
     * @param[in] s The input string that is to be split.
     * @param[in] delimiter The string that serves as the delimiter, which separates the substrings.
     * 
     * @return std::vector<std::string> A vector containing the substrings obtained by splitting 
     * the input string `s` based on the delimiter.
     * 
     * @note If the delimiter is not found within the input string, the entire string will be 
     * returned as the only element in the vector.
     * 
     * @note If the delimiter is an empty string, the function will return a vector containing 
     * the original string as the only element. This is because an empty delimiter does not 
     * provide a valid splitting criterion.
     * 
     * @note If the input string `s` is empty, the function will return a vector with a single 
     * empty string element. This behavior ensures that the function always returns a non-empty 
     * vector, which might be useful in certain contexts.
     * 
     * @note If the delimiter appears at the start or end of the input string, the resulting 
     * vector will include empty strings as the first or last elements, respectively. Similarly, 
     * consecutive delimiters within the string will result in empty strings being added to the 
     * vector at those positions.
     * 
     * @example
     * @code
     * std::string text = "apple,orange,banana";
     * std::string delimiter = ",";
     * std::vector<std::string> result = split_string(text, delimiter);
     * // result now contains: {"apple", "orange", "banana"}
     * 
     * text = "apple,,banana";
     * result = split_string(text, delimiter);
     * // result now contains: {"apple", "", "banana"}
     * 
     * text = ",apple,orange,banana,";
     * result = split_string(text, delimiter);
     * // result now contains: {"", "apple", "orange", "banana", ""}
     * 
     * text = "apple";
     * delimiter = "";
     * result = split_string(text, delimiter);
     * // result now contains: {"apple"}
     * 
     * text = "";
     * delimiter = ",";
     * result = split_string(text, delimiter);
     * // result now contains: {""}
     * @endcode
     */
    std::vector<std::string> split_string(const std::string& s, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t pos = 0;
        std::string token;
        std::string copy = s;

        while ((pos = copy.find(delimiter)) != std::string::npos) {
            token = copy.substr(0, pos);
            tokens.push_back(token);
            copy.erase(0, pos + delimiter.length());
        }
        tokens.push_back(copy);

        return tokens;
    }

    /**
     * @brief Converts a standard C++ string to a wide string (std::wstring).
     * 
     * This function converts a standard C++ string (std::string) to a wide string (std::wstring).
     * It supports both UTF-8 and ANSI encoding based on the `_isUtf8Enabled` parameter.
     * 
     * @param[in] _string The string to be converted.
     * @param[in] _isUtf8Enabled A flag to enable UTF-8 encoding (default is true). If false, ANSI encoding is used.
     * 
     * @return std::wstring The converted wide string.
     * 
     * @note This function is particularly useful when interacting with Windows API functions that require wide strings.
     * 
     * @example
     * @code
     * std::string text = "Hello, World!";
     * std::wstring wideText = StringToWString(text);
     * @endcode
     */
    std::wstring StringToWString(const std::string& _string, bool _isUtf8Enabled = true)
    {
        int _len;
        int _stringLength = static_cast<int>(_string.length() + 1);
        _len = MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), _stringLength, nullptr, 0);
        std::wstring _buffer(_len, 0);
        MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), _stringLength, &_buffer[0], _len);
        return _buffer;
    }

    /**
     * @brief Converts a standard C++ string to a wide character string (LPCWSTR).
     * 
     * This function converts a standard C++ string (std::string) to a wide character string (LPCWSTR),
     * which is commonly used in Windows API functions.
     * 
     * @param[in] _string The string to be converted.
     * 
     * @return LPCWSTR The converted wide character string.
     * 
     * @note The returned LPCWSTR points to the internal data of a std::wstring, so it should not be modified or freed manually.
     * 
     * @example
     * @code
     * std::string text = "Hello, Windows!";
     * LPCWSTR wideText = StringToLPCWSTR(text);
     * @endcode
     */
    LPCWSTR StringToLPCWSTR(const std::string& _string) {
        std::wstring _wstr = StringToWString(_string);
        return _wstr.c_str();
    }

    /**
     * @brief Creates a Windows API 32-bit popup message box.
     * 
     * This function creates a simple popup message using the Windows API (32-bit version). 
     * It only works on Windows systems. The popup allows different types of message boxes 
     * based on the provided `_type` parameter.
     * 
     * @param[in] _title The title of the popup message.
     * @param[in] _message The content of the popup message.
     * @param[in] _type The type of the popup message:
     *              1 - AbortRetryIgnore
     *              2 - OkCancel
     *              3 - CancelTryContinue
     *              4 - YesNoCancel
     *              5 - YesNo
     *              6 - OK
     * 
     * @return int The ID of the selected option:
     *             1 - OK, 2 - Cancel, 3 - Abort, 4 - Retry, 5 - Ignore,
     *             6 - Yes, 7 - No, 10 - Try, 11 - Continue.
     * 
     * @note This function is useful for creating simple user notifications or prompts within a Windows application.
     * 
     * @example
     * @code
     * int result = CreateWinAPI32MessageBox("Warning", "Are you sure you want to exit?", 5);
     * if (result == 6) {
     *     // User clicked "Yes"
     * }
     * @endcode
     */
    int CreateWinAPI32MessageBox(const std::string& _title, const std::string& _message, int _type)
    {
        LPCWSTR _szTitle = StringToLPCWSTR(_title);
        LPCWSTR _szMessage = StringToLPCWSTR(_message);
        UINT _popupType;

        switch (_type)
        {
            case 1: _popupType = MB_ABORTRETRYIGNORE; break;
            case 2: _popupType = MB_OKCANCEL; break;
            case 3: _popupType = MB_CANCELTRYCONTINUE; break;
            case 4: _popupType = MB_YESNOCANCEL; break;
            case 5: _popupType = MB_YESNO; break;
            case 6: _popupType = MB_OK; break;
            default: 
                std::cerr << _type << " : Unknown Windows API 32-BIT-VERSION POPUP MESSAGE type '" << _type << "' !\n";
                exit(1); 
                break;
        }

        int _msgboxID = MessageBoxW(NULL, _szMessage, _szTitle, _popupType);
        return _msgboxID;
    }

    /**
     * @brief Creates a Windows API notification using the Win32 API.
     *
     * This function creates and displays a notification in the Windows system tray.
     * The notification includes a title, a message, and an icon, and can be customized
     * to represent different types of notifications such as information, error, warning, or none.
     *
     * @param[in] _title The title of the notification. This is a string that will be displayed as the notification's title.
     * @param[in] _message The message of the notification. This is the content text of the notification.
     * @param[in] _type The type of the notification. It is an integer that defines the icon and behavior of the notification:
     *        - 0: Information notification (default)
     *        - 1: Error notification
     *        - 2: Warning notification
     *        - 3: No icon notification
     *        If an invalid type is provided, the function will print an error message and terminate the program.
     *
     * @note The function internally uses the Win32 `Shell_NotifyIconW` function to add and modify the notification.
     * 
     * @warning The function calls `exit(1)` if an invalid notification type is passed. This will terminate the program.
     *
     * @details
     * - The notification is configured using a `NOTIFYICONDATAW` structure, which is filled with the appropriate information
     *   based on the provided parameters.
     * - The notification's icon is loaded using the `LoadIcon` function with the `IDI_INFORMATION` icon.
     * - The title and message strings are converted to wide strings (`std::wstring`) using a utility function (`Utils::StringToWString`).
     * - The notification's tooltip and other text fields are set using the `StringCchCopyW` function to ensure buffer safety.
     * - The notification will be displayed for 2000 milliseconds (2 seconds) before disappearing.
     * 
     * @note The function does not handle the notification's window handle (`hWnd`) and uses `NULL` for it. If the notification
     *       needs to interact with a specific window, this would need to be modified.
     * 
     * @example
     * @code
     * Utils::CreateWinAPI32BallonNotification("Update", "Your application has been updated successfully.", 0);
     * @endcode
     */
    void CreateWinAPI32BallonNotification(std::string _title, std::string _message, int _type = 0)
    {
        HINSTANCE _hInstance = GetModuleHandle(NULL);
        NOTIFYICONDATAW _nid = {};
        _nid.cbSize = sizeof(NOTIFYICONDATAW);
        _nid.hWnd = NULL;

        switch (_type)
        {
            case 0:
                _nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;  
                _nid.dwInfoFlags = NIIF_INFO;  
                _nid.uID = 1001;
                break;

            case 1:
                _nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;  
                _nid.dwInfoFlags = NIIF_ERROR; 
                _nid.uID = 1002;
                break;

            case 2:
                _nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;  
                _nid.dwInfoFlags = NIIF_WARNING;
                _nid.uID = 1003;
                break;
            
            case 3:
                _nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;  
                _nid.dwInfoFlags = NIIF_NONE; 
                _nid.uID = 1004;
                break;

            default:
                std::cout << "Unknown Notification type!";
                exit(1);
        }

        _nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);

        std::wstring _safeTitle = Utils::StringToWString(_title);
        std::wstring _safeMessage = Utils::StringToWString(_message);

        // Directly assigning wide strings to test the notification behavior
        StringCchCopyW(_nid.szTip, ARRAYSIZE(_nid.szTip), L"My App Notification"); // Tooltip text
        StringCchCopyW(_nid.szInfo, ARRAYSIZE(_nid.szInfo), _safeMessage.c_str());
        StringCchCopyW(_nid.szInfoTitle, ARRAYSIZE(_nid.szInfoTitle), _safeTitle.c_str());

        _nid.uTimeout = 2000;

        Shell_NotifyIconW(NIM_ADD, &_nid); // Use the Unicode version of the function
        Shell_NotifyIconW(NIM_MODIFY, &_nid); // Use the Unicode version of the function
    }  

    /**
     * @brief Retrieves the screen size of the specified monitor.
     * 
     * This function retrieves the size of the screen for a specified monitor index.
     * The size includes the full monitor area, not just the working area.
     * 
     * @param[in] monitorIndex The index of the monitor to retrieve the size for.
     * 
     * @return RECT The screen size of the monitor.
     * 
     * @note This function is useful for applications that need to handle multiple monitors or work with screen dimensions.
     * 
     * @warning If the monitor index does not exist, the function will print an error message and terminate the program.
     * 
     * @example
     * @code
     * RECT screenSize = GetMaximizedScreenSize(0);
     * int width = screenSize.right - screenSize.left;
     * int height = screenSize.bottom - screenSize.top;
     * @endcode
     */
    RECT GetMaximizedScreenSize(int monitorIndex) {
        DISPLAY_DEVICE dd = { sizeof(dd) };
        int deviceIndex = 0;
        bool monitorFound = false;
        RECT screenSize = {0, 0, 0, 0};

        // Enumerate display devices
        while (EnumDisplayDevices(NULL, deviceIndex, &dd, 0)) {
            if (deviceIndex == monitorIndex) {
                monitorFound = true;
                break;
            }
            deviceIndex++;
        }

        if (!monitorFound) {
            std::cerr << "Monitor index " << monitorIndex << " does not exist.\n";
            exit(1);
        }

        // Get the monitor info
        HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
           screenSize = monitorInfo.rcMonitor;  // Use rcMonitor instead of rcWork
        } else {
            std::cerr << "Failed to get monitor info.\n";
            exit(1);
        }

        return screenSize;
    } 

    /**
     * @brief Trims whitespace from the start of a string.
     * 
     * This function removes leading whitespace characters from the input string `s`.
     * 
     * @param[in, out] s The string to be trimmed.
     * 
     * @return std::string& A reference to the trimmed string.
     * 
     * @note This function modifies the original string and returns a reference to it.
     * 
     * @example
     * @code
     * std::string text = "   Hello, World!   ";
     * std::string trimmed = Utils::ltrim(text);
     * // trimmed now contains "Hello, World!   "
     * @endcode
     */
    inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                    [](int ch) { return !std::isspace(ch); }));
        return s;
    }

    /**
     * @brief Trims whitespace from the end of a string.
     * 
     * This function removes trailing whitespace characters from the input string `s`.
     * 
     * @param[in, out] s The string to be trimmed.
     * 
     * @return std::string& A reference to the trimmed string.
     * 
     * @note This function modifies the original string and returns a reference to it.
     * 
     * @example
     * @code
     * std::string text = "Hello, World!   ";
     * std::string trimmed = Utils::trim(text);
     * // trimmed now contains "Hello, World!"
     * @endcode
     */
    inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                    [](int ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
    }

    /**
     * @brief Trims whitespace from both ends of a string.
     * 
     * This function removes both leading and trailing whitespace characters from the input string `s`.
     * 
     * @param[in, out] s The string to be trimmed.
     * 
     * @return std::string& A reference to the trimmed string.
     * 
     * @note This function modifies the original string and returns a reference to it.
     * 
     * @example
     * @code
     * std::string text = "   Hello, World!   ";
     * std::string trimmed = Utils::trim(text);
     * // trimmed now contains "Hello, World!"
     * @endcode
     */
    inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
    }
   
    /**
     * @brief Logs a message to the console based on the provided boolean flag.
     * 
     * This function prints a message to the console if the `isPrintTrue` parameter is set to `true`.
     * It provides a simple mechanism for conditional logging, which can be controlled at runtime.
     * 
     * @param[in] str The message to be logged.
     * @param[in] isPrintTrue A boolean flag that determines whether the message should be printed (default is true).
     * 
     * @example
     * @code
     * Utils::general_log("This is a log message.", true); // This will print the message
     * Utils::general_log("This message will not be printed.", false); // This will not print the message
     * @endcode
     */
    int general_log(const std::string &str, bool isPrintTrue = true)
    {
        if (!isPrintTrue)
        {
            return 0;
        }

        std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/logs.txt", std::ios::app);

        if (!outputFile.is_open()) {
            std::cerr << "[general_log] Failed to open file." << std::endl;
            return 1;
        }

        outputFile << str << "\n"; // Write the string to the file
        outputFile.close();

        return 0;
    }

    /**
     * @brief Clears the content of the logs file.
     *
     * This function opens the file located at "C:/coding-projects/CPP-Dev/bassil/assets/logs.txt" 
     * in output mode, effectively clearing its content. If the file cannot be opened, 
     * an error message is printed to the standard error stream, and the function 
     * returns 1. On successful completion, the function returns 0.
     *
     * @return int Returns 0 on success, or 1 if the file could not be opened.
     */
    int clear_logs()
    {
        std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/logs.txt", std::ios::out);
        
        if (!outputFile.is_open()) {
            std::cerr << "[clear_logs] Failed to open file." << std::endl;
            return 1;
        }
        
        outputFile << ""; 
        outputFile.close();

        return 0;
    }

    /**
     * @brief Clears the content of the lexical analysis output file.
     *
     * This function opens the file located at "C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt" 
     * in output mode, effectively clearing its content. If the file cannot be opened, 
     * an error message is printed to the standard error stream, and the function 
     * returns 1. On successful completion, the function returns 0.
     *
     * @return int Returns 0 on success, or 1 if the file could not be opened.
     */
    int clear_lex_out()
    {
        std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/assets/after_lex.txt", std::ios::out);
        
        if (!outputFile.is_open()) {
            std::cerr << "[clear_lex_out] Failed to open file." << std::endl;
            return 1;
        }
        
        outputFile << ""; 
        outputFile.close();

        return 0;
    }

    /**
     * @brief Reads the entire content of a file into a string.
     *
     * This function opens the specified file, reads its content line by line, 
     * and stores it in a string. If the file cannot be opened, an error message 
     * is printed to the standard error stream, and an empty string is returned.
     *
     * @param filename The path to the file that should be read.
     * @return std::string The content of the file as a string, or an empty string if the file could not be opened.
     */
    std::string readFileToString(const std::string& filename) {
        std::ifstream file(filename);  // Open the file
        std::stringstream buffer;      // Create a stringstream to hold the file content
        std::string line;

        if (file.is_open()) {
            while (std::getline(file, line)) {  // Read each line from the file
                buffer << line << "\n";         // Append the line and a newline character to the buffer
            }
            file.close();  // Close the file
        } else {
            std::cerr << "[readFileToString] Unable to open file";
        }

        return buffer.str();  // Convert the buffer content to a string and return it
    }

    /**
     * @brief Enables ANSI escape sequences for console output.
     *
     * This function modifies the console mode to enable virtual terminal processing, 
     * which allows ANSI escape sequences to be recognized by the console. 
     * If the console mode cannot be retrieved, an error message is printed to the standard error stream.
     */
    void enableAnsiInConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(hConsole, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, mode);
        } else {
            std::cerr << "Failed to get console mode." << std::endl;
        }
    }

    /**
     * @brief Checks if ANSI escape sequences are enabled in the console.
     *
     * This function retrieves the current console mode and checks if the 
     * `ENABLE_VIRTUAL_TERMINAL_PROCESSING` flag is set, indicating that 
     * ANSI escape sequences are enabled.
     *
     * @return bool Returns true if ANSI escape sequences are enabled, false otherwise.
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
     *
     * This function checks if the provided string is a valid hex color code 
     * in the format `#RRGGBB`, where `RR`, `GG`, and `BB` are two-digit 
     * hexadecimal values.
     *
     * @param colorCode The hex color code to validate.
     * @return bool Returns true if the color code is valid, false otherwise.
     */
    bool isValidHexColor(const std::string& colorCode) {
        const std::regex hexPattern("^#([A-Fa-f0-9]{6})$");
        return std::regex_match(colorCode, hexPattern);
    }

    /**
     * @brief Applies a hex color code to a given text string using ANSI escape sequences.
     *
     * This function formats the provided text with the specified hex color code using 
     * ANSI escape sequences. The hex color code must be in the format `#RRGGBB`. If the 
     * color code is invalid, the function returns "Invalid color code!".
     *
     * @param text The text to be colored.
     * @param colorCode The hex color code to apply.
     * @return std::string The text formatted with the specified color, or an error message if the color code is invalid.
     */
    std::string colorText(const std::string& text, const std::string& colorCode) {
        const std::regex hexPattern("^#([A-Fa-f0-9]{6})$");

        if (std::regex_match(colorCode, hexPattern)) {
            // Extract RGB components from the hex color code
            std::string r = colorCode.substr(1, 2);
            std::string g = colorCode.substr(3, 2);
            std::string b = colorCode.substr(5, 2);

            int ri = std::stoi(r, nullptr, 16);
            int gi = std::stoi(g, nullptr, 16);
            int bi = std::stoi(b, nullptr, 16);

            // Construct the ANSI escape code for the color
            return "\033[38;2;" + std::to_string(ri) + ";" + std::to_string(gi) + ";" + std::to_string(bi) + "m" + text + "\033[0m";
        } else {
            // If the color code is invalid, return the text unmodified
            return "Invalid color code!";
        }
    }

    /**
     * @brief Formats text as bold using ANSI escape sequences.
     *
     * This function formats the provided text as bold using ANSI escape sequences. 
     * If ANSI escape sequences are not enabled in the console, an error message 
     * is printed to the standard error stream, and the program exits with a status code of 1.
     *
     * @param text The text to be formatted as bold.
     * @return std::string The bold-formatted text, or an error message if ANSI is not enabled.
     */
    std::string boldText(const std::string& text) {
        if (isAnsiEnabledInConsole() == true)
        {
            return "\033[1m" + text + "\033[0m";
        }

        else
        {
            std::cerr << "Error when calling boldText() method, likely enableAnsiInConsole() was not called" << std::endl;
            exit(1);
        }
    }

    /**
     * @brief Formats text as italic using ANSI escape sequences.
     *
     * This function formats the provided text as italic using ANSI escape sequences. 
     * If ANSI escape sequences are not enabled in the console, an error message 
     * is printed to the standard error stream, and the program exits with a status code of 1.
     *
     * @param text The text to be formatted as italic.
     * @return std::string The italic-formatted text, or an error message if ANSI is not enabled.
     */
    std::string italicText(const std::string& text) {
        if (isAnsiEnabledInConsole() == true)
        {
            return "\033[3m" + text + "\033[0m";
        }

        else
        {
            std::cerr << "Error when calling italicText() method, likely enableAnsiInConsole() was not called" << std::endl;
            exit(1);
        }
    }

    /**
     * @brief Formats text with an underline using ANSI escape sequences.
     *
     * This function formats the provided text with an underline using ANSI escape sequences. 
     * If ANSI escape sequences are not enabled in the console, an error message 
     * is printed to the standard error stream, and the program exits with a status code of 1.
     *
     * @param text The text to be underlined.
     * @return std::string The underlined text, or an error message if ANSI is not enabled.
     */
    std::string underlineText(const std::string& text) {
        if (isAnsiEnabledInConsole() == true)
        {
            return "\033[4m" + text + "\033[0m";
        }

        else
        {
            std::cerr << "Error when calling underlineText() method, likely enableAnsiInConsole() was not called" << std::endl;
            exit(1);
        }
    }
}
#endif // UTILS_H