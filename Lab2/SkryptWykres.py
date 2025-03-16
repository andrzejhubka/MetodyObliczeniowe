import math

import numpy as np
import matplotlib.pyplot as plt
import re

# Funkcja do wczytania danych z pliku
def load_data(filename):
    logx= []
    bladWzgledny= []

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
                bladWzgledny.append(float(parts[3]))

    return np.array(logx), np.array(bladWzgledny)


# Wczytanie danych z pliku
log10x, bladWzgledny= load_data('wyniki.txt')

# Tworzenie wykresu
plt.figure(figsize=(12, 8))
plt.plot(log10x, bladWzgledny, 'b-', linewidth=1.5)
#plt.scatter(log10_x, log_rel_error, s=10, c='red', alpha=0.5)

# Dodanie siatki
plt.grid(True, linestyle='--', alpha=0.7)

# Dodanie tytułu i etykiet osi
plt.title('Zależność logarytmu błędu względnego od logarytmu z x', fontsize=16)
plt.xlabel('log10(x)', fontsize=14)
plt.ylabel('log10(błąd względny)', fontsize=14)

# Dodanie linii poziomej dla błędu maszynowego (około 1e-16 dla double)

plt.axhline(y=math.log10(pow(1,-16)), color='g', linestyle='--', label='Błąd maszynowy (~1e-16)')
# Dodanie legendy
plt.legend(fontsize=12)

# Zapisanie wykresu do pliku
plt.savefig('blad_wzgledny.png', dpi=300, bbox_inches='tight')

# Wyświetlenie wykresu
plt.show()

print("Wykres został zapisany do pliku 'blad_wzgledny.png'")
