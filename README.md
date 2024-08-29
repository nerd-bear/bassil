# Bassil Language Project

## Overview
Bassil is a custom programming language implementation tool, focusing on lexical analysis with plans for further parsing and command processing. It is built with Windows in mind, utilizing various Windows API features.

## Features
- Lexical Analysis: Tokenizes input into different types like identifiers, arguments, strings, and integers.
- Windows Integration: Uses Windows API for notifications and message boxes.
- File I/O: Reads input from and writes output to files.
- Utility Functions: Includes a rich set of utility functions for various operations.
- ANSI Color Support: Implements functions to add color and formatting to console output.

## Prerequisites
- Windows 10 or later
- G++ compiler (tested with version 13.2.0)
- Visual Studio Code (recommended)

## Installation
1. Clone the repository:
   ```
   git clone https://github.com/yourusername/bassil.git
   cd bassil
   ```

2. Ensure you have G++ installed and added to your system PATH.

3. If using Visual Studio Code, install the C/C++ extension.

## Project Structure
- `src/`
  - `main.cpp`: Entry point of the application
  - `headers/`
    - `lexer.h`: Header file for lexical analysis
    - `utils.h`: Header file for utility functions
  - `cpp/`
    - `lexer.cpp`: Implementation of lexical analysis
    - `utils.cpp`: Implementation of utility functions
- `assets/`
  - `to_lex.txt`: Input file for lexical analysis
  - `after_lex.txt`: Output file for lexical analysis results
  - `logs.txt`: Log file for general logging
- `build/`: Directory for build outputs

## Building the Project
### Using G++ from Command Line
```
g++ ./src/main.cpp ./src/cpp/utils.cpp ./src/cpp/lexer.cpp -o ./build/Bassil-Main-Build-ORS-A01 -lgdi32 -luser32 -lshell32
```

### Using Visual Studio Code
1. Open the project folder in VSCode.
2. Press `Ctrl+Shift+B` to run the build task.

## Usage
1. Prepare your input in `assets/to_lex.txt`.
2. Run the compiled executable.
3. Check the output in `assets/after_lex.txt` and logs in `assets/logs.txt`.

## Contributing
Contributions to Bassil are welcome! Please feel free to submit a Pull Request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Thanks to all contributors and supporters of the Bassil project.

## Contact
For any queries or suggestions, please open an issue in the GitHub repository.
