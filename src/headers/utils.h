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

namespace Utils
{
    std::vector<std::string> split_string(const std::string& s, const std::string& delimiter);
    std::wstring StringToWString(const std::string& _string, bool _isUtf8Enabled = true);
    LPCWSTR StringToLPCWSTR(const std::string& _string);
    int CreateWinAPI32MessageBox(const std::string& _title, const std::string& _message, int _type);
    void CreateWinAPI32BallonNotification(std::string _title, std::string _message, int _type = 0);
    RECT GetMaximizedScreenSize(int monitorIndex);
    std::string &ltrim(std::string &s);
    std::string &rtrim(std::string &s);
    std::string &trim(std::string &s);
    int general_log(const std::string &str, bool isPrintTrue = true);
    int clear_lex_out();
    int clear_logs();
    std::string readFileToString(const std::string& filename);
}  

#endif // UTILS_H