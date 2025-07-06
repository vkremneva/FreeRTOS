#!/usr/bin/python3

import pandas as pd
import plotly.express as px
import plotly.graph_objects as go

df = pd.read_csv('./myProject/log.csv')
df["Event Code"] = df["Event Code"].astype("category")
fig = px.scatter(df, x="Time", y="Entity", color='Event Code', size=[20]*len(df))

res_task_df = df[df["Entity"].str.startswith("ResTask")]
groups = res_task_df.groupby(["Event Code", "Entity"])
for _, group in groups:
  item1 = group.iloc[0, :]
  item2 = group.iloc[1, :]
  fig.add_trace(go.Line(x=[item1[0], item2[0]], y=[item1["Entity"]]*2))
fig.show()