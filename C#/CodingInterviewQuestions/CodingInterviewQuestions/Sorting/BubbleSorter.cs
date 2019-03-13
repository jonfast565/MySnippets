using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CodingInterviewQuestions.Comparator;

namespace CodingInterviewQuestions.Sorting
{
    public class BubbleSorter<T> : ISortable<T>
    {
        private readonly IComparator<T> _comparator;

        public BubbleSorter(IComparator<T> comparator)
        {
            _comparator = comparator;
        }

        private static void Swap(IList<T> array, int firstIndex, int secondIndex)
        {
            var first = array[firstIndex];
            var temp = array[secondIndex];
            array[firstIndex] = temp;
            array[secondIndex] = first;
        }

        public IEnumerable<T> Sort(IEnumerable<T> unsorted)
        {
            var sortedArray = unsorted.ToArray();
            for (var i = 0; i < sortedArray.Length - 1; i++)
            {
                for (var j = 0; j < sortedArray.Length - i - 1; j++)
                {
                    if (_comparator.Greater(sortedArray[j], sortedArray[j + 1]))
                    {
                        Swap(sortedArray, j, j + 1);
                    }
                }
            }

            return sortedArray;
        }
    }
}
