#include <sstream>
#include <memory>
#include <filesystem>

#include "gtest/gtest.h"

#include "commands/command.h"
#include "commands/assignment_command.h"
#include "commands/cat_command.h"
#include "commands/echo_command.h"
#include "commands/exit_command.h"
#include "commands/external_command.h"
#include "commands/pwd_command.h"
#include "commands/wc_command.h"
#include "commands/cd_command.h"
#include "commands/ls_command.h"

namespace {
    using namespace NCLI;
    using namespace NCLI::NCommand;

    TEST(Commands, AssignmentCommand) {
        std::stringstream input("Input"), output;
        Environment env;
        env.set_variable("simple", "not");
        AssignmentCommand command = AssignmentCommand(env, "simple", "yup");

        auto result = command.execute(input, output);
        EXPECT_EQ("yup", env.get_variable("simple"));
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, CatCommandStream) {
        std::stringstream input("Input"), output;
        CatCommand command = CatCommand();

        auto result = command.execute(input, output);
        EXPECT_EQ("Input", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, CatCommandFile) {
        std::stringstream input("Input"), output;
        CatCommand command = CatCommand("../test/resources/test.txt");
        std::stringstream expected;
        expected << "Example text!\nHi\n";

        auto result = command.execute(input, output);
        EXPECT_EQ(expected.str(), output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, CatCommandUnknownFile) {
        std::stringstream input("Input"), output;
        CatCommand command = CatCommand("../test/resources/unknown.txt");

        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::error, result.status);
        EXPECT_EQ("Can't open file: " +
                std::string(std::filesystem::absolute("../test/resources/unknown.txt")) + "\n",
                result.message);
    }

    TEST(Commands, EchoCommand) {
        std::stringstream input("Input"), output;
        EchoCommand command = EchoCommand("text text");

        auto result = command.execute(input, output);
        EXPECT_EQ("text text", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, ExitCommand) {
        std::stringstream input("Input"), output;
        ExitCommand command = ExitCommand();

        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::exit, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, PwdCommand) {
        std::stringstream input("Input"), output;
        PwdCommand command = PwdCommand();

        auto result = command.execute(input, output);
        EXPECT_EQ(std::string(std::filesystem::current_path()) + "\n", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, WcCommandStream) {
        std::stringstream input("Input"), output;
        WcCommand command = WcCommand();

        auto result = command.execute(input, output);
        EXPECT_EQ("0 1 5\n", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, WcCommandFile) {
        std::stringstream input("Input"), output;
        WcCommand command = WcCommand("../test/resources/test.txt");

        auto result = command.execute(input, output);
        EXPECT_EQ("2 3 17\n", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
    }

    TEST(Commands, WcCommandUnknownFile) {
        std::stringstream input("Input"), output;
        WcCommand command = WcCommand("../test/resources/unknown.txt");

        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::error, result.status);
        EXPECT_EQ("Can't open file: " +
                std::string(std::filesystem::absolute("../test/resources/unknown.txt")) + "\n",
                result.message);
    }

    void cdCommandNoHome() {
        std::stringstream input("Input"), output;
        Environment env;
        CdCommand command = CdCommand(env);
        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::error, result.status);
        EXPECT_EQ("HOME variable is not specified\n",
                result.message);
    }

    void cdCommandHome() {
        std::stringstream input("Input"), output;
        Environment env;
        env.set_variable("HOME", "../test/resources");
        auto expectedPath = std::filesystem::canonical(std::filesystem::absolute(std::filesystem::path("../test/resources")));
        CdCommand command = CdCommand(env);
        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
        EXPECT_EQ(std::filesystem::current_path(), expectedPath);
        CdCommand(env, "../../build").execute(input, output);
    }

    void cdCommand() {
        std::stringstream input("Input"), output;
        Environment env;
        auto path = std::filesystem::canonical(std::filesystem::absolute(std::filesystem::path("../test/resources")));
        CdCommand command = CdCommand(env, path);
        auto result = command.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
        EXPECT_EQ(std::filesystem::current_path(), path);
        CdCommand(env, "../../build").execute(input, output);
    }

    void cdCommandBack() {
        std::stringstream input("Input"), output;
        Environment env;
        auto path = std::filesystem::canonical(std::filesystem::absolute(std::filesystem::path("../test/resources")));
        CdCommand command = CdCommand(env, path);
        command.execute(input, output);
        CdCommand backCommand = CdCommand(env, std::filesystem::path(".."));
        auto result = backCommand.execute(input, output);
        EXPECT_EQ("", output.str());
        EXPECT_EQ(ExecutionStatus::success, result.status);
        EXPECT_EQ("", result.message);
        EXPECT_EQ(std::filesystem::current_path(), path.parent_path());
    }

    TEST(Commands, CdCommand) {
        cdCommandNoHome();
        cdCommandHome();
        cdCommand();
        cdCommandBack();
    }

} // namespace
