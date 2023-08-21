//
// Created by sheikhhosseini on 2023-08-19.
//


#include <string>

enum DataType {
    TINY_INT, // 0
    INT, // 1
    BIG_INT, // 2
    DOUBLE, // 3
    DECIMAL, // 4
    STRING, // 5
    CHAR, // 6
    CLOB, // 7
    BLOB, // 8
    TIMESTAMP, // 9
    DATE, // 10
    TIME, // 11
    NILL, // 12
};

struct SqlCell {
    std::string key;
    std::string value;
    DataType type;
};