
from PythonModules.Year2021Day18 import (
    SnailFishNumber, readAllNumbers, 
    sumNumberList, getMaximumPairMagnitude
)
import pytest


def test_SnailFishNumber__given_listInput__returns_numberWithExpectedStr():
    # Arrange
    nums = (
        1,
        [[1, 9], [8, 5]],
        [[1, 2], 3]
    )

    # Act & Assert
    for value in nums:
        assert(SnailFishNumber(value) == value)
        assert(SnailFishNumber(value) == SnailFishNumber(value))


def test_SnailFishNumber__given_iterationOfTree__returns_depthFirstLeftFirstSearch():
    # Arrange
    num = SnailFishNumber([[1, 2], [3, 4]])
    expected_traversal = [1, 2, 3, 4]

    # Act & Assert
    i = 0
    for leaf in num:
        assert(leaf.value == expected_traversal[i])
        i += 1
    
    assert(i == 4)


def test_SnailFishNumber_incrementDepth__returns_deeperTree():
    # Arrange
    num = SnailFishNumber([[1, 2], 3])
    expected_depth = [3, 3, 2]

    # Act
    num.incrementDepth()

    # Assert
    i = 0
    for leaf in num:
        assert(leaf.depth == expected_depth[i])
        i += 1

    assert(i == 3)


def test_SnailFishNumber_explode_given_treeDepth4__returns_sameTree():
    # Arrange
    depth_four_list = [[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]
    num = SnailFishNumber(depth_four_list)

    # Act
    result = num.explode()

    # Assert
    assert(result == False)
    assert(num == depth_four_list)


def test_SnailFishNumber_explode_given_needsToFuseRight__returns_explodedTree():
    # Arrange
    depth_five_list = [[[[[9,8],1],2],3],4]
    expected_result = [[[[0,9],2],3],4]
    num = SnailFishNumber(depth_five_list)

    # Act
    result = num.explode()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_explode_given_needsToFuseLeft__returns_explodedTree():
    # Arrange
    depth_five_list = [7,[6,[5,[4,[3,2]]]]]
    expected_result = [7,[6,[5,[7,0]]]]
    num = SnailFishNumber(depth_five_list)

    # Act
    result = num.explode()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_explode_given_needsToFuseRightLeft__returns_explodedTree():
    # Arrange
    depth_five_list = [[6,[5,[4,[3,2]]]],1]
    expected_result = [[6,[5,[7,0]]],3]
    num = SnailFishNumber(depth_five_list)

    # Act
    result = num.explode()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_explode_given_needsTwoExposions__returns_treeWithOneExplosion():
    # Arrange
    depth_five_list = [[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]
    expected_result = [[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]
    num = SnailFishNumber(depth_five_list)

    # Act
    result = num.explode()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_split_given_needsOneSplit__returns_splitTree():
    # Arrange
    high_value_list = [[[[0,7],4],[[7,8],[0,12]]],[1,1]]
    expected_result = [[[[0,7],4],[[7,8],[0,[6,6]]]],[1,1]]
    num = SnailFishNumber(high_value_list)

    # Act
    result = num.split()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_split_given_needsTwoSplits__returns_treeWithOneSplit():
    # Arrange
    high_value_list = [[[[0,7],4],[15,[0,13]]],[1,1]]
    expected_result = [[[[0,7],4],[[7,8],[0,13]]],[1,1]]
    num = SnailFishNumber(high_value_list)

    # Act
    result = num.split()

    # Assert
    assert(result == True)
    assert(num == expected_result)


def test_SnailFishNumber_add_given_twoNumbers__returns_expectedNumber():
    # Arrange
    num_1 = SnailFishNumber([[[[4,3],4],4],[7,[[8,4],9]]])
    num_2 = SnailFishNumber([1, 1])
    expected_result = [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]

    # Act
    result = num_1 + num_2

    # Assert
    assert(result == expected_result)


def test_SnailFishNumber_reduce_given_number__returns_expectedNumber():
    # Arrange
    num = SnailFishNumber([[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]])
    expected_result = [[[[0,7],4],[[7,8],[6,0]]],[8,1]]

    # Act
    num.reduce()

    # Assert
    assert(num == expected_result)


def test_SnailFishNumber_magnitude_given_number__returns_expectedMagnitude():
    # Arrange
    magnitude_nums = [
        [[1,2],[[3,4],5]],
        [[[[0,7],4],[[7,8],[6,0]]],[8,1]],
        [[[[5,0],[7,4]],[5,5]],[6,6]],
        [[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]
    ]
    expected_results = [143, 1384, 1137, 3488]

    # Act
    results = [SnailFishNumber(num).magnitude() for num in magnitude_nums]

    # Assert
    for result, expected in zip(results, expected_results):
        assert(result == expected)


def test_readAllNumbers__given_smallList__returns_expectedNumbers():
    path = 'data/2021Day18/small_list.txt'
    expected_results = ([1,1], [2,2], [3,3], [4,4], [5,5], [6,6])

    # Act
    nums = readAllNumbers(path)

    for result, expected in zip(nums, expected_results):
        assert(result == expected)


def test_readAllNumbers__given_smallList__returns_expectedNumbers():
    path = 'data/2021Day18/small_list.txt'
    expected_results = ([1,1], [2,2], [3,3], [4,4], [5,5], [6,6])

    # Act
    nums = readAllNumbers(path)

    for result, expected in zip(nums, expected_results):
        assert(result == expected)


def test_sumNumberList__given_smallList2__returns_expectedResult():
    path = 'data/2021Day18/small_list2.txt'
    expected_result = [[[[0,7],4],[[7,8],[6,0]]],[8,1]]

    # Act & Assert
    assert(sumNumberList(path) == expected_result)


def test_sumNumberList__given_smallList1__returns_expectedResult():
    path = 'data/2021Day18/small_list.txt'
    expected_result = [[[[5,0],[7,4]],[5,5]],[6,6]]

    # Act & Assert
    assert(sumNumberList(path) == expected_result)

def test_sumNumberList__given_example2List__returns_expectedResult():
    path = 'data/2021Day18/example2.txt'
    expected_result = [[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]

    # Act & Assert
    assert(sumNumberList(path) == expected_result)


def test_sumNumberList__given_exampleMagnitude__returns_expectedResult():
    path = 'data/2021Day18/example.txt'
    expected_magnitude = 4140

    # Act & Assert
    assert(sumNumberList(path).magnitude() == expected_magnitude)


def test_getMaximumPairMagnitude__given_exampleMagnitudes__returns_expectedResult():
    path = 'data/2021Day18/example.txt'
    expected_magnitude = 3993

    # Act & Assert
    assert(getMaximumPairMagnitude(path) == expected_magnitude)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])