
import pytest
import numpy as np

import build.Release.AdventModule as m


def test_CalorieCollection__given_smallInput__has_expectedTotals():
    # Act
    calories = m.CalorieCollection('data/2022Day02/small_example.txt')

    # Assert
    assert((calories.totals == np.array([4000, 6000, 10000, 11000, 24000])).all())


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
