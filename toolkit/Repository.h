//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_REPOSITORY_H
#define ENTITY_MANAGER_REPOSITORY_H

#include <list>
#include <map>
#include <iostream>
#include <vector>
#include "SqlColumn.cpp"
#include "Database.h"

class Repository {
    std::string tableName;
private:
    static void parsePreparedStatement(const std::vector<SqlColumn> &params, sql::PreparedStatement *preparedStatement) ;
public:
    explicit Repository(std::string tableName);

    void save(std::vector<SqlColumn> &params);
    void merge(std::vector<SqlColumn> &params,SqlColumn &clause);

    void saveAll(std::vector<std::vector<SqlColumn>> &eList);

    std::vector<SqlColumn> findById(long id);

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