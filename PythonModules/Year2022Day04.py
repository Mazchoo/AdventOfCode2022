
import build.Release.AdventModule as m

rotas = m.CleaningRotas('data/2022Day04/puzzle_input.txt')
print(rotas.get_number_fully_cotained())
print(rotas.get_number_overlaps())
