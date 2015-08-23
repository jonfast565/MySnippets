# Python merge sort implementation
# Jon Fast
# 12/8/2014

# best/worst o(n log n)
def merge_sort(aList):
    assert(type(aList) is list)
    if len(aList) <= 1:
        return aList

    left = []
    right = []

    middleIndex = len(aList) / 2

    for i in range(len(aList)):
        if i < middleIndex:
            left.append(aList[i])
        else:
            right.append(aList[i])
        
    left = merge_sort(left)
    right = merge_sort(right)

    return merge(left, right)

def firstOf(aList):
    assert(type(aList) is list)
    assert(len(aList) > 0)
    return aList[0]

def restOf(aList):
    assert(type(aList) is list)
    if len(aList) == 0:
        return []
    else:
        return aList[1: len(aList)]
    

def merge(left, right):
    assert(type(left) is list)
    assert(type(right) is list)
    result = []

    while len(left) > 0 or len(right) > 0:
        if len(left) > 0 and len(right) > 0:
            if firstOf(left) <= firstOf(right):
                result.append(firstOf(left))
                left = restOf(left)
            else:
                result.append(firstOf(right))
                right = restOf(right)
        elif len(left) > 0:
            result.append(firstOf(left))
            left = restOf(left)
        elif len(right) > 0:
            result.append(firstOf(right))
            right = restOf(right)

    return result

# quicksort: best is O(n log n), worst is O(n^2)
def quicksort(aList, first, last):
    assert(type(aList) is list)
    assert(type(first) is int)
    assert(type(last) is int)

def main():
    print(merge_sort([1, 3, 2, 7, 6, 9, 10, 20, 19, 100, 45, 50, 23, 22, 80, 11]))

main()
