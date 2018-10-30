#include <ctype.h>
#include <stdio.h>

//atof: convert string s to double
double atof(char s[])
{
    double val, power;
    int i, sign;

    // if there is any leading 'white space', step index past it
    // keep stepping index until other than white space encountered
    for (i = 0; isspace(s[i]); i++)
        ;

    // if there is a '-' char 
    // then indicate value is negative 
    // else assume value is positive
    // format is: result = (condition)? true value : false value
    sign = (s[i] == '-') ? -1 : 1; 

    // if there is a sign byte, step index past it
    if (s[i] == '+' || s[i] == '-')
        i++;

    // initialize the result 'val'
    // then loop through following characters
    for (val = 0.0; isdigit(s[i]); i++)
        // digits are in the range 0x30 through 0x39
        // make them integers by subtracting 0x30 ('0')
        // and update the result 'val'
        // remembering that each successive digit pushes the current result 'val'
        // to 10 times the old value then add the new 'converted' digit
        val = 10.0 * val + (s[i] - '0');
        // this ends the 'for' code block

     // when execution gets here, encountered something other than a digit
    // when a '.' encountered, step the index past it
    if (s[i] == '.')
        i++;

    // the 'power' value is indicating how much to divide the resulting
    // 'val' by to place the decimal point (if there was a decimal point)
    // into the correct position
    // if other than a digit encountered, exit loop
    for (power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0'); // see above comment about a similar line of code
        power *= 10.0;
    } // end for

    // calculate the actual value by allowing for any sign (+ or -)
    // then dividing that result by 'power' to properly place the decimal point
    return sign * val / power;
}  // end function: atof
