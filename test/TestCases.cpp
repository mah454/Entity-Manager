//
// Created by sheikhhosseini on 2023-08-27.
//

#include <iostream>
#include <gtest/gtest.h>
#include "../toolkit/Database.h"
#include "repository/PersonRepository.h"
/*
 * Before test do this steps manually :
 * 1) Connect to database
 * 2) Create database : [create database sample;]
 * 3) Create table : [create table person (id int NOT NULL primary key AUTO_INCREMENT,name varchar(20),family varchar(20));]
 * */

using namespace std;

std::string gen_random(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}


TEST(BasicOperation, INITIALIZE) {
    auto *configuration = new Configuration();
    configuration->setConnectionUrl("tcp://127.0.0.1:3306/sample");
    configuration->setUsername("root");
    configuration->setPassword("rootpass");
    configuration->setPoolSize(5);

    Database::getInstance().setConfiguration(*configuration);
    Database::getInstance().initPool();
};

TEST(BasicOperation, INSERT) {
    PersonRepository personRepository("person");
    std::vector<SqlCell> parameters;
    parameters.push_back({"name", "n1", DataType::STRING});
    parameters.push_back({"family", "f1", DataType::STRING});
    int status = personRepository.save(parameters);
    ASSERT_EQ(status, 0);
}

TEST(BasicOperation, UPDATE) {
    PersonRepository personRepository("person");
    std::vector<SqlCell> parameters;
    parameters.push_back({"name", "Updated n1", DataType::STRING});
    parameters.push_back({"family", "f1", DataType::STRING});
    std::string whereClause = "id=1";
    int status = personRepository.merge(parameters, whereClause);
    ASSERT_EQ(status, 0);
}

TEST(BasicOperation, SELECT) {
    PersonRepository personRepository("person");
    std::vector<SqlCell> resultColumns = personRepository.findById(1);
    ASSERT_EQ(resultColumns.size(), 0);
    ASSERT_EQ(resultColumns[0].value, "1");
    ASSERT_EQ(resultColumns[1].value, "Updated n1");
    ASSERT_EQ(resultColumns[2].value, "f1");
}

TEST(BasicOperation, REMOVE) {
    PersonRepository personRepository("person");
    int status = personRepository.removeById(1);
    ASSERT_EQ(status, 0);
}

TEST(BasicOperation, MULTIPLE_INSERT) {
    PersonRepository personRepository("person");
    sql::Connection *conn = Database::getInstance().getConnection();
    Database::txBegin(conn);
    for (int i = 0; i < 100; ++i) {
        std::vector<SqlCell> parameters;
        parameters.push_back({"name", gen_random(8), DataType::STRING});
        parameters.push_back({"family", gen_random(8), DataType::STRING});
        int status = personRepository.save(parameters, conn);
        ASSERT_EQ(status, 0);
    }
    Database::txCommit(conn);
}


TEST(BasicOperation, REMOVE_ALL) {
    PersonRepository personRepository("person");
    std::vector<std::vector<SqlCell>> list = personRepository.findAll();
    sql::Connection *conn = Database::getInstance().getConnection();
    Database::txBegin(conn);
    for (const auto &item: list) {
        int status = personRepository.removeById(std::stol(item[0].value), conn);
        ASSERT_EQ(status, 0);
    }
    Database::txCommit(conn);
}
