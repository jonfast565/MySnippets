using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.Hashing
{
    public class HashNode<TKey, TValue>
    {
        public HashNode(TKey key, TValue value)
        {
            Key = key;
            Value = value;
        }

        public TKey Key { get; internal set; }
        public TValue Value { get; internal set; }
    }
}
