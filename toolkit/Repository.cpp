//
// Created by sheikhhosseini on 2023-08-16.
//

#include <string>
#include "Repository.h"
#include "Database.h"
#include <fmt/format.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include <utility>

void Repository::save(std::vector<SqlParameter> &params) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();

    std::cout << "Try to insert new row" << std::endl;
    std::string query = "insert into {} ({}) values ({})";
    std::string keys;
    std::string values;
    for (const auto &item: params) {
        keys += item.key + ",";
        values += "?,";
    }

    keys = keys.substr(0, keys.length() - 1);
    values = values.substr(0, values.length() - 1);
    std::string formattedQuery = fmt::format(query, tableName, keys, values);

    std::cout << "Formatted Query: " + formattedQuery << std::endl;
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);

    parsePreparedStatement(params, preparedStatement);

    preparedStatement->executeUpdate();
    preparedStatement->close();
    delete preparedStatement;
    instance.releaseConnection(connection);
}

void Repository::merge(std::vector<SqlParameter> &params,SqlParameter &whereClause) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();

    std::cout << "Try to insert new row" << std::endl;
    std::string query = "update {} set {} where {}";
    std::string sets;
    for (const auto &item: params) {
        sets += item.key + "=?," ;
    }

    std::string clause_statement ;
    DataType wct = whereClause.type;
    std::string wck = whereClause.key;
    std::string wcv = whereClause.value;
    switch (wct) {
        case STRING: {
            clause_statement = wck + "='" + wcv + "'";
            break;
        }
        case INT:
        case INT64:
        case UNSIGNED_INT:
        case UNSIGNED_INT64:
        case DOUBLE:
        case BIG_INT: {
            clause_statement = wck + "=" + wcv + "";
            break;
        }
        default: {
            std::cout << "Unknown data type for where clause" << std::endl;
            std::cout << "Current support data types: [STRING,INT,INT64,UNSIGNED_INT,UNSIGNED_INT64,DOUBLE,BIG_INT]" << std::endl;
            EXIT_FAILURE;
        }
    }

    sets = sets.substr(0, sets.length() - 1);
    std::string formattedQuery = fmt::format(query, tableName, sets, clause_statement);

    std::cout << "Formatted Query: " + formattedQuery << std::endl;
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);

    parsePreparedStatement(params, preparedStatement);

    preparedStatement->executeUpdate();
    preparedStatement->close();
    delete preparedStatement;
}

void Repository::saveAll(std::vector<std::vector<SqlParameter>> &eList) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    Database::txBegin(connection);
    for (std::vector<SqlParameter> &item : eList) {
        save(item);
    }
    Database::txCommit(connection);
}

std::map<std::string, std::string> Repository::findById(long id) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "select * from {} where id={}";
    std::string formattedQuery = fmt::format(query, tableName, id);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    sql::ResultSet *rs = preparedStatement->executeQuery();
    while (rs->next()) {

    }
    return {};
}

std::list<std::map<std::string, std::string>> Repository::findAll() {
    return {};
}

bool Repository::existsById(long id) {
    return false;
}

void Repository::removeById(long id) {

}

void Repository::remove(std::map<std::string, std::string> &e) {

}

void Repository::removeAll() {

}

void Repository::removeAll(std::list<std::map<std::string, std::string>> &eList) {

}

void Repository::removeAllById(std::list<long> *eList) {

}

long Repository::count() {
    return 0;
}

Repository::Repository(std::string tableName) : tableName(std::move(tableName)) {

}

void Repository::parsePreparedStatement(const std::vector<SqlParameter> &params,
                                        sql::PreparedStatement *preparedStatement) {
    for (int i = 0; i < params.size(); ++i) {
        DataType type = params[i].type;
        std::string value = params[i].value;

        switch (type) {
            case INT: {
                preparedStatement->setInt(i + 1, static_cast<int32_t>(std::stoul(value)));
                break;
            }
            case INT64:
                preparedStatement->setInt64(i + 1, static_cast<int64_t>(std::stoul(value)));
                break;
            case BIG_INT:
                preparedStatement->setBigInt(i + 1, sql::SQLString(value));
                break;
            case DOUBLE:
                preparedStatement->setDouble(i + 1, std::stod(value));
                break;
            case UNSIGNED_INT:
                preparedStatement->setUInt(i + 1, static_cast<uint32_t>(std::stoul(value)));
                break;
            case UNSIGNED_INT64:
                preparedStatement->setUInt64(i + 1, static_cast<uint64_t>(std::stoul(value)));
                break;
            case BOOLEAN:
                preparedStatement->setBoolean(i + 1, value == "true" || value == "1");
                break;
            case BLOB: {
                std::istringstream is(value);
                preparedStatement->setBlob(i + 1, &is);
                break;
            }
            case STRING:
                preparedStatement->setString(i + 1, value);
                break;
            case DATE_TIME:
                preparedStatement->setDateTime(i + 1, sql::SQLString(value));
                break;
            case NULL_VALUE:
                preparedStatement->setNull(i + 1, sql::DataType::SQLNULL);
                break;
        }
    }
}
