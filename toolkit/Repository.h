//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_REPOSITORY_H
#define ENTITY_MANAGER_REPOSITORY_H

#include <list>
#include <map>
#include <iostream>
#include <vector>
#include "SqlParameter.cpp"

class Repository {
public:
    void save(std::vector<SqlParameter> &params,const std::string& table);

    void saveAll(std::list<std::map<std::string, std::string>> *eList);

    std::map<std::string, std::string> findById(long id);

    std::list<std::map<std::string, std::string>> findAll();

    bool existsById(long id);

    long count();

    void removeById(long id);

    void remove(std::map<std::string, std::string>& e);

    void removeAll();

    void removeAll(std::list<std::map<std::string, std::string>>& eList);

    void removeAllById(std::list<long> *eList);
};

#endif //ENTITY_MANAGER_REPOSITORY_H