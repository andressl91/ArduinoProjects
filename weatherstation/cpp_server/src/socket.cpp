#include <iostream>
#include <string>
#include "sensor_collector.h"


int main() {
    SensorServer serv(1234);
    serv.startServerIO();
    std::string db_path = "mydb.db";
    return 0;
}
