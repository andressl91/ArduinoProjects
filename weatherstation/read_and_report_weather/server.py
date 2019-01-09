import socket
import time
import logging

# Set log to show in terminal
logging.basicConfig(level=logging.INFO)


def get_white_list():
    """White list servers"""
    return ["S1"]


class WeatherServer:

    def __init__(self, ip_address: str, port: int):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._whitelist = get_white_list()
        self._logger = logging.getLogger("ServerMonitor")

    def _start_server(self):
        server_address = (self.ip_address, self.port)
        self.sock.bind(server_address)
        self.sock.listen(1)

    def _stop_server(self):
        # Close the socket. It cannot be used after this call.
        self.sock.close()

    def listen_for_sensors(self) -> socket.socket:
        """ Returns a socket obj for each connection"""
        connection, client_address = self.sock.accept()
        self._logger.info(f"Incoming connection from {client_address}")
        return connection

    def data_stream_stripper(self, data_stream: str):
        """Assumes ',' separated stream with type:value"""
        sensor_data = {}
        try:
            # data_stream = str(data_stream)
            print("data_stripper", data_stream)
            for data in data_stream.split(","):
                data_type = data.split(":")[0]
                data_value = data.split(":")[1]
                sensor_data[data_type] = float(data_value)
        except RuntimeError:
            return False
        return sensor_data

    def get_sensor_data(self, connection: socket.socket):
        """Recieve sensor TCP/IP message with data"""
        max_wait = 3.0
        sensor_read_time = 0
        data = ""
        
        transmission_start = time.time()
        #while data.decode('utf-8') != "End" and max_wait > sensor_read_time:
        while max_wait > sensor_read_time:
            data = connection.recv(2**4)
            data = str(data, encoding='utf-8')
            if data == "End":
                break
            sensor_read_time = time.time() - transmission_start
            self.data_stream_stripper(data)

    def connection_handler(self, connection: socket.socket):
        tag = connection.recv(2**4).decode('utf-8')
        if tag in self._whitelist:
            self._logger.info(f"Connection is in whitelist with tag {tag}")
            self.get_sensor_data(connection)
        else:
            self._logger.info(f"Connection didn't make valid entry with {tag}, closing connection.")
            connection.close()

    def __call__(self):
        self._logger.info("Starting Server")
        self._start_server()
        self._logger.info("Listning for incomming connections.")
        while True:
            connection = self.listen_for_sensors()
            self.connection_handler(connection)


if __name__ == "__main__":

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    port = 10000
    server_address = ('192.168.62.120', port)
    sensor_server = WeatherServer(*server_address)
    sensor_server()
