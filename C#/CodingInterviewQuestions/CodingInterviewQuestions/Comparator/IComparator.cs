using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Comparator
{
    public interface IComparator<in T>
    {
        bool Less(T left, T right);
        bool LessThanEqual(T left, T right);
        bool Greater(T left, T right);
    }
}
