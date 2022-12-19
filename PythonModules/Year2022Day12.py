
import matplotlib.pyplot as plt

import build.Release.AdventModule as m

hill_map = m.HillClimbing('data/2022Day12/puzzle_input.txt')

plt.imshow(hill_map.image, cmap='seismic')
plt.show()

print(hill_map.path_length())
plt.imshow(hill_map.distances, cmap='seismic')
plt.show()
