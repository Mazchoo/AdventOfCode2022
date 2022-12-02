
import numpy as np
from itertools import product, combinations

def _appendNumpyScanToPointClouds(scan_list: list, point_clouds: list):
    if scan_list is None: return
    try:
        scan_np = np.array(scan_list, dtype=int)
    except:
        print(f'Bad scan {scan_list}')
    else:
        point_clouds.append(scan_np)


def readProbeData(file_name: str):
    with open(file_name,"r") as f:
        lines = f.readlines()

    point_clouds = []
    current_scan = None

    for line in lines:
        if "scanner" in line:
            _appendNumpyScanToPointClouds(current_scan, point_clouds)
            current_scan = []
        else:
            split_line = line.split(',')
            if len(split_line) == 3:
                current_scan.append(split_line)
    _appendNumpyScanToPointClouds(current_scan, point_clouds)
    
    return point_clouds


SIN = {0: 0, 90: 1, 180: 0, 270:-1}
COS = {0: 1, 90: 0, 180: -1, 270:0}
X_ANGLES = [0, 90, 180, 270]
Z_ANGLES = [0, 180]

def getXRotation(degrees: int):
    return np.array([
        [COS[degrees], -SIN[degrees], 0],
        [SIN[degrees],  COS[degrees], 0], 
        [0           , 0            , 1]
    ], dtype=int)


def getZOrientation(degrees: int):
    return np.array([
        [1, 0           , 0            ],
        [0, COS[degrees], -SIN[degrees]], 
        [0, SIN[degrees],  COS[degrees]]
    ], dtype=int)


def getCoordinatePermutation(i: int):
    return np.array([
        [i == 0, i == 1, i == 2],
        [i == 2, i == 0, i == 1], 
        [i == 1, i == 2, i == 0]
    ], dtype=int)


def getAllRotationMatrices():
    x_rotations = [getXRotation(theta) for theta in X_ANGLES]
    z_orientations = [getZOrientation(phi) for phi in Z_ANGLES]
    coord_perms = [getCoordinatePermutation(i) for i in range(3)]
    return [m1 @ m2 @ m3 for m1, m2, m3 in product(x_rotations, z_orientations, coord_perms)]


MATRICES = getAllRotationMatrices()


def matchesAPointInList(point: np.ndarray, point_cloud: np.ndarray):
    point_difference = point_cloud - point
    return (point_difference == 0).all(axis=1).any(axis=-1)


def getPointCloudMatches(point_cloud1, point_cloud2):
    return np.array([matchesAPointInList(point_cloud1, p) for p in point_cloud2], dtype=bool)


def getNewPointsFromCloud(base_cloud, point_cloud, match_limit):
    rotated_clouds = [point_cloud @ m for m in MATRICES]
    for cloud in rotated_clouds:
        for point in cloud:
            for base_point in base_cloud:
                alignment =  base_point - point
                aligned_cloud = cloud + alignment
                matches = getPointCloudMatches(base_cloud, aligned_cloud)
                if matches.sum() >= match_limit:
                    return aligned_cloud[~matches], alignment
    return None, None


def getAllBeaconsFromArray(probe_data, match_limit):
    base_cloud = probe_data[0]
    completed_clouds = [False] * (len(probe_data) - 1)
    alignments = []

    j = 1
    # With lower criteria could only match newly found points
    # End result then has to be converted to set
    while not(all(completed_clouds)):
        print(f"--- run {j} ---")
        for i, point_cloud in enumerate(probe_data[1:]):
            if not completed_clouds[i]:
                print(f'--- cloud {i + 1} ---')
                new_points, alignment = getNewPointsFromCloud(base_cloud, point_cloud, match_limit)
                if new_points is not None:
                    completed_clouds[i] = True
                    alignments.append(alignment)
                    base_cloud = np.vstack((base_cloud, new_points))
                    print(f'Cloud {sum(completed_clouds)}/{len(completed_clouds)} completed!')
        j += 1

    return base_cloud, alignments

def getNumberOfBeacons(file_name: str, match_limit):
    probe_data = readProbeData(file_name)
    full_beacons, _ = getAllBeaconsFromArray(probe_data, match_limit)
    return len(full_beacons)


def getLargestDifferenceBetweenPoints(file_name: str, match_limit):
    probe_data = readProbeData(file_name)
    _, alignments = getAllBeaconsFromArray(probe_data, match_limit)
    return max([np.abs(p1 - p2).sum() for p1, p2 in combinations(alignments, 2)])


if __name__ == '__main__':
    #print(getNumberOfBeacons('data/2021Day19/puzzle_input.txt', 8))
    print(getLargestDifferenceBetweenPoints('data/2021Day19/puzzle_input.txt', 8))