
import matplotlib.pyplot as plt
import build.Release.AdventModule as m


tree_map_full = m.TreeMap('data/2022Day08/puzzle_input.txt')
plt.imshow(tree_map_full.image, cmap='seismic')
print(tree_map_full.visibility)
plt.show()

plt.imshow(tree_map_full.down_max, cmap='seismic')
plt.show()

plt.imshow(tree_map_full.up_max, cmap='seismic')
plt.show()

plt.imshow(tree_map_full.right_max, cmap='seismic')
plt.show()

plt.imshow(tree_map_full.left_max, cmap='seismic')
plt.show()

tree_map_full = m.TreeMap('data/2022Day08/puzzle_input.txt')
scenic_scores = tree_map_full.scenic_scores
plt.imshow(scenic_scores, cmap='seismic')
print(scenic_scores.max())
plt.show()
