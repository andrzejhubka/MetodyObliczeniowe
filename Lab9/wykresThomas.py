import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Wczytaj dane
data = pd.read_csv("wyniki.csv")
data = data[data['dx'] == 0.0066000000000000]
x = data['x']
u_analitycznie = data['analityczne']
u_thomas = data['thomas']


plt.figure(figsize=(10, 7))
plt.plot(x, u_thomas, 'o-', label='Metoda thomasa', color='#22A7F0')
plt.plot(x, u_analitycznie, 'o-', label='Rozw analityczne', color='blue')


plt.xlabel('x')
plt.ylabel('u_analitycznie')
plt.legend()
plt.grid(True, which='both', linestyle='--')
plt.tight_layout()
plt.show()
