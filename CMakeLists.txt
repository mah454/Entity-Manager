cmake_minimum_required(VERSION 3.0)

project(entity-manager)
SET(CMAKE_BUILD_TYPE Debug)
add_executable(entity-manager
        main.cpp
        app/model/Person.cpp
        toolkit/Configuration.cpp
        toolkit/Database.cpp
        toolkit/Repository.cpp
        app/repository/PersonRepository.cpp toolkit/SqlColumn.cpp)

target_link_libraries(entity-manager PRIVATE fmt mysqlcppconn)

install(TARGETS entity-manager RUNTIME DESTINATION bin)
