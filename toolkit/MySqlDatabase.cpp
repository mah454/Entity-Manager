//
// Created by mah454 on 8/15/23.
//

#include <mutex>
#include "MySqlDatabase.h"

void MySqlDatabase::connect() {
    driver = get_driver_instance();
    sql::Connection *connection;
    for (int i = 0; i < defaultPoolSize; i++) {
        connection = createConnection();
        connections.push(connection);
    }
}

sql::ResultSet *MySqlDatabase::executeQuery(const std::string &query, sql::Statement &statement) {
    return statement.executeQuery(query);
}

void MySqlDatabase::execute(const std::string &query, sql::Statement &statement) {
    statement.executeUpdate(query);
}

sql::Connection *MySqlDatabase::getConnection() {
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

void MySqlDatabase::releaseConnection(sql::Connection *connection) {
    std::unique_lock<std::mutex> lock(mutex);
    connections.push(connection);
}

void MySqlDatabase::destroyPool() {
    std::unique_lock<std::mutex> lock(mutex);
    while (!connections.empty()) {
        sql::Connection *connection = connections.front();
        connections.pop();
        delete connection;
    }
}

void MySqlDatabase::txBegin(sql::Connection *connection) {
    if (!connection->getAutoCommit()) connection->setAutoCommit(false);
}

void MySqlDatabase::txCommit(sql::Connection *connection) {
    if (!connection->getAutoCommit()) connection->commit();
}

sql::Connection *MySqlDatabase::createConnection() {
    return driver->connect(url, username, password);
}
