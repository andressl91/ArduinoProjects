#include "sensor_collector.h"

#include <thread>
#include <chrono>

SensorServer::SensorServer(int port) {
    listen_port = port;
    }

int SensorServer::getListenPort(){
    return listen_port;
}

void SensorServer::sendResponse(tcp::socket & socket, const std::string& message) {
    const std::string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}


bool SensorServer::isWhiteListed(tcp::socket & socket){
    std::string key = parseDataStream(socket);
    std::cout << "Unit connected with key: " << key << std::endl;
    std::set<std::string>::iterator it = whitelist.find(key);
    if (it != whitelist.end()) {
        std::cout << "In whitelist \n";
        return true;
    }
    return false;
}

void SensorServer::persistData(tcp::socket & socket){
    std::string datastream = parseDataStream(socket);
    std::cout << "Recieved data: " << datastream << std::endl;

}

std::string SensorServer::parseDataStream(tcp::socket & socket){
        
        // Set size of streambuf more explicit
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n" );

        std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        // Remove end line char from data stream
        data.erase (data.end()-1);

        return data;
}

void SensorServer::startServerIO(){
    io_service ios_;
    tcp::endpoint tcp_endpoint{tcp::v4(), listen_port};
    tcp::acceptor tcp_acceptor{ios_, tcp_endpoint};

        for(;;) {
            tcp::socket tcp_socket(ios_);
            tcp_acceptor.accept(tcp_socket);
        //For testing respone time      
        //std::this_thread::sleep_for (std::chrono::seconds(2));
            
            if(isWhiteListed(tcp_socket)){
                
                persistData(tcp_socket);
                std::string response = "Got the stuff";
                std::cout << "Sending response" << std::endl;
                sendResponse(tcp_socket, response);

            }
            else {
                std::cout << "NOT IN  whitelist \n";
            }
             
            //std::string message = parseDataStream(tcp_socket);
            //std::cout << "Client sent: " << message << std::endl;


        }
}
