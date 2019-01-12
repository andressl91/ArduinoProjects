import socket
import time
import logging
from typing import List, Dict
from pathlib import Path



import shyft.api as sa

from squealer.sql_table_tools import DataTableTools
from squealer.sqlite_session import SqliteSession

# Set log to show in terminal
logging.basicConfig(level=logging.INFO)


def get_white_list():
    """White list servers"""
    return ["S1"]


class WeatherServer:
    """Simple weatherserver for recieving weatherdata from Arduino"""

    def __init__(self, ip_address: str, port: int, db_path: str):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._whitelist = get_white_list()
        # TODO: IMPLEMENT
        self._logger = logging.getLogger("ServerMonitor")
        self._db_connection = SqliteSession(db_path=db_path)
        self._db_tool = DataTableTools(sql_session=self._db_connection)
        self._sql_table = None
        self._table_name = "dht22"
        self._nor_cal = sa.Calendar("Europe/Oslo")


    @property
    def t_now(self):
        return int(sa.utctime_now())

    def _start_server(self):
        # TODO: Update squealer for better handling of existing tables
        # TODO: squealer connection must activate connection when getting
        # cursor, if not existing.
        # TODO: Should be handlet automatic in db_tools, pass path to db in
        # _db_tools
        self._db_connection.connect()
        if not self._db_tool._does_table_exist(sql_ses=self._db_connection, 
                                               table_name=self._table_name):
            categories = {"time": "REAL", "temperature": "REAL", "humidity":
                          "REAL", "status": "INTEGER"}
            self._db_tool.create_table(table_name=self._table_name,
                                       categories=categories)

        self._sql_table = self._db_tool.tables[self._table_name]

        server_address = (self.ip_address, self.port)
        self.sock.bind(server_address)
        self.sock.listen(1)

    def write_data_to_sql(self, sensor_data: Dict[str, str]):
        sensor_data["time"] = self.t_now
        print(sensor_data)
        self._sql_table.write(sensor_data)

    def _stop_server(self):
        # Close the socket. It cannot be used after this call.
        self.sock.close()

    def listen_for_sensors(self) -> socket.socket:
        """ Returns a socket obj for each connection"""
        connection, client_address = self.sock.accept()
        self._logger.info(f"Incoming connection from {client_address}")
        return connection

    def data_stream_stripper(self, data_stream: List):
        """Assumes ',' separated stream with type:value"""
        sensor_data = {}
        try:
            data = data_stream.split(",")
            for sensor in data:
                data_type, data_value = sensor.split(":")
                data_type = data_type.strip(" ").lower()
                data_value = str(data_value).strip(" ")
                sensor_data[data_type] = int(data_value)

        except RuntimeError:
            return False
        return sensor_data
    
    def store_sensor_data(self):
        pass

    def get_sensor_data(self, connection: socket.socket):
        """Recieve sensor TCP/IP message with data"""
        max_wait = 3.0
        sensor_read_time = 0
        data = ""
        
        transmission_start = time.time()
        #while data.decode('utf-8') != "End" and max_wait > sensor_read_time:
        while max_wait > sensor_read_time and data == "":
            data = connection.recv(2**6)
            data = str(data, encoding='utf-8')
            sensor_read_time = time.time() - transmission_start
        
        self._logger.info((f"Sendt in {data} to stripper"))
        sensor_data = self.data_stream_stripper(data)
        status = f"""Status {sensor_data["status"]}"""

        if sensor_data["status"] == 0:
            print(sensor_data)
            self._logger.info((f"Complete sensor data protocol"))

        else:
            self._logger.info((f"Sensor data disturbed"))

        connection.send(str.encode(status))
        self.write_data_to_sql(sensor_data)

    def connection_handler(self, connection: socket.socket):
        tag = connection.recv(2**4).decode('utf-8')
        if tag in self._whitelist:
            self._logger.info(f"Connection is in whitelist with tag {tag}")
            self.get_sensor_data(connection)
        else:
            self._logger.info(f"Connection didn't make valid entry with {tag}, closing connection.")
            connection.close()
    #TODO: Close connections
    def __call__(self):
        self._logger.info("Starting Server")
        self._start_server()
        self._logger.info("Listning for incomming connections.")

        while True:
            connection = self.listen_for_sensors()
            self.connection_handler(connection)


if __name__ == "__main__":
    db_path = Path("data/dht22_sensor.db")
    port = 10000
    server_address = ('localhost', port)
    
    # server_address = ('192.168.62.120', port)
    sensor_server = WeatherServer(*server_address, db_path)
    sensor_server()
