using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class ParkingGarage : IParkingGarage
    {
        private readonly IList<IParkingSpace> _spaces;

        private ParkingGarage(IList<IParkingSpace> spaces)
        {
            _spaces = spaces;
        }

        public static IParkingGarage CreateParkingGarageWithSpaceCounts(IDictionary<SpaceType, int> typeCount)
        {
            var spaceList = new List<IParkingSpace>();

            foreach (var type in typeCount)
            {
                switch (type.Key)
                {
                    case SpaceType.Regular:
                        spaceList.Add(new RegularSpace());
                        break;
                    case SpaceType.Compact:
                        spaceList.Add(new CompactSpace());
                        break;
                    case SpaceType.DoubleWide:
                        spaceList.Add(new DoubleWideSpace());
                        break;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(typeCount));
                }
            }

            return new ParkingGarage(spaceList);
        }

        public ParkingSpaceId Park(AbstractParkable vehicle)
        {
            vehicle.SetParked(,this);
        }

        public void UnPark(ParkingSpaceId space)
        {
            
        }
    }
}
