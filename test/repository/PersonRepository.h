//
// Created by sheikhhosseini on 2023-08-19.
//

#ifndef ENTITY_MANAGER_PERSONREPOSITORY_H
#define ENTITY_MANAGER_PERSONREPOSITORY_H


#include "../../toolkit/Repository.h"

class PersonRepository: public Repository {
public:
    explicit PersonRepository(const std::string &tableName);
};


#endif //ENTITY_MANAGER_PERSONREPOSITORY_H
