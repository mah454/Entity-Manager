#include "app/repository/PersonRepository.h"

int main() {

    std::vector<SqlParameter> parameters;
    parameters.push_back({"id", "1", INT});
    parameters.push_back({"name", "ali", STRING});

    PersonRepository *personRepository;
    personRepository->save(parameters, "person");

    return 0;
}