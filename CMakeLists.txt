cmake_minimum_required(VERSION 3.0)

project(entity-manager)

add_executable(entity-manager main.cpp toolkit/MySqlDatabase.cpp toolkit/MySqlDatabase.h app/Person.cpp app/Person.h app/PersonRepository.cpp app/PersonRepository.h toolkit/Repository.cpp toolkit/Repository.h)

target_link_libraries(entity-manager mysqlcppconn)

install(TARGETS entity-manager RUNTIME DESTINATION bin)
