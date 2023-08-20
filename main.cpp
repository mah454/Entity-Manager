#include "app/repository/PersonRepository.h"
#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>

int main() {
    std::vector<SqlColumn> parameters;
    parameters.push_back({"id", "1", DataType::INT});
    parameters.push_back({"name", "aaa", DataType::STRING});

    PersonRepository personRepository("person");
//    personRepository.save(parameters);

//    SqlColumn whereClause = {"id", "2", INT};
//    personRepository.merge(parameters, whereClause);


//    std::vector<SqlColumn> vector = personRepository.findById(1);
//    for (const auto &item : vector) {
//        std::cout << item.key + "  " + item.value + "  " + std::to_string(item.type) << std::endl;
//    }


//    std::vector<std::vector<SqlColumn>> list = personRepository.findAll();
//    for (const auto &vector : list) {
//        for (const auto &item : vector) {
//            std::cout << item.key + "  " + item.value + "  " + std::to_string(item.type) << std::endl;
//        }
//        std::cout << "--------------------" << std::endl;
//    }

    bool exists = personRepository.existsById(1);
    std::cout << exists << std::endl;


//    personRepository.removeById(1);
//    personRepository.removeAll();

long count = personRepository.count();
    std::cout << count << std::endl;

    return 0;
}