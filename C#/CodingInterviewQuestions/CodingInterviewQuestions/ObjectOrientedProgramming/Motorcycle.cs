using System;
using System.Collections.Generic;
using System.Text;

namespace CodingInterviewQuestions.ObjectOrientedProgramming
{
    public class Motorcycle : AbstractParkable, IVehicle
    {
        public int Width { get; } = 2;
        public int Length { get; } = 4;
    }
}
