import sys
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv(
    sys.argv[1],
    header=None,
    names=["Time", "Kick In", "Snare In", "Kick Out","Kick Schmidt", "Snare Out", "Snare Schmidt"],
    index_col=False
)
df['Snare In'] += 1.0
df['Snare Out'] += 1.0
df["Snare Schmidt"] += 1.0
 
print(df.head)

if len(sys.argv) < 3:
    stch = None
else:
    stch = sys.argv[2]
if stch == "b":
    df_melt = df[(9.75 < df['Time']) & (df['Time'] < 11)].iloc[1:].melt("Time", var_name="cols", value_name="Signal")
elif stch == "k":
    df_melt = df[(9.8 < df['Time']) & (df['Time'] < 10.05)].iloc[1:].melt("Time", var_name="cols", value_name="Signal")
else:
    df_melt = df.iloc[1:].melt("Time", var_name="cols", value_name="Signal")

print("df melted")
ax = sns.lineplot(
    data=df_melt,
    x="Time",
    y="Signal",
    hue="cols",
    errorbar=None
)
print("showing")

plt.show()