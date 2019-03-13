using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public abstract class AbstractParkable
    {
        protected bool Parked;
        protected ParkingSpaceId ParkingSpace;
        protected IParkingGarage ParkingGarage;

        public void SetParked(ParkingSpaceId space, IParkingGarage garage)
        {
            ParkingSpace = space;
            ParkingGarage = garage;
            Parked = true;
        }

        public void SetLeft()
        {
            ParkingGarage.UnPark(ParkingSpace);
            ParkingSpace = null;
            ParkingGarage = null;
            Parked = false;
        }
    }
}
