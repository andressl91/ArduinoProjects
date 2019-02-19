#include <iostream>
#include "connection.h"



int main() {
   std::string name = "student.db";
   Connection con(name);

//    "DROP TABLE IF EXISTS STUDENTS;"
    std::string sql = "DROP TABLE IF EXISTS PERSON;"
                      "CREATE TABLE PERSON("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "SURNAME          TEXT     NOT NULL, "
                      "AGE            INT     NOT NULL, "
                      "ADDRESS        CHAR(50), "
                      "SALARY         REAL );"; 
   con.query(sql);

    return 0;
}
