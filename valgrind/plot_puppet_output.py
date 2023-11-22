import sys
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv(
    sys.argv[1],
    header=None,
    names=["Time", "Kick In", "Snare In", "Kick Out", "Snare Out"],
    index_col=False
)
df['Snare In'] += 1.0
df['Snare Out'] += 1.0

print(df.head)
df_melt = df[(9.15 < df['Time']) & (df['Time'] < 9.22)].iloc[1:].melt("Time", var_name="cols", value_name="Signal")
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