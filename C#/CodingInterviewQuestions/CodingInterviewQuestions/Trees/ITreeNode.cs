using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Trees
{
    public interface ITreeNode<T>
    {
        void InsertValue(T value);
        void DeleteValue(T value);
        bool FindValue(T value);
    }
}
