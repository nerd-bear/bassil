/**
 * @file utils.h
 * @brief Header file containing utility functions for the Bassil language project.
 *
 * This file declares a collection of utility functions that provide various
 * string manipulation, Windows API interaction, file handling, and console
 * output formatting capabilities. These utilities are designed to support
 * the core functionality of the Bassil language project.
 *
 * @note This file requires Windows-specific headers and may not be
 * compatible with non-Windows environments.
 *
 * @author Nerd Bear
 * @date 31 August
 * @version 0.2.3
 *
 * @copyright Copyright (c) 2024 Bassil
 */

#ifndef UTILS_H
#define UTILS_H

#pragma comment(lib, "shell32.lib") // Link with shell32.lib for SHChangeNotify

#ifdef _WIN32
#include <windows.h>
#endif

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
     *
     * @param s The input string to be split.
     * @param delimiter The string that serves as the delimiter for splitting.
     * @return std::vector<std::string> A vector containing the substrings resulting from splitting the input string.
     */
    std::vector<std::string> split_string(const std::string &s, const std::string &delimiter);

    /**
     * @brief Converts a standard string to a wide string.
     *
     * @param _string The input string to be converted.
     * @param _isUtf8Enabled Flag to indicate if UTF-8 encoding should be used. Default is true.
     * @return std::wstring The resulting wide string after conversion.
     */
    std::wstring StringToWString(const std::string &_string, bool _isUtf8Enabled = true);

    /**
     * @brief Converts a standard string to a wide character string (LPCWSTR).
     *
     * @param _string The input string to be converted.
     * @return LPCWSTR The resulting wide character string after conversion.
     */
    LPCWSTR StringToLPCWSTR(const std::string &_string);

    /**
     * @brief Creates a Windows API 32-bit message box.
     *
     * @param _title The title of the message box.
     * @param _message The content of the message box.
     * @param _type The type of the message box, determining which buttons are displayed (1-6).
     * @return int The ID of the button clicked by the user.
     */
    int CreateWinAPI32MessageBox(const std::string &_title, const std::string &_message, int _type);

    /**
     * @brief Creates a Windows API balloon notification.
     *
     * @param _title The title of the notification.
     * @param _message The content of the notification.
     * @param _type The type of the notification, determining the icon displayed (0-3). Default is 0.
     */
    void CreateWinAPI32BallonNotification(const std::string &_title, const std::string &_message, int _type = 0);

    /**
     * @brief Retrieves the maximum screen size for a given monitor.
     *
     * @param monitorIndex The index of the monitor (0-based).
     * @return RECT A structure containing the screen dimensions.
     */
    RECT GetMaximizedScreenSize(int monitorIndex);

    /**
     * @brief Trims leading whitespace from a string.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     * @return std::string& A reference to the modified string, allowing for method chaining.
     */
    std::string &ltrim(std::string &s);

    /**
     * @brief Trims trailing whitespace from a string.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     * @return std::string& A reference to the modified string, allowing for method chaining.
     */
    std::string &rtrim(std::string &s);

    /**
     * @brief Trims both leading and trailing whitespace from a string.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     * @return std::string& A reference to the modified string, allowing for method chaining.
     */
    std::string &trim(std::string &s);

    /**
     * @brief Logs a message to a file and optionally prints it.
     *
     * @param str The message to be logged.
     * @param isPrintTrue Flag to indicate if the message should be printed to the console. Default is true.
     * @return int Returns 0 on successful logging, 1 if there was an error opening the log file.
     */
    int general_log(const std::string &str, bool isPrintTrue = true);

    int clear_file(const std::string &filename);

    /**
     * @brief Reads the entire content of a file into a string.
     *
     * @param filename The path to the file to be read.
     * @return std::string The content of the file as a string.
     */
    std::string readFileToString(const std::string &filename);

    /**
     * @brief Enables ANSI escape sequences for console output.
     *
     * This function enables ANSI escape sequence processing for the console output,
     * allowing the use of colored text and other formatting in the console.
     */
    int enableAnsiInConsole();

    /**
     * @brief Checks if ANSI escape sequences are enabled in the console.
     *
     * @return bool Returns true if ANSI escape sequences are enabled, false otherwise.
     */
    bool isAnsiEnabledInConsole();

    /**
     * @brief Validates a hex color code.
     *
     * @param colorCode The string to be validated as a hex color code.
     * @return bool Returns true if the color code is a valid hex color, false otherwise.
     */
    bool isValidHexColor(const std::string &colorCode);

    /**
     * @brief Applies a hex color code to a given text string using ANSI escape sequences.
     *
     * @param text The text to be colored.
     * @param colorCode The hex color code to apply (e.g., "#FF0000" for red).
     * @return std::string The text formatted with ANSI escape sequences for the specified color.
     */
    std::string colorText(const std::string &text, const std::string &colorCode);

    /**
     * @brief Formats text as bold using ANSI escape sequences.
     *
     * @param text The text to be formatted as bold.
     * @return std::string The bold-formatted text.
     */
    std::string boldText(const std::string &text);

    /**
     * @brief Formats text as italic using ANSI escape sequences.
     *
     * @param text The text to be formatted as italic.
     * @return std::string The italic-formatted text.
     */
    std::string italicText(const std::string &text);

    /**
     * @brief Formats text with an underline using ANSI escape sequences.
     *
     * @param text The text to be underlined.
     * @return std::string The underlined text.
     */
    std::string underlineText(const std::string &text);

    /**
     * @brief Applies multiple text formatting options using ANSI escape sequences.
     *
     * @param text The text to be formatted.
     * @param bold Apply bold formatting if true.
     * @param italic Apply italic formatting if true.
     * @param underline Apply underline formatting if true.
     * @param colorCode Hex color code to apply (e.g., "#FF0000" for red).
     * @return std::string The formatted text.
     */
    std::string formatText(const std::string &text, bool bold, bool italic, bool underline, const std::string &colorCode);

    /**
     * @brief Removes all ANSI escape sequences from a string.
     *
     * @param text The text containing ANSI escape sequences.
     * @return std::string The text with all ANSI escape sequences removed.
     */
    std::string stripAnsiEscapeCodes(const std::string &text);

    /**
     * @brief Truncates a string to a specified length, adding an ellipsis if truncated.
     *
     * @param text The input text to truncate.
     * @param maxLength The maximum length of the truncated string, including the ellipsis.
     * @return std::string The truncated string.
     */
    std::string truncateString(const std::string &text, size_t maxLength);

    /**
     * @brief Centers a string within a field of a given width.
     *
     * @param text The text to be centered.
     * @param width The width of the field in which to center the text.
     * @param fillChar The character to use for padding (default is space).
     * @return std::string The centered string.
     */
    std::string centerString(const std::string &text, size_t width, char fillChar = ' ');

    /**
     * @brief Wraps text to a specified line length.
     *
     * @param text The input text to wrap.
     * @param lineLength The maximum length of each line.
     * @return std::string The wrapped text.
     */
    std::string wrapText(const std::string &text, size_t lineLength);

    /**
     * @brief Sets the file association icon for a specific file extension.
     *
     * @param fileExtension The file extension to associate the icon with (e.g., L".txt").
     * @param iconPath The full path to the icon file.
     * @return bool Returns true if the association was set successfully, false otherwise.
     */
    bool SetFileAssociationIcon(const std::wstring &fileExtension, const std::wstring &iconPath);

    /**
     * @brief Reads a specific line from a file.
     *
     * @param file A reference to an open std::fstream object.
     * @param lineNum The line number to read (1-based index).
     * @return std::string The content of the specified line.
     */
    std::string readLineFromFile(std::fstream &file, unsigned int lineNum);

    std::string leftPad(std::string str, int amount);

    std::string rightPad(std::string str, int amount);
    
    std::string pad(std::string str, int amount);
}

#endif // UTILS_H