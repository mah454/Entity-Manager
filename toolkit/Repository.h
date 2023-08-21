//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_REPOSITORY_H
#define ENTITY_MANAGER_REPOSITORY_H

#include <map>
#include <iostream>
#include <vector>
#include "SQL.cpp"
#include "Database.h"

class Repository {
    std::string tableName;
private:
    static void parsePreparedStatement(const std::vector<SqlCell> &params, sql::PreparedStatement *preparedStatement);

    static void mapToSqlColumn(sql::ResultSet *rs, std::vector<SqlCell> &result);

public:
    explicit Repository(std::string tableName);

    void save(std::vector<SqlCell> &params, sql::Connection *connection = Database::getInstance().getConnection());

    void merge(std::vector<SqlCell> &params, std::string &whereClause, sql::Connection *connection = Database::getInstance().getConnection());

    void saveAll(std::vector<std::vector<SqlCell>> &eList);

    std::vector<SqlCell> findById(long id);

    std::vector<std::vector<SqlCell>> findAll();

    std::vector<std::vector<SqlCell>> find(std::string &whereClause);

    bool existsById(long id);

    long count();

    void removeById(long id);

    void removeAll();

    void removeAllById(std::vector<long> &eList);
};

#endif //ENTITY_MANAGER_REPOSITORY_H