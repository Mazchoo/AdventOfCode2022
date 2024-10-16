
import pytest
import numpy as np

import build.Debug.AdventModule as m


def test_ParticleFilter__given_invalidPath__evaluates_toFalse():
    # Act
    particle_filter = m.ParticleFilter('')

    # Assert
    assert not particle_filter


def test_ParticleFilter__given_tinyInput__has_listContents():
    # Arrange
    expected_output = [(1, 2, 3, 4)]

    # Act
    particle_filter = m.ParticleFilter('data/2022Day15/tiny_input.txt')

    # Assert
    assert particle_filter
    assert np.all(particle_filter.scans == expected_output)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
