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
    secret = secret_json["key"].get<std::string>();
    secret_file.close();
}

bool Secret::is_good() {
    return good;
}

std::string Secret::get_secret() {
    return secret;
}