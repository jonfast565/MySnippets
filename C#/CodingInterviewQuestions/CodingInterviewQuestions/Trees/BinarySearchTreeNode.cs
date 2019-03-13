using System;
using System.Collections.Generic;
using System.Text;
using CodingInterviewQuestions.Comparator;

namespace CodingInterviewQuestions.Trees
{
    public class BinarySearchTreeNode<T> : ITreeNode<T>
    {
        private readonly IComparator<T> _comparator;

        private T _value;

        private BinarySearchTreeNode<T> _leftNode;

        private BinarySearchTreeNode<T> _rightNode;

        private BinarySearchTreeNode(
            IComparator<T> comparator, 
            BinarySearchTreeNode<T> leftNode, 
            BinarySearchTreeNode<T> rightNode, 
            T value)
        {
            _comparator = comparator;
            _leftNode = leftNode;
            _rightNode = rightNode;
            _value = value;
        }

        public BinarySearchTreeNode(IComparator<T> comparator)
        {
            _comparator = comparator;
            _leftNode = null;
            _rightNode = null;
            _value = default(T);
        }

        public void InsertValue(T value)
        {
            InsertValueInternal(value, this);
        }

        private static void InsertValueInternal(T value, BinarySearchTreeNode<T> currentNode)
        {
            if (EqualityComparer<T>.Default.Equals(currentNode._value, default(T)))
            {
                currentNode._value = value;
                return;
            }

            if (currentNode._comparator.Less(value, currentNode._value))
            {
                if (currentNode._leftNode == null)
                {
                    currentNode._leftNode = new BinarySearchTreeNode<T>(currentNode._comparator, null, null, value);
                }
                else
                {
                    InsertValueInternal(value, currentNode._leftNode);
                }
            }
            else if (currentNode._comparator.Greater(value, currentNode._value))
            {
                if (currentNode._rightNode == null)
                {
                    currentNode._rightNode = new BinarySearchTreeNode<T>(currentNode._comparator, null, null, value);
                }
                else
                {
                    InsertValueInternal(value, currentNode._rightNode);
                }
            }
            else
            {
                currentNode._value = value;
            }
        }

        public void DeleteValue(T value)
        {
            throw new NotImplementedException();
        }

        public bool FindValue(T value)
        {
            return false;
        }
    }
}
