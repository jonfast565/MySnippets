using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CodingInterviewQuestions.Comparator;

namespace CodingInterviewQuestions.Sorting
{
    public class MergeSorter<T> : ISortable<T>
    {
        private readonly IComparator<T> _comparator;

        public MergeSorter(IComparator<T> comparator)
        {
            _comparator = comparator;
        }

        private T[] Merge(T[] firstArray, T[] secondArray)
        {
            if (firstArray.Length == 0
                && secondArray.Length == 0)
                return new T[] { };

            if (firstArray.Length == 0)
                return secondArray;

            if (secondArray.Length == 0)
                return firstArray;

            var resultArray = new T[firstArray.Length + secondArray.Length];
            var resultCounter = 0;

            var firstArrayCounter = 0;
            var secondArrayCounter = 0;
            while (firstArrayCounter < firstArray.Length
                   && secondArrayCounter < secondArray.Length)
            {
                var firstValue = firstArray[firstArrayCounter];
                var secondValue = secondArray[secondArrayCounter];

                if (_comparator.LessThanEqual(firstValue, secondValue))
                {
                    resultArray[resultCounter] = firstValue;
                    firstArrayCounter++;
                }
                else
                {
                    resultArray[resultCounter] = secondValue;
                    secondArrayCounter++;
                }

                resultCounter++;
            }

            while (firstArrayCounter < firstArray.Length)
            {
                var firstValue = firstArray[firstArrayCounter];
                resultArray[resultCounter] = firstValue;
                firstArrayCounter++;
                resultCounter++;
            }

            while (secondArrayCounter < secondArray.Length)
            {
                var secondValue = secondArray[secondArrayCounter];
                resultArray[resultCounter] = secondValue;
                secondArrayCounter++;
                resultCounter++;
            }

            Debug.Assert(resultCounter == resultArray.Length);

            return resultArray;
        }

        private T[] SortInternal(T[] unsorted)
        {
            if (unsorted.Length <= 1)
                return unsorted;

            const int leftIndex = 0;
            var midPoint = (unsorted.Length - 1) / 2;
            var rightIndex = unsorted.Length - 1;

            var leftArray = Slice(unsorted, leftIndex, midPoint);
            var rightArray = Slice(unsorted, midPoint + 1, rightIndex);
            var firstResult = SortInternal(leftArray.ToArray());
            var secondResult = SortInternal(rightArray.ToArray());
            var result = Merge(firstResult, secondResult);

            return result;
        }

        private static IEnumerable<T> Slice(IReadOnlyList<T> unsorted, int leftIndex, int rightIndex)
        {
            var result = new List<T>();
            for (var i = leftIndex; i <= rightIndex; i++)
            {
                result.Add(unsorted[i]);
            }

            return result.ToArray();
        }

        public IEnumerable<T> Sort(IEnumerable<T> unsorted)
        {
            var unsortedArray = unsorted.ToArray();
            var sortedArray = SortInternal(unsortedArray);
            return sortedArray;
        }
    }
}
