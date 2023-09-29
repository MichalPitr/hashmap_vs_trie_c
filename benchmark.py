import subprocess
import statistics
import pickle
from collections import defaultdict

def execute_command(command):
    result = subprocess.run(command, stdout=subprocess.PIPE)
    return float(result.stdout.strip())

def main():
    results = defaultdict(list)

    for size in [1, 2, 4, 8, 16, 32, 64, 128]:
        for _ in range(500):
            command = ["./a.out", f"code{size}.clox"]
            output = execute_command(command)
            results[size].append(output)

        print(f"size {size}")
        mean = statistics.mean(results[size])
        std_dev = statistics.stdev(results[size])
        print(f"Mean: {mean}")
        print(f"Standard Deviation: {std_dev}")
        print("")

    with open("results_switch.pkl", "wb") as file:
        pickle.dump(results, file)


    # Calculate mean and standard deviation

if __name__ == "__main__":
    main()