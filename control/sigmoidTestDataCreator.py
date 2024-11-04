import numpy as np
import matplotlib.pyplot as plt
import os

from scipy.optimize import curve_fit

def sigmoid(x, L, k, x0, C):
    return L / (1 + np.exp(-k * (x - x0))) + C

noiseAmplitude = 10
numberOfDatapoints = 1001
time_s = 10

# noise generation
# Generate Gaussian noise
noise = np.random.normal(0, 1, numberOfDatapoints)

# Normalize to the range [-1, 1]
noise = 2 * (noise - np.min(noise)) / (np.max(noise) - np.min(noise)) - 1



x = np.linspace(0,time_s,numberOfDatapoints)

y = []


for xx, nn in zip(x, noise):
    y.append(sigmoid(xx,100,2,4,10) + noiseAmplitude * nn)

########


# Example x_data and y_data (replace with your actual data)
x_data = x  # Your x data
y_data = y  # Your y data

# Initial guesses for L, k, x0, and C
initial_guess = [max(y_data), 1, np.median(x_data), min(y_data)]

# Fit the curve
params, _ = curve_fit(sigmoid, x_data, y_data, p0=initial_guess)

# Extract the fitted parameters
L, k, x0, C = params
print(f"Fitted parameters: L={L}, k={k}, x0={x0}, C={C}")

fitted_y = []

for xx in x:
    fitted_y.append(sigmoid(xx,L,k,x0,C))




plt.plot(x,y)
plt.plot(x,fitted_y)
plt.plot(x0, L/2 + C, 'go')
plt.axvline(x0, color = 'g', label = 'axvline - full height')
plt.annotate("L = "  + str(np.round(L,3)),[8,10])
plt.annotate("k = "  + str(np.round(k,3)),[8,15])
plt.annotate("x0 = "  + str(np.round(x0,3)),[8,20])
plt.annotate("C = "  + str(np.round(C,3)),[8,25])
plt.show()

#input("Press Enter to continue...\n\r")




