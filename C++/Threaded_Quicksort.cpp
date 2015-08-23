//
//  main.cpp
//  Quicksort Tests
//
//  Created by Jonathan Fast on 10/25/13.
//  Copyright (c) 2013 CProgTutorial. All rights reserved.
//

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <memory>
#include <functional>

using namespace std;

// debug variable
static bool debug = false;

// prototypes
void swap(long int*, long int, long int);
long int partition(long int*, long int, long int, long int);

// swap function
void swap(long int* array, long int index1, long int index2)
{
    // swap
    long int first = array[index1];
    long int temp = array[index2];
    array[index1] = temp;
    array[index2] = first;
    
    if (debug)
        cout << "Op: " << first << " at index " << index1 << " swapped with " << temp << " at index " << index2 << ".";
}

// partition function
long int partition(long int* array, long int first, long int last, long int pivot_idx) {
    
    // set the pivot value to the first index if blank
    // and get the pivot value
    long int pivot_val = array[pivot_idx];
    
    // swap the pivot and the rightmost value
    swap(array, pivot_idx, last);
    
    // store the first index for later
    long int store_idx = first;
    
    // iterate through the array
    // (ie. move pivot to the end)
    for (auto i = first; i < last - 1; i++)
    {
        // if the item at i is less than the pivot value
        // then it needs to be sorted
        if (array[i] <= pivot_val)
        {
            // swap the pivot and the stored index
            swap(array, i, store_idx);
            
            // increment
            store_idx += 1;
        }
    }
    
    // done so swap the stored index and rightmost element
    swap(array, store_idx, last);
    
    // return the stored index
    return store_idx;
}

void quicksort(long int* array, long int length, long int first, long int last)
{
    // check to see if the length of the array is
    // greater than 2
    // assert(length > 2);
    assert(length > 2);
    
    // if the first element is less than the last
    // left value must always be less than the right
    if (first < last)
    {
        // choose a pivot element
        // ensure the index is always an integer
        //default_random_engine rand;
        //uniform_int_distribution<long int> distrib(first, last);
        //auto generator = bind(distrib, rand);
        long int pivot_elem = ((last - first) / 2) + first;
        
        // assert correct indices
        assert(first <= pivot_elem);
        assert(pivot_elem <= last);
        
        // get the new pivot
        long int pivot_new = partition(array, first, last, pivot_elem);
        
        assert(first <= pivot_new);
        assert(pivot_new <= last);
        
        // long int first_len = (pivot_new - 1) - first;
        // long int second_len = last - (pivot_new + 1);
        // recursively sort smaller and larger elements
        quicksort(array, length, first, pivot_new - 1);
        quicksort(array, length, pivot_new + 1, last);
    }
}

long int* make_random_array(long int size_n, long int seed_m, long int limit=100000)
{
    // assert the seeding value is less than the limit
    assert(seed_m < limit);
    
    // create an array
    long int* rand_list = new long int[size_n];
    
    // create a random seed value and then append the
    // new value to the array
    default_random_engine rand;
    uniform_int_distribution<long int> distrib(0, limit);
    auto generator = bind(distrib, rand);

    for (auto i = 0; i < size_n; i++)
    {
        rand_list[i] = generator();
        //and_list[i] = rand() % limit - 1;
    }
    
    // return the new list
    return rand_list;
}

