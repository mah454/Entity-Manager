//
// Created by mah454 on 8/15/23.
//

#include "Person.h"

int Person::getId() const {
    return id;
}

void Person::setId(int id) {
    Person::id = id;
}

int Person::getAge() const {
    return age;
}

void Person::setAge(int age) {
    Person::age = age;
}

const std::string &Person::getName() const {
    return name;
}

void Person::setName(const std::string &name) {
    Person::name = name;
}
