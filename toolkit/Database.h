//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_DATABASE_H
#define ENTITY_MANAGER_DATABASE_H

#include <mutex>

class Database {
private:
    virtual sql::Connection *createConnection() = 0;
    virtual void connect() = 0;


public:
    std::mutex mutex;

    /* Connection info */
    sql::Driver *driver;
    std::string &url;
    std::string &username;
    std::string &password;

    /* Connection pool */
    int defaultPoolSize = 5;
    std::queue<sql::Connection *> connections;

    Database(std::string &url,
             std::string &username,
             std::string &password,
             int poolSize) : url(url), username(username), password(password) {
        /* Force set minimum pool size */
        if (poolSize < defaultPoolSize) defaultPoolSize = poolSize;
    }

    /*
     * Used for select queries
     * */
    virtual sql::ResultSet *executeQuery(const std::string &query, sql::Statement &statement) = 0;

    /*
     * Used for insert, delete, update, queries
     * */
    virtual void execute(const std::string &query, sql::Statement &statement) = 0;

    /*
     * get a connection from pool
     * */
    virtual sql::Connection *getConnection() = 0;

    virtual void releaseConnection(sql::Connection *connection) = 0;

    virtual void destroyPool() = 0;

    virtual void txBegin(sql::Connection *connection) = 0;

    virtual void txCommit(sql::Connection *connection) = 0;
};

#endif //ENTITY_MANAGER_DATABASE_H
