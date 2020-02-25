#include "commands/ls_command.h"

namespace NCLI::NCommand {

    Result<std::shared_ptr<Command>, std::string> LsCommand::create_command(
            const std::vector<std::string>& args) {
        if (args.size() <= 1) {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new LsCommand())));
        } else {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new LsCommand(args[1]))));
        }
    }

    ExecutionResult LsCommand::execute_helper(const std::filesystem::path& path, std::ostream& os) {
        if (std::filesystem::is_directory(path)) {
            for (const auto & entry : std::filesystem::directory_iterator(path))
                if (std::filesystem::is_directory(entry.path())) {
                    os << std::string(entry.path().stem()) << '\n';
                } else {
                    os << std::string(entry.path().filename()) << '\n';
                }
        } else {
            os << std::string(path) << '\n';
        }

        return ExecutionResult(ExecutionStatus::success, "");
    }

    ExecutionResult LsCommand::execute(std::istream&, std::ostream& os) {
        if (!std::filesystem::exists(path_)) {
            return ExecutionResult(NCommand::ExecutionStatus::error,
                    "Path does not exist " + path_.string() + '\n');
        }
        return execute_helper(path_, os);
    }

} // namespace NCLI::NCommand

