#pragma once

#include <string>
#include <filesystem>

#include "result.h"
#include "environment.h"
#include "commands/command.h"
#include "commands/execution_result.h"

namespace NCLI::NCommand {

    /**
     * Command that changes the current path to the given path or
     * to the HOME environment variable's value if path is not specified
     */
    class CdCommand : public Command {
    public:
        /**
         * Command changes the current path to the HOME environment variable's value
         */
        CdCommand(Environment& env) : env_(env) {};
        /**
         * Command changes the current path to the given path
         */
        CdCommand(Environment& env, const std::filesystem::path& path)
            : env_(env), path_(std::filesystem::absolute(path)) {};
        /**
         * Changes the current path to the given path or
         * to the HOME environment variable's value if path is not specified
         * If path is not correct or HOME variable is not set, returns result with ExecutionStatus::error
         */
        virtual ExecutionResult execute(std::istream& is, std::ostream& os) override;

        /**
         * If args[1] given, uses it as a path to create the command,
         * otherwise creates command that uses HOME environment variable
         */
        static Result<std::shared_ptr<Command>, std::string> create_command(
                Environment& env, const std::vector<std::string>& args);
    private:
        ExecutionResult execute_helper(const std::filesystem::path& path);
    private:
        Environment& env_;
        const std::filesystem::path path_;
    };

} // namespace NCLI::NCommand
