
import build.Release.AdventModule as m

if __name__ == '__main__':
    collection = m.CalorieCollection('data/2022Day01/puzzle_input.txt')
    total_calories = collection.totals
    print(total_calories[-1])
    print(total_calories[-3] + total_calories[-2] + total_calories[-1])
