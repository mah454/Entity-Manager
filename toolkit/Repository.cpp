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

void Repository::merge(std::vector<SqlColumn> &params, SqlColumn &whereClause) {
    Database &instance = Database::getInstance();
    sql::Connection *connection = instance.getConnection();

    std::cout << "Try to insert new row" << std::endl;
    std::string query = "update {} set {} where {}";
    std::string sets;
    for (const auto &item: params) {
        sets += item.key + "=?,";
    }

    std::string clause_statement;
    int wct = whereClause.type;
    std::string wck = whereClause.key;
    std::string wcv = whereClause.value;
    switch (wct) {
        case sql::DataType::DECIMAL:
        case sql::DataType::NUMERIC:
        case sql::DataType::CHAR:
        case sql::DataType::VARCHAR:
        case sql::DataType::VARBINARY:
        case sql::DataType::GEOMETRY:
        case sql::DataType::JSON:
        case sql::DataType::DATE:
        case sql::DataType::TIME:
        case sql::DataType::TIMESTAMP:
        case sql::DataType::ENUM: {
            clause_statement = wck + "='" + wcv + "'";
            break;
        }
        case sql::DataType::BIT:
        case sql::DataType::SMALLINT:
        case sql::DataType::TINYINT:
        case sql::DataType::MEDIUMINT:
        case sql::DataType::YEAR:
        case sql::DataType::INTEGER: {
            clause_statement = wck + "=" + wcv + "";
            break;
        }
        default: {
            std::cout << "Unknown data type for where clause" << std::endl;
            std::cout << "Current support data types: [STRING,INT,INT64,UNSIGNED_INT,UNSIGNED_INT64,DOUBLE,BIG_INT]"
                      << std::endl;
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
    std::string query = "select * from {} where id={}";
    std::string formattedQuery = fmt::format(query, tableName, id);
    std::cout << "Query : " + formattedQuery << std::endl;
    sql::PreparedStatement *preparedStatement = connection->prepareStatement(formattedQuery);
    sql::ResultSet *rs = preparedStatement->executeQuery();
    sql::ResultSetMetaData *metaData = rs->getMetaData();

    std::vector<SqlColumn> result;
    int i = 1;
    while (rs->next()) {
        std::string columnName = metaData->getColumnLabel(i);
        std::string columnTypeName = metaData->getColumnTypeName(i);

        SqlColumn column = {columnName, ""};

        if (std::equal(columnTypeName.begin(), columnTypeName.end(), "BIT")) {
            column.type = sql::DataType::BIT;
            column.value = std::to_string(rs->getInt(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "TINYINT")) {
            column.type = sql::DataType::TINYINT;
            column.value = std::to_string(rs->getInt(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "SMALLINT")) {
            column.type = sql::DataType::SMALLINT;
            column.value = std::to_string(rs->getInt(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "MEDIUMINT")) {
            column.type = sql::DataType::MEDIUMINT;
            column.value = std::to_string(rs->getInt(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "INTEGER")) {
            column.type = sql::DataType::INTEGER;
            column.value = std::to_string(rs->getInt(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "BIGINT")) {
            column.type = sql::DataType::BIGINT;
            column.value = std::to_string(rs->getInt64(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "REAL")) {
            column.type = sql::DataType::REAL;
            column.value = std::to_string(rs->getDouble(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "DOUBLE")) {
            column.type = sql::DataType::DOUBLE;
            column.value = std::to_string(rs->getDouble(i));
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "DECIMAL")) {
            column.type = sql::DataType::DECIMAL;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "NUMERIC")) {
            column.type = sql::DataType::NUMERIC;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "CHAR")) {
            column.type = sql::DataType::CHAR;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "BINARY")) {
            column.type = sql::DataType::BINARY;
            std::istream *is = rs->getBlob(i);
            std::stringstream buffer;
            buffer << is->rdbuf();
            std::string binaryData = buffer.str();
            column.value = binaryData;
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "VARCHAR")) {
            column.type = sql::DataType::VARCHAR;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "VARBINARY")) {
            column.type = sql::DataType::VARBINARY;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "LONGVARCHAR")) {
            column.type = sql::DataType::LONGVARCHAR;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "LONGVARBINARY")) {
            column.type = sql::DataType::LONGVARBINARY;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "TIMESTAMP")) {
            column.type = sql::DataType::TIMESTAMP;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "DATE")) {
            column.type = sql::DataType::DATE;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "TIME")) {
            column.type = sql::DataType::TIME;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "YEAR")) {
            column.type = sql::DataType::YEAR;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "GEOMETRY")) {
            column.type = sql::DataType::GEOMETRY;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "ENUM")) {
            column.type = sql::DataType::ENUM;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "SET")) {
            column.type = sql::DataType::SET;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "SQLNULL")) {
            column.type = sql::DataType::SQLNULL;
            column.value = "";
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "JSON")) {
            column.type = sql::DataType::JSON;
            column.value = rs->getString(i);
        } else if (std::equal(columnTypeName.begin(), columnTypeName.end(), "UNKNOWN")) {
            column.type = sql::DataType::UNKNOWN;
            column.value = "";
        }
        result.push_back(column);
        i++;
    }

    return result;
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

void Repository::parsePreparedStatement(const std::vector<SqlColumn> &params, sql::PreparedStatement *preparedStatement) {
    for (int i = 0; i < params.size(); ++i) {
        int type = params[i].type;
        std::string value = params[i].value;

        switch (type) {
            case sql::DataType::BIT:
            case sql::DataType::SMALLINT:
            case sql::DataType::TINYINT:
            case sql::DataType::MEDIUMINT:
            case sql::DataType::YEAR:
            case sql::DataType::INTEGER: {
                preparedStatement->setInt(i + 1, static_cast<int32_t>(std::stoul(value)));
                break;
            }
            case sql::DataType::REAL:
            case sql::DataType::DOUBLE:
                preparedStatement->setDouble(i + 1, static_cast<double >(std::stoul(value)));
                break;
            case sql::DataType::BIGINT:
                preparedStatement->setBigInt(i + 1, value);
                break;
            case sql::DataType::DECIMAL:
            case sql::DataType::NUMERIC:
            case sql::DataType::CHAR:
            case sql::DataType::VARCHAR:
            case sql::DataType::VARBINARY:
            case sql::DataType::GEOMETRY:
            case sql::DataType::JSON:
            case sql::DataType::DATE:
            case sql::DataType::TIME:
            case sql::DataType::TIMESTAMP:
            case sql::DataType::ENUM:
                preparedStatement->setString(i + 1, value);
                break;
            case sql::DataType::LONGVARCHAR:
            case sql::DataType::LONGVARBINARY: {
                std::istringstream is(value);
                preparedStatement->setBlob(i + 1, &is);
                break;
            }
            default:
                std::cout << "Unknown data type" << std::endl;
        }
    }
}
