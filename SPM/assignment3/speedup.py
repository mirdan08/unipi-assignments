import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv('bigfiles_task_results_averaged.csv')

# Get sequential compile time
seq_compile_time = df.loc[df['schedule_type'] == 'sequential', 'compile_time(s)'].values[0]

# Calculate compile speedup
df['compile_speedup'] = seq_compile_time / df['compile_time(s)']

# Ignore the sequential row
df = df[df['schedule_type'] != 'sequential']

# For each schedule_type and threads_number, pick the row with maximum compile_speedup
best_rows = df.loc[df.groupby(['schedule_type', 'threads_number'])['compile_speedup'].idxmax()]

# Reset index for clean display
best_rows = best_rows.reset_index(drop=True)

# Show result
print(best_rows)

# Plotting
plt.figure(figsize=(10, 6))

# Plot one line per schedule_type
for schedule in best_rows['schedule_type'].unique():
    subset = best_rows[best_rows['schedule_type'] == schedule]
    plt.plot(subset['threads_number'], subset['compile_speedup'], marker='o', label=schedule)

plt.xticks([1,2,4,8,16,32])

plt.xlabel('Number of Threads')
plt.ylabel('Compile Speedup')
plt.title('task based speedup results')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Save to file instead of showing
plt.savefig('bigfiles_task_speedup_plot.png')