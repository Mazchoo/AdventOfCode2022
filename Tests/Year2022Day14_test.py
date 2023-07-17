
import pytest
import numpy as np

import build.Debug.AdventModule as m


def test_FallingSandGame__given_invalidPath__evaluates_toFalse():
    # Act
    falling_sand = m.FallingSandGame('')

    # Assert
    assert not falling_sand


def test_FallingSandGame__given_tinyInput__has_listContents():
    # Arrange
    expected_output = np.zeros((3, 5), dtype=np.uint8)
    expected_output[2, :] = 1

    # Act
    falling_sand = m.FallingSandGame('data/2022Day14/tiny_input.txt')

    # Assert
    assert falling_sand
    assert np.all(falling_sand.image == expected_output)


def test_FallingSandGame__given_smallInput__has_exectedTotal():
    # Arrange
    expected_total = 20

    # Act
    falling_sand = m.FallingSandGame('data/2022Day14/small_input.txt')

    # Assert
    assert falling_sand
    assert falling_sand.image.sum() == expected_total


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
