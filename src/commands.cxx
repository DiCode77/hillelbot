#include <commands.hxx>

#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <fstream>

int generate_random_number(int from, int to) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(from, to);

    // Генерація випадкового числа
    return distrib(gen);
}

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

std::string Joke::execute(std::string cmd) {
    std::vector<std::string> jokes = {
        "Why don't skeletons fight each other? They don't have the guts.",
        "Why did the scarecrow win an award ? Because he was outstanding in his field!",
        "Why don't scientists trust atoms ? Because they make up everything!",
        "Why did the bicycle fall over ? It was two - tired.",
        "What do you call fake spaghetti ? An impasta!",
        "How does the ocean say hello ? It waves.",
        "Why did the golfer bring two pairs of pants ? In case he got a hole in one!",
        "What do you get when you cross a snowman and a vampire ? Frostbite.",
        "Why was the math book sad ? Because it had too many problems.",
        "What do you call cheese that isn't yours ? Nacho cheese!"
    };

    return jokes[generate_random_number(0, 9)];
}

std::string Add::execute(std::string cmd) {
    std::string str;
    int64_t max_num(0);
    
    if (cmd.size() > std::string("/add ").size()){
        cmd.erase(0, std::string("/add ").size());
        
        for (int i = 0; i < cmd.length(); i++) {
            if (!((int(cmd[i]) >= 48 && 57 >= int(cmd[i])) || int(cmd[i]) == 32)){
                str.clear();
                
                int64_t pos = cmd.rfind(" ", i);
                for (int64_t j = (pos != -1) ? pos +1 : 0; j < cmd.length() && cmd[j] != 32; j++){
                    str += cmd[j];
                }
                
                str.append(" is not an integer number.");
                break;
            }
            else{
                str.append((cmd[i] != ' ') ? std::string(1, cmd[i]) : "");
                
                if (cmd[i +1] == ' ' || cmd[i +1] == '\0'){
                    max_num += std::atol(str.c_str());
                    str.clear();
                }
            }
        }

    }
    else{
        return std::string("0");
    }

    return std::string((str.length() == 0) ? std::to_string(max_num) : str);
}


std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> Commands::commands() {
    std::unordered_map<std::string, std::shared_ptr<AbstractCommand>> cmds = {
        { "/echo ", std::make_shared<Echo>()  },
        { "/hello", std::make_shared<Hello>() },
        { "/joke",  std::make_shared<Joke>()  },
        { "/add",   std::make_shared<Add>()  }
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
