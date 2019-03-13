using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public interface IParkingGarage
    {
        ParkingSpaceId Park(AbstractParkable vehicle);
        void UnPark(ParkingSpaceId space);
    }
}
