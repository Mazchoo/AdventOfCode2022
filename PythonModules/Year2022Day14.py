import matplotlib.pyplot as plt

import build.Release.AdventModule as m


falling_sand = m.FallingSandGame('data/2022Day14/puzzle_input.txt')
plt.imshow(falling_sand.image)
plt.show()
