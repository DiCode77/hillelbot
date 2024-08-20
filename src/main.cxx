#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>


#include <Secret.hxx>
#include <config.hxx>

// Команда echo - команда, яка просто видає вам назад ту саму строку,
// яку ви відіслали боту у команді.
// TODO: Треба розрізняти різні типи помилок.
int main() {
    Secret s{ std::string(SecretFilename) };
    if (!s.is_good()) {
        std::cout << "ERROR: unable to open the secret file." << std::endl;
        return -1;
    }

    // Виконати GET-запит до публічного API
    std::string base_url = (std::stringstream() << "http://api.telegram.org/" << s.get_secret()).str();
    cpr::Response r = cpr::Get(cpr::Url{ base_url, "/getUpdates?offset=137803088" });

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
        // check that starts with `/echo `
        std::string user_text = user_message["message"]["text"].get<std::string>();
        if (!user_text.starts_with("/echo ")) {
            continue;
        }
        user_text.erase(0, strlen("/echo "));


        cpr::Payload p{
            {"chat_id", std::to_string(chat_id)},
            {"text", user_text}
        };

        std::cout << "Chat ID? => " << chat_id << std::endl;

        cpr::Response postR = cpr::Post(cpr::Url{ base_url, "/sendMessage" }, p);
        std::cout << "Post Status Code? => " << postR.status_code << std::endl;
    }

    return 0;
}
