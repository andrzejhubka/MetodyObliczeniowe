import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Wczytaj dane
df = pd.read_csv("wyniki.csv")

# Grupuj po dx i wyciągnij maksymalny błąd dla każdej metody
grouped = df.groupby('dx').agg({
    'strzaly_blad': 'max',
    'thomas_blad': 'max'
}).reset_index()

# Zamień na log10
grouped['log10_dx'] = np.log10(grouped['dx'])
grouped['log10_strzaly_blad'] = np.log10(grouped['strzaly_blad'])
grouped['log10_thomas_blad'] = np.log10(grouped['thomas_blad'])

plt.figure(figsize=(10, 7))
plt.plot(grouped['log10_dx'], grouped['log10_strzaly_blad'], 'o-', label='Metoda strzałów', color='#22A7F0')
plt.plot(grouped['log10_dx'], grouped['log10_thomas_blad'], 's-', label='Metoda Thomasa', color='#FFAA40')

# Linie referencyjne O(h) i O(h^2)
# Wybierz punkt startowy (np. pierwszy punkt wykresu)
x0 = grouped['log10_dx'].min()
y0 = grouped['log10_strzaly_blad'].min()  # lub inny punkt na wykresie

# O(h): nachylenie 1
plt.plot(grouped['log10_dx'], y0 + (grouped['log10_dx'] - x0)*1, 'k--', label='O(h)')
# O(h^2): nachylenie 2
plt.plot(grouped['log10_dx'], y0 + (grouped['log10_dx'] - x0)*2, 'k:', label='O(h²)')

plt.xlabel('log10(h)')
plt.ylabel('log10(max |błąd|)')
plt.legend()
plt.grid(True, which='both', linestyle='--')
plt.tight_layout()
plt.show()
