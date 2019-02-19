#ifndef SENSOR_COLLECTOR
#define SENSOR_COLLECTOR

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <set>


using namespace boost::asio;
using namespace boost::asio::ip;


class SensorServer{
    private:
    unsigned int listen_port;
    std::set<std::string> whitelist = {"S2" ,"S1"};



    public:
    SensorServer(int port);
    void listenForSensors();
    void recieveSensorData(tcp::socket &socket);
    bool isWhiteListed(tcp::socket &socket);
    void persistData(tcp::socket &socket);
    std::string parseDataStream(tcp::socket &socket);
    int getListenPort();
    void startServerIO();
    void sendResponse(tcp::socket & socket, const std::string& message);

};


#endif

