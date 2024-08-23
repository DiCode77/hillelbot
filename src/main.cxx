#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>

// WARNING: BOT CLEARS AVAILABLE MESSAGES EVERY 24 HOURS (OR 2 MINUTES?!)

#include <Secret.hxx>
#include <config.hxx>
#include <commands.hxx>

// Команда echo - команда, яка просто видає вам назад ту саму строку,
// яку ви відіслали боту у команді.
// TODO: Треба розрізняти різні типи помилок.
int main() {
    Secret s{ std::string(SecretFilename) };
    if (!s.is_good()) {
        std::cout << "ERROR: unable to open the secret file." << std::endl;
        return -1;
    }

    std::ifstream read_settings_file("settings.json");
    nlohmann::json read_settings;
    int64_t last_offset_num = -1;
    if (read_settings_file.is_open() && read_settings_file.good()) {
        read_settings_file >> read_settings;
        if (read_settings["last_offset"].is_number()) {
            last_offset_num = read_settings["last_offset"].get<int64_t>();
            std::cout << "INFO: got last offset from settings file: " << last_offset_num << std::endl;
        }
    }

    std::stringstream base_url;
    base_url << "http://api.telegram.org/" << s.get_secret();
    // Виконати GET-запит до публічного API
    std::stringstream url;
    url << base_url.str() << "/getUpdates";
    if (last_offset_num != -1) url << "?offset=" << last_offset_num;

    cpr::Response r = cpr::Get(cpr::Url{ url.str() });

    // Вивести статус код відповіді
    std::cout << "Status Code: " << r.status_code << std::endl;

    // Парсити JSON відповідь
    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);
    std::cout << std::setw(4) << jsonResponse << std::endl;

    auto result_size = jsonResponse["result"].size();
    if(!result_size) {
        std::cout << "WARNING: there has been no updates on the bot. Not POSTing anything.\n";
        return 0;
    }

    for (auto& user_message : jsonResponse["result"]) {
        int64_t chat_id = user_message["message"]["chat"]["id"].get<int64_t>();
        // check that starts with `/echo `
        std::string user_text = user_message["message"]["text"].get<std::string>();

        auto command_result = Commands::get_command(user_text)->execute(user_text);
        if (command_result.empty()) {
            continue;
        }

        std::cout << "Command result: " << command_result << std::endl;

        cpr::Payload p{
            {"chat_id", std::to_string(chat_id)},
            {"text", command_result}
        };

        std::cout << "Chat ID? => " << chat_id << std::endl;

        cpr::Response postR = cpr::Post(cpr::Url{ base_url.str(), "/sendMessage"}, p);
        std::cout << "Post Status Code? => " << postR.status_code << std::endl;
    }

    auto last_offset = jsonResponse["result"][result_size - 1]["update_id"].get<int64_t>() + 1;
    nlohmann::json settings;
    settings["last_offset"] = last_offset;

    std::cout << "Last update ID: " << last_offset << std::endl;
    std::ofstream settings_file("settings.json", std::ios::trunc);
    settings_file << settings;
    settings_file.close();

    return 0;
}
