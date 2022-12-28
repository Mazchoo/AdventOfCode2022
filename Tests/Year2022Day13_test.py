
import pytest

import build.Debug.AdventModule as m


def test_DistressSignalParser__given_invalidPath__evaluates_toFalse():
    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals)


def test_DistressSignalParser__given_validInput__has_listContents():
    # Arrange
    expected_output = (
        [1, [2, [3, [4, [5, 6, 7]]]], 8, 9],
        [1, [2, [3, [4, [5, 6, 0]]]], 8, 9]
    )

    # Act
    distress_signals = m.DistressSignalParser('data/2022Day13/tiny_input.txt')

    # Assert
    assert(distress_signals)
    for output in distress_signals:
        assert(output == expected_output)


def test_DistressSignalParser__given_listPair1__returns_expectedOutcome():
    # Arrange
    list_1 = [1, 1, 3, 1, 1]
    list_2 = [1, 1, 5, 1, 1]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(distress_signals.compare_lists(list_1, list_2))

if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
