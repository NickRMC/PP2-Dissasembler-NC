/*
 * binToDec
 *
 * This function converts the binary digits from string[begin] to
 * string[end] to a decimal integer.
 *
 * int binToDec(char string[], int begin, int end)
 *   Pre-condition:  begin and end are valid indices in array string;
 *   		     every character from string[begin] to string[end]
 *		     (including those two) has a character value '0' or '1'
 *   Returns: the decimal equivalent of the binary number represented by
 *	      the characters in string[begin] - string[end].
 *            (If begin > end, this function arbitrarily returns 0.)
 *
 * For example, assume A is a character array that contains the
 *    following characters:
 *            '1'  '0'  '1'  '1'  '0'  '1'  '0'  '0'  '1'
 *    The call binToDec(A, 2, 5) converts the binary number 1101
 *    (represented by the substring A[2] - A[5]) to the decimal integer
 *    13 and returns it.
 *
 * Note: The decimal value returned may appear to be negative if the
 * string being converted is long enough (end - begin + 1 == 32)
 * and if the first character is '1'.
 *
 * NOTE: I quickly noticed the fact that a signed 32 bit int cannot
 *		hold a value greater than 2,147,483,647. This could be fixed 
 *		by including the library that has unsigned 32 bit ints, but 
 *		this library isn't always consistent between systems, so the
 *		second option is using long long, a 64 bit integer, but this 
 *		is overkill and wasn't part of the project description. One
 *		of the test cases addresses this (Test Case 8).
 *
 * Author:  Nicolas McCabe
 *
 * Creation Date:  April 15th
 *
 */

int power (int base, int exponent);

int binToDec (char string[], int begin, int end)
{
	if (begin > end || begin < 0 || end > 31)
	{
		return 0;
	}

	int i;
	int decimal = 0;
	double exp = end - begin; /* Calculating the highest exponent to use */

	for (i = begin; i <= end; i++)
	{
		decimal += (((string[i] == '1') ? 1 : 0) * power (2,exp));
		exp--;
	}

	return decimal;
}

/* I was having trouble accessing the pow() function available when including math.h,
 * so I added this power function which uses recursion to accomplish the math of exponents.
 * I also discovered by doing this that there is no ^ operator in C.
 */
int power (int base, int exponent)
{
	if (exponent > 1)
	{
		return base * power(base, exponent - 1);
	}
	else if (exponent == 0)
	{
		return 1;
	}
	else
	{
		return base;
	}
}
