#include "app/repository/PersonRepository.h"

int main() {
        std::vector<SqlParameter> parameters;
        parameters.push_back({"id", "4", INT});
        parameters.push_back({"name", "aaa", STRING});

        PersonRepository personRepository("person");
        personRepository.save(parameters);

    SqlParameter whereClause = {"id","2",INT};
    personRepository.merge(parameters,whereClause);

    return 0;
}