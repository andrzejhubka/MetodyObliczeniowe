import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Wczytaj dane
df = pd.read_csv('wyniki.csv')

# Grupuj po dt i wyciągnij maksymalny błąd dla każdej metody
grouped = df.groupby('dt').agg({
    'BME_blad': 'max',
    'PME_blad': 'max',
    'PMT_blad': 'max'
}).reset_index()

# Rysuj wykres log-log
plt.figure(figsize=(10,6))
plt.loglog(grouped['dt'], grouped['BME_blad'], 'o-', label='Euler jawny')
plt.loglog(grouped['dt'], grouped['PME_blad'], 's-', label='Euler niejawny')
plt.loglog(grouped['dt'], grouped['PMT_blad'], 'd-', label='Trapezów')

# Linie referencyjne O(dt) i O(dt^2)
dt_ref = np.array(sorted(grouped['dt']))
plt.loglog(dt_ref, dt_ref, 'k--', label='O(dt)')
plt.loglog(dt_ref, dt_ref**2, 'k:', label='O(dt^2)')

plt.xlabel('dt')
plt.ylabel('Maksymalny błąd')
plt.title('Zależność maksymalnego błędu od kroku czasowego (log-log)')
plt.legend()
plt.grid(True, which='both', linestyle='--')
plt.show()
