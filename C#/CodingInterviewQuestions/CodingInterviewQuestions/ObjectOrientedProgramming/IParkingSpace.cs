using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public interface IParkingSpace
    {
        int Width { get; }
        bool Occupied { get; }
        ParkingSpaceId GetSpaceId();
    }
}
