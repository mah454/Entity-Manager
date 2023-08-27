cmake_minimum_required(VERSION 3.0)

project(entity-manager)
SET(CMAKE_BUILD_TYPE Debug)
add_library(entity-manager
        toolkit/Configuration.cpp
        toolkit/Database.cpp
        toolkit/Repository.cpp
        toolkit/SQL.cpp)

add_executable(test
        test/main.cpp
        test/repository/PersonRepository.cpp
        test/model/Person.cpp
        test/EnittyManagerTest.cpp)

target_link_libraries(entity-manager PRIVATE fmt mysqlcppconn)
target_link_libraries(test PRIVATE entity-manager gtest)

install(TARGETS entity-manager RUNTIME DESTINATION bin)
