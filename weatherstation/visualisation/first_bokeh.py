from pathlib import Path

from bokeh.plotting import figure, show, output_file
from bokeh.layouts import row
from bokeh.io import curdoc


from squealer.sql_table_tools import DataTableTools


sqlite_db_path = Path.cwd().parent / "data" / "dht11_sensor.db"

db_tool = DataTableTools(db_path=sqlite_db_path)
dht11_table = db_tool.tables["dht11"]
res = dht11_table.select(["time"])    
print(res)


x = [1, 2, 3]
y = [3, 4, 5]

p = figure(title="simple")

p.line(x, y, legend="Temp", line_width=2)

output_file("simple.html", title="simple first bokeh")
#curdoc().add_root(row(p))
#show(p)
