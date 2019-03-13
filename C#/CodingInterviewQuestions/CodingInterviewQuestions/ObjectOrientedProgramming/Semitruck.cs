using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class SemiTruck : AbstractParkable, IVehicle
    {
        public int Width { get; } = 8;
        public int Length { get; } = 20;

        public void Park(IParkingGarage garage)
        {
            ParkingSpace = garage.Park(this);
            Parked = true;
        }

        public void UnPark()
        {
            throw new NotImplementedException();
        }
    }
}
