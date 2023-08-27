//
// Created by sheikhhosseini on 2023-08-27.
//

#include <iostream>
#include <gtest/gtest.h>

/*
 * Before test do this steps manually :
 * 1) Connect to database
 * 2) Create database : [create database sample;]
 * 3) Create table : [create table person (id int NOT NULL primary key AUTO_INCREMENT,name varchar(20),family varchar(20));]
 * */

namespace {
    TEST(BBBB, B1) {
        std::cout << "A3" << std::endl;
    };

    TEST(BBBB, B2) {
        std::cout << "A4" << std::endl;
    };
}