
from itertools import permutations

MAX_ITERATIONS = 1000

class SnailFishNumber:
    def __init__(self, num: list, parent=None, depth=0):
        self.parent = parent
        self.depth = depth

        if isinstance(num, int) or num is None:
            self.value = num
            self.left = None
            self.right = None
        else:
            self.value = None
            self.left = SnailFishNumber(num[0], parent=self, depth=depth+1)
            self.right = SnailFishNumber(num[1], parent=self, depth=depth+1)

    def incrementDepth(self):
        self.depth += 1
        if self.value is None:
            self.left.incrementDepth()
            self.right.incrementDepth()

    def explode(self) -> bool:
        last_leaf = None
        exploded_node = None
        add_to_last_leaf = False
        for leaf in self:
            if leaf.depth == 5:
                if exploded_node is None:
                    if last_leaf is not None:
                        last_leaf.value += leaf.value
                    exploded_node = leaf.parent
                    add_to_last_leaf = True
            if add_to_last_leaf and leaf.parent is not exploded_node:
                leaf.value += last_leaf.value
                add_to_last_leaf = False
                    
            last_leaf = leaf

        explosion_made = exploded_node is not None
        if explosion_made:
            fuse_parent = exploded_node.parent
            if exploded_node is fuse_parent.left:
                fuse_parent.left = SnailFishNumber(0, parent=fuse_parent, depth=4)
            elif exploded_node is fuse_parent.right:
                fuse_parent.right = SnailFishNumber(0, parent=fuse_parent, depth=4)
            else:
                print("Parent is not connected to child!")

        return explosion_made

    def split(self) -> bool:
        for leaf in self:
            if leaf.value > 9:
                left_new_value = leaf.value // 2
                right_new_value = leaf.value - left_new_value

                leaf.left = SnailFishNumber(left_new_value, parent=leaf, depth=leaf.depth+1)
                leaf.right = SnailFishNumber(right_new_value, parent=leaf, depth=leaf.depth+1)
                leaf.value = None
                return True
            
        return False

    def reduce(self):
        iterations = 0
        while True:
            iterations += 1
            if iterations > MAX_ITERATIONS:
                print('Max iterations reached')
                break

            if self.explode():
                continue
            if self.split():
                continue
            break

    def magnitude(self) -> int:
        if self.value is not None:
            total_magnitude = self.value
        else:
            total_magnitude = 3 * self.left.magnitude() + 2 * self.right.magnitude()
        return total_magnitude

    def copy(self):
        return SnailFishNumber(eval(str(self)))

    def __eq__ (self, other):
        return str(self) == str(other)

    def __add__(self, other):
        new_root = SnailFishNumber(None)
        self_new = self.copy()
        other_new = other.copy()
        self_new.parent = new_root
        other_new.parent = new_root
        self_new.incrementDepth()
        other_new.incrementDepth()

        new_root.left = self_new
        new_root.right = other_new
        return new_root

    def __radd__(self, other):
        new_root = SnailFishNumber(None)
        self.parent = new_root
        other.parent = new_root
        self.incrementDepth()
        other.incrementDepth()

        new_root.left = self
        new_root.right = other
        return new_root

    def __str__(self):
        if self.value is not None:
            return str(self.value)
        else:
            return f"[{self.left}, {self.right}]"

    def __repr__(self):
        return f"SnailFishNumber({self}, depth={self.depth})"

    def __iter__(self):
        if self.value is not None:
            yield self
        else:
            for num in self.left:
                yield num
            for num in self.right:
                yield num


def readAllNumbers(txt_list: str) -> list:
    with open(txt_list, "r") as f:
        output = f.readlines()
    return [SnailFishNumber(eval(num)) for num in output]


def sumNumberList(txt_list: str) -> SnailFishNumber:
    nums = readAllNumbers(txt_list)
    working_num = nums[0]
    for num in nums[1:]:
        working_num += num
        working_num.reduce()
    return working_num


def getMaxTotalOfTwoNums(num1, num2, max_magnitude) -> int:
    num_total = num1 + num2
    num_total.reduce()
    return max(max_magnitude, num_total.magnitude())


def getMaximumPairMagnitude(txt_list: str) -> int:
    nums = readAllNumbers(txt_list)
    max_magnitude = 0
    for num1, num2 in list(permutations(nums, 2)):
        max_magnitude = getMaxTotalOfTwoNums(num1, num2, max_magnitude)
    return max_magnitude


if __name__ == '__main__':
    print('This should work')
    #print(sumNumberList('data/2021Day18/puzzle_input.txt').magnitude())
    print(getMaximumPairMagnitude('data/2021Day18/puzzle_input.txt'))
