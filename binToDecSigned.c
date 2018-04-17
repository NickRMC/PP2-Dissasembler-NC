/*
 * binToDecSigned
 *
 * This function converts the two's complement binary digits from
 * string[begin] to string[end] to a signed decimal integer.
 *
 * int binToDecSigned(char string[], int begin, int end)
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
 *    The call binToDec(A, 2, 5) converts the two's complement binary
 *    number 1101 (represented by the substring A[2] - A[5]) to the
 *    decimal integer -3 and returns it.
 *
 * Implementation: 
 *    This function directly sets the bits in the result from right to
 *    left using a single bit bit mask. Negative number are sign
 *    extended by initializing all bits in the result to '1' before the
 *    bit setting loop. 
 * 
 * Author:  Nathan Sprague
 *	Based on binToDec skeleton created by Alyce Brady.
 *
 * Creation Date:  1/25/08
 *	modified: Modification_Date	reason
 *	modified: Modification_Date	reason
 *
 */


int binToDecSigned(char string[], int begin, int end)
{
	int i;
	int decimal = 0;
	
	unsigned int mask = 0x1;

	if (string[begin] == '1') 
	  decimal |= 0xffffffff; //sign extension for negative numbers

	for (i = end; i>= begin; i--)
	  {
	    if (string[i] == '1') 
	      decimal = decimal | mask; //set the current bit.
	    else 
	      decimal = decimal & ~mask;//clear the current bit.
	    mask <<= 1;                 //move the current bit to the left.
	  }
	
	return decimal;
}
