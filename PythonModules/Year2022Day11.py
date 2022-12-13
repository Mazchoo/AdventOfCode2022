
import build.Release.AdventModule as m

monkeys = m.MonkeyMadness('data/2022Day11/puzzle_input.txt')
monkeys.do_rounds(20)

print('Part 1', monkeys.inspections[-1] * monkeys.inspections[-2])

bad_monkeys = m.MonkeyMadness('data/2022Day11/puzzle_input.txt', False)
bad_monkeys.do_rounds(10000)

print('Part 2', bad_monkeys.inspections[-1] * bad_monkeys.inspections[-2])
