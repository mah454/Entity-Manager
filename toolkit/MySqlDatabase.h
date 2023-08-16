//
// Created by mah454 on 8/15/23.
//

#ifndef SOCKET_ENTITYMANAGER_H
#define SOCKET_ENTITYMANAGER_H

#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <queue>

class MySqlDatabase {

    std::mutex mutex;

    /* Connection info */
    sql::Driver *driver;
    std::string &url;
    std::string &username;
    std::string &password;

    /* Connection pool */
    int defaultPoolSize = 5;
    std::queue<sql::Connection *> connections;
public:
    MySqlDatabase(std::string &url, std::string &username, std::string &password, int poolSize)
            : password(password), url(url), username(username), defaultPoolSize(poolSize) {
        this->url = url;
        this->username = username;
        this->password = password;

        /* Force set minimum pool size */
        if (poolSize < defaultPoolSize) defaultPoolSize = poolSize;
    }

    /*
     * Used for select queries
     * */
    static sql::ResultSet *executeQuery(const std::string &query, sql::Statement &statement);

    /*
     * Used for insert, delete, update, queries
     * */
    static void execute(const std::string &query, sql::Statement &statement);

    /*
     * get a connection from pool
     * */
    sql::Connection *getConnection();

    void releaseConnection(sql::Connection *connection);

    void destroyPool();

    static void txBegin(sql::Connection *connection);

    static void txCommit(sql::Connection *connection);

private:
    sql::Connection *createConnection();

    void connect();
};


#endif //SOCKET_ENTITYMANAGER_H
