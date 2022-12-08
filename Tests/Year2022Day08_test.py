import pytest
import numpy as np

import build.Debug.AdventModule as m

def test_TreeMap__given_invalidPath__evaluates_toFalse():
    # Act
    tree_map = m.TreeMap('')

    # Assert
    assert(not tree_map)


def test_TreeMap__given_validPath__has_expectedContents():
    # Arrange
    expected_output = np.array([
        [3, 0, 3, 7, 3],
        [2, 5, 5, 1, 2],
        [6, 5, 3, 3, 2],
        [3, 3, 5, 4, 9],
        [3, 5, 3, 9, 0],
    ], dtype=np.uint8)

    # Act
    tree_map = m.TreeMap('data/2022Day08/small_input.txt')

    # Assert
    assert(tree_map)
    assert((tree_map.image == expected_output).all())


def test_TreeMap_visibility__given_validPath__returns_expectedResult():
    # Act
    tree_map = m.TreeMap('data/2022Day08/small_input.txt')

    # Assert
    assert(tree_map.visibility == 21)
    

def test_TreeMap_scenic_score__given_validPath__has_expectedMaximum():
    # Arrange
    tree_map = m.TreeMap('data/2022Day08/small_input.txt')

    # Act & Assert
    assert(tree_map.scenic_scores.max() == 8)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])