using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CodingInterviewQuestions.Numerical
{
    class NumericalManipulation
    {
        internal static string Add(string num1, string num2)
        {
            var brokenNumber1 = num1.ToCharArray().Reverse().ToArray();
            var brokenNumber2 = num2.ToCharArray().Reverse().ToArray();

            var addLength = brokenNumber1.Count() >= brokenNumber2.Count()
                ? brokenNumber1.Count()
                : brokenNumber2.Count();

            var carry = 0;
            var resultString = string.Empty;
            for (var i = 0; i < addLength; i++)
            {
                var brokenDigit1 = i >= brokenNumber1.Length ? 0 : brokenNumber1[i] - '0';
                var brokenDigit2 = i >= brokenNumber2.Length ? 0 : brokenNumber2[i] - '0';

                var result = brokenDigit1 + brokenDigit2 + carry;

                if (result >= 10)
                {
                    result %= 10;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }

                resultString += result;
            }

            if (carry == 1)
            {
                resultString += "1";
            }

            var car = resultString.ToCharArray();
            Array.Reverse(car);
            return new string(car);
        }

        internal static string Multiple(string num1, string num2)
        {
            return "";
        }
    }
}
