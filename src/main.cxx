#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>

// Команда echo - команда, яка просто видає вам назад ту саму строку,
// яку ви відіслали боту у команді.
int main() {
    std::ifstream secret_file("secret.json");
    bool good = (secret_file.good() && secret_file.is_open());

    if (!good) {
        std::cout << "ERROR: unable to open the secret file." << std::endl;
        return -1;
    }

    nlohmann::json secret_json;

    secret_file >> secret_json;
    auto secret = secret_json["key"].get<std::string>();
    secret_file.close();

    // Виконати GET-запит до публічного API
    std::string base_url = (std::stringstream() << "http://api.telegram.org/" << secret).str();
    cpr::Response r = cpr::Get(cpr::Url{ base_url, "/getUpdates?offset=137803087" });

    // Вивести статус код відповіді
    std::cout << "Status Code: " << r.status_code << std::endl;

    // Парсити JSON відповідь
    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);

    if(!jsonResponse["result"].size()) {
        std::cout << "WARNING: there has been no updates on the bot. Not POSTing anything.\n";
        return 0;
    }

    for (auto& user_message : jsonResponse["result"]) {
        int64_t chat_id = user_message["message"]["chat"]["id"].get<int64_t>();

        cpr::Payload p{
            {"chat_id", std::to_string(chat_id)},
            {"text", "NEW BOT MESSAGE!!11"}
        };

        std::cout << "Chat ID? => " << chat_id << std::endl;

        cpr::Response postR = cpr::Post(cpr::Url{ base_url, "/sendMessage" }, p);
        std::cout << "Post Status Code? => " << postR.status_code << std::endl;
    }

    return 0;
}
