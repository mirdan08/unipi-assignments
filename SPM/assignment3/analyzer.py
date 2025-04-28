import pandas as pd

# Load the CSV
filename = "bigfiles_task_results.csv"  # Change this to your file path
df = pd.read_csv(filename)

# Group by schedule_type, threads_number, block_sizes(bytes)
grouped = df.drop(columns=["iteration"]).groupby(["schedule_type", "threads_number", "block_sizes(bytes)"])


grouped_mean= grouped.mean()
grouped_dev= grouped.std()

# Step 1: Concatenate them side-by-side
combined = pd.concat([grouped_mean, grouped_dev], axis=1)

# Step 2: Reorder the columns in pairwise order
new_cols = []
for col in grouped_dev.columns:
    new_cols.append(col)       # column from df1
    new_cols.append(col + '_std') # column from df2

# Step 3: Rename second table's columns to avoid duplicate names
grouped_mean.columns = [col + '_std' for col in grouped_mean.columns]

# Step 4: Concatenate again properly
combined = pd.concat([grouped_mean, grouped_dev], axis=1)[new_cols]

combined.to_csv("bigfiles_task_results_averaged.csv")

