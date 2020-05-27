#include "commands/cd_command.h"
#include <cstring>

namespace NCLI::NCommand {

    Result<std::shared_ptr<Command>, std::string> CdCommand::create_command(
            Environment& env,
            const std::vector<std::string>& args) {
        if (args.size() <= 1) {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new CdCommand(env))));
        } else if (args.size() == 2) {
            return Result<std::shared_ptr<Command>, std::string>(
                    Ok(std::shared_ptr<Command>(new CdCommand(env, args[1]))));
        } else {
            return Result<std::shared_ptr<Command>, std::string>(
                        Error((std::string)"Wrong number of arguments."));
        }
    }

    ExecutionResult CdCommand::execute_helper(const std::filesystem::path& path) {
        std::filesystem::current_path(path);

        return ExecutionResult(ExecutionStatus::success, "");
    }

    ExecutionResult CdCommand::execute(std::istream&, std::ostream&) {
        if (!path_.empty()) {
            if (!std::filesystem::exists(path_)) {
                return ExecutionResult(NCommand::ExecutionStatus::error,
                        "Path does not exist " + path_.string() + '\n');
            }
            if (!std::filesystem::is_directory(path_)) {
                return ExecutionResult(NCommand::ExecutionStatus::error,
                        path_.string() + " is not a directory\n");
            }
            return execute_helper(path_);
        } else {
            std::string home = env_.get_variable("HOME");
            if (home.empty()) {
                char *homeEnv = std::getenv("HOME");
                if (homeEnv == nullptr || std::strlen(homeEnv) == 0) {
                    return ExecutionResult(NCommand::ExecutionStatus::error,
                            "HOME variable is not specified\n");
                }
                home = std::string(homeEnv);
            }
            auto homePath = std::filesystem::path(home);
            if (!std::filesystem::exists(homePath)) {
                return ExecutionResult(NCommand::ExecutionStatus::error,
                        "Path does not exist " + home + '\n');
            }
            return execute_helper(std::filesystem::absolute(homePath));
        }
    }

} // namespace NCLI::NCommand

