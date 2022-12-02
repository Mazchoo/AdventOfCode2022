
import build.Release.AdventModule as m

games = m.RockPaperScissorGames('data/2022Day02/puzzle_input.txt')
print(games.total_score())
print(games.total_real_score())
