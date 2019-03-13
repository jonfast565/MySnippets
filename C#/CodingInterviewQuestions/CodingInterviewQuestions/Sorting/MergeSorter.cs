using System;
using System.Collections.Generic;
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

        private void Merge(T[] resultArray, int leftIndex, int middleIndex, int rightIndex)
        {
            var leftArray = new T[middleIndex - leftIndex + 1];
            var rightArray = new T[rightIndex - middleIndex];

            Array.ConstrainedCopy(resultArray, 
                leftIndex, 
                leftArray, 
                0, 
                leftArray.Length);

            Array.ConstrainedCopy(resultArray, 
                middleIndex + 1, 
                rightArray, 
                0, 
                rightArray.Length);

            int leftCounter = 0, rightCounter = 0, mergeCounter = leftIndex;

            // get the bulk of it
            while (leftCounter < leftArray.Length && rightCounter < rightArray.Length)
            {
                if (_comparator.LessThanEqual(leftArray[leftCounter], rightArray[rightCounter]))
                {
                    // populate with i value and move forward by 1
                    resultArray[mergeCounter] = leftArray[leftCounter];
                    leftCounter++;
                }
                else
                {
                    // populate with j value and move forward by 1
                    resultArray[mergeCounter] = rightArray[rightCounter];
                    rightCounter++;
                }

                // move k forward, as it has a new value
                mergeCounter++;
            }

            // get the remaining elements from the left resultArray
            while (leftCounter < leftArray.Length)
            {
                resultArray[mergeCounter] = leftArray[leftCounter];
                leftCounter++;
                mergeCounter++;
            }

            // get the remaining elements from the right resultArray
            while (rightCounter < rightArray.Length)
            {
                resultArray[mergeCounter] = rightArray[rightCounter];
                rightCounter++;
                mergeCounter++;
            }
        }

        public void SortInternal(T[] unsorted, int leftIndex, int rightIndex)
        {
            if (leftIndex >= rightIndex) return;
            var midPoint = leftIndex + (rightIndex - 1) / 2;
            SortInternal(unsorted, leftIndex, midPoint);
            SortInternal(unsorted, midPoint + 1, rightIndex);
            Merge(unsorted, leftIndex, midPoint, rightIndex);
        }

        public IEnumerable<T> Sort(IEnumerable<T> unsorted)
        {
            var sortedArray = unsorted.ToArray();
            SortInternal(sortedArray, 0, sortedArray.Length - 1);
            return sortedArray;
        }
    }
}
