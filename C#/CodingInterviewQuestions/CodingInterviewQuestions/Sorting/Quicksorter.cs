using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using CodingInterviewQuestions.Comparator;

namespace CodingInterviewQuestions.Sorting
{
    public class QuickSorter<T> : ISortable<T>
    {
        private readonly IComparator<T> _comparator;

        public QuickSorter(IComparator<T> comparator)
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

        private int Partition(IList<T> array, int lowIndex, int highIndex)
        {
            var pivotValue = array[highIndex];
            var i = lowIndex - 1;

            for (var j = lowIndex; j <= highIndex - 1; j++)
            {
                if (!_comparator.LessThanEqual(array[j], pivotValue)) continue;
                i++;
                Swap(array, i, j);
            }
            Swap(array, i + 1, highIndex);
            return i + 1;
        }

        private void SortInternal(IList<T> array, int lowIndex, int highIndex)
        {
            if (lowIndex >= highIndex) return;

            var partitionIndex = Partition(array, lowIndex, highIndex);
            SortInternal(array, lowIndex, partitionIndex - 1);
            SortInternal(array, partitionIndex + 1, highIndex);
        }

        public IEnumerable<T> Sort(IEnumerable<T> unsorted)
        {
            var sortedArray = unsorted.ToArray();
            SortInternal(sortedArray, 0, sortedArray.Length - 1);
            return sortedArray;
        }
    }
}
