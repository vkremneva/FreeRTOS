#!/usr/bin/python3

import pandas as pd
import plotly.express as px

df = pd.read_csv('./myProject/log.csv')
df["Event Code"] = df["Event Code"].astype("category")
fig = px.scatter(df, x="Time", y="Entity", color='Event Code', size=[20]*len(df))
fig.show()