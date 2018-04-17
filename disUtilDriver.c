/*
 * Test Driver to test verifyMIPSInstruction and binToDec.
 *
 * This program reads lines from a file.  For each line, the program
 *      calls verifyMIPSInstruction.  If the line contains characters
 *      representing a valid MIPS instruction, the program calls
 *      binToDec for various segments of the instruction to test the
 *      binToDec function.
 *
 * Usage:
 *          name [ filename ] [ 0|1 ]
 *      where "name" is the name of the executable, "filename" is an
 *      optional file containing the input to read, and " 0" or
 *      "1" specifies that debugging should be turned off or on,
 *      respectively, regardless of any calls to debug_on, debug_off,
 *      or debug_restore in the program.  Both arguments are optional;
 *      if both are present they may appear in either order.  If no
 *      filename is provided, the program reads its input from stdin.
 *      If no debugging choice is provided, the program prints debugging
 *      messages, or not, depending on indications in the code.
 *
 * Input:
 *      The program reads its input from a file passed in as a parameter
 *      on the command line, or reads from the standard input.
 *      To test verifyMIPSInstruction, the file should contain ...
 *
 *              DESCRIBE NECESSARY TEST CASES HERE
 *
 * Output:
 *      For each valid line, the program prints a series of decimal
 *      numbers to test binToDec, representing
 *
 *              DESCRIBE NECESSARY TEST CASES HERE
 *
 *      For each invalid line, the program prints an error message to
 *      stderr saying so.  (The program also prints an error message if
 *      it cannot open the file.)
 *
 * Implementation:
 *		I am uncertain if this is a Windows thing or a general thing, 
 *		but there is not just a new line ('\n') at the end of a line,
 *		there is also a carriage return ('\r'). In order to account for
 *		this I simply used the same logic for removing the new line and 
 *		repeated it for a carriage return. 
 *
 * Author:  Nicolas McCabe
 *
 * Creation Date:  April 15th
 *
 */

/* include files go here */
#include "disUtil.h"

const int SAME = 0;		/* useful for making strcmp readable */
                                /* e.g., if (strcmp (str1, str2) == SAME) */

int main (int argc, char *argv[])
{
    FILE * fptr;               /* file pointer */
    char   buffer[BUFSIZ];     /* place to store line that is read in */
    int    length;             /* length of line read in */
    int    lineNum = 0;        /* keep track of input line numbers */

    /* Process command-line arguments (if any) -- input file name
     *    and/or debugging indicator (1 = on; 0 = off).
     */
    fptr = process_arguments(argc, argv);
    if (fptr == NULL)
    {
        return 1;   /* Fatal error when processing arguments */
    }

    /* Can turn debugging on or off here (debug_on() or debug_off())
     * if not specified on the command line.
     */

    /* Continuously read next line of input until EOF is encountered.
     * Each line should contain a valid MIPS machine language instruction
     * (represented as 32 character '0's and '1's) and newline.
     */
    while (fgets(buffer, BUFSIZ, fptr))   /* fgets returns NULL if EOF */
    {
        lineNum++;

        /* If the last character in the string is a newline or a carriage return,
		 * "remove" it by replacing it with a null byte.
         */
        length = strlen(buffer);

		if (buffer[length - 1] == '\n')
		{
			buffer[--length] = '\0';      /* remove; pre-decrement length */
		}

		if (buffer[length - 1] == '\r')
		{
			buffer[--length] = '\0';
		}

        printf("\nLine %d: %s\n", lineNum, buffer);
        printDebug("Length: %d\n", length);

        /* Verify that the string contains 32 '0' and '1' characters.  If
         * it does, do various tests to ensure that binToDec works correctly.
         * If the string is invalid, verifyMIPSinstruction should print
         * an informative error message.
         */
		if (verifyMIPSInstruction(lineNum, buffer))
		{
			/* Guaranteed Working Test Case -- Should return a real number */
			printf("\tbits 4 to 8: %.5s is equal to %d\n", &buffer[3], binToDec(buffer, 3, 7));

			/* Begin > End Test Case -- Should return 0 */
			printf("\tbits 8 to 2: %.6s is equal to %d\n", &buffer[7], binToDec(buffer, 7, 1));

			/* Begin < 0 Test Case -- Should return 0 */
			printf("\tbits -10 to 7: %.17s is equal to %d\n", &buffer[-10], binToDec(buffer, -10, 6));

			/* End > 32 Test Case -- Should return 0 */
			printf("\tbits 30 to 33: %.4s is equal to %d\n", &buffer[29], binToDec(buffer, 29, 32));

			/* Full Line Conversion -- Should return -1 as an overflow */
			printf("\tbits 1 to 32: %.32s is equal to %d\n", &buffer[0], binToDec(buffer, 0, 31));
		}
    }

    /* End-of-file encountered; close the file. */
    fclose(fptr);
    return 0;
}
