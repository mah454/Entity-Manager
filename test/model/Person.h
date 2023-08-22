//
// Created by mah454 on 8/15/23.
//

#ifndef ENTITY_MANAGER_PERSON_H
#define ENTITY_MANAGER_PERSON_H


#include <string>

class Person {
private:
    int id;
    int age;
    std::string name;
public:
    int getId() const;

    void setId(int id);

    int getAge() const;

    void setAge(int age);

    const std::string &getName() const;

    void setName(const std::string &name);
};


#endif //ENTITY_MANAGER_PERSON_H
