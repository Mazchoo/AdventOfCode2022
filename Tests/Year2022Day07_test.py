import pytest
import build.Debug.AdventModule as m

def test_FileCommands__given_invalidPath__evaluates_toFalse():
    # Act
    commands = m.FileCommands('')

    # Assert
    assert(not commands)


def test_FileCommands__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [
        "$ cd /",
        "$ ls",
        "dir a",
        "14848514 b.txt",
        "8504156 c.dat"  
    ]

    # Act
    commands = m.FileCommands('data/2022Day07/tiny_input.txt')

    # Assert
    assert(commands)
    for i, command in enumerate(commands):
        assert(command == expected_output[i])


def test_FileCommands_total_size__given_validPath__has_expectedTotalSize():
    # Act
    commands = m.FileCommands('data/2022Day07/small_input.txt')
    total_file_size = commands.total_size

    # Assert
    assert(commands)
    assert(total_file_size == 48381165)


def test_FileCommands_total_below_thresh__given_validPath__has_expectedTotalSize():
    # Act
    commands = m.FileCommands('data/2022Day07/small_input.txt')
    expected_value = commands.total_below_thresh(100000)

    # Assert
    assert(expected_value == 95437)


def test_FileCommands_dir_to_delete__given_validPath__has_directorySize():
    # Act
    commands = m.FileCommands('data/2022Day07/small_input.txt')
    expected_value = commands.dir_to_delete(8381165)

    # Assert
    assert(expected_value == 24933642)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])