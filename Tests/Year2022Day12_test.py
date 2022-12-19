import pytest
import numpy as np

import build.Debug.AdventModule as m

def test_HillClimbing__given_invalidPath__evaluates_toFalse():
    # Act
    hill_climbing = m.HillClimbing('')

    # Assert
    assert(not hill_climbing)


def test_HillClimbing__given_validInput__has_expectedHillImage():
    # Arrange
    expected_output = [
        "Sabqponm",
        "abcryxxl",
        "accszExk",
        "acctuvwj",
        "abdefghi"
    ]
    expected_output = [[ord(ch) for ch in line] for line in expected_output]

    # Act
    hill_climbing = m.HillClimbing('data/2022Day12/small_input.txt')

    # Assert
    assert((hill_climbing.image == np.array(expected_output, dtype=np.uint8)).all())
    

def test_HillClimbing_path_length__given_validInput__returns_expectedPathLength():
    # Act
    hill_climbing = m.HillClimbing('data/2022Day12/small_input.txt')

    # Assert
    assert(hill_climbing.path_length() == 31)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])