
from PythonModules.Year2021Day20 import (
    readImageEhanceData, convertBinaryToInteger, extentBinaryImage, runEhancement
)
import pytest
import numpy as np

def test_readImageEhanceData__given_smallInputData__returns_expectedShapes():
    # Act
    program, image = readImageEhanceData('data/2021Day20/small_input.txt')

    # Assert
    assert(len(program) == 512)
    assert(image.shape == (5, 5))


def test_convertBinaryToInteger__given_binarYArray__returns_expectedInt():
    # Arrange
    arr = np.array([False, False, False, True, False, False, False, True, False])

    # Act & Assert
    assert(convertBinaryToInteger(arr) == 34)


def test_extentBinaryImage__given_image_returns__extendedImage():
    # Arrange
    image = np.ones((3, 3), dtype=bool)

    # Act
    extended_image = extentBinaryImage(image)

    # Assert
    assert(extended_image.shape == (7, 7))
    assert(extended_image.sum() == 9)


def test_runEhancement__given_smallInput__returns_expectedTotals():
    # Arrange
    program, image = readImageEhanceData('data/2021Day20/small_input.txt')

    # Act
    enchanced = runEhancement(image, program)
    enchanced = runEhancement(enchanced, program)

    # Assert
    assert(enchanced.sum() == 35)


if __name__ == '__main__':
    pytest.main([__file__, '-x', '--verbose'])
