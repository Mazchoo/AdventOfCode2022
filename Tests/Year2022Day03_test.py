
import pytest
import build.Release.AdventModule as m

def test_RucksackContents__given_invalidPath__evaluates_toFalse():
    # Act
    rucksacks = m.RucksackContents('')

    # Assert
    assert(not rucksacks)


def test_RucksackContents__given_validPath__has_expectedSplitContents():
    # Arrange
    expected_output = [
        ("vJrwpWtwJgWr", "hcsFMMfFFhFp"),
        ("jqHRNqRjqzjGDLGL", "rsFMfFZSrLrFZsSL"),
        ("PmmdzqPrV", "vPwwTWBwg"),
        ("wMqvLMZHhHMvwLH", "jbvcjnnSBnvTQFn"),
        ("ttgJtRGJ", "QctTZtZT"),
        ("CrZsJsPPZsGz", "wwsLwLmpwMDw")
    ]

    # Act
    rucksacks = m.RucksackContents('data/2022Day03/small_input.txt')

    # Assert
    assert(rucksacks)
    for i, rucksack in enumerate(rucksacks.get_rucksacks):
        assert(rucksack == expected_output[i])


def test_find_shared_characters__returns_expectedSharedChars():
    # Arrange
    expected_output = ['p', 'L', 'P', 'v', 't', 's']

    # Act
    rucksacks = m.RucksackContents('data/2022Day03/small_input.txt')

    # Assert
    assert(rucksacks)
    for i, char in enumerate(rucksacks.find_shared_characters()):
        assert(char == expected_output[i])


def test_calculate_total_error__given_validPath__has_expectedSharedChars():
    # Act
    rucksacks = m.RucksackContents('data/2022Day03/small_input.txt')

    # Assert
    assert(rucksacks.calculate_total_error() == 157)


def test_calculate_total_error__given_validPath__has_expectedSharedChars():
    # Arrange
    expected_output = ['r', 'Z']

    # Act
    rucksacks = m.RucksackContents('data/2022Day03/small_input.txt')

    # Assert
    assert(rucksacks)
    for i, char in enumerate(rucksacks.find_shared_shared_group_chars()):
        assert(char == expected_output[i])


def test_calculate_total_group_error__given_validPath__has_expectedSharedChars():
    # Act
    rucksacks = m.RucksackContents('data/2022Day03/small_input.txt')

    # Assert
    assert(rucksacks.calculate_total_group_error() == 70)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])