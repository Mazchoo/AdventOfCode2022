
import pytest
import numpy as np
from itertools import combinations

from PythonModules.Year2021Day19 import (
    readProbeData, getAllRotationMatrices, matchesAPointInList, getPointCloudMatches,
    getNewPointsFromCloud, getAllBeaconsFromArray, getNumberOfBeacons,
    getLargestDifferenceBetweenPoints
)

def test_readProbeData__given_probeData__returns_numpyArrays():
    # Arrange
    expected_data = [np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int),
                     np.array([[-1,-1,0], [-5,0,0], [-2,1,0]], dtype=int)]

    # Act
    probe_data = readProbeData('data/2021Day19/two_scanners.txt')

    # Assert
    assert(len(probe_data) == 2)
    assert((probe_data[0] == expected_data[0]).all())
    assert((probe_data[1] == expected_data[1]).all())


def test_getAllRotationMatrices__returns__twentyFourDistinctMatricesWithPositiveDeterminant():
    # Act
    matrices = getAllRotationMatrices()

    # Assert
    assert(len(matrices) == 24)
    for m1, m2 in combinations(matrices, 2):
        assert(not (m1 == m2).all())
    for m in matrices:
        assert(np.linalg.det(m) == 1.)


def test_matchesAPointInList__given_pointMatches_returns_True():
    # Arrange
    point_cloud = np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int)
    point = np.array([4, 1, 0], dtype=int)

    # Act & Assert
    assert(matchesAPointInList(point, point_cloud))


def test_matchesAPointInList__given_noPointMatches_returns_False():
    # Arrange
    point_cloud = np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int)
    point = np.array([4, 2, 0], dtype=int)

    # Act & Assert
    assert(not matchesAPointInList(point, point_cloud))


def test_getPointCloudMatches__givenTwoMatches_returns_matchArray():
    # Arrange
    point_cloud_1 = np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int)
    point_cloud_2 = np.array([[4,2,0], [3,3,0], [0,2,0]], dtype=int)

    # Act
    matches = getPointCloudMatches(point_cloud_1, point_cloud_2)

    # Assert
    assert((matches == [False, True, True]).all())


def test_getNewPointsFromCloud__given_extraPointInCloud__returns_newPoint():
    # Arrange
    cloud_1 = np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int)
    cloud_2 = np.array([[-1,-1,0], [-5,0,0], [-2,1,0], [0, 0, 0]], dtype=int)

    # Act
    new_points, alignment = getNewPointsFromCloud(cloud_1, cloud_2, 3)

    # Assert
    assert((new_points == [[5, 2, 0]]).all()) # [5, 2, 0] is the aligment of the clouds
    assert((alignment == [5, 2, 0]).all())


def test_getAllBeaconsFromArray__given_extraPointInCloud__returns_cloudWithExtraElement():
    # Arrange
    cloud_1 = np.array([[0,2,0], [4,1,0], [3,3,0]], dtype=int)
    cloud_2 = np.array([[-1,-1,0], [-5,0,0], [-2,1,0], [0, 0, 0]], dtype=int)
    expected_result = np.vstack((cloud_1, [[5, 2, 0]]))

    # Act
    full_cloud, _ = getAllBeaconsFromArray([cloud_1, cloud_2], 3)

    # Assert
    assert((full_cloud == expected_result).all()) # [5, 2, 0] is the aligment of the clouds


def test_getNumberOfBeacons__given_twoScanners__returns_expectedResult():
    # Act & Assert
    assert(getNumberOfBeacons('data/2021Day19/two_scanners.txt', 3) == 3)


def test_getNumberOfBeacons__given_scannerMultipleAngles__returns_expectedResult():
    # Act & Assert
    assert(getNumberOfBeacons('data/2021Day19/scanner_same_position_multiple_angles.txt', 6) == 6)


def test_getNumberOfBeacons__given_smallExample__returns_expectedResult():
    # Act & Assert
    assert(getNumberOfBeacons('data/2021Day19/small_full_example.txt', 6) == 79)


def test_getLargestDifference__given_smallExample__returns_epxectedResult():
    # Act & Assert
    assert(getLargestDifferenceBetweenPoints('data/2021Day19/small_full_example.txt', 6) == 3621)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
