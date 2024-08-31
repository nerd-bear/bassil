/**
 * @file utils.h
 * @brief Header file containing utility functions for the Bassil language project.
 */

#ifndef UTILS_H
#define UTILS_H

#pragma comment(lib, "shell32.lib")  // Link with shell32.lib for SHChangeNotify

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <regex>
#include <shlobj.h>

namespace Utils
{
    /**
     * @brief Splits a string into a vector of substrings based on a specified delimiter.
     * @param s The input string to be split.
     * @param delimiter The string that serves as the delimiter.
     * @return std::vector<std::string> A vector containing the substrings.
     */
    std::vector<std::string> split_string(const std::string& s, const std::string& delimiter);

    /**
     * @brief Converts a standard string to a wide string.
     * @param _string The input string to be converted.
     * @param _isUtf8Enabled Flag to indicate if UTF-8 encoding should be used (default: true).
     * @return std::wstring The resulting wide string.
     */
    std::wstring StringToWString(const std::string& _string, bool _isUtf8Enabled = true);

    /**
     * @brief Converts a standard string to a wide character string (LPCWSTR).
     * @param _string The input string to be converted.
     * @return LPCWSTR The resulting wide character string.
     */
    LPCWSTR StringToLPCWSTR(const std::string& _string);

    /**
     * @brief Creates a Windows API 32-bit message box.
     * @param _title The title of the message box.
     * @param _message The content of the message box.
     * @param _type The type of the message box (1-6).
     * @return int The ID of the button clicked by the user.
     */
    int CreateWinAPI32MessageBox(const std::string& _title, const std::string& _message, int _type);

    /**
     * @brief Creates a Windows API balloon notification.
     * @param _title The title of the notification.
     * @param _message The content of the notification.
     * @param _type The type of the notification (0-3, default: 0).
     */
    void CreateWinAPI32BallonNotification(const std::string& _title, const std::string& _message, int _type = 0);

    /**
     * @brief Retrieves the maximum screen size for a given monitor.
     * @param monitorIndex The index of the monitor.
     * @return RECT The screen dimensions.
     */
    RECT GetMaximizedScreenSize(int monitorIndex);

    /**
     * @brief Trims leading whitespace from a string.
     * @param s The string to be trimmed.
     * @return std::string& Reference to the trimmed string.
     */
    std::string& ltrim(std::string& s);

    /**
     * @brief Trims trailing whitespace from a string.
     * @param s The string to be trimmed.
     * @return std::string& Reference to the trimmed string.
     */
    std::string& rtrim(std::string& s);

    /**
     * @brief Trims both leading and trailing whitespace from a string.
     * @param s The string to be trimmed.
     * @return std::string& Reference to the trimmed string.
     */
    std::string& trim(std::string& s);

    /**
     * @brief Logs a message to a file and optionally prints it.
     * @param str The message to be logged.
     * @param isPrintTrue Flag to indicate if the message should be printed (default: true).
     * @return int 0 on success, 1 on failure.
     */
    int general_log(const std::string& str, bool isPrintTrue = true);

    /**
     * @brief Clears the content of the lexical analysis output file.
     * @return int 0 on success, 1 on failure.
     */
    int clear_lex_out();

    /**
     * @brief Clears the content of the log file.
     * @return int 0 on success, 1 on failure.
     */
    int clear_logs();

    /**
     * @brief Reads the entire content of a file into a string.
     * @param filename The path to the file to be read.
     * @return std::string The content of the file.
     */
    std::string readFileToString(const std::string& filename);

    /**
     * @brief Enables ANSI escape sequences for console output.
     */
    void enableAnsiInConsole();

    /**
     * @brief Checks if ANSI escape sequences are enabled in the console.
     * @return bool True if ANSI is enabled, false otherwise.
     */
    bool isAnsiEnabledInConsole();

    /**
     * @brief Validates a hex color code.
     * @param colorCode The hex color code to validate.
     * @return bool True if the color code is valid, false otherwise.
     */
    bool isValidHexColor(const std::string& colorCode);

    /**
     * @brief Applies a hex color code to a given text string using ANSI escape sequences.
     * @param text The text to be colored.
     * @param colorCode The hex color code to apply.
     * @return std::string The text formatted with the specified color.
     */
    std::string colorText(const std::string& text, const std::string& colorCode);

    /**
     * @brief Formats text as bold using ANSI escape sequences.
     * @param text The text to be formatted as bold.
     * @return std::string The bold-formatted text.
     */
    std::string boldText(const std::string& text);

    /**
     * @brief Formats text as italic using ANSI escape sequences.
     * @param text The text to be formatted as italic.
     * @return std::string The italic-formatted text.
     */
    std::string italicText(const std::string& text);

    /**
     * @brief Formats text with an underline using ANSI escape sequences.
     * @param text The text to be underlined.
     * @return std::string The underlined text.
     */
    std::string underlineText(const std::string& text);


    bool SetFileAssociationIcon(const std::wstring& fileExtension, const std::wstring& iconPath);

    std::string readLineFromFile(std::fstream &file, unsigned int lineNum);
}

#endif // UTILS_H