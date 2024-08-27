#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h"
#include <fstream>
#include <sstream>
#include <string>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    Utils::CreateWinAPI32BallonNotification("Started Bassil Shell", "Started Bassil Shell, compiled using G++ from main.cpp using windows.h. Please remember that errors can occur!", 0);
    
    Utils::clear_logs();
    Utils::clear_lex_out();

    std::ifstream file("C:/coding-projects/CPP-Dev/bassil/assets/to_lex.txt");  // Open the file
    std::stringstream buffer;                                                        // Create a stringstream to hold the file content
    std::string bufferStr;

    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {  // Read each line from the file
            buffer << line << "\n";         // Append the line and a newline character to the buffer
        }
        file.close();  // Close the file
    } else {
        std::cerr << "Unable to open file";
    }

    bufferStr = buffer.str();
    lex(bufferStr);
    
    return 0;
}