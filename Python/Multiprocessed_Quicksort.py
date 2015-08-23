# --------------------------------- +
# Threaded Quicksort in Python      |
# (a better version)                |
#                                   |
# Authors: Jon F. and Luke H        |
# Date Modified: Oct 21, 2013       |
#                                   |
# Taken from a Wikipedia article :D |
# --------------------------------- +

# imports
import random
import copy
import multiprocessing
import time
import sys

# uncomment to redirect all printed output to a file
# import sys
# sys.stdout = open('quicksort_test_verbose.txt', 'w')

# global debug value
DEBUG = False

# swap the given values in an array (by value)
def swap(array, idx1, idx2):

    # precondition check
    assert isinstance(array, list)
    assert isinstance(idx1, int)
    assert isinstance(idx2, int)

    # swap
    first = array[idx1]
    temp = array[idx2]
    array[idx1] = temp
    array[idx2] = first

    # debug print
    if DEBUG:
        print("Op:", first, "at index", idx1, "swapped with", temp, "at index", idx2)

# partition function
def partition(array, first, last, pivot_idx=-1):

    # precondition check
    assert isinstance(array, list)
    assert isinstance(first, int)
    assert isinstance(last, int)
    assert isinstance(pivot_idx, int)

    # set the pivot value to the first index if blank
    # and get the pivot value
    if pivot_idx == -1:
        pivot_idx = first
    pivot_val = array[pivot_idx]

    # swap the pivot and the rightmost value
    swap(array, pivot_idx, last)

    # store the first index for later
    store_idx = first

    # iterate through the list
    # (ie. move pivot to end)
    for i in range(first, last - 1):

        # if the item at i is less than the pivot value
        # and thus needs to be sorted...
        if array[i] <= pivot_val:
            # swap the pivot and the stored index
            swap(array, i, store_idx)

            # increment
            store_idx += 1

    # done, so swap the stored index and the rightmost element
    swap(array, store_idx, last)

    # return the final pivot value, after moving last time
    return store_idx

# quicksort function
def quicksort(array, first, last):

    # precondition check
    assert isinstance(array, list)
    assert isinstance(first, int)
    assert isinstance(last, int)
    assert len(array) > 2

    # if the first element is less than the last
    # left value must always be less than the right
    if first < last:

        # choose a pivot element
        # ensure index is an integer (and it should always be the nearest int)
        pivot_elem = int(first + (last - first) / 2)

        # lets check our indices here
        assert first <= pivot_elem
        assert pivot_elem <= last

        # get the new pivot
        pivot_new = partition(array, first, last, pivot_elem)

        # recursively sort smaller and larger elements
        quicksort(array, first, pivot_new - 1)
        quicksort(array, pivot_new + 1, last)

# make a random test list
def make_random_list(size_n, seed_m, limit=1000000):

    # test functions
    assert isinstance(size_n, int)
    assert isinstance(seed_m, int)
    assert seed_m < limit

    # create a list
    rand_list = []

    for i in range(size_n):
        random.seed(seed_m)
        rand_list.append(random.randint(0, limit))

    # done!
    return rand_list

# run our tests with a list limits and specific number
# of tests per iteration
# also set an increment value for list size tests
def run_tests(limit=10000, num_tests=10, inc_val=1000):

    # preconditions
    assert isinstance(limit, int)
    assert isinstance(num_tests, int)
    assert isinstance(inc_val, int)
    assert limit >= inc_val
    assert num_tests >= 1

    # set max recursion depth to a very high number
    sys.setrecursionlimit(limit)

    # define a list size and start value based on limit
    list_size = inc_val
    start_val = random.randint(0, list_size)

    # performing ever increasing test sizes
    while start_val <= limit and list_size <= limit:

        # define time slots
        threaded_time = 0
        non_threaded_time = 0

        # print test message
        print("*** Performing", num_tests, "tests on list of size", list_size, "with start value of", start_val,
              ". ***\n")

        for test in range(num_tests):
            # print the test number
            print("Test #", test + 1, "out of", num_tests, ":")

            # create some input
            test_input_yes_t = make_random_list(list_size, start_val)

            # copy for non-threaded version
            test_input_no_t = copy.deepcopy(test_input_yes_t)

            # now partition out our lists in a threaded manner
            first = 0
            last = len(test_input_yes_t) - 1

            # call the partition method
            br_1 = partition(test_input_yes_t, first, last)

            # break up the results again
            br_2 = partition(test_input_yes_t, first, br_1 - 1)
            br_3 = partition(test_input_yes_t, br_1 + 1, last)

            # call quicksort using processes
            thread_1 = multiprocessing.Process(target=quicksort, args=(test_input_yes_t, first, br_2 - 1))
            thread_2 = multiprocessing.Process(target=quicksort, args=(test_input_yes_t, br_2 + 1, br_1 - 1))
            thread_3 = multiprocessing.Process(target=quicksort, args=(test_input_yes_t, br_1 + 1, br_3 - 1))
            thread_4 = multiprocessing.Process(target=quicksort, args=(test_input_yes_t, br_3 + 1, last))

            # start all processes
            thread_1.start()
            thread_2.start()
            thread_3.start()
            thread_4.start()

            # start processes and timer
            start_time = time.time()
            # call join on processes
            thread_1.join()
            thread_2.join()
            thread_3.join()
            thread_4.join()

            # end time
            end_time = time.time()
            num_seconds_t = end_time - start_time

            # print multiprocess time
            print("Time elapsed (multiprocessed):", num_seconds_t)

            # add to the multiproc time
            threaded_time += num_seconds_t
            num_seconds_t = 0

            # now call the non-threaded quicksort
            # and calculate time differential
            start_time = time.time()
            quicksort(test_input_no_t, first, last)
            end_time = time.time()
            num_seconds_t = end_time - start_time

            # print non-threaded time
            print("Time elapsed (non-multiproc):", num_seconds_t)

            # extra new line for experimental break
            print()

            # Add the time to the non threaded timer
            non_threaded_time += num_seconds_t

        # once finished, perform an average calculation of the average time
        # for each list quicksort based on the number of tests
        threaded_avg = threaded_time / num_tests
        non_threaded_avg = non_threaded_time / num_tests

        # print averages
        print("Multiproc average for", num_tests, "tests on list of size", list_size, "with start value of", start_val,
              "is", threaded_avg, ".")
        print("Non-multiproc average for", num_tests, "tests on list of size", list_size, "with start value of",
              start_val, "is", non_threaded_avg, ".")
        print()

        # write all info to files (might want to write more, considering a lot more info)
        # first open
        file_1 = open('multiproc_results.csv', 'a')
        file_2 = open('non_multiproc_results.csv', 'a')

        # then write
        file_1.write(str(list_size) + ',' + "{0:.3f}".format(threaded_avg) + '\n')
        file_2.write(str(list_size) + ',' + "{0:.3f}".format(non_threaded_avg) + '\n')

        # close
        file_1.close()
        file_2.close()

        # increase the start value
        # and list size
        list_size += inc_val
        start_val = random.randint(0, list_size)


def main():
    # run all this crap...
    # limit = 10000
    print("Starting quicksort tests...\n")
    run_tests()

# call main!
main()



