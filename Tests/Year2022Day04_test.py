import pytest
import build.Debug.AdventModule as m

def test_CleaningRotas__given_invalidPath__evaluates_toFalse():
    # Act
    rotas = m.CleaningRotas('')

    # Assert
    assert(not rotas)


def test_CleaningRotas__given_validPath__has_expectedContents():
    # Arrange
    expected_output = [
        ([2, 4], [6, 8]),
        ([2, 3], [4, 5]),
    ]

    # Act
    rotas = m.CleaningRotas('data/2022Day04/tiny_input.txt')

    # Assert
    assert(rotas)
    for i, rota in enumerate(rotas):
        assert(rota == expected_output[i])


def test_CleaningRotas__given_validPath__has_expectedFullyContained():
    # Act
    rotas = m.CleaningRotas('data/2022Day04/small_input.txt')

    # Assert
    assert(rotas.get_number_fully_cotained() == 2)


def test_CleaningRotas__given_validPath__has_expectedOverlaps():
    # Act
    rotas = m.CleaningRotas('data/2022Day04/small_input.txt')

    # Assert
    assert(rotas.get_number_overlaps() == 4)

if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])