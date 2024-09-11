/**
 * @file utils.cpp
 * @brief Comprehensive implementation of utility functions for the Bassil language project.
 *
 * This file contains a collection of utility functions that provide various
 * string manipulation, Windows API interaction, file handling, and console
 * output formatting capabilities. These utilities are designed to support
 * the core functionality of the Bassil language project.
 *
 * @note This file requires Windows-specific headers and may not be
 * compatible with non-Windows environments.
 *
 * @author Nerd Bear
 * @date 31 August 2024
 * @version 0.2.3
 *
 * @copyright Copyright (c) 2024 Bassil
 *
 * @see utils.h
 */

#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <limits>
#include <strsafe.h> // include for StringCchCopyW

/**
 * @brief Fallback implementation of StringCchCopyW if not available in the system headers.
 *
 * This macro provides a simple implementation of StringCchCopyW using wcscpy_s
 * if the function is not available in the included headers. It ensures
 * compatibility across different Windows environments.
 *
 * @param dest Destination wide-character string buffer.
 * @param destSize Size of the destination buffer in characters.
 * @param src Source wide-character string to be copied.
 *
 * @note This macro should only be used if StringCchCopyW is not defined.
 */
#ifndef StringCchCopyW
#define StringCchCopyW(dest, destSize, src) wcscpy_s(dest, destSize, src)
#endif

namespace Utils
{

