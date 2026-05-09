import csv
import matplotlib.pyplot as plt

data = {}
with open('10_05.csv') as f:
    for row in csv.DictReader(f):
        n = row['name']
        if n not in data:
            data[n] = ([], [])
        data[n][0].append(int(row['strings']))
        data[n][1].append(int(row['collisions']))

for name, (x, y) in sorted(data.items()):
    plt.plot(x, y, label=name)

plt.xlabel('strings hashed')
plt.ylabel('collisions')
plt.title('hash function collision comparison')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
