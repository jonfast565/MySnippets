using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Searching
{
    public interface ISearchable<in T>
    {
        bool SearchFor(IEnumerable<T> searchable, T needle);
    }
}
