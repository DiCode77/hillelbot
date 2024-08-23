#include <commands.hxx>


std::string Echo::execute(std::string cmd) {
    cmd.erase(0, strlen("/echo "));
    return cmd;
}

std::string DoNothing::execute(std::string cmd) {
    return std::string();
}

std::string Hello::execute(std::string cmd) {
    return std::string("Welcome to using the best bot in the world lol");
}



std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> Commands::commands() {
    std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> cmds = {
        { "/echo ", std::make_shared<Echo>() },
        { "/hello", std::make_shared<Hello>() },
    };
    return cmds;
}

std::string Commands::is_valid_command(const std::string& maybe_command) {
    for (auto& pair : commands()) {
        if (maybe_command.starts_with(pair.first)) {
            return std::string(pair.first);
        }
    }
    return std::string();
}

std::shared_ptr<AbstractCommand> Commands::get_command(const std::string maybe_command_name) {
    std::string command_name = is_valid_command(maybe_command_name);
    if (!command_name.empty()) {
        return std::move(commands()[command_name]);
    }
    else {
        return std::make_shared<DoNothing>();
    }
}
