#include "app/repository/PersonRepository.h"
#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>

int main() {
    std::vector<SqlColumn> parameters;
    parameters.push_back({"id", "1", sql::DataType::INTEGER});
    parameters.push_back({"name", "aaa", sql::DataType::VARCHAR});

    PersonRepository personRepository("person");
//    personRepository.save(parameters);

//    SqlColumn whereClause = {"id", "2", INT};
//    personRepository.merge(parameters, whereClause);


    std::vector<SqlColumn> vector = personRepository.findById(1);
    for (const auto &item : vector) {
        std::cout << item.key + "  " + item.value + "  " + std::to_string(item.type) << std::endl;
    }


    return 0;
}