    /**
     * @brief Splits a string into a vector of substrings based on a specified delimiter.
     *
     * This function takes an input string and a delimiter, and splits the input
     * string into multiple substrings wherever the delimiter is found. The
     * resulting substrings are stored in a vector and returned.
     *
     * @param s The input string to be split.
     * @param delimiter The string that serves as the delimiter for splitting.
     *
     * @return std::vector<std::string> A vector containing the substrings resulting
     *         from splitting the input string.
     *
     * @note The delimiter itself is not included in any of the resulting substrings.
     * @note If the delimiter is not found in the input string, the entire input
     *       string will be returned as a single element in the vector.
     * @note Empty substrings are included in the result if multiple delimiters
     *       are found consecutively.
     *
     * @see std::string::find
     * @see std::string::substr
     *
     * @par Example:
     * @code
     * std::string input = "apple,banana,cherry";
     * std::vector<std::string> result = Utils::split_string(input, ",");
     * // result now contains {"apple", "banana", "cherry"}
     * @endcode
     */
    std::vector<std::string> split_string(const std::string &s, const std::string &delimiter = " ")
    {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = s.find(delimiter, start)) != std::string::npos)
        {
            tokens.push_back(s.substr(start, end - start));
            start = end + delimiter.length();
        }
        tokens.push_back(s.substr(start));
        return tokens;
    }

    /**
     * @brief Converts a standard string to a wide string.
     *
     * This function takes a standard string (std::string) and converts it to
     * a wide string (std::wstring). It supports both UTF-8 and ASCII encodings,
     * determined by the _isUtf8Enabled flag.
     *
     * @param _string The input string to be converted.
     * @param _isUtf8Enabled Flag to indicate if UTF-8 encoding should be used.
     *        If true, the function uses UTF-8 encoding; if false, it uses ASCII.
     *        Default value is true.
     *
     * @return std::wstring The resulting wide string after conversion.
     *
     * @note This function uses the Windows API function MultiByteToWideChar for
     *       the conversion process.
     * @note The function first calculates the required buffer size, then
     *       allocates the buffer and performs the conversion.
     * @note If the conversion fails, an empty wide string is returned.
     *
     * @see MultiByteToWideChar (Windows API)
     *
     * @par Example:
     * @code
     * std::string narrowStr = "Hello, 世界";
     * std::wstring wideStr = Utils::StringToWString(narrowStr, true);
     * @endcode
     */
    std::wstring StringToWString(const std::string &_string, bool _isUtf8Enabled)
    {
        int len = MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), -1, NULL, 0);
        std::wstring wstr(len - 1, 0);
        MultiByteToWideChar(_isUtf8Enabled ? CP_UTF8 : CP_ACP, 0, _string.c_str(), -1, &wstr[0], len);
        return wstr;
    }

    /**
     * @brief Converts a standard string to a wide character string (LPCWSTR).
     *
     * This function takes a standard string (std::string) and converts it to
     * a wide character string (LPCWSTR). It's particularly useful when working
     * with Windows API functions that require LPCWSTR parameters.
     *
     * @param _string The input string to be converted.
     *
     * @return LPCWSTR The resulting wide character string after conversion.
     *
     * @note This function internally uses the StringToWString function for conversion.
     * @note The returned LPCWSTR points to a static std::wstring member. This means
     *       the pointer is valid only until the next call to this function.
     * @warning The returned pointer should not be stored or used after subsequent
     *          calls to this function, as it may be invalidated.
     *
     * @see StringToWString
     *
     * @par Example:
     * @code
     * std::string narrowStr = "Hello, World!";
     * LPCWSTR wideStr = Utils::StringToLPCWSTR(narrowStr);
     * // Use wideStr with Windows API functions
     * @endcode
     */
    LPCWSTR StringToLPCWSTR(const std::string &_string)
    {
        static std::wstring wstr;
        wstr = StringToWString(_string);
        return wstr.c_str();
    }

    /**
     * @brief Creates a Windows API 32-bit message box.
     *
     * This function displays a message box using the Windows API. It allows
     * customization of the title, message content, and the type of buttons
     * displayed in the message box.
     *
     * @param _title The title of the message box.
     * @param _message The content of the message box.
     * @param _type The type of the message box, determining which buttons are displayed.
     *        Valid values are:
     *        - 1: MB_ABORTRETRYIGNORE (Abort, Retry, Ignore buttons)
     *        - 2: MB_OKCANCEL (OK, Cancel buttons)
     *        - 3: MB_CANCELTRYCONTINUE (Cancel, Try Again, Continue buttons)
     *        - 4: MB_YESNOCANCEL (Yes, No, Cancel buttons)
     *        - 5: MB_YESNO (Yes, No buttons)
     *        - 6: MB_OK (OK button)
     *
     * @return int The ID of the button clicked by the user. The return value
     *         depends on the type of message box displayed.
     *
     * @throw std::runtime_error if an invalid message box type is provided.
     *
     * @note This function uses the Windows API function MessageBoxW for displaying
     *       the message box.
     * @note The title and message are converted to wide strings using StringToLPCWSTR.
     *
     * @see MessageBoxW (Windows API)
     * @see StringToLPCWSTR
     *
     * @par Example:
     * @code
     * int result = Utils::CreateWinAPI32MessageBox("Warning", "Are you sure?", 5);
     * if (result == IDYES) {
     *     // User clicked Yes
     * } else {
     *     // User clicked No
     * }
     * @endcode
     */
    int CreateWinAPI32MessageBox(const std::string &_title, const std::string &_message, int _type)
    {
#ifdef _WINDOWS_
        UINT _popupType;
        switch (_type)
        {
        case 1:
            _popupType = MB_ABORTRETRYIGNORE;
            break;
        case 2:
            _popupType = MB_OKCANCEL;
            break;
        case 3:
            _popupType = MB_CANCELTRYCONTINUE;
            break;
        case 4:
            _popupType = MB_YESNOCANCEL;
            break;
        case 5:
            _popupType = MB_YESNO;
            break;
        case 6:
            _popupType = MB_OK;
            break;
        default:
            throw std::runtime_error("Unknown Windows API 32-BIT-VERSION POPUP MESSAGE type");
        }
        return MessageBoxW(NULL, StringToLPCWSTR(_message), StringToLPCWSTR(_title), _popupType);
#else
        std::cerr << "Windows API 32-BIT-VERSION not available\n";
        exit(-1);
        return -1;
#endif
    }

    /**
     * @brief Creates a Windows API balloon notification.
     *
     * This function displays a balloon notification (also known as a toast notification)
     * using the Windows API. It allows customization of the title, message content,
     * and the type of notification icon displayed.
     *
     * @param _title The title of the notification.
     * @param _message The content of the notification.
     * @param _type The type of the notification, determining the icon displayed:
     *        - 0: NIIF_INFO (Information icon)
     *        - 1: NIIF_ERROR (Error icon)
     *        - 2: NIIF_WARNING (Warning icon)
     *        - 3: NIIF_NONE (No icon)
     *
     * @throw std::runtime_error if an invalid notification type is provided.
     *
     * @note This function uses the Windows API function Shell_NotifyIconW for
     *       displaying the notification.
     * @note The notification is displayed using the system tray (notification area).
     * @note The notification will automatically disappear after a system-defined timeout.
     * @note This function does not create a persistent notification icon in the system tray.
     *
     * @see Shell_NotifyIconW (Windows API)
     * @see NOTIFYICONDATAW (Windows API structure)
     *
     * @par Example:
     * @code
     * Utils::CreateWinAPI32BallonNotification("Update Available", "A new version is ready to install.", 0);
     * @endcode
     */
    void CreateWinAPI32BallonNotification(const std::string &_title, const std::string &_message, int _type)
    {
#ifdef _WINDOWS_
        NOTIFYICONDATAW nid = {};
        nid.cbSize = sizeof(NOTIFYICONDATAW);
        nid.hWnd = NULL;
        nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;

        switch (_type)
        {
        case 0:
            nid.dwInfoFlags = NIIF_INFO;
            nid.uID = 1001;
            break;
        case 1:
            nid.dwInfoFlags = NIIF_ERROR;
            nid.uID = 1002;
            break;
        case 2:
            nid.dwInfoFlags = NIIF_WARNING;
            nid.uID = 1003;
            break;
        case 3:
            nid.dwInfoFlags = NIIF_NONE;
            nid.uID = 1004;
            break;
        default:
            throw std::runtime_error("Unknown Notification type");
        }

        nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
        StringCchCopyW(nid.szTip, ARRAYSIZE(nid.szTip), L"My App Notification");
        StringCchCopyW(nid.szInfo, ARRAYSIZE(nid.szInfo), StringToWString(_message).c_str());
        StringCchCopyW(nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle), StringToWString(_title).c_str());
        nid.uTimeout = 2000;

        Shell_NotifyIconW(NIM_ADD, &nid);
        Shell_NotifyIconW(NIM_MODIFY, &nid);
