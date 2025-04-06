import pandas as pd
import matplotlib.pyplot as plt

# Load CSV file
df = pd.read_csv('results.csv')

# Group by chunk size and number of threads, then compute mean and variance across iterations
grouped = df.groupby(['chunk size', 'num threads'])

# Calculate mean and variance for all scheduling methods
stats = grouped.agg(['mean', 'var'])

stats=stats.drop(columns=['iteration'])

#print(stats)

# Flatten MultiIndex columns (rename columns to "method_stat")
stats.columns = ['_'.join(col).strip() for col in stats.columns]

# Extract mean execution times for speedup calculations
mean_exec_time = stats.filter(like='_mean')

# Avoid division by zero or NaN errors when computing speedup
speedup = pd.DataFrame()
speedup['speedup_dynamic'] = mean_exec_time['sequential_mean'] / mean_exec_time['dynamic_mean'].replace(0, float('nan'))
speedup['speedup_static_block_cyclic'] = mean_exec_time['sequential_mean'] / mean_exec_time['static block cyclic_mean'].replace(0, float('nan'))

plot_speedup=speedup[['dynamic','static block cyclic']]=speedup[['speedup_dynamic','speedup_static_block_cyclic']]
plot_speedup.plot(kind='bar',figsize=(10,5))
print(plot_speedup)
plt.title("speedup comparison")
plt.ylabel('speedup')
plt.xticks(rotation=45)
plt.show()

plot_stats=pd.DataFrame()

plot_stats[['dynamic','static block cyclic']]=stats[['dynamic_mean','static block cyclic_mean']]

plot_stats.plot(kind='bar',figsize=(10,5))
print(plot_stats)
plt.title("times comparison")
plt.ylabel('time in seconds')
plt.xticks(rotation=45)
plt.show()
