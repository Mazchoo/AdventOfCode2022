
import build.Debug.AdventModule as m

rucksacks = m.RucksackContents('data/2022Day03/puzzle_input.txt')

print(rucksacks.calculate_total_error())
print(rucksacks.calculate_total_group_error())
