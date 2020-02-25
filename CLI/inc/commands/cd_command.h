#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <filesystem>

#include "result.h"
#include "environment.h"
#include "commands/command.h"
#include "commands/execution_result.h"

namespace NCLI::NCommand {

    class CdCommand : public Command {
    public:

        CdCommand(const Environment& env) : env_(env) {};

        CdCommand(const Environment& env, const std::filesystem::path& path)
            : env_(env), path_(path) {};

        virtual ExecutionResult execute(std::istream& is, std::ostream& os) override;

        static Result<std::shared_ptr<Command>, std::string> create_command(
                const Environment& env, const std::vector<std::string>& args);
    private:
        ExecutionResult execute_helper(const std::filesystem::path& path);
    private:
        const Environment& env_;
        const std::filesystem::path path_;
    };

} // namespace NCLI::NCommand
