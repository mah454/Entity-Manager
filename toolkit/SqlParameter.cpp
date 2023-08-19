//
// Created by sheikhhosseini on 2023-08-19.
//


#include <string>

enum DataType {
    INT,
    INT64,
    BIG_INT,
    DOUBLE,
    UNSIGNED_INT,
    UNSIGNED_INT64,
    BOOLEAN,
    BLOB,
    STRING,
    DATE_TIME,
    NULL_VALUE
};


struct SqlParameter {
    std::string key ;
    std::string value ;
    DataType type ;
};