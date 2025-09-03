#!/usr/bin/python3

import pandas as pd
import plotly.express as px
import plotly.graph_objects as go

df = pd.read_csv('./myProject/log.csv')

# custom order for y axis
y_axis_order = ["Event Generator Task", "Dispatcher Task", "Police", "Ambulance", "Firefighters", "Corona"]
res_names = [item for item in df["Entity"].unique() if item.startswith("Res")]
res_names.sort()
y_axis_order.extend(res_names)

# map event codes to "code - name"
event_labels = {
    "1": "1 - Police",
    "2": "2 - Ambulance",
    "3": "3 - Firefighters",
    "4": "4 - Corona"
}
df["Event Code"] = df["Event Code"].astype(str)
df["Event Name"] = df["Event Code"].map(event_labels).fillna(df["Event Code"])

# explicit order for legend
event_order = ["1 - Police", "2 - Ambulance", "3 - Firefighters", "4 - Corona"]

fig = px.scatter(
    df,
    y="Entity",
    x="Time",
    category_orders={
        "Entity": y_axis_order,
        "Event Name": event_order
    },
    color="Event Name",
    symbol="Event Name"
)

fig.update_traces(marker_size=8)
fig.show()