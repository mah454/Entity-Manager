//
// Created by sheikhhosseini on 2023-08-16.
//

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include "Database.h"

sql::Connection *Database::createConnection() {
    return driver->connect(url, username, password);
}

void Database::connect() {
    sql::Connection *connection;
    for (int i = 0; i < defaultPoolSize; i++) {
        connection = createConnection();
        connections.push(connection);
    }
}

sql::ResultSet *Database::executeQuery(const std::string &query) {
    sql::Connection *connection = getConnection();
    sql::Statement *statement = connection->createStatement();
    return statement->executeQuery(query);
}

void Database::execute(const std::string &query) {
    sql::Connection *connection = getConnection();
    sql::Statement *statement = connection->createStatement();
    statement->executeUpdate(query);
}

sql::Connection *Database::getConnection() {
    std::unique_lock<std::mutex> lock(mutex);
    while (connections.empty()) {}

    sql::Connection *connection = connections.front();
    connections.pop();

    if (!connection->isValid()) {
        delete connection;
        connection = createConnection();
    }

    return connection;
}

void Database::releaseConnection(sql::Connection *connection) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.push(connection);
}

void Database::destroyPool() {
    std::unique_lock<std::mutex> lock(mutex);
    while (!connections.empty()) {
        sql::Connection *connection = connections.front();
        connections.pop();
        delete connection;
    }
}

void Database::txBegin(sql::Connection *connection) {
    if (!connection->getAutoCommit()) connection->setAutoCommit(false);
}

void Database::txCommit(sql::Connection *connection) {
    if (!connection->getAutoCommit()) connection->commit();
}

