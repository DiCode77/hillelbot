#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Secret.hxx"


Secret::Secret(const std::string& filename) : filename(filename), secret(""), good(false) {
    std::ifstream secret_file(filename);
    good = (secret_file.good() && secret_file.is_open());
    if (!good) { return; }

    nlohmann::json secret_json;

    secret_file >> secret_json;
    bool key_exists = (secret_json.find("key") != secret_json.end());
    if (key_exists) {
        secret = secret_json["key"].get<std::string>();
    }
    else {
        good = false;
    }
    
    secret_file.close();
}

bool Secret::is_good() {
    return good;
}

std::string Secret::get_secret() {
    return secret;
}