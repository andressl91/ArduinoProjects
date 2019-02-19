#include <iostream>
#include <string>
#include "sensor_collector.h"

void startServer() {
    SensorServer serv(1234);
    serv.startServerIO();
}

int main() {
    //startServer();    
    std::string db_path = "mydb.db";
    //SqliteConnection sql(db_path);
    //sql.openDB();
    //sql.closeDB();
    return 0;
}
