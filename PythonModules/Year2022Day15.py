import matplotlib.pyplot as plt

import build.Release.AdventModule as m


def show_state(sand: m.FallingSandGame):
    plt.imshow(falling_sand.image, cmap='plasma', vmin=0, vmax=3)
    plt.title(f'There are {falling_sand.nrSand} blocks of sand')
    plt.show()


falling_sand = m.FallingSandGame('data/2022Day14/puzzle_input_extend.txt', 500, 0)
falling_sand.updateUntilAtRest()

for i in range(1, 100000):
    falling_sand.updateUntilAtRest()
    if i % 1000 == 0:
        show_state(falling_sand.image)
