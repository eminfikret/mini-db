#include <iostream>
#include <string>

int main() {
    std::string prompt = "db > ";
    std::cout << prompt;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == ".exit") {
            return 0;
        } else {
            std::cout << "Unrecognized command '" << line << "'." << '\n';
        }
        std::cout << prompt;
    } 

    return 0;
}