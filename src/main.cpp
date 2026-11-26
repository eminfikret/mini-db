#include <iostream>
#include <string>

enum class MetaCommandResult { Exit, Unrecognized };

MetaCommandResult do_meta_command(std::string line) {
    if (line == ".exit") {
        return MetaCommandResult::Exit;
    }

    return MetaCommandResult::Unrecognized;
}

int main() {
    std::string prompt = "db > ";
    std::cout << prompt;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.starts_with('.')) {
            MetaCommandResult result = do_meta_command(line);

            switch (result) {
                case MetaCommandResult::Exit:
                    return 0;
                case MetaCommandResult::Unrecognized:
                    std::cout << "Unrecognized command '" << line << "'." << '\n';
                    break;
            }
        } else {
            std::cout << "Unrecognized command '" << line << "'." << '\n';
        }

        std::cout << prompt;
    }

    return 0;
}
