
import build.Release.AdventModule as m

distress_signals = m.DistressSignalParser('data/2022Day13/puzzle_input.txt')
print(distress_signals.compare_all_input())
print(distress_signals.get_sorted_key())
