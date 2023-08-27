//
// Created by mah454 on 8/16/23.
//

#include "Configuration.h"

#pragma clang diagnostic ignored "-Wshadow"

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
    if (conn_url.empty()) Configuration::conn_url = getenv("MYSQL_CONNECTION_URL");
}

void Configuration::setUsername(const std::string &username) {
    Configuration::username = username;
    if (username.empty()) Configuration::username = getenv("MYSQL_CONNECTION_USERNAME");
}

void Configuration::setPassword(const std::string &password) {
    Configuration::password = password;
    if (password.empty()) Configuration::password = getenv("MYSQL_CONNECTION_PASSWORD");
}

void Configuration::setPoolSize(int poolSize) {
    Configuration::poolSize = std::to_string(poolSize);
    if (poolSize <= 0) Configuration::poolSize = getenv("MYSQL_CONNECTION_POOL_SIZE");
}