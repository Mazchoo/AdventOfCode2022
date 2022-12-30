
import pytest

import build.Debug.AdventModule as m


def test_DistressSignalParser__given_invalidPath__evaluates_toFalse():
    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals)


def test_DistressSignalParser__given_tinyInput__has_listContents():
    # Arrange
    expected_output = (
        [1, [2, [3, [4, [5, 6, 7]]]], 8, 9],
        [1, [2, [3, [10, []]]], 8, 9]
    )

    # Act
    distress_signals = m.DistressSignalParser('data/2022Day13/tiny_input.txt')

    # Assert
    assert(distress_signals)
    for output in distress_signals:
        assert(output == expected_output)


def test_DistressSignalParser__given_smallInput__has_listContents():
    # Arrange
    expected_output = [
        ([1,1,3,1,1], [1,1,5,1,1]),
        ([[1],[2,3,4]], [[1],4]),
        ([9], [[8,7,6]]),
        ([[4,4],4,4], [[4,4],4,4,4]),
        ([7,7,7,7], [7,7,7]),
        ([], [3]),
        ([[[]]], [[]]),
        ([1,[2,[3,[4,[5,6,7]]]],8,9], [1,[2,[3,[4,[5,6,0]]]],8,9])
    ]

    # Act
    distress_signals = m.DistressSignalParser('data/2022Day13/small_input.txt')

    # Assert
    assert(distress_signals)
    for i, output in enumerate(distress_signals):
        assert(output == expected_output[i])


def test_DistressSignalParser__given_listPair1__returns_expectedOutcome():
    # Arrange
    list_1 = [1, 1, 3, 1, 1]
    list_2 = [1, 1, 5, 1, 1]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair2__returns_expectedOutcome():
    # Arrange
    list_1 = [[1], [2, 3, 4]]
    list_2 = [[1], 4]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair3__returns_expectedOutcome():
    # Arrange
    list_1 = [9]
    list_2 = [[8, 7, 6]]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair4__returns_expectedOutcome():
    # Arrange
    list_1 = [[4, 4], 4, 4]
    list_2 = [[4, 4], 4, 4, 4]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair5__returns_expectedOutcome():
    # Arrange
    list_1 = [7, 7, 7, 7]
    list_2 = [7, 7, 7]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair6__returns_expectedOutcome():
    # Arrange
    list_1 = []
    list_2 = [3]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair7__returns_expectedOutcome():
    # Arrange
    list_1 = [[[]]]
    list_2 = [[]]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser__given_listPair8__returns_expectedOutcome():
    # Arrange
    list_1 = [1, [2, [3, [4, [5, 6, 7]]]], 8, 9]
    list_2 = [1, [2, [3, [4, [5, 6, 0]]]], 8, 9]

    # Act
    distress_signals = m.DistressSignalParser('')

    # Assert
    assert(not distress_signals.compare_lists(list_1, list_2))


def test_DistressSignalParser_compare_all_input__given_smallInput__returns_expectedOutcome():
    # Act
    distress_signals = m.DistressSignalParser('data/2022Day13/small_input.txt')

    # Assert
    assert(distress_signals.compare_all_input() == 13)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