#else
        std::cerr << "Windows API 32-BIT-VERSION not available\n";
        exit(-1);
#endif
    }

    /**
     * @brief Retrieves the maximum screen size for a given monitor.
     *
     * This function returns the dimensions of the specified monitor in a Windows
     * multi-monitor setup. It's useful for determining the available screen space
     * on a particular monitor.
     *
     * @param monitorIndex The index of the monitor (0-based).
     *
     * @return RECT A structure containing the screen dimensions:
     *         - left: The x-coordinate of the upper-left corner of the rectangle.
     *         - top: The y-coordinate of the upper-left corner of the rectangle.
     *         - right: The x-coordinate of the lower-right corner of the rectangle.
     *         - bottom: The y-coordinate of the lower-right corner of the rectangle.
     *
     * @throw std::runtime_error if the monitor index does not exist or if unable to get monitor info.
     *
     * @note This function uses Windows API functions EnumDisplayDevices and GetMonitorInfo.
     * @note The function assumes that monitor indices are contiguous and start from 0.
     * @note For single-monitor systems, use index 0 to get the primary monitor dimensions.
     *
     * @see EnumDisplayDevices (Windows API)
     * @see GetMonitorInfo (Windows API)
     * @see MONITORINFO (Windows API structure)
     *
     * @par Example:
     * @code
     * try {
     *     RECT screenSize = Utils::GetMaximizedScreenSize(0); // Primary monitor
     *     std::cout << "Width: " << (screenSize.right - screenSize.left) << std::endl;
     *     std::cout << "Height: " << (screenSize.bottom - screenSize.top) << std::endl;
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    RECT GetMaximizedScreenSize(int monitorIndex)
    {
#ifdef _WINDOWS_
        DISPLAY_DEVICE dd = {sizeof(dd)};
        int deviceIndex = 0;

        while (EnumDisplayDevices(NULL, deviceIndex, &dd, 0))
        {
            if (deviceIndex == monitorIndex)
            {
                HMONITOR hMonitor = MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);
                MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
                if (GetMonitorInfo(hMonitor, &monitorInfo))
                {
                    return monitorInfo.rcMonitor;
                }
                else
                {
                    throw std::runtime_error("Failed to get monitor info");
                }
            }
            deviceIndex++;
        }
        throw std::runtime_error("Monitor index does not exist");
#else
        std::cerr << "Windows API 32-BIT-VERSION not available\n";
        exit(-1)
#endif
    }

    /**
     * @brief Trims leading whitespace from a string.
     *
     * This function removes all leading whitespace characters from the input string.
     * Whitespace characters include spaces, tabs, line feeds, carriage returns, etc.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     *
     * @see std::isspace
     * @see rtrim
     * @see trim
     *
     * @par Example:
     * @code
     * std::string str = "   Hello, World!";
     * Utils::ltrim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
        return s;
    }

    /**
     * @brief Trims trailing whitespace from a string.
     *
     * This function removes all trailing whitespace characters from the input string.
     * Whitespace characters include spaces, tabs, line feeds, carriage returns, etc.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     * @note The function uses reverse iterators for efficient trimming from the end of the string.
     *
     * @see std::isspace
     * @see ltrim
     * @see trim
     *
     * @par Example:
     * @code
     * std::string str = "Hello, World!   ";
     * Utils::rtrim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
        return s;
    }

    /**
     * @brief Trims both leading and trailing whitespace from a string.
     *
     * This function removes all leading and trailing whitespace characters from the input string.
     * It combines the functionality of ltrim() and rtrim() into a single function call.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     * @note This function is equivalent to calling ltrim() followed by rtrim().
     *
     * @see std::isspace
     * @see ltrim
     * @see rtrim
     *
     * @par Example:
     * @code
     * std::string str = "   Hello, World!   ";
     * Utils::trim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &trim(std::string &s)
    {
        return ltrim(rtrim(s));
    }

    /**
     * @brief Logs a message to a file and optionally prints it.
     *
     * This function writes a log message to a specified file and optionally prints it to the console.
     * It's useful for maintaining a record of program execution and debugging.
     *
     * @param str The message to be logged.
     * @param isPrintTrue Flag to indicate if the message should be printed to the console.
     *        If true, the message is both logged to the file and printed to the console.
     *        If false, the message is only logged to the file. Default is true.
     *
     * @return int Returns 0 on successful logging, 1 if there was an error opening the log file.
     *
     * @note The log file is located at "C:/coding-projects/CPP-Dev/bassil/output/Run-0001/logs.log".
     * @note The function opens the file in append mode, so new log entries are added to the end of the file.
     * @note Each log entry is written on a new line.
     *
     * @see std::ofstream
     *
     * @par Example:
     * @code
     * int result = Utils::general_log("Application started", true);
     * if (result != 0) {
     *     std::cerr << "Failed to write to log file" << std::endl;
     * }
     * @endcode
     */
    int general_log(const std::string &str, bool isPrintTrue)
    {
        if (!isPrintTrue)
        {
            return 0;
        }

        std::ofstream outputFile("C:/coding-projects/CPP-Dev/bassil/output/logs.log", std::ios::app);
        if (!outputFile.is_open())
        {
            std::cerr << "[general_log] Failed to open file." << std::endl;
            return 1;
        }
        outputFile << str << "\n";
        outputFile.close();
        return 0;
    }

    int clear_file(const std::string &filename)
    {
        std::ofstream outputFile(filename, std::ios::trunc);
        if (!outputFile.is_open())
        {
            std::cerr << "[clear_logs] Failed to open file." << std::endl;
            return 1;
        }
        outputFile.close();
        return 0;
    }

    /**
     * @brief Reads the entire content of a file into a string.
     *
     * This function opens the specified file and reads its entire content into a string.
     * It's useful for loading configuration files, templates, or any text-based file
     * into memory for further processing.
     *
     * @param filename The path to the file to be read.
     *
     * @return std::string The content of the file as a string.
     *
     * @throw std::runtime_error if unable to open the file.
     *
     * @note The function uses std::istreambuf_iterator for efficient reading of the entire file.
     * @note Large files may consume significant memory. Use with caution for very large files.
     *
     * @see std::ifstream
     * @see std::istreambuf_iterator
     *
     * @par Example:
     * @code
     * try {
     *     std::string content = Utils::readFileToString("config.txt");
     *     std::cout << "File content: " << content << std::endl;
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string readFileToString(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("[readFileToString] Unable to open file");
        }
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    /**
     * @brief Enables ANSI escape sequences for console output.
     *
     * This function enables ANSI escape sequence processing for the console output,
     * allowing the use of colored text and other formatting in the console.
     * It's typically used in Windows environments where ANSI support is not enabled by default.
     *
     * @throw std::runtime_error if unable to get or set console mode.
     *
     * @note This function uses Windows API functions GetStdHandle, GetConsoleMode, and SetConsoleMode.
     * @note ANSI escape sequences are used for text formatting, colors, cursor movement, etc.
     * @note This function should be called once at the beginning of the program if ANSI support is needed.
     *
     * @see GetStdHandle (Windows API)
     * @see GetConsoleMode (Windows API)
     * @see SetConsoleMode (Windows API)
     *
     * @par Example:
     * @code
     * try {
     *     Utils::enableAnsiInConsole();
     *     std::cout << "\033[31mThis text is red\033[0m" << std::endl;
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    int enableAnsiInConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(hConsole, &mode))
        {
            return 1;
        }
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hConsole, mode))
        {
            return 1;
        }

        return 0;
    }

    /**
     * @brief Checks if ANSI escape sequences are enabled in the console.
     *
     * This function determines whether ANSI escape sequence processing is currently
     * enabled for the console output. It's useful for conditional formatting or
     * for deciding whether to use ANSI escape sequences in console output.
     *
     * @return bool Returns true if ANSI escape sequences are enabled, false otherwise.
     *
     * @note This function uses Windows API functions GetStdHandle and GetConsoleMode.
     * @note The function checks for the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag in the console mode.
     *
     * @see GetStdHandle (Windows API)
     * @see GetConsoleMode (Windows API)
     * @see ENABLE_VIRTUAL_TERMINAL_PROCESSING
     *
     * @par Example:
     * @code
     * if (Utils::isAnsiEnabledInConsole()) {
     *     std::cout << "\033[32mANSI sequences are supported!\033[0m" << std::endl;
     * } else {
     *     std::cout << "ANSI sequences are not supported." << std::endl;
     * }
     * @endcode
     */
    bool isAnsiEnabledInConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(hConsole, &mode))
        {
            return false;
        }
        return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }

    /**
     * @brief Validates a hex color code.
     *
     * This function checks if a given string represents a valid hexadecimal color code.
     * A valid hex color code starts with a '#' followed by exactly 6 hexadecimal digits.
     *
     * @param colorCode The string to be validated as a hex color code.
     *
     * @return bool Returns true if the color code is a valid hex color, false otherwise.
     *
     * @note The function is case-insensitive, so both uppercase and lowercase letters are accepted.
     * @note The function uses a regular expression for validation.
     *
     * @see std::regex
     * @see std::regex_match
     *
     * @par Example:
     * @code
     * std::cout << Utils::isValidHexColor("#FF00FF") << std::endl; // Outputs: 1 (true)
     * std::cout << Utils::isValidHexColor("#G12345") << std::endl; // Outputs: 0 (false)
     * std::cout << Utils::isValidHexColor("FF00FF") << std::endl;  // Outputs: 0 (false, missing #)
     * @endcode
     */
    bool isValidHexColor(const std::string &colorCode)
    {
        static const std::regex hexPattern("^#([A-Fa-f0-9]{6})$");
        return std::regex_match(colorCode, hexPattern);
    }

    /**
     * @brief Applies a hex color code to a given text string using ANSI escape sequences.
     *
     * This function takes a text string and a hex color code, and returns the text
     * formatted with ANSI escape sequences to display the text in the specified color.
     *
     * @param text The text to be colored.
     * @param colorCode The hex color code to apply (e.g., "#FF0000" for red).
     *
     * @return std::string The text formatted with ANSI escape sequences for the specified color.
     *
     * @note The function first validates the hex color code using isValidHexColor().
     * @note If the color code is invalid, the function returns an error message instead of the formatted text.
     * @note The function converts the hex color to RGB values for use in the ANSI escape sequence.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent color bleeding.
     *
     * @see isValidHexColor()
     * @see std::stoi()
     *
     * @par Example:
     * @code
     * std::string coloredText = Utils::colorText("Hello, World!", "#00FF00");
     * std::cout << coloredText << std::endl; // Outputs "Hello, World!" in green
     * @endcode
     */
    std::string colorText(const std::string &text, const std::string &colorCode)
    {
        if (!isValidHexColor(colorCode))
        {
            return "Invalid color code!";
        }

        int r = std::stoi(colorCode.substr(1, 2), nullptr, 16);
        int g = std::stoi(colorCode.substr(3, 2), nullptr, 16);
        int b = std::stoi(colorCode.substr(5, 2), nullptr, 16);

        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + text + "\033[0m";
    }

    /**
     * @brief Formats text as bold using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted as bold
     * using ANSI escape sequences.
     *
     * @param text The text to be formatted as bold.
     *
     * @return std::string The bold-formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string boldText = Utils::boldText("Important Message");
     *     std::cout << boldText << std::endl; // Outputs "Important Message" in bold
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string boldText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[1m" + text + "\033[0m";
    }

    /**
     * @brief Formats text as italic using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted as italic
     * using ANSI escape sequences.
     *
     * @param text The text to be formatted as italic.
     *
     * @return std::string The italic-formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     * @note Not all console fonts support italic text. The result may vary depending on the console used.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string italicText = Utils::italicText("Emphasized text");
     *     std::cout << italicText << std::endl; // Outputs "Emphasized text" in italic
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string italicText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[3m" + text + "\033[0m";
    }

    /**
     * @brief Formats text with an underline using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted with an underline
     * using ANSI escape sequences.
     *
     * @param text The text to be underlined.
     *
     * @return std::string The underlined text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string underlinedText = Utils::underlineText("Important link");
     *     std::cout << underlinedText << std::endl; // Outputs "Important link" with an underline
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string underlineText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[4m" + text + "\033[0m";
    }

    /**
     * @brief Applies multiple text formatting options using ANSI escape sequences.
     *
     * This function takes a text string and applies multiple formatting options
     * (bold, italic, underline, and color) using ANSI escape sequences.
     *
     * @param text The text to be formatted.
     * @param bold Apply bold formatting if true.
     * @param italic Apply italic formatting if true.
     * @param underline Apply underline formatting if true.
     * @param colorCode Hex color code to apply (e.g., "#FF0000" for red).
     *
     * @return std::string The formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The color is applied first, followed by bold, italic, and underline formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     * @note If the color code is invalid or empty, no color will be applied.
     *
     * @see isAnsiEnabledInConsole()
     * @see colorText()
     *
     * @par Example:
     * @code
     * try {
     *     std::string formattedText = Utils::formatText("Formatted Text", true, false, true, "#00FF00");
     *     std::cout << formattedText << std::endl; // Outputs "Formatted Text" in green, bold, and underlined
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string formatText(const std::string &text, bool bold, bool italic, bool underline, const std::string &colorCode)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }

        std::string result;
        if (!colorCode.empty())
        {
            result += colorText("", colorCode); // Apply color
        }
        if (bold)
            result += "\033[1m";
        if (italic)
            result += "\033[3m";
        if (underline)
            result += "\033[4m";

        result += text + "\033[0m"; // Add text and reset formatting
        return result;
    }

    /**
     * @brief Removes all ANSI escape sequences from a string.
     *
     * This function takes a string that may contain ANSI escape sequences and
     * removes all such sequences, returning the plain text content.
     *
     * @param text The text containing ANSI escape sequences.
     *
     * @return std::string The text with all ANSI escape sequences removed.
     *
     * @note This function uses a regular expression to identify and remove ANSI escape sequences.
     * @note The function will remove all sequences starting with '\033[' and ending with a letter.
     *
     * @see std::regex
     * @see std::regex_replace
     *
     * @par Example:
     * @code
     * std::string coloredText = "\033[31mRed Text\033[0m \033[1mBold Text\033[0m";
     * std::string plainText = Utils::stripAnsiEscapeCodes(coloredText);
     * std::cout << plainText << std::endl; // Outputs: "Red Text Bold Text"
     * @endcode
     */
    std::string stripAnsiEscapeCodes(const std::string &text)
    {
        static const std::regex ansiEscapePattern("\033\\[[0-9;]*[A-Za-z]");
        return std::regex_replace(text, ansiEscapePattern, "");
    }

    /**
     * @brief Truncates a string to a specified length, adding an ellipsis if truncated.
     *
     * This function takes an input string and a maximum length, and returns a string
     * that is no longer than the specified length. If the input string is longer than
     * the maximum length, it is truncated and an ellipsis (...) is appended.
     *
     * @param text The input text to truncate.
     * @param maxLength The maximum length of the truncated string, including the ellipsis.
     *
     * @return std::string The truncated string.
     *
     * @note If the input string is shorter than or equal to maxLength, it is returned unchanged.
     * @note The ellipsis (...) counts towards the maxLength.
     * @note If maxLength is less than 4, the function will still attempt to add the ellipsis,
     *       which may result in a string longer than maxLength.
     *
     * @par Example:
     * @code
     * std::string longText = "This is a very long string that needs truncating";
     * std::string truncated = Utils::truncateString(longText, 20);
     * std::cout << truncated << std::endl; // Outputs: "This is a very lon..."
     * @endcode
     */
    std::string truncateString(const std::string &text, size_t maxLength)
    {
        if (text.length() <= maxLength)
        {
            return text;
        }
        return text.substr(0, maxLength - 3) + "...";
    }

    /**
     * @brief Centers a string within a field of a given width.
     *
     * This function takes a string and centers it within a field of a specified width.
     * If the string is shorter than the specified width, padding characters are added
     * on both sides to center the string. If the string is longer than or equal to the
     * specified width, it is returned unchanged.
     *
     * @param text The text to be centered.
     * @param width The width of the field in which to center the text.
     * @param fillChar The character to use for padding (default is space).
     *
     * @return std::string The centered string.
     *
     * @note If the difference between the field width and the string length is odd,
     *       the extra padding character is added to the right side.
     * @note If the string is longer than the specified width, no truncation occurs.
     *
     * @par Example:
     * @code
     * std::string centered = Utils::centerString("Hello", 11, '-');
     * std::cout << centered << std::endl; // Outputs: "---Hello---"
     * @endcode
     */
    std::string centerString(const std::string &text, size_t width, char fillChar)
    {
        if (text.length() >= width)
        {
            return text;
        }
        size_t padding = width - text.length();
        size_t leftPadding = padding / 2;
        size_t rightPadding = padding - leftPadding;
        return std::string(leftPadding, fillChar) + text + std::string(rightPadding, fillChar);
    }

    /**
     * @brief Wraps text to a specified line length.
     *
     * This function takes an input text and wraps it to ensure that no line exceeds
     * the specified maximum line length. It attempts to break lines at word boundaries
     * to maintain readability.
     *
     * @param text The input text to wrap.
     * @param lineLength The maximum length of each line.
     *
     * @return std::string The wrapped text.
     *
     * @note The function does not break words that are longer than the specified line length.
     * @note The function preserves existing line breaks in the input text.
     * @note Consecutive spaces between words are collapsed to a single space.
     *
     * @see std::istringstream
     * @see std::ostringstream
     *
     * @par Example:
     * @code
     * std::string longText = "This is a long string that needs to be wrapped to multiple lines for better readability.";
     * std::string wrapped = Utils::wrapText(longText, 20);
     * std::cout << wrapped << std::endl;
     * // Outputs:
     * // This is a long
     * // string that needs to
     * // be wrapped to
     * // multiple lines for
     * // better readability.
     * @endcode
     */
    std::string wrapText(const std::string &text, size_t lineLength)
    {
        std::istringstream words(text);
        std::ostringstream wrapped;
        std::string word;

        size_t currentLineLength = 0;
        while (words >> word)
        {
            if (currentLineLength + word.length() > lineLength)
            {
                wrapped << '\n';
                currentLineLength = 0;
            }
            if (currentLineLength > 0)
            {
                wrapped << ' ';
                currentLineLength++;
            }
            wrapped << word;
            currentLineLength += word.length();
        }

        return wrapped.str();
    }

    /**
     * @brief Sets the file association icon for a specific file extension.
     *
     * This function associates an icon with a specific file extension in the Windows registry.
     * When successful, files with the specified extension will display the associated icon in
     * Windows Explorer and other file management interfaces.
     *
     * @param fileExtension The file extension to associate the icon with (e.g., L".txt").
     * @param iconPath The full path to the icon file.
     *
     * @return bool Returns true if the association was set successfully, false otherwise.
     *
     * @note This function requires administrative privileges to modify the registry.
     * @note The function modifies the HKEY_CURRENT_USER registry hive.
     * @note After setting the association, the function notifies the system to update its file association cache.
     *
     * @see RegCreateKeyExW (Windows API)
     * @see RegSetValueExW (Windows API)
     * @see SHChangeNotify (Windows API)
     *
     * @par Example:
     * @code
     * bool success = Utils::SetFileAssociationIcon(L".myext", L"C:\\Path\\To\\Icon.ico");
     * if (success) {
     *     std::wcout << L"Icon association set successfully." << std::endl;
     * } else {
     *     std::wcerr << L"Failed to set icon association." << std::endl;
     * }
     * @endcode
     */
    bool SetFileAssociationIcon(const std::wstring &fileExtension, const std::wstring &iconPath)
    {
        HKEY hKey;
        std::wstring regPath = L"SOFTWARE\\Classes\\" + fileExtension;

        // Create or open the key for the file extension
        LONG result = RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
                                      REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
        if (result != ERROR_SUCCESS)
        {
            std::wcerr << L"Failed to create/open registry key for file extension" << std::endl;
            return false;
        }
        RegCloseKey(hKey);

        // Create DefaultIcon subkey
        regPath += L"\\DefaultIcon";
        result = RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
                                 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
        if (result != ERROR_SUCCESS)
        {
            std::wcerr << L"Failed to create DefaultIcon subkey" << std::endl;
            return false;
        }

        // Set the default value of DefaultIcon to the icon path
        result = RegSetValueExW(hKey, NULL, 0, REG_SZ,
                                reinterpret_cast<const BYTE *>(iconPath.c_str()),
                                (iconPath.length() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);

        if (result != ERROR_SUCCESS)
        {
            std::wcerr << L"Failed to set icon path in registry" << std::endl;
            return false;
        }

        // Notify the system about the change
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

        return true;
    }

    /**
     * @brief Reads a specific line from a file.
     *
     * This function reads and returns a specific line from the given file stream.
     * It's useful when you need to access a particular line of a file without
     * reading the entire file into memory.
     *
     * @param file A reference to an open std::fstream object.
     * @param lineNum The line number to read (1-based index).
     *
     * @return std::string The content of the specified line.
     *
     * @throw std::runtime_error if the file is not open.
     * @throw std::invalid_argument if the line number is 0.
     * @throw std::out_of_range if the line number exceeds the number of lines in the file.
     * @throw std::runtime_error if there's an error reading from the file.
     *
     * @note This function preserves the original position of the file stream.
     * @note Line numbers start at 1, not 0.
     * @note The function does not strip newline characters from the end of the line.
     *
     * @see std::fstream
     * @see std::getline
     *
     * @par Example:
     * @code
     * std::fstream file("example.txt", std::ios::in);
     * try {
     *     std::string line = Utils::readLineFromFile(file, 3);
     *     std::cout << "Third line: " << line << std::endl;
     * } catch (const std::exception& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * file.close();
     * @endcode
     */
    std::string readLineFromFile(std::fstream &file, unsigned int lineNum)
    {
        if (!file.is_open())
        {
            throw std::runtime_error("File is not open");
        }

        if (lineNum == 0)
        {
            throw std::invalid_argument("Line number must be greater than 0");
        }

        // Save the current position
        std::streampos originalPos = file.tellg();

        // Move to the beginning of the file
        file.seekg(0, std::ios::beg);

        std::string line;
        for (unsigned int currentLine = 1; currentLine <= lineNum; ++currentLine)
        {
            if (file.eof())
            {
                // Restore the original position
                file.clear();
                file.seekg(originalPos);
                throw std::out_of_range("Line number exceeds the number of lines in the file");
            }

            std::getline(file, line);

            if (file.fail() && !file.eof())
            {
                // Restore the original position
                file.clear();
                file.seekg(originalPos);
                throw std::runtime_error("Error reading from file");
            }
        }

        // Restore the original position
        file.clear();
        file.seekg(originalPos);

        return line;
    }

    std::string leftPad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return std::string(amount, ' ') + str;
    }

    std::string rightPad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return str + std::string(amount, ' ');
    }

    std::string pad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return rightPad(leftPad(str, amount), amount);
    }
} // namespace Utils