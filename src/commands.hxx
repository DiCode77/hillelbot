#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>



class AbstractCommand {
public:
    virtual std::string execute(std::string cmd) = 0;
    virtual ~AbstractCommand() {};
};

class Echo : public AbstractCommand {
public: 
    std::string execute(std::string cmd) override;
};

class DoNothing : public AbstractCommand {
public:
    std::string execute(std::string cmd) override;
};

class Hello : public AbstractCommand {
public:
    std::string execute(std::string cmd) override;
};

namespace Commands {
    std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> commands();

    std::string is_valid_command(const std::string& maybe_command);

    std::shared_ptr<AbstractCommand> get_command(const std::string maybe_command_name);
};
