
#include <vector>
#include "repository/PersonRepository.h"
#include <syslog.h>

int main() {

    openlog("entity-manager",LOG_PID,LOG_LOCAL0);

    std::cout << getenv("MYSQL_CONNECTION_URL") << std::endl;

    auto *configuration = new Configuration();
    configuration->setConnectionUrl("tcp://172.17.0.2:3306/sample");
    configuration->setUsername("root");
    configuration->setPassword("rootpass");
    configuration->setPoolSize(5);

    Database::getInstance().setConfiguration(*configuration);
    Database::getInstance().initPool();

    sql::Connection *conn = Database::getInstance().getConnection();

    std::vector<SqlCell> parameters;
    parameters.push_back({"name", "z2", DataType::STRING});
    parameters.push_back({"fami22ly", "aaaa", DataType::STRING});

    PersonRepository personRepository("person");


    Database::txBegin(conn);
    int status = personRepository.save(parameters, conn);
    Database::txCommit(conn);


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