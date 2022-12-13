import pytest
import numpy as np

import build.Debug.AdventModule as m

def test_MonkeyMadness__given_invalidPath__evaluates_toFalse():
    # Act
    monkeys = m.MonkeyMadness('')

    # Assert
    assert(not monkeys)


def test_MonkeyMadness__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [{
        'items': [79, 98],
        'func': ('multiply', 19),
        'test': 23,
        'true': 2,
        'false': 3
    },
    {
        'items': [54, 65, 75, 74],
        'func' : ('add', 6),
        'test': 19,
        'true': 2,
        'false': 0
    }]

    # Act
    monkeys = m.MonkeyMadness('data/2022Day11/tiny_input.txt')

    # Assert
    assert(monkeys)
    for i, monkey in enumerate(monkeys):
        assert(monkey == expected_output[i])


def test_MonkeyMadness__given_oneRound__returns_expectedItemDistribution():
    # Arrange
    expected_output = [
        [20, 23, 27, 26],
        [2080, 25, 167, 207, 401, 1046],
        [],
        []
    ]
    
    # Act
    monkeys = m.MonkeyMadness('data/2022Day11/small_input.txt')
    monkeys.do_rounds(1)

    # Assert
    for i, monkey in enumerate(monkeys):
        assert(monkey['items'] == expected_output[i])


def test_MonkeyMadness__given_twentyRounds__returns_expectedItemDistribution():
    # Arrange
    expected_output = [
        [10, 12, 14, 26, 34],
        [245, 93, 53, 199, 115],
        [],
        []
    ]
    
    # Act
    monkeys = m.MonkeyMadness('data/2022Day11/small_input.txt')
    monkeys.do_rounds(20)

    # Assert
    for i, monkey in enumerate(monkeys):
        assert(monkey['items'] == expected_output[i])
    

def test_MonkeyMadness__given_twentyRounds__has_expectedNrInspections():
    # Arrange
    expected_output = [7, 95, 101, 105]
    
    # Act
    monkeys = m.MonkeyMadness('data/2022Day11/small_input.txt')
    monkeys.do_rounds(20)

    # Assert
    assert(monkeys.inspections == expected_output)
    

def test_MonkeyMadness__given_twentyRoundsWithWorrying__has_expectedNrInspections():
    # Arrange
    expected_output = [8, 97, 99, 103]
    
    # Act
    monkeys = m.MonkeyMadness('data/2022Day11/small_input.txt', False)
    monkeys.do_rounds(20)

    # Assert
    assert(monkeys.inspections == expected_output)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])