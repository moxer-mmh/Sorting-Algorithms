import matplotlib.pyplot as plt
import csv
import subprocess
import os


def create_time_plots():
    sizes = []
    times = {
        "BubbleSort": [],
        "BubbleSortOpt": [],
        "GnomeSort": [],
        "RadixSort": [],
        "QuickSort": [],
        "HeapSort": [],
    }

    # Read data from CSV
    with open("sorting_results.csv", "r") as file:
        csv_reader = next(csv.reader(file))  # Skip header
        for row in csv.reader(file):
            sizes.append(int(row[0]))
            for i, algo in enumerate(times.keys()):
                times[algo].append(float(row[i + 1]))

    # Create Graphs directory if it doesn't exist
    graphs_dir = "Graphs"
    if not os.path.exists(graphs_dir):
        os.makedirs(graphs_dir)

    # Create individual plots for each algorithm
    for algo_name, algo_times in times.items():
        plt.figure(figsize=(10, 6))
        plt.plot(sizes, algo_times, "o-", label=algo_name)
        plt.xlabel("Array Size (n)")
        plt.ylabel("Time (seconds)")
        plt.title(f"{algo_name} - Time Complexity")
        plt.grid(True)
        plt.legend()
        plt.savefig(os.path.join(graphs_dir, f"{algo_name.lower()}_analysis.png"))
        plt.close()

    # Create combined plot
    plt.figure(figsize=(12, 8))
    for algo_name, algo_times in times.items():
        plt.plot(sizes, algo_times, "o-", label=algo_name)

    plt.xlabel("Array Size (n)")
    plt.ylabel("Time (seconds)")
    plt.title("Sorting Algorithms - Time Complexity Comparison")
    plt.grid(True)
    plt.legend()
    plt.savefig(os.path.join(graphs_dir, "sorting_comparison.png"))
    plt.close()

    # Print average times for analysis
    print("\nAverage execution times (seconds):")
    for algo_name, algo_times in times.items():
        print(f"{algo_name}:")
        for size, time in zip(sizes, algo_times):
            print(f"  n={size}: {time:.6f}")


if __name__ == "__main__":
    if not os.path.exists("sorting_results.csv"):
        print("Compiling and running C program...")
        subprocess.run(["gcc", "sorting.c", "-o", "sorting", "-lm"])
        subprocess.run(["./sorting"])

    create_time_plots()
    print("\nGraphs have been generated successfully!")
    print("Check the 'Graphs' directory for PNG files:")
    print("- Individual algorithm analysis: bubblesort_analysis.png, etc.")
    print("- Combined comparison: sorting_comparison.png")
