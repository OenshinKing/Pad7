import numpy as np

data = [39.8,65.9,259,3.24,93.4,120,64.2,114,73.5,153,88.1,72.2,81.7,30.6,43.3,22.6]

data = np.array(data)

mean1 = data.mean()
print(f"Algeber average:\t{mean1:.2f}")
mean2 = np.exp(np.log(data).mean())
print(f"Geometry average:\t{mean2:.2f}")
median = np.median(data)
print(f"Median:\t\t\t{median:.2f}")
Range = data.max() - data.min()
print(f"Range:\t\t\t{Range:.2f}")
MD = np.abs(data-data.mean()).mean()
print(f"Mean deviation:\t\t{MD:.2f}")
var = np.var(data,ddof=1)
print(f"Variation:\t\t{var:.2f}")
std = np.std(data,ddof=1)
print(f"Standard variation:\t{std:.2f}")
