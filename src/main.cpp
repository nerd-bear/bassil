/**
 * @file main.cpp
 * @brief Main entry point for the Bassil language lexer application.
 */

#include <windows.h>
// #include <iostream>
// #include <string>
#include <fstream>
#include <sstream>
// #include <C:/coding-projects/CPP-Dev/bassil/Libs/curl/include/curl/curl.h>
#include <vector>
#include <stdexcept>
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/utils.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/error_repport.h"
#include "C:/coding-projects/CPP-Dev/bassil/src/headers/lexer.h" // Changed from lexer.cpp to lexer.h

// size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
//     size_t total_size = size * nmemb;
//     output->append((char*)contents, total_size);
//     return total_size;
// }

/**
 * @brief Main entry point for the Windows application.
 * 
 * This function initializes the application, reads input from a file,
 * performs lexical analysis, and handles any errors that occur during the process.
 * 
 * @param hInstance Handle to the current instance of the application.
 * @param hPrevInstance Handle to the previous instance of the application (always NULL for Win32 apps).
 * @param lpCmdLine Command line arguments as a single string.
 * @param nShowCmd Control how the window should be shown.
 * @return int Returns 0 on success, non-zero on failure.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    try {
        const std::string inputFilePath = "C:/coding-projects/CPP-Dev/bassil/assets/main.basl";

        reportError(inputFilePath, 5, 10, "Unkown token '=', expected ; at line token end termanation with no segment end. 8 bit overflow with 12 bit hex 0x0087");

        if (Utils::SetFileAssociationIcon(L".basl", L"C:/coding-projects/CPP-Dev/bassil/assets/img/logo.ico")) {
            std::wcout << L"File association icon set successfully!" << std::endl;
        } else {
            std::wcout << L"Failed to set file association icon." << std::endl;
        }

        // Initialize the application
        Utils::CreateWinAPI32BallonNotification("Started Bassil Lang", "Started Bassil Lang, compiled using G++ from main.cpp using windows.h.", 0);

        // Clear previous logs and lexical analysis output
        Utils::clear_logs();
        Utils::clear_lex_out();

        // Read input file
        std::string inputContent = Utils::readFileToString(inputFilePath);

        if (inputContent.empty()) {
            throw std::runtime_error("Input file is empty or could not be read.");
        }

        Utils::general_log("Input string: " + inputContent, true);

        // Perform lexical analysis
        std::vector<Token> tokens = lex(inputContent);

        // Display and save tokens
        display_tokens(tokens);
        save_tokens(tokens);

        Utils::CreateWinAPI32BallonNotification("Lexical Analysis Complete", "Lexical analysis has been completed successfully.", 0);

        return 0;
    } catch (const std::exception& e) {
        // Handle any exceptions that occurred during execution
        std::string errorMessage = "An error occurred: " + std::string(e.what());
        Utils::general_log(errorMessage, true);
        Utils::CreateWinAPI32BallonNotification("Error", errorMessage, 1);
        return 1;
    }
}