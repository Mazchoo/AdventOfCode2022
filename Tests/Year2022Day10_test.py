import pytest
import numpy as np

import build.Debug.AdventModule as m

def test_CathodeRayTube__given_invalidPath__evaluates_toFalse():
    # Act
    crt = m.CathodeRayTube('')

    # Assert
    assert(not crt)


def test_CathodeRayTube__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [
        ('addx', 5),
        ('noop', 0)
    ]

    # Act
    crt = m.CathodeRayTube('data/2022Day10/tiny_input.txt')

    # Assert
    assert(crt)
    for i, move in enumerate(crt):
        assert(move == expected_output[i])


def test_CathodeRayTube__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [
        ('addx', 5),
        ('noop', 0)
    ]

    # Act
    crt = m.CathodeRayTube('data/2022Day10/tiny_input.txt')

    # Assert
    assert(crt)
    for i, move in enumerate(crt):
        assert(move == expected_output[i])


def test_CathodeRayTube_signal_sum__given_smallInput__returns_expectedValue():
    # Arrange
    crt = m.CathodeRayTube('data/2022Day10/small_input.txt')
    crt.do_all_ops()

    # Act & Assert
    assert(crt.signal_sum == 13140)




if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])