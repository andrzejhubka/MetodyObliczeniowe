import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj dane
df = pd.read_csv('wyniki.csv')  # lub 'wyniki.csv' jeśli tak się nazywa plik

# Rysuj wykres
plt.figure(figsize=(10, 6))
plt.plot(df['t'], df['analityczne'], label='Analityczne', linewidth=2)
plt.plot(df['t'], df['BME'], label='BME', linestyle='--')
plt.plot(df['t'], df['PME'], label='PME', linestyle='-.')
plt.plot(df['t'], df['PMT'], label='PMT', linestyle=':')

plt.xlabel('t')
plt.ylabel('y')
plt.title('Porównanie metod numerycznych z rozwiązaniem analitycznym')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
