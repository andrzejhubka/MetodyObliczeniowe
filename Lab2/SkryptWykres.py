import math
import numpy as np
import matplotlib.pyplot as plt
import re
from matplotlib.backend_bases import DrawEvent


# Funkcja do wczytania danych z pliku
def load_data(filename):
    logx= []
    bladWzglednyF1= []
    bladWzglednyF2= []

    with open(filename, 'r') as file:
        # Pomijamy nagłówki (pierwsze 3 linie)
        for _ in range(3):
            next(file)

        for line in file:
            line = line.strip()
            if not line or line.startswith('='):
                continue

            # Podział linii na kolumny
            parts = re.split(r'\s+', line)
            if len(parts) >= 3:
                logx.append(float(parts[0]))
                bladWzglednyF1.append(float(parts[4]))
                bladWzglednyF2.append(float(parts[6]))

    return np.array(logx), np.array(bladWzglednyF1), np.array(bladWzglednyF2)

def draw_plot(x, y, filename, title, xlab, ylab):
    # Tworzenie wykresu
    plt.figure(figsize=(12, 8))
    plt.plot(x, y, 'b-', linewidth=1.5)

    # Dodanie siatki
    plt.grid(True, linestyle='--', alpha=0.7)

    # Dodanie tytułu i etykiet osi
    plt.title(title, fontsize=16)
    plt.xlabel(xlab, fontsize=14)
    plt.ylabel(ylab, fontsize=14)

    # Dodanie linii poziomej dla błędu maszynowego (około 1e-16 dla double)
    plt.axhline(y=math.log(pow(10,-16), 10), color='g', linestyle='--', label='Log10(~1e-16)')
    # Dodanie legendy
    plt.legend(fontsize=12)

    # Zapisanie wykresu do pliku
    plt.savefig(filename, dpi=300, bbox_inches='tight')

    # Wyświetlenie wykresu
    plt.show()

    print(f"Wykres został zapisany do pliku {filename}")


# Wczytanie danych z pliku
log10x, bladWzglednyF1, bladWzglednyF2 = load_data('wyniki.txt')

# Rysuj wykresy
draw_plot(log10x, bladWzglednyF1, "wykresF1.jpg",title="Zaleznosc bledu wzglednego E wyników funkcji F1 od x", xlab="log10(x)", ylab="log10(E)")
draw_plot(log10x, bladWzglednyF2, "wykresF2.jpg", title="Zaleznosc bledu wzglednego E wyników funkcji F2 od x", xlab="log10(x)", ylab="log10(E)")

