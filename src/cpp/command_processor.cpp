// #define _COMMAND_PROCESSER_

// #ifndef _VECTOR_
//     #include <vector>
// #endif

// #ifndef _IOSTREAM_ 
//     #include <iostream>
// #endif

// #ifndef _STRING_
//     #include <string>
// #endif

// #ifndef _UNORDEDMAP_
//     #include <unordered_map>
// #endif

// #ifndef _FUNCTIONAL_
//     #include <functional>
// #endif

// #ifndef _UTILS_
//     #include <C:\coding-projects\CPP-Dev\bassil\src\headers\utils.hpp>
// #endif

// enum func_stat_return_code {Success,
//                             Invalid_Command,
//                             Invalid_Arguments,
//                             Unknown_Issue,
//                             Program_issue};

// func_stat_return_code help (std::vector<std::string> _arguments) 
// {
//     std::cout << "Originating from 1 : " << "Jack" << "\n";
//     return Success;
// }

// func_stat_return_code echo (std::vector<std::string> _arguments) 
// {
//     std::cout << "Originating from 2 : " << "Jack" << "\n";
//     return Success;
// }

// func_stat_return_code exit (std::vector<std::string> _arguments) 
// {
//     std::exit(0);
//     return Success;
// }

// int main () 
// {
//     std::unordered_map<std::string, std::function<func_stat_return_code(std::vector<std::string>)>> func_map;

//     func_map["help"] = help;
//     func_map["h"] = help;
//     func_map["info"] = help;
//     func_map["echo"] = echo;


//     while (true)
//     {
//         std::cout << "SQS : ";

//         std::string _user_input;
//         std::cin >> _user_input;

//         std::vector<std::string> _arguments = Utils::split_string(_user_input, " ");

//         std::string _function_name = _arguments[0];

//         if (func_map.find(_function_name) == end(func_map)) 
//         {
//             std::cout << "[Fatal Error] ad : The term '" << _function_name << "' is not recognized as the name of a SquishyTermCommand, custom Squishy Function, executable in path, or system program. Check the spelling of the fuction.\nFunction exit code: 1\n";
//             continue;
//         };

//         func_stat_return_code _return_code = func_map[_function_name](_arguments);
//         std::cout << "Function exit code: " << _return_code << "\n";
//     }
    
    
//     return 0;
// }