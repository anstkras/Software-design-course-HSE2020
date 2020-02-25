#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <filesystem>

#include "result.h"
#include "environment.h"
#include "commands/command.h"
#include "commands/execution_result.h"

namespace NCLI::NCommand {

    class LsCommand : public Command {
    public:

        LsCommand() {};

        LsCommand(const std::filesystem::path& path)
            : path_(path) {};

        virtual ExecutionResult execute(std::istream& is, std::ostream& os) override;

        static Result<std::shared_ptr<Command>, std::string> create_command(
                const std::vector<std::string>& args);
    private:
        ExecutionResult execute_helper(const std::filesystem::path& path, std::ostream& os);
    private:
        const std::filesystem::path path_;
    };

} // namespace NCLI::NCommand
