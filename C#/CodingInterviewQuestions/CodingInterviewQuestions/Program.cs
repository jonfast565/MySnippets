using System;
using CodingInterviewQuestions.Comparator;
using CodingInterviewQuestions.Hashing;
using CodingInterviewQuestions.Searching;
using CodingInterviewQuestions.Sorting;

namespace CodingInterviewQuestions
{
    class Program
    {
        static void Main(string[] args)
        {
            // HashTest();
            QuickSortTest();
            BubbleSortTest();
            MergeSortTest();
            BinarySearchTest();

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
    }
}
