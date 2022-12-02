
import numpy as np
import matplotlib.pyplot as plt


def convertLineToBinaryArray(line: str):
    return np.array([x == "#" for x in line[:-1]], dtype=bool)


def readImageEhanceData(file_name: str):
    with open(file_name,"r") as f:
        lines = f.readlines()

    binaryProgram = convertLineToBinaryArray(lines[0])
    image = np.vstack([convertLineToBinaryArray(x) for x in lines[2:]])
    return binaryProgram, image


def convertBinaryToInteger(arr):
    output = 0
    exponent = 1
    for x in reversed(arr):
        output += int(x) * exponent
        exponent *= 2
    return output


def extentBinaryImage(image, default):
    np_init = np.ones if default else np.zeros
    height, width = image.shape
    extend_image = np.vstack([
        np_init((2, width), dtype=bool),
        image,
        np_init((2, width), dtype=bool)
    ])
    extend_image = np.hstack([
        np_init((height + 4, 2), dtype=bool), 
        extend_image,
        np_init((height + 4, 2), dtype=bool)
    ])
    return extend_image


def runEhancement(image, program, default=False):
    origin_height, origin_width = image.shape
    extended_image = extentBinaryImage(image, default)
    height, width = extended_image.shape
    output_image = np.zeros((origin_height+2, origin_width+2), dtype=bool)
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            roi = extended_image[i-1:i+2, j-1:j+2].flatten()
            result = convertBinaryToInteger(roi)
            output_image[i-1, j-1] = program[result]
    return output_image


def showBinaryImage(image):
    plt.imshow(image.astype(float))
    plt.show()


if __name__ == '__main__':
    program, image = readImageEhanceData('data/2021Day20/puzzle_input.txt')
    showBinaryImage(image)

    enchanced = image
    for i in range(50):
        print(i)
        enchanced = runEhancement(enchanced, program, default=(i %  2 == 1))
    
    print(enchanced.sum())