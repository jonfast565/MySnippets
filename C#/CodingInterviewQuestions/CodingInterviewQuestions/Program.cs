using System;
using System.Collections.Generic;
using CodingInterviewQuestions.Comparator;
using CodingInterviewQuestions.Hashing;
using CodingInterviewQuestions.Numerical;
using CodingInterviewQuestions.ObjectOrientedProgramming;
using CodingInterviewQuestions.Searching;
using CodingInterviewQuestions.Sorting;
using CodingInterviewQuestions.Trees;

namespace CodingInterviewQuestions
{
    class Program
    {
        static void Main(string[] args)
        {
            // HashTest();
            // QuickSortTest();
            // BubbleSortTest();
            // MergeSortTest();
            // BinarySearchTest();
            // BinarySearchTreeTest();
            // OopTest();
            NumericalManipulationTest();

            // wait until the application is finished
            Console.ReadKey();
        }

        private static void HashTest()
        {
            var hashMap = new HashMap<string, string>(10);
            hashMap.Add("something", "something");
            hashMap.Add("blah", "blah");
            hashMap.Add("ackackack", "ackackack");
            var value = hashMap.Get("something");
            Console.WriteLine(value);
        }

        private static void QuickSortTest()
        {
            var elements = new[] {6, 5, 4, 3, 2, 1, 0};
            var sorter = new QuickSorter<int>(new IntegerComparator());
            var sorted = sorter.Sort(elements);
            Console.WriteLine("[" + string.Join(", ", sorted) + "]");
        }

        private static void MergeSortTest()
        {
            var elements = new[] { 6, 5, 4, 3, 2, 1, 0 };
            var sorter = new MergeSorter<int>(new IntegerComparator());
            var sorted = sorter.Sort(elements);
            Console.WriteLine("[" + string.Join(", ", sorted) + "]");
        }

        private static void BubbleSortTest()
        {
            var elements = new[] { 6, 5, 4, 3, 2, 1, 0 };
            var sorter = new BubbleSorter<int>(new IntegerComparator());
            var sorted = sorter.Sort(elements);
            Console.WriteLine("[" + string.Join(", ", sorted) + "]");
        }

        private static void BinarySearchTest()
        {
            var elements = new[] { 6, 5, 4, 3, 2, 1, 0 };
            var searcher =
                new BinarySearcher<int>(
                    new IntegerComparator(), 
                    new QuickSorter<int>(new IntegerComparator()));
            var found10 = searcher.SearchFor(elements, 10);
            var found6 = searcher.SearchFor(elements, 6);
            Console.WriteLine("Found 10: " + found10);
            Console.WriteLine("Found 6: " + found6);
        }

        private static void BinarySearchTreeTest()
        {
            var elements = new[] { 100, 50, 150, 7, 6, 9, 8 };
            var searchTree = new BinarySearchTreeNode<int>(new IntegerComparator());
            foreach (var element in elements)
            {
                searchTree.InsertValue(element);
            }
            Console.WriteLine(searchTree);
        }

        private static void OopTest()
        {
            var parkingGarage = ParkingGarage.CreateParkingGarageWithSpaceCounts(new Dictionary<SpaceType, int>
            {
                { SpaceType.Regular, 10 },
                { SpaceType.Compact, 1 },
                { SpaceType.DoubleWide, 1 }
            });
            var motorcycle = new Motorcycle();
            var car = new Car();
            var semi = new SemiTruck();

            parkingGarage.Park(motorcycle);

        }

        private static void NumericalManipulationTest()
        {
            const string num1 = "999";
            const string num2 = "99";
            var results = NumericalManipulation.Add(num1, num2);
            Console.WriteLine(results);
        }
    }
}
