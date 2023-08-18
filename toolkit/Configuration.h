//
// Created by mah454 on 8/16/23.
//

#ifndef ENTITY_MANAGER_CONFIGURATION_H
#define ENTITY_MANAGER_CONFIGURATION_H


#include <string>

class Configuration {
private:
    std::string getUrl ;
    std::string username ;
    std::string password ;
    int poolSize ;
public:
    const std::string &getGetUrl() const;

    const std::string &getUsername() const;

    const std::string &getPassword() const;

    const int &getPoolSize() const;

};


#endif //ENTITY_MANAGER_CONFIGURATION_H
