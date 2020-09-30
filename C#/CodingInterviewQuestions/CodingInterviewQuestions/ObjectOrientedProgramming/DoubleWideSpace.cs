using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class DoubleWideSpace : IParkingSpace
    {
        public int Width { get; } = 10;
        public bool Occupied { get; } = false;
        public ParkingSpaceId GetSpaceId()
        {
            throw new NotImplementedException();
        }
    }
}
