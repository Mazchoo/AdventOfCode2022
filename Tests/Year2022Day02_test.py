
import pytest
import build.Debug.AdventModule as m


def test_RockPaperScissorGames__has_expectedMap():
    # Act
    games = m.RockPaperScissorGames('')

    # Assert
    assert(len(games.get_map()) == 9)
    assert(sum(games.get_map().values()) == 45)


def test_RockPaperScissorGames__given_invalidPath__has_invalidParsedVariable():
    # Act
    games = m.RockPaperScissorGames('')

    # Assert
    assert(not games.get_parsed)


def test_RockPaperScissorGames__given_smallInput__has_expectedTotal():
    # Act
    games = m.RockPaperScissorGames('data/2022Day02/small_input.txt')

    # Assert
    assert(games.total_score() == 15)


def test_RockPaperScissorGames__given_smallInput__has_expectedRealTotal():
    # Act
    games = m.RockPaperScissorGames('data/2022Day02/small_input.txt')

    # Assert
    assert(games.total_real_score() == 12)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])