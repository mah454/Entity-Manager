//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_DATABASE_H
#define ENTITY_MANAGER_DATABASE_H

#include <mutex>
#include <cppconn/connection.h>
#include <queue>
#include <cppconn/resultset.h>
#include "Configuration.h"
#include <cppconn/driver.h>

class Database {
private:
    Database() {
        /* Set default pool size */
        defaultPoolSize = 5;

        Configuration *configuration;
        url = configuration->getGetUrl();
        username = configuration->getUsername();
        password = configuration->getPassword();
        int poolSize = configuration->getPoolSize();
        /* Force set minimum pool size */
        if (poolSize > defaultPoolSize) defaultPoolSize = poolSize;
    }

    std::mutex mutex;

    /* Connection info */
    sql::Driver *driver = get_driver_instance();
    std::string url = "";
    std::string username = "";
    std::string password = "";

    /* Connection pool */
    int defaultPoolSize = 6;
    std::queue<sql::Connection *> connections;

    sql::Connection *createConnection();

    void connect();

public:


    static Database &getInstance() {
        static Database instance;
        return instance;
    }

    /*
     * Used for select queries
     * */
    sql::ResultSet *executeQuery(const std::string &query);

    /*
     * Used for insert, delete, update, queries
     * */
    void execute(const std::string &query);

    /*
     * get a connection from pool
     * */
    sql::Connection *getConnection();

    void releaseConnection(sql::Connection *connection);

    void destroyPool();

    static void txBegin(sql::Connection *connection);

    static void txCommit(sql::Connection *connection);
};

#endif //ENTITY_MANAGER_DATABASE_H
