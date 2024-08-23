#pragma once
#include <string>
#include <iostream>
#include <unordered_map>



class AbstractCommand {
public:
    virtual std::string execute(std::string cmd) = 0;
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
    std::unordered_map<std::string, AbstractCommand*> commands();

    std::string* is_valid_command(const std::string& maybe_command);

    AbstractCommand* get_command(const std::string maybe_command_name);
};
