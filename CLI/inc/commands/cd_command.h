#pragma once

#include <string>
#include <filesystem>

#include "result.h"
#include "environment.h"
#include "commands/command.h"
#include "commands/execution_result.h"

namespace NCLI::NCommand {

    class CdCommand : public Command {
    public:

        CdCommand(Environment& env) : env_(env) {};

        CdCommand(Environment& env, const std::filesystem::path& path)
            : env_(env), path_(std::filesystem::absolute(path)) {};

        virtual ExecutionResult execute(std::istream& is, std::ostream& os) override;

        static Result<std::shared_ptr<Command>, std::string> create_command(
                Environment& env, const std::vector<std::string>& args);
    private:
        ExecutionResult execute_helper(const std::filesystem::path& path);
    private:
        Environment& env_;
        const std::filesystem::path path_;
    };

} // namespace NCLI::NCommand
