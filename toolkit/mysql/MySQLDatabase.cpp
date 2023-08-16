//
// Created by mah454 on 8/15/23.
//

#include <mutex>
#include "MySQLDatabase.h"

sql::Connection *Database::createConnection() {
    return driver->connect(url, username, password);
}

void Database::connect() {
    driver = get_driver_instance();
    sql::Connection *connection;
    for (int i = 0; i < defaultPoolSize; i++) {
        connection = createConnection();
        connections.push(connection);
    }
}

sql::ResultSet *Database::executeQuery(const std::string &query, sql::Statement &statement) {
    return statement.executeQuery(query);
}

void Database::execute(const std::string &query, sql::Statement &statement) {
    statement.executeUpdate(query);
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
