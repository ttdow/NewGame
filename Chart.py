import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

data = pd.read_csv("points.csv")

x = data['x'].to_numpy()
z = data['z'].to_numpy()

plt.scatter(x=x, y=z)
plt.show()