
import build.Release.AdventModule as m

'''
            [C]         [N] [R]    
[J] [T]     [H]         [P] [L]    
[F] [S] [T] [B]         [M] [D]    
[C] [L] [J] [Z] [S]     [L] [B]    
[N] [Q] [G] [J] [J]     [F] [F] [R]
[D] [V] [B] [L] [B] [Q] [D] [M] [T]
[B] [Z] [Z] [T] [V] [S] [V] [S] [D]
[W] [P] [P] [D] [G] [P] [B] [P] [V]
'''

input_stacks = [
    list('JFCNDBW'),
    list('TSLQVZP'),
    list('TJGBZP'),
    list('CHBZJLTD'),
    list('SJBVG'),
    list('QSP'),
    list('NPMLFDVB'),
    list('RLDBFMSP'),
    list('RTDV')
]

stacks = m.MovingStacks('data/2022Day05/puzzle_input.txt')
stacks.current_stack = input_stacks
current_stack = stacks.do_all_moves()
print(''.join([stack[-1] for stack in current_stack]))

stacks.current_stack = input_stacks
current_stack = stacks.do_all_moves_keep_order()
print(''.join([stack[-1] for stack in current_stack]))
