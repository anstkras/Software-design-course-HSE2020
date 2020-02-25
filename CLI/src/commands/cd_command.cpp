#include <fstream>

#include "commands/cd_command.h"

namespace NCLI::NCommand {

    Result<std::shared_ptr<Command>, std::string> CdCommand::create_command(
            Environment& env,
            const std::vector<std::string>& args) {
        if (args.size() <= 1) {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new CdCommand(env))));
        } else {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new CdCommand(env, args[1]))));
        }
    }

    ExecutionResult CdCommand::execute_helper(const std::filesystem::path& path) {
        std::filesystem::current_path(path);

        return ExecutionResult(ExecutionStatus::success, "");
    }

    ExecutionResult CdCommand::execute(std::istream& is, std::ostream& os) {
        if (!path_.empty()) {
            if (!std::filesystem::exists(path_)) {
                return ExecutionResult(NCommand::ExecutionStatus::error,
                        "Path does not exist " + path_.string() + '\n');
            }
            return execute_helper(path_);
        } else {
            const std::string& home = env_.get_variable("HOME");
            if (home.empty()) {
                return ExecutionResult(NCommand::ExecutionStatus::error,
                        "HOME variable is not specified \n");
            }
            // TODO check if path is correct
            return execute_helper(std::filesystem::path(home));
        }
    }

} // namespace NCLI::NCommand

