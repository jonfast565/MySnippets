using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class CompactSpace : IParkingSpace
    {
        public int Width { get; } = 2;
        public bool Occupied { get; } = false;
    }
}
