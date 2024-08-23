#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Secret.hxx"


Secret::Secret(const std::string& filename) : filename(filename), secret(""), good(false) {
    std::ifstream secret_file(filename);
    std::cout << "filename: " << filename << std::endl;
    good = (secret_file.good() && secret_file.is_open());
    std::cout << "good => " << secret_file.good() << std::endl;
    std::cout << "open => " << secret_file.is_open() << std::endl;
    std::cout << "total => " << good << std::endl;
    if (!good) {
        std::cout << "not good?!" << std::endl;
        return;
    }

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