//
// Created by mah454 on 8/16/23.
//

#include "Configuration.h"

std::string Configuration::getGetUrl() const {
    return "tcp://127.0.0.1:3306/sample";
}

std::string Configuration::getUsername() const {
    return "root";
}

std::string Configuration::getPassword() const {
    return "rootpass";
}

int Configuration::getPoolSize() const {
    return 10;
}
