from scipy.fft import fft, ifft
import numpy as np
from pandas import DataFrame
import seaborn as sns
import matplotlib.pyplot as plt
import random 

last_kick_i = -100


def kick_mod(i):
    bin_scale = 1000
    if (
        i - last_kick_i > bin_scale
        and (
            (i % 7 * bin_scale == 0) 
            or (
                (i % 7 * bin_scale)
                % 5 * bin_scale == 4
            )
        )
    ):
        return 1.0
    return 0

def get_data():
    return np.array([
        1.0 if kick_mod(i + random.randint(0, 3)) else 0.0
        for i in range(50000) 
    ])

def get_peaks(data):
    out = []
    d_prev = data[0]
    d_prev2 = data[0]
    on = False
    for i, d in enumerate(data):
 
        if  d_prev2 < d_prev and d_prev > d:
            out.append(i-1)
        d_prev2 = d_prev
        d_prev = d
 
    return out

din = get_data()
y = fft(din)

print(din)
#print([e for e in np.abs(y)])

df = DataFrame(
    {
        'fft': np.abs(y),
        #'din': din
    }
).reset_index()

sns.pointplot(
    data=df.melt("index", var_name="signa", value_name="Signal"),
    x=df.index,
    y="Signal",
    errorbar=None
)
pks = get_peaks(np.abs(y))
print([l-e for e, l in zip(pks[:-1], pks[1:])])
# for e in pks:
#     print(e)

for i, e in enumerate(din):
    if e > 0:
        print (i, e)

plt.show()