#include <iostream>
#include <string>
#include <optional>

enum class MetaCommandResult { Exit, Unrecognized };

MetaCommandResult do_meta_command(std::string line) {
    if (line == ".exit") {
        return MetaCommandResult::Exit;
    }

    return MetaCommandResult::Unrecognized;
}

enum class StatementType { Insert, Select };

struct Statement {
    StatementType type;
};

std::optional<Statement> prepare_statement(std::string line) {
    Statement statement = {};

    if (line.starts_with("insert")) {
        statement.type = StatementType::Insert;
        return statement;
    }

    if (line == "select") {
        statement.type = StatementType::Select;
        return statement;
    }

    return std::nullopt;
}

void execute_statement(const Statement statement) {
    switch (statement.type) {
        case StatementType::Insert:
            std::cout << "This is where we would do an insert." << '\n';
            break;
        case StatementType::Select:
            std::cout << "This is where we would do a select." << '\n';
            break;
    }
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
            std::optional<Statement> opt_statement = prepare_statement(line);

            if (opt_statement.has_value()) {
                execute_statement(opt_statement.value());
                std::cout << "Executed." << '\n';
            } else {
                std::cout << "Unrecognized keyword at start of '" << line << "'." << '\n';
            }
        }

        std::cout << prompt;
    }

    return 0;
}
