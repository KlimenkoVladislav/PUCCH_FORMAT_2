import matplotlib.pyplot as plt
import numpy as np
import sys
import os

INPUT_FILE = "../output.txt"

COLORS = {
    2:  "red",
    4:  "blue",
    6:  "green",
    8:  "orange",
    11: "purple",
}

MARKERS = {
    2:  "o",
    4:  "s",
    6:  "^",
    8:  "D",
    11: "v",
}

def read_data(filename):
    if not os.path.exists(filename):
        print(f"Ошибка: файл {filename} не найден", file=sys.stderr)
        sys.exit(1)
    
    data = {2: ([], []), 4: ([], []), 6: ([], []), 8: ([], []), 11: ([], [])}
    
    with open(filename, "r") as f:
        for line_num, line in enumerate(f, start=1):
            line = line.strip()
            if not line:
                continue
            
            parts = line.split()
            if len(parts) < 4:
                print(f"Предупреждение: строка {line_num} пропущена (мало полей)", file=sys.stderr)
                continue
            
            try:
                snr = float(parts[0])
                n = int(parts[1])
                bler = float(parts[3])
            except ValueError:
                print(f"Предупреждение: строка {line_num} пропущена (ошибка парсинга)", file=sys.stderr)
                continue
            
            if n not in data:
                print(f"Предупреждение: неизвестное n={n} в строке {line_num}", file=sys.stderr)
                continue
            
            data[n][0].append(snr)
            data[n][1].append(bler)
    
    result = {}
    for n in data:
        snr_list, bler_list = data[n]
        if len(snr_list) == 0:
            result[n] = (None, None)
            continue
        
        snr_arr = np.array(snr_list)
        bler_arr = np.array(bler_list)
        
        idx = np.argsort(snr_arr)
        result[n] = (snr_arr[idx], bler_arr[idx])
    
    return result

def plot_bler(data):
    plt.figure(figsize=(10, 7))
    
    for n in sorted(data.keys()):
        snr, bler = data[n]
        
        if snr is None or len(snr) == 0:
            print(f"Предупреждение: нет данных для n={n}", file=sys.stderr)
            continue
        
        plt.plot(
            snr, bler,
            color=COLORS.get(n, "black"),
            marker=MARKERS.get(n, "o"),
            markersize=4,
            markevery=max(1, len(snr) // 20),
            linewidth=1.5,
            label=f"n = {n}"
        )
    
    plt.xlabel("SNR (дБ)", fontsize=12)
    plt.ylabel("BLER", fontsize=12)
    plt.title("BLER vs SNR для PUCCH Format 2", fontsize=14)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend(fontsize=11)
    plt.xlim(-10, 30)
    plt.ylim(0.0, 1.0)
    
    plt.tight_layout()
    plt.show()

def main():
    data = read_data(INPUT_FILE)
    plot_bler(data)

if __name__ == "__main__":
    main()