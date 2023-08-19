//
// Created by sheikhhosseini on 2023-08-16.
//

#include <string>
#include "Repository.h"
#include "Database.h"
#include <fmt/format.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include "SqlParameter.cpp"

void Repository::save(std::vector<SqlParameter> &params, const std::string &table) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();

    std::cout << "Try to save row" << std::endl;
    std::string query = "insert into {} ({}) values ({})";
    std::string keys;
    std::string values;
    for (const auto &item: params) {
        keys += item.key + ",";
        values += "?,";
    }

    keys = keys.substr(0, keys.length() - 1);
    values = values.substr(0, keys.length() - 1);
    std::string formattedQuery = fmt::format(query, table, keys, values);

    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);

    preparedStatement->setInt(1, 213123);

    for (int i = 0; i < params.size(); ++i) {
        DataType type = params[i].type;
        std::string value = params[i].value;
        switch (type) {
            case INT:
                preparedStatement->setInt(i, static_cast<int32_t>(std::stoul(values)));
                break;
            case INT64:
                preparedStatement->setInt64(i, static_cast<int64_t>(std::stoul(values)));
                break;
            case BIG_INT:
                preparedStatement->setBigInt(i, values);
                break;
            case DOUBLE:
                preparedStatement->setDouble(i, static_cast<double>(std::stoul(values)));
                break;
            case UNSIGNED_INT:
                preparedStatement->setUInt(i, static_cast<uint32_t>(std::stoul(values)));
                break;
            case UNSIGNED_INT64:
                preparedStatement->setUInt64(i, static_cast<uint64_t>(std::stoul(values)));
                break;
            case BOOLEAN:
                preparedStatement->setBoolean(i, static_cast<bool>(std::stoul(values)));
                break;
            case BLOB: {
                std::istringstream is(value);
                preparedStatement->setBlob(i, &is);
                break;
            }
            case STRING:
                preparedStatement->setString(i, value);
                break;
            case DATE_TIME:
                preparedStatement->setDateTime(i, value);
                break;
            case NULL_VALUE:
                preparedStatement->setNull(i,0);
                break;
        }

    }

    preparedStatement->executeUpdate();
    preparedStatement->close();
}

void Repository::saveAll(std::list<std::map<std::string, std::string>> *eList) {

}

std::map<std::string, std::string> Repository::findById(long id) {
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
