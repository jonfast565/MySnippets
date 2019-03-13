using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodingInterviewQuestions.Hashing
{
    public class HashMapInternal<TKey, TValue>
    {
        public ArrayList Table { get; set; }
        public int Size { get; set; }

        public object this[int index]
        {
            get => Table[index];
            set => Table[index] = value;
        }

        public HashMapInternal(int size)
        {
            Size = size;
            Table = new ArrayList();
            InitializeCapacity();
        }

        public int GetBucketIndex(object key)
        {
            var hashCode = Math.Abs(key.GetHashCode());
            var index = hashCode % Size;
            return index;
        }

        // ReSharper disable once SuggestBaseTypeForParameter
        private void InitializeCapacity()
        {
            for (var i = 0; i < Size; i++)
            {
                Table.Add(null);
            }
        }

        public bool Empty()
        {
            return Size == 0;
        }

        private static void Copy(HashMapInternal<TKey, TValue> s1, HashMapInternal<TKey, TValue> s2)
        {
            foreach (var item in s1.Table)
            {
                if (item == null) continue;

                var value = (HashNode<TKey, TValue>) item;
                var index = s2.GetBucketIndex(value.Key);
                s2.Table[index] = value;
            }
        }

        private void SmashTogether(HashMapInternal<TKey, TValue> s)
        {
            Table = s.Table;
            Size = s.Size;
        }

        public void IncreaseSize()
        {
            var newSize = Size + (int) (Size * 0.50);
            var newTable = new HashMapInternal<TKey, TValue>(newSize);
            Copy(this, newTable);
            SmashTogether(newTable);
        }

        public bool AlmostFull()
        {
            var tableCount = Table.ToArray().Select(x => x != null).Count();
            var maxSizePercentage = (int)(0.75 * Size);
            return  tableCount >= maxSizePercentage;
        }
    }
}
