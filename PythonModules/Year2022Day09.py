
import matplotlib.pyplot as plt
import build.Release.AdventModule as m


rope_bridge = m.RopeBridge('data/2022Day09/puzzle_input.txt')
total_places = rope_bridge.do_all_moves()
plt.imshow(rope_bridge.tail_move_map, cmap='copper')
print(total_places)
plt.show()


rope_bridge = m.RopeBridge('data/2022Day09/medium_input.txt', 10)
for move in rope_bridge:
    rope_bridge.do_move(move)
    print(move)
    plt.imshow(rope_bridge.chain, cmap='copper')
    plt.show()


plt.imshow(rope_bridge.tail_move_map, cmap='copper')
plt.show()


rope_bridge = m.RopeBridge('data/2022Day09/puzzle_input.txt', 10)
total_places = rope_bridge.do_all_moves()
plt.imshow(rope_bridge.tail_move_map, cmap='copper')
print(total_places)
plt.show()
