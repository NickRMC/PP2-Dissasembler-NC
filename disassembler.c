/*
 * MIPS Disassembler
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

int main(int argc, char *argv[])
{
	FILE * fptr;               /* file pointer */
	char   input[BUFSIZ];      /* place to store line that is read in */
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
	while (fgets(input, BUFSIZ, fptr))   /* fgets returns NULL if EOF */
	{
		lineNum++;

		/* If the last character in the string is a newline or a carriage return,
		 * "remove" it by replacing it with a null byte.
		 */
		length = strlen(input);

		if (input[length - 1] == '\n')
		{
			input[--length] = '\0';      /* remove; pre-decrement length */
		}

		if (input[length - 1] == '\r')
		{
			input[--length] = '\0';
		}

		printf("\nLine %d: %s\n", lineNum, input);
		printDebug("Length: %d\n", length);

		/* Verify that the string contains 32 '0' and '1' characters.  If
		 * it does, do various tests to ensure that binToDec works correctly.
		 * If the string is invalid, verifyMIPSinstruction should print
		 * an informative error message.
		 */
		if (verifyMIPSInstruction(lineNum, buffer))
		{
			printf("Line %d: %s\n", lineNum, processRaw);
			checkErrorCount();
		}
	}

	/* End-of-file encountered; close the file. */
	fclose(fptr);
	return 0;
}

/*Process Raw*/
char * processRaw(input)
{
	switch (binToDec(input, 0 ,5)) /* Checks the first 6 digits in the MIPS */
	{
		case 0 :
			return processR(input);

		case (2 || 3) :
			return processJ(input);

		default:
			return processI(input);
	}
}

/*Process R*/
char * processR(input)
{
	switch (binToDec(input, 26, 31))
	{

	}
}

/*Process I*/
char * processI(input)
{

}


/*Process J*/
char * processJ(input)
{

}
