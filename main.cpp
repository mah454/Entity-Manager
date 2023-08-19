
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

int main() {
// Set up MySQL Connector/C++ environment
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::mysql::MySQL_Connection *mysql_con;

    // Create a connection
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "username", "password");

    // Prepare the INSERT statement
    sql::PreparedStatement *stmt;
    stmt = con->prepareStatement("INSERT INTO your_table (column_name) VALUES (?)");

    // Set the parameter value
    stmt->setString(1, "Value 1");

    // Execute the INSERT statement
    stmt->executeUpdate();

    // Clean up resources
    delete stmt;
    delete con;

    return 0;
}