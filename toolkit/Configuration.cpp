//
// Created by mah454 on 8/16/23.
//

#include "Configuration.h"

std::string Configuration::getConnectionUrl() const {
    return conn_url;
}

std::string Configuration::getUsername() const {
    return username;
}

std::string Configuration::getPassword() const {
    return password;
}

int Configuration::getPoolSize() const {
    return std::stoi(poolSize);
}

void Configuration::setConnectionUrl(const std::string &conn_url) {
    Configuration::conn_url = conn_url;
}

void Configuration::setUsername(const std::string &username) {
    Configuration::username = username;
}

void Configuration::setPassword(const std::string &password) {
    Configuration::password = password;
}

void Configuration::setPoolSize(int poolSize) {
    Configuration::poolSize = poolSize;
}
