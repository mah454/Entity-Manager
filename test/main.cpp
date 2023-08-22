
#include <vector>
#include "repository/PersonRepository.h"

int main() {
    std::vector<SqlCell> parameters;
    parameters.push_back({"name", "z2", DataType::STRING});
    parameters.push_back({"dtm", "2021-12-10 10:22:12.965", DataType::TIMESTAMP});

    PersonRepository personRepository("person");
    sql::Connection *conn = Database::getInstance().getConnection();
//    Database::txBegin(conn);
//    personRepository.save(parameters,conn);
//    Database::txCommit(conn);

//    SqlCell whereClause = {"id", "2", INT};
//    personRepository.merge(parameters, whereClause);


//    std::vector<SqlCell> vector = personRepository.findById(1);
//    for (const auto &item : vector) {
//        std::cout << item.key + "  " + item.value + "  " + std::to_string(item.type) << std::endl;
//    }


//    std::vector<std::vector<SqlCell>> list = personRepository.findAll();
//    for (const auto &vector : list) {
//        for (const auto &item : vector) {
//            std::cout << item.key + "  " + item.value + "  " + std::to_string(item.type) << std::endl;
//        }
//        std::cout << "--------------------" << std::endl;
//    }

//    bool exists = personRepository.existsById(1);
//    std::cout << exists << std::endl;


//    personRepository.removeById(1);
//    personRepository.removeAll();

//    long count = personRepository.count();
//    std::cout << count << std::endl;

    return 0;
}