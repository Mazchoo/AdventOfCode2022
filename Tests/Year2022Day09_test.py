import pytest
import numpy as np

import build.Debug.AdventModule as m

def test_RopeBridge__given_invalidPath__evaluates_toFalse():
    # Act
    rope_bridge = m.RopeBridge('')

    # Assert
    assert(not rope_bridge)


def test_RopeBridge__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [
        ('R', 4),
        ('U', 2),
        ('L', 3),
        ('D', 1)
    ]

    # Act
    rope_bridge = m.RopeBridge('data/2022Day09/tiny_input.txt')

    # Assert
    assert(rope_bridge)
    for i, move in enumerate(rope_bridge):
        assert(move == expected_output[i])


def test_TreeMap_do_all_moves__given_validPath__returns_expectedResult():
    # Act
    rope_bridge = m.RopeBridge('data/2022Day09/small_input.txt')
    nr_tail_places = rope_bridge.do_all_moves()

    # Assert
    assert(nr_tail_places == 13)


def test_TreeMap_tail_move_map__given_validPath__returns_expectedResult():
    # Arrange
    expected_output = np.array([
        [False, False, True,  True,  False], 
        [False, False, False, True,  True], 
        [False, True,  True,  True,  True], 
        [False, False, False, False, True], 
        [True,  True,  True,  True,  False], 
    ], dtype=bool)

    # Act
    rope_bridge = m.RopeBridge('data/2022Day09/small_input.txt')
    rope_bridge.do_all_moves()
    tail_move_map = rope_bridge.tail_move_map

    # Assert
    assert((tail_move_map == expected_output).all())


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])