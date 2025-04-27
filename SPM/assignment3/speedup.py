import pandas as pd

# Load CSV
df = pd.read_csv('smallfiles_results_averaged.csv')

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
