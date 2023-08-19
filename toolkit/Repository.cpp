//
// Created by sheikhhosseini on 2023-08-16.
//

#include <string>
#include "Repository.h"
#include "Database.h"
#include <fmt/format.h>
#include <cppconn/prepared_statement.h>
#include <sstream>

void Repository::save(std::vector<SqlParameter> &params, const std::string &table) {
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
    std::string formattedQuery = fmt::format(query, table, keys, values);

    std::cout << "Formatted Query: " + formattedQuery << std::endl;
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);


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

    preparedStatement->executeUpdate();
    preparedStatement->close();
    delete preparedStatement;
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
