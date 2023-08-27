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
    std::mutex mutex;

    /* Connection info */
    sql::Driver *driver = get_driver_instance();
    std::string url;
    std::string username;
    std::string password;

    /* Connection pool */
    int defaultPoolSize = 5;
    std::queue<sql::Connection *> connections;

    /* Constructor */
    Database();

    /* Private methods */
    sql::Connection *createConnection();

public:
    static Database &getInstance() ;

    void setConfiguration(Configuration &configuration) ;
    void initPool();

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
