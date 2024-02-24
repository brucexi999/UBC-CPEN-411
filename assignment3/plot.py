import matplotlib.pyplot as plt
import numpy as np

metrics = ("Cumulative IPC", "L1D Miss Rate", "L2C Miss Rate", "LLC Miss Rate", "Branch Prediction Accuracy")

data = {
    "matmul_basic": (0.77, 0.085, 0.463, 0.044, 0.992),
    "matmul_opt1": (1.55, 0.014, 0.502, 0.359, 0.992),
    "matmul_opt2": (1.85, 0.00206, 0.0537, 0, 0.992)
}

x = np.arange(len(metrics))  # the label locations
width = 0.25  # the width of the bars
multiplier = 0

fig, ax = plt.subplots(layout='constrained')

for attribute, measurement in data.items():
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=attribute)
    ax.bar_label(rects, padding=3)
    multiplier += 1

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Data')
ax.set_xticks(x + width, metrics)
ax.legend(loc='upper left', ncols=3)
ax.set_ylim(0, 2)

plt.show()
