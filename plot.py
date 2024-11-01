import matplotlib.pyplot as plt
import numpy as np

# Data
labels = [
    'L1-dcache-loads', 'L1-dcache-load-misses', 'L1-dcache-stores',
    'LLC-loads', 'LLC-load-misses', 'LLC-stores', 'LLC-store-misses'
]

first_set = [
    207346260, 18587362, 137857784, 
    7198589, 1254792, 3372797, 1876987
]

second_set = [
    45933034, 4816331, 32028448, 
    2397312, 210888, 571225, 105171
]

x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars

fig, ax = plt.subplots()
bars1 = ax.bar(x - width/2, first_set, width, label='Without Hugepages')
bars2 = ax.bar(x + width/2, second_set, width, label='With Hugepages')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Counts')
ax.set_title('Performance Counter Stats Comparison')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()

# Adding value labels on the bars
for bar in bars1 + bars2:
    yval = bar.get_height()
    ax.text(bar.get_x() + bar.get_width()/2, yval, int(yval), ha='center', va='bottom')

plt.xticks(rotation=45)
plt.tight_layout()
plt.show()