// run tests with a list, limites, and a specific
// number of tests per iteration
// also set an increment value for list size tests
void run_tests(long int limit, long int num_tests, long int inc_val)
{
    // basic tests
    assert(limit >= inc_val);
    assert(num_tests >= 1);
    
    // get the list size from the increment value
    long int list_size = inc_val;

    default_random_engine rand;
    uniform_int_distribution<long int> distrib(0, list_size);
    auto generator = bind(distrib, rand);
    
    // get the start value from the random generator
    long int start_val = generator();
    
    // create a time representation for the start
    // and end of each thread
    chrono::time_point<chrono::system_clock> threaded_start, threaded_stop, non_threaded_start, non_threaded_stop;
    
    // keep total time for each tests
    double total_threaded_time = 0, total_non_threaded_time = 0;
    
    while (start_val <= limit && list_size <= limit)
    {
        // starting up the tests
        cout << "*** Performing " << num_tests << " tests on list of size " << list_size << " with start value of " << start_val << ". ***" << endl << endl;
        
        // keep track of objects
        unique_ptr<long int> test_input_threaded;
        unique_ptr<long int> test_input_non_threaded;
        
        for (auto current_test = 0; current_test < num_tests; current_test++)
        {
            // give the test number
            cout << "Test #" << current_test + 1 << " out of " << num_tests << ": " << endl;
            
            // create the first test array
            test_input_threaded.reset(make_random_array(list_size, start_val, limit));
            
            // create the second test array by copying the first
            test_input_non_threaded.reset(new long int[list_size]);
            memcpy(test_input_threaded.get(), test_input_non_threaded.get(), list_size);
            
            // now partition out our lists in a threaded manner
            long int first = 0;
            long int last = list_size - 1;
            
            // call out partition method
            long int br_1 = partition(test_input_threaded.get(), first, last, first);
            
            // break up again
            long int br_2 = partition(test_input_threaded.get(), first, br_1 - 1, first);
            long int br_3 = partition(test_input_threaded.get(), br_1 + 1, last, first);
            
            // call quicksort using threads
            // and start all threads automatically
            threaded_start = chrono::system_clock::now();
            
            thread t1([&] {
                quicksort(test_input_threaded.get(), list_size, first, br_2 - 1);
            });
            thread t2([&] {
                quicksort(test_input_threaded.get(), list_size, br_2 + 1, br_1 - 1);
            });
            thread t3([&] {
                quicksort(test_input_threaded.get(), list_size, br_1 + 1, br_3 - 1);
            });
            thread t4([&] {
                quicksort(test_input_threaded.get(), list_size, br_3 + 1, last);
            });
            
            // wait for all threads to finish
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            
            // stop clock
            threaded_stop = chrono::system_clock::now();
            chrono::duration<double> threaded_elapsed = threaded_stop - threaded_start;
            
            // report time and add to the total
            cout << "Time elapsed (threaded): " << threaded_elapsed.count() << "." << endl;
            total_threaded_time += threaded_elapsed.count();
            
            // now call the non-threaded quicksort
            non_threaded_start = chrono::system_clock::now();
            quicksort(test_input_non_threaded.get(), list_size, first, last);
            non_threaded_stop = chrono::system_clock::now();
            chrono::duration<double> non_threaded_elapsed = non_threaded_stop - non_threaded_start;
            
            // print this time and add to total
            cout << "Time elapsed (non-threaded): " << non_threaded_elapsed.count() << "." << endl << endl;
            total_non_threaded_time += non_threaded_elapsed.count();
            
        }
        
        // once finished, perform an average calculation of the average time
        // for each list quicksort based on the number of tests
        double threaded_avg = total_threaded_time / (double) num_tests;
        double non_threaded_avg = total_non_threaded_time / (double) num_tests;
        
        // print averages
        cout << "Threaded average for " << num_tests << " tests on list of size " << list_size << " with start value of " << start_val << " is " << threaded_avg << "." << endl;
        cout << "Non-threaded average for " << num_tests << " tests on list of size " << list_size << " with start value of " << start_val << " is " << non_threaded_avg << "." << endl;
        
        // write the results to files
        ofstream threaded_results("threaded_results.csv", ios::app);
        ofstream non_threaded_results("non_threaded_results.csv", ios::app);
        
        threaded_results << list_size << ", " << threaded_avg << endl;
        non_threaded_results << list_size << ", " << non_threaded_avg << endl;
        threaded_results.close();
        non_threaded_results.close();
        
        // increase the start value and list size
        list_size += inc_val;
        
        // randomly select a new start value
        default_random_engine rand2;
        uniform_int_distribution<long int> distrib2(0, list_size);
        auto generator2 = bind(distrib2, rand2);
        start_val = generator2();
        
        // some cleanup
        test_input_threaded.reset();
        test_input_non_threaded.reset();
    }
}

int main(int argc, const char * argv[])
{
    cout << "Starting quicksort tests!" << endl;
    run_tests(100000, 10, 10000);
    return 0;
}

