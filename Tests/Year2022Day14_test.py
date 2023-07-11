
import pytest

import build.Debug.AdventModule as m


def test_FallingSandGame__given_invalidPath__evaluates_toFalse():
    # Act
    falling_sand = m.FallingSandGame('')

    # Assert
    assert not falling_sand


'''
def test_DistressSignalParser__given_tinyInput__has_listContents():
    # Arrange
    expected_output = (
        [1, [2, [3, [4, [5, 6, 7]]]], 8, 9],
        [1, [2, [3, [10, []]]], 8, 9]
    )

    # Act
    distress_signals = m.DistressSignalParser('data/2022Day13/tiny_input.txt')

    # Assert
    assert distress_signals
    for output in distress_signals:
        assert output == expected_output
'''

if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
