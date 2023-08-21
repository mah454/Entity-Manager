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

void Repository::save(std::vector<SqlColumn> &params) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
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
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);

    parsePreparedStatement(params, preparedStatement);

    preparedStatement->executeUpdate();
    preparedStatement->close();
    delete preparedStatement;
    instance.releaseConnection(connection);
}

void Repository::merge(std::vector<SqlColumn> &params, std::string &whereClause) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();

    std::string query = "update {} set {} where {}";
    std::string sets;
    for (const auto &item: params) {
        sets += item.key + "=?,";
    }

    sets = sets.substr(0, sets.length() - 1);
    std::string formattedQuery = fmt::format(query, tableName, sets, whereClause);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    parsePreparedStatement(params, preparedStatement);
    preparedStatement->executeUpdate();
    preparedStatement->close();
    delete preparedStatement;
}

void Repository::saveAll(std::vector<std::vector<SqlColumn>> &eList) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    Database::txBegin(connection);
    for (std::vector<SqlColumn> &item: eList) {
        save(item);
    }
    Database::txCommit(connection);
}

std::vector<SqlColumn> Repository::findById(long id) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "SELECT * FROM {} WHERE id=?";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    preparedStatement->setBigInt(1, std::to_string(id));
    sql::ResultSet *rs = preparedStatement->executeQuery();
    std::vector<SqlColumn> result;
    while (rs->next()) mapToSqlColumn(rs, result);
    return result;
}


std::vector<std::vector<SqlColumn>> Repository::find(std::string &whereClause) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "SELECT * FROM {} where {}";
    std::string formattedQuery = fmt::format(query, tableName, whereClause);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    sql::ResultSet *rs = preparedStatement->executeQuery();

    std::vector<std::vector<SqlColumn>> result;
    while (rs->next()) {
        std::vector<SqlColumn> row;
        mapToSqlColumn(rs, row);
        result.push_back(row);
    }
    return result;
}

std::vector<std::vector<SqlColumn>> Repository::findAll() {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "SELECT * FROM {}";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    sql::ResultSet *rs = preparedStatement->executeQuery();

    std::vector<std::vector<SqlColumn>> result;
    while (rs->next()) {
        std::vector<SqlColumn> row;
        mapToSqlColumn(rs, row);
        result.push_back(row);
    }
    return result;
}

bool Repository::existsById(long id) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "SELECT * FROM {} WHERE id=?";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    preparedStatement->setBigInt(1, std::to_string(id));
    sql::ResultSet *rs = preparedStatement->executeQuery();
    std::vector<SqlColumn> result;
    return rs->next();
}

void Repository::removeById(long id) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "DELETE FROM {} WHERE id=?";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    preparedStatement->setBigInt(1, std::to_string(id));
    preparedStatement->executeUpdate();
}

void Repository::removeAll() {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "truncate {}";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    preparedStatement->executeUpdate();
}

void Repository::removeAllById(std::list<long> &eList) {
    for (const auto &item: eList) {
        removeById(item);
    }
}

long Repository::count() {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();
    std::string query = "select count(*) from {}";
    std::string formattedQuery = fmt::format(query, tableName);
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    sql::ResultSet *rs = preparedStatement->executeQuery();
    if (rs->next()) return rs->getInt64(1);
    return 0;
}

Repository::Repository(std::string tableName) : tableName(std::move(tableName)) {

}


/* Private Methods */

void Repository::parsePreparedStatement(const std::vector<SqlColumn> &params, sql::PreparedStatement *preparedStatement) {
    for (int i = 0; i < params.size(); ++i) {
        DataType type = params[i].type;
        std::string value = params[i].value;

        switch (type) {
            case DataType::TINY_INT:
                preparedStatement->setUInt(i + 1, static_cast<uint32_t>(std::stoul(value)));
                break;
            case DataType::INT: {
                preparedStatement->setUInt64(i + 1, static_cast<uint64_t>(std::stoul(value)));
                break;
            }
            case DataType::BIG_INT:
                preparedStatement->setBigInt(i + 1, value);
                break;
            case DataType::DOUBLE:
                preparedStatement->setDouble(i + 1, std::stod(value));
                break;
            case DataType::CHAR:
            case DataType::DECIMAL:
            case DataType::STRING:
            case DataType::CLOB:
                preparedStatement->setString(i + 1, value);
                break;
            case BLOB: {
                std::istringstream is(value);
                preparedStatement->setBlob(i + 1, &is);
                break;
            }
            case DataType::TIMESTAMP:
            case DataType::DATE:
            case DataType::TIME:
                preparedStatement->setDateTime(i + 1, value);
                break;
            case DataType::NILL:
                preparedStatement->setNull(i + 1, sql::DataType::SQLNULL);
                break;
        }
    }
}

void Repository::mapToSqlColumn(sql::ResultSet *rs, std::vector<SqlColumn> &result) const {
    sql::ResultSetMetaData *metaData = rs->getMetaData();
    unsigned int column_count = metaData->getColumnCount();
    for (int i = 1; i < column_count + 1; ++i) {
        std::string columnName = metaData->getColumnLabel(i);
        std::string columnTypeName = metaData->getColumnTypeName(i);

        SqlColumn column = {columnName, ""};
        if (columnTypeName == "TINYINT") {
            column.type = TINY_INT;
            column.value = std::to_string(rs->getInt(i));
        } else if (columnTypeName == "INT") {
            column.type = INT;
            column.value = std::to_string(rs->getInt(i));
        } else if (columnTypeName == "BIGINT") {
            column.type = BIG_INT;
            column.value = std::to_string(rs->getInt64(i));
        } else if (columnTypeName == "DOUBLE") {
            column.type = DOUBLE;
            column.value = std::to_string(rs->getDouble(i));
        } else if (columnTypeName == "DECIMAL") {
            column.type = DECIMAL;
            column.value = rs->getString(i);
        } else if (columnTypeName == "CHAR") {
            column.type = CHAR;
            column.value = rs->getString(i);
        } else if (columnTypeName == "BINARY") {
            column.type = BLOB;
            std::istream *is = rs->getBlob(i);
            std::stringstream buffer;
            buffer << is->rdbuf();
            std::string binaryData = buffer.str();
            column.value = binaryData;
        } else if (columnTypeName == "VARCHAR") {
            column.type = STRING;
            column.value = rs->getString(i);
        } else if (columnTypeName == "LONGVARCHAR") {
            column.type = CLOB;
            column.value = rs->getString(i);
        } else if (columnTypeName == "LONGVARBINARY") {
            column.type = BLOB;
            column.value = rs->getString(i);
        } else if (columnTypeName == "TIMESTAMP") {
            column.type = TIMESTAMP;
            column.value = rs->getString(i);
        } else if (columnTypeName == "DATETIME") {
            column.type = DATE;
            column.value = rs->getString(i);
        } else if (columnTypeName == "TIME") {
            column.type = TIME;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "SQLNULL")) {
            column.type = NILL;
            column.value = "";
        }
        result.push_back(column);
    }
}
