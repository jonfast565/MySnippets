using System;
using System.Collections.Generic;
using System.Linq;
using CodingInterviewQuestions.Comparator;
using CodingInterviewQuestions.Sorting;

namespace CodingInterviewQuestions.Searching
{
    public class BinarySearcher<T> : ISearchable<T>
    {
        private readonly IComparator<T> _comparator;
        private readonly ISortable<T> _sorter;

        public BinarySearcher(IComparator<T> comparator, ISortable<T> sorter)
        {
            _comparator = comparator;
            _sorter = sorter;
        }

        public bool Sorted(IEnumerable<T> sortable)
        {
            var array = sortable.ToArray();
            for (var i = 0; i < array.Length - 1; i++)
            {
                var isLess = _comparator.Less(array[i], array[i + 1]);
                if (!isLess)
                    return false;
            }
            return true;
        }

        public IEnumerable<T> Sort(IEnumerable<T> array, ISortable<T> sorter)
        {
            return sorter.Sort(array);
        }

        public bool SearchFor(IEnumerable<T> searchable, T needle)
        {
            var searchableArray = searchable as T[] ?? searchable.ToArray();
            if (!Sorted(searchableArray))
                searchableArray = Sort(searchableArray, _sorter).ToArray();

            return SearchForInternal(searchableArray, searchableArray.Length, needle);
        }

        private bool SearchForInternal(IReadOnlyList<T> searchable, int searchZone, T needle)
        {
            var leftBound = 0;
            var rightBound = searchZone - 1;

            while (leftBound <= rightBound)
            {
                var midPoint = (double) (leftBound + rightBound) / 2;
                var flooredMidPoint = (int) Math.Floor(midPoint);
                var valueAtMidpoint = searchable[flooredMidPoint];

                if (_comparator.Less(valueAtMidpoint, needle))
                {
                    leftBound = flooredMidPoint + 1;
                }
                else if (_comparator.Greater(valueAtMidpoint, needle))
                {
                    rightBound = flooredMidPoint - 1;
                }
                else
                {
                    // can we find it? yes we can!
                    return true;
                }
            }

            // failed af
            return false;
        }
    }
}
