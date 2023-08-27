//
// Created by mah454 on 8/16/23.
//

#ifndef ENTITY_MANAGER_CONFIGURATION_H
#define ENTITY_MANAGER_CONFIGURATION_H


#include <string>

class Configuration {
private:
    std::string conn_url ;
    std::string username ;
    std::string password ;
    std::string poolSize ;
public:
    std::string getConnectionUrl() const;

    std::string getUsername() const;

    std::string getPassword() const;

    int getPoolSize() const;

    void setConnectionUrl(const std::string &url);

    void setUsername(const std::string &username);

    void setPassword(const std::string &password);

    void setPoolSize(int poolSize);
};


#endif //ENTITY_MANAGER_CONFIGURATION_H
