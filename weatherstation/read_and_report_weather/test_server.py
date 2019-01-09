from server import WeatherServer

ws = WeatherServer("", 12345)

ds = "temperature:20,humidity:40"

data_dict = ws.data_stream_stripper(ds)

for i, j in data_dict.items():
    print(i, j)

