using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class Car : AbstractParkable, IVehicle
    {
        public int Width { get; } = 4;
        public int Length { get; } = 6;
    }
}
