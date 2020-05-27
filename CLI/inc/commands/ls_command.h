#pragma once

#include <string>
#include <filesystem>

#include "result.h"
#include "commands/command.h"
#include "commands/execution_result.h"

namespace NCLI::NCommand {

    /**
     * Command that lists the content of the given path
     * or of the current directory if path is not specified
     */
    class LsCommand : public Command {
    public:
        /**
         * Command lists the content of the current directory to the
         * output stream
         */
        LsCommand() : path_(std::filesystem::current_path()) {};

        /**
         * Command lists the content of the given path to the
         * output stream
         */
        LsCommand(const std::filesystem::path& path)
            : path_(std::filesystem::absolute(path)) {};

        /**
         * Lists the content of the given path or
         * of the current directory if path is not specified
         * to the output stream
         */
        virtual ExecutionResult execute(std::istream& is, std::ostream& os) override;

        /**
         * If args[1] given, uses it as the path to create command,
         * otherwise creates ls command that lists the content of the current directory
         */
        static Result<std::shared_ptr<Command>, std::string> create_command(
                const std::vector<std::string>& args);
    private:
        ExecutionResult execute_helper(const std::filesystem::path& path, std::ostream& os);
    private:
        const std::filesystem::path path_;
    };

} // namespace NCLI::NCommand
