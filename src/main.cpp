#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    Utils::CreateWinAPI32BallonNotification("Started Bassil Lang", "Started Bassil Lang, compiled using G++ from main.cpp using windows.h. Please remember that errors can occur!", 0);

    Utils::clear_logs();
    Utils::clear_lex_out();

    std::ifstream file("C:/coding-projects/CPP-Dev/bassil/assets/to_lex.txt");
    std::stringstream buffer;
    std::string bufferStr;
    std::string line;

    if (file.is_open()) {
        Utils::general_log("Opened input file successfully.", true);
        while (std::getline(file, line)) {
            buffer << line << "\n";
        }
        file.close();
    } else {
        Utils::general_log("Unable to open input file.", true);
        file.close();
        return 1;
    }

    bufferStr = buffer.str();
    Utils::general_log("Input string: " + bufferStr, true);
    
    std::vector<Token> tokens = lex(bufferStr);

    return 0;
}
