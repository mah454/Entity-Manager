//
// Created by sheikhhosseini on 2023-08-16.
//

#include <string>
#include "Repository.h"
#include <fmt/format.h>

void Repository::save(std::map<std::string, std::string>& map,const std::string& table) {
    std::string query = "insert into {} ({}) values ({})";
    std::string keys;
    std::string values;
    for (const auto& pair : map) {
        keys += pair.first + ",";
        values += "'" + pair.second + "',";
    }

    keys = keys.substr(0,keys.length() - 1);
    values = values.substr(0,keys.length() - 1);

    std::string formattedQuery = fmt::format(query,table,keys,values);
}

void Repository::saveAll(std::list<std::map<std::string, std::string>> *eList) {

}

std::map<std::string, std::string> Repository::findById(long id) {
    return std::map<std::string, std::string>();
}

std::list<std::map<std::string, std::string>> Repository::findAll() {
    return std::list<std::map<std::string, std::string>>();
}

bool Repository::existsById(long id) {
    return false;
}

long Repository::count() {
    return 0;
}

void Repository::removeById(long id) {

}

void Repository::remove(std::map<std::string, std::string>& e) {

}

void Repository::removeAll() {

}

void Repository::removeAll(std::list<std::map<std::string, std::string>>& eList) {

}

void Repository::removeAllById(std::list<long> *eList) {

}
