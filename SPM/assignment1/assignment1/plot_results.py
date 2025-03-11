import numpy as np
import matplotlib.pyplot as plt

# Data from the table
K_values = np.logspace(1, 9, num=9)
Plain = np.array([1.0508e-05, 9.6198e-06, 1.674240e-05, 8.708720e-05, 8.074710e-04, 7.907155e-03, 8.076488e-02, 8.079018e-01, 8.124380e+00])
AVX = np.array([2.158800e-05, 1.111880e-05, 3.366000e-06, 2.615320e-05, 1.499420e-04, 1.391732e-03, 1.896524e-02, 1.885668e-01, 1.896400e+00])
Auto = np.array([2.999080e-05, 2.180460e-05, 1.764060e-05, 2.900880e-05, 1.550350e-04, 1.457404e-03, 2.156016e-02, 2.171010e-01, 2.202150e+00])
Auto_speedup = np.array([0.487, 0.865, 4.97, 3.33, 5.39, 5.68, 4.26, 4.29, 4.28])
AVX_speedup = np.array([0.35, 0.44, 0.95, 3.00, 5.21, 5.43, 3.75, 3.72, 3.69])

# Plot execution times
plt.figure(figsize=(10, 6))
plt.plot(K_values, Plain, 'o-', label='Plain')
plt.plot(K_values, AVX, 's-', label='AVX')
plt.plot(K_values, Auto, '^-', label='Auto')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('K-Value')
plt.ylabel('Execution Time (s)')
plt.title('Execution Time vs K-Value')
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.show()

# Plot speedups (AVX)
plt.figure(figsize=(10, 6))
plt.plot(K_values, AVX_speedup, 's-', color='blue', label='AVX Speedup')
plt.plot(K_values, Auto_speedup, 's-', color='red', label='Auto Speedup')
plt.xscale('log')
plt.xlabel('K-Value')
plt.ylabel('Speedup Factor')
plt.title('AVX Speedup vs K-Value')
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend()
plt.show()