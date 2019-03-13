using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Comparator
{
    public class IntegerComparator : IComparator<int>
    {
        public bool Less(int left, int right)
        {
            return left < right;
        }

        public bool LessThanEqual(int left, int right)
        {
            return left <= right;
        }

        public bool Greater(int left, int right)
        {
            return left > right;
        }
    }
}
