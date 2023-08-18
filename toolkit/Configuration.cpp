//
// Created by mah454 on 8/16/23.
//

#include "Configuration.h"

const std::string &Configuration::getGetUrl() const {
    return getUrl;
}

const std::string &Configuration::getUsername() const {
    return username;
}

const std::string &Configuration::getPassword() const {
    return password;
}

const int &Configuration::getPoolSize() const {
    return poolSize;
}
