using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class RegularSpace : IParkingSpace
    {
        private readonly Guid _spaceId = Guid.NewGuid();

        public int Width { get; } = 4;
        public bool Occupied { get; } = false;

        public ParkingSpaceId GetSpaceId()
        {
            return new ParkingSpaceId {Value = _spaceId};
        }
    }
}
