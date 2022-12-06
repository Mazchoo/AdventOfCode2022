import pytest
import build.Debug.AdventModule as m

def test_SignalPacket__given_invalidPath__evaluates_toFalse():
    # Act
    packets = m.SignalPacket('')

    # Assert
    assert(not packets)


def test_SignalPacket__given_validPath__has_expectedContents():
    # Arrange
    expected_output = "mjqjpqmgbljsphdztnvjfqwrcgsmlb"

    # Act
    packets = m.SignalPacket('data/2022Day06/tiny_input.txt')

    # Assert
    assert(packets)
    for packet in packets:
        assert(packet == expected_output)
        

def test_SignalPacket_get_packet_starts__given_tinyInput__returns_expectedPosition():
    # Arrange
    expected_output = [7]

    # Act
    packets = m.SignalPacket('data/2022Day06/tiny_input.txt')

    # Assert
    assert(packets.get_packet_starts(4) == expected_output)


def test_SignalPacket_get_packet_starts__given_smallExample__returns_expectedPositions():
    # Arrange
    expected_output = [5, 6, 10, 11]

    # Act
    packets = m.SignalPacket('data/2022Day06/small_input.txt')

    # Assert
    assert(packets.get_packet_starts(4) == expected_output)


def test_SignalPacket_get_packet_starts__given_longerNumberChars__returns_expectedPosition():
    # Arrange
    expected_output = [19]

    # Act
    packets = m.SignalPacket('data/2022Day06/another_tiny_input.txt')

    # Assert
    assert(packets.get_packet_starts(14) == expected_output)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])