
import build.Release.AdventModule as m

commands = m.FileCommands('data/2022Day07/puzzle_input.txt')
print(commands.total_below_thresh(100000))

total_disk_size = 70000000
space_required = 30000000
total_size = commands.total_size
space_required = total_size - total_disk_size + space_required

print('total size', total_size)
print('free space', total_disk_size - total_size)
print('additional space required', space_required)
print('dir size to delete', commands.dir_to_delete(space_required))
