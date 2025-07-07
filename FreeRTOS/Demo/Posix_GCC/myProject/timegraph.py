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

#df["Event Code"] = df["Event Code"].astype("category")
#fig = px.scatter(df, x="Time", y="Entity", color='Event Code', size=[0.5]*len(df))

df["Event Code"] = df["Event Code"].astype(str)
fig = px.scatter(df, y="Entity", x="Time", category_orders={"Entity": y_axis_order}, color="Event Code", symbol="Event Code")
fig.update_traces(marker_size=8)

#res_task_df = df[df["Entity"].str.startswith("ResTask")]
#groups = res_task_df.groupby(["Event Code", "Entity"])
#for _, group in groups:
#  if len(group) == 2:
#    item1 = group.iloc[0, :]
#    item2 = group.iloc[1, :]
#    fig.add_trace(go.Line(x=[item1[0], item2[0]], y=[item1["Entity"]]*2))
fig.show()