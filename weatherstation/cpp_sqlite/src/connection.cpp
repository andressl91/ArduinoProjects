#include "connection.h"
#include <iostream>

Connection::Connection(std::string db_path) {
    db_path = db_path;    
}

void Connection::openDB() {
    
  //sqlite3 *DB;
 
  char *err_message = 0;

  int rc = sqlite3_open("student.db", &DB);

  if (rc != SQLITE_OK) {

    sqlite3_close(DB);
    //return 1;

  } else {
      std::cout <<  "Connected successfully \n";
  }
}

void Connection::closeDB() {
    sqlite3_close(DB);
}


void Connection::query(std::string sql) {


//    "DROP TABLE IF EXISTS STUDENTS;"
/*
    std::string sql = "CREATE TABLE PERSON("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "SURNAME          TEXT     NOT NULL, "
                      "AGE            INT     NOT NULL, "
                      "ADDRESS        CHAR(50), "
                      "SALARY         REAL );"; 
   
    */
    int exit = 0; 
    exit = sqlite3_open("example.db", &DB); 
    char* messaggeError; 
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); 
  
    if (exit != SQLITE_OK) { 
        std::cerr << "Error Create Table" << std::endl; 
        sqlite3_free(messaggeError); 
    } 
    else
        std::cout << "Table created Successfully" << std::endl; 
    sqlite3_close(DB); 

}
