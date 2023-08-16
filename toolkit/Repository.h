//
// Created by sheikhhosseini on 2023-08-16.
//

#ifndef ENTITY_MANAGER_REPOSITORY_H
#define ENTITY_MANAGER_REPOSITORY_H

#include <list>

template<typename ENTITY>
class Repository {
    void save(ENTITY *e);

    void saveAll(std::list<ENTITY> *eList);

    ENTITY findById(long id);

    std::list<ENTITY> findAll();

    bool existsById(long id);

    long count();

    void removeById(long id);

    void remove(ENTITY *e);

    void removeAll();

    void removeAll(std::list<ENTITY> *eList);

    void removeAllById(std::list<long> *eList);
};

#endif //ENTITY_MANAGER_REPOSITORY_H
