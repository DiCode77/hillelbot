#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <cstdint>


// - Побачити апдейти (кілька повідомлень), що прийшли до бота
// - Спробувати відправити якесь базове повідомлення назад
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
    cpr::Response r = cpr::Get(cpr::Url{ base_url, "/getUpdates?offset=137803083" });
    std::cout << "base url? => " << base_url << std::endl;

    // Вивести статус код відповіді
    std::cout << "Status Code: " << r.status_code << std::endl;

    // Парсити JSON відповідь
    nlohmann::json jsonResponse = nlohmann::json::parse(r.text);

    if(!jsonResponse["result"].size()) {
        std::cout << "WARNING: there has been no updates on the bot. Not POSTing anything.\n";
        return 0;
    }

    // Вивести певне поле з JSON відповіді
    std::cout << "===JSON===\n" << std::setw(4) << jsonResponse << std::endl;
    int64_t chat_id = jsonResponse["result"][0]["message"]["chat"]["id"].get<int64_t>();

    cpr::Payload p {
        {"chat_id", std::to_string(chat_id)},
        {"text", "HELLO IM BOT!!!1"}
    };

    std::cout << "Chat ID? => " << chat_id << std::endl;

    cpr::Response postR = cpr::Post(cpr::Url{ base_url, "/sendMessage" }, p);
    std::cout << "Post Status Code? => " << postR.status_code << std::endl;;
    // Парсити JSON відповідь
    nlohmann::json postResponse = nlohmann::json::parse(postR.text);

    // Вивести певне поле з JSON відповіді
    std::cout << "===JSON===\n" << std::setw(4) << postResponse << std::endl;
 
    return 0;
}
