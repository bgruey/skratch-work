"""
    Scratch work sorting out how to look at input data from the drum trigger--experiments with a mic first.

    Largely, a low pass filter feels pretty good.
"""

import librosa
import sys
import struct
import seaborn as sns
import matplotlib.pyplot as plt

audio_in = sys.argv[1]
data, sample_rate = librosa.load(audio_in, sr=5000)

dt = 1.0 / sample_rate

min_t = -5.3
max_t = 99999999999999996.3
avg_window = 50e-3  # seconds of averaging
n_samples = int(avg_window * sample_rate)
avg_factor = 1.0 / n_samples
avg_samples = list(range(n_samples))
sample_i = 0
drop_i = 0
start = False
low_pass_f = 25  # Hz, 50 seemed better
alpha = low_pass_f * 2 * 3.14 * dt / (1 + low_pass_f * 2 * 3.14 * dt)


def low_pass(x_i, y_im1, alpha):
    return alpha * x_i + (1.0 - alpha) * y_im1


print("Num pts", len(data), sample_rate)
t = 0.0


def get_adc(fval):
    return abs(fval)


ofilename = "waveforms.dat" if len(sys.argv) < 2 else sys.argv[2]
with open(ofilename, "wb") as f:
    f.write(struct.pack("i", sample_rate))
    for d in data:
        # print(d, get_adc(d))
        f.write(struct.pack("d", get_adc(d)))

ld = len(data)
#sns.pointplot(data=data[int(ld*0.715):int(ld*0.725)])
#sns.pointplot(data=[get_adc(e) for e in data[int(ld*0.275):int(ld*0.285)]])
#sns.lineplot(data=data)
plt.show()

sys.exit()


with open(sys.argv[1] + ".csv", "w") as f:

    t = 0.0
    rc_y = 0.0
    avg_sum = 0.0
    for l in data:
        labs = abs(l)
        avg_sum += labs
        avg_samples[sample_i] = labs

        new_sample_i = (sample_i + 1) % n_samples
        if sample_i > new_sample_i:
            start = True            

        sample_i = new_sample_i
        if not start:
            rc_y = alpha * labs
            continue
        rc_y = low_pass(labs, rc_y, alpha)
        avg = avg_sum * avg_factor
        avg_sum -= avg_samples[drop_i]
        drop_i = (drop_i + 1) % n_samples

        if t > min_t:
            #f.write(f"{t},{l},{labs},{avg},{rc_y}\n")
            f.write(f"{t},{l}\n")
            #f.write(f"{t},{labs},{rc_y}\n")
        if t > max_t:
            break

        t += dt