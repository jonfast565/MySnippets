using System;

namespace CodingInterviewQuestions.Hashing
{
    public class HashMap<TKey, TValue>
    {
        public HashMapInternal<TKey, TValue> Table { get; set; }

        public HashMap(int initialSize)
        {
            // new empty list
            Table = new HashMapInternal<TKey, TValue>(initialSize);
        }

        public bool Empty()
        {
            return Table.Empty();
        }

        public void Add(TKey key, TValue value)
        {
            var index = Table.GetBucketIndex(key);
            if (Table[index] != null)
                throw new Exception("Hash collision, cannot insert");

            Table[index] = new HashNode<TKey, TValue>(key, value);

            if (Table.AlmostFull())
            {
                Table.IncreaseSize();
            }
        }

        public TValue Get(TKey key)
        {
            var index = Table.GetBucketIndex(key);
            if (Table[index] == null)
                return default(TValue);

            var node = (HashNode<TKey, TValue>) Table[index];
            return node.Value;
        }

        public void Remove(TKey key)
        {
            var index = Table.GetBucketIndex(key);
            if (Table[index] == null)
                throw new Exception("Cannot remove value that doesn't exist");

            Table[index] = 0;
        }
    }
}
