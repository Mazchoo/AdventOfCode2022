import pytest
import build.Debug.AdventModule as m

def test_MovingStacks__given_invalidPath__evaluates_toFalse():
    # Act
    stacks = m.MovingStacks('')

    # Assert
    assert(not stacks)


def test_MovingStacks__given_validPath__has_expectedContents():
    # Arrange
    expected_moves = [
        [1, 1, 0],
        [3, 0, 2],
        [2, 1, 0],
        [1, 0, 1]
    ]

    # Act
    stacks = m.MovingStacks('data/2022Day05/small_input.txt')

    # Assert
    assert(stacks)
    for i, move in enumerate(stacks):
        assert(move == expected_moves[i])


def test_MovingStacks__given_validStacks__has_expectedStacks():
    # Arrange
    input_stacks = [
        ['N', 'Z'],
        ['D', 'C', 'M'],
        ['P']
    ]
    expected_stack = [list(reversed(stack)) for stack in input_stacks]
    
    # Act
    stacks = m.MovingStacks('data/2022Day05/small_input.txt')
    stacks.current_stack = input_stacks

    # Assert
    assert(stacks.current_stack == expected_stack)


def test_MovingStacks_do_all_moves__given_validStacks__makesExpectedMoves():
    # Arrange
    input_stacks = [
        ['N', 'Z'],
        ['D', 'C', 'M'],
        ['P']
    ]
    expected_stack = [
        ['C'],
        ['M'],
        ['P', 'D', 'N', 'Z']
    ]
    
    # Act
    stacks = m.MovingStacks('data/2022Day05/small_input.txt')
    stacks.current_stack = input_stacks
    stacks.do_all_moves()
    
    # Assert
    assert(stacks.current_stack == expected_stack)


def test_MovingStacks_do_all_moves_keep_order__given_validStacks__makesExpectedMoves():
    # Arrange
    input_stacks = [
        ['N', 'Z'],
        ['D', 'C', 'M'],
        ['P']
    ]
    expected_stack = [
        ['M'],
        ['C'],
        ['P', 'Z', 'N', 'D']
    ]
    
    # Act
    stacks = m.MovingStacks('data/2022Day05/small_input.txt')
    stacks.current_stack = input_stacks
    stacks.do_all_moves_keep_order()
    
    # Assert
    assert(stacks.current_stack == expected_stack)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])