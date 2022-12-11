
import matplotlib.pyplot as plt
import build.Release.AdventModule as m

crt = m.CathodeRayTube('data/2022Day10/puzzle_input.txt')
crt.do_all_ops()

plt.imshow(crt.image, cmap='gnuplot')
print(crt.signal_sum)
plt.show()
