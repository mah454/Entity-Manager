//
// Created by sheikhhosseini on 2023-08-16.
//

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include "Database.h"
#include <syslog.h>

Database &Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() {
    std::cout << "Create instance " << std::endl;
}

void Database::setConfiguration(Configuration &configuration) {
    url = configuration.getConnectionUrl();
    username = configuration.getUsername();
    password = configuration.getPassword();

    /* Force set minimum pool size */
    if (configuration.getPoolSize() > defaultPoolSize) defaultPoolSize = configuration.getPoolSize();

    syslog(LOG_INFO, "Database Configuration url:[%s] username:[%s] password:[%s] pool_size:[%d]", url.c_str(), username.c_str(), password.c_str(), defaultPoolSize);
}

sql::Connection *Database::createConnection() {
    syslog(LOG_INFO, "Try connect to database with url:[%s] username:[%s] password:[%s]", url.c_str(), username.c_str(), password.c_str());
    return driver->connect(url, username, password);
}

void Database::initPool() {
    try {
        sql::Connection *connection;
        for (int i = 0; i < defaultPoolSize; i++) {
            connection = createConnection();
            connections.push(connection);
        }
    } catch (std::exception &e) {
        syslog(LOG_ERR, "Failed connect to database");
        exit(1);
    }
}

sql::Connection *Database::getConnection() {
    std::unique_lock<std::mutex> lock(mutex);

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
    if (connection->getAutoCommit()) connection->setAutoCommit(false);
}

void Database::txCommit(sql::Connection *connection) {
    if (!connection->getAutoCommit()) connection->commit();
}