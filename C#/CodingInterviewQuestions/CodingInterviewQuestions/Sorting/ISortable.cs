using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Sorting
{
    public interface ISortable<T>
    {
        IEnumerable<T> Sort(IEnumerable<T> unsorted);
    }
}
