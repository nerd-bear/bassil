#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H

#include <string>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"

void reportAnsiError(const std::string &filePath, int lineNumber, int startColumn, int endColumn, const std::string &line, const std::string &msg);
void reportNonAnsiError(const std::string &filePath, int lineNumber, int startColumn, int endColumn, const std::string &line, const std::string &msg);
int reportError(const std::string &filePath, int lineNumber, int startColumn, int endColumn, const std::string &msg);

#endif