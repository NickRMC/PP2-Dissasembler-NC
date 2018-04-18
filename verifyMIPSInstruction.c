/*
* verifyMIPSInstruction
*
* This function verifies that the string provided to it as a paramater
* contains 32 characters representing binary digits (characters '0' and
* '1'), followed by a null byte.  If the string contains the wrong
* number of characters or contains invalid charaters (not '0' or '1'),
* then the function prints an error message to stderr giving the line
* number and an appropriate error message.
*
*   int verifyMIPSInstruction (int lineNum, char * instruction);
*
*	    Pre-condition:  instruction is a null-terminated string
*	    Post-condition: the characters in "instruction" have not
*			    been modified
*	    Returns: 1 if instr contained 32 characters representing binary
*			    digits ('0' and '1') followed by a null byte
*		     0 if instr has the wrong number of characters or
*		            contains invalid characters
*	    Output: Prints an error message to stderr if necessary
*
* Author: Nicolas McCabe
*
* Creation Date:  April 15th 2018
*/

#include "disassembler.h"

static const int INSTR_LENGTH = 32;

int verifyMIPSInstruction (int lineNum, char * instr)
/*  Returns 1 if instr contains INSTR_LENGTH characters representing binary
*  digits ('0' and '1'); 0 otherwise
*/
{
	printDebug("Verifying MIPS...\n");

	int length;

	length = strlen(instr);

	printDebug("Verifying MIPS Length...\n");
	printDebug("MIPS Length: %d\n", length);

	if (length != INSTR_LENGTH)
	{
		printError("Error: Line %d does not have %d chars.\n", lineNum, INSTR_LENGTH);
		return 0;
	}

	printDebug("Length Verified.\n");
	printDebug("Verifying MIPS Content...\n");

	char * ptr;

	for (ptr = instr; ptr < instr + INSTR_LENGTH; ptr++)
	{
		if (*ptr == '1' || *ptr == '0')
		{
			continue;
		}
		else
		{
			printError("Error: Line %d had characters that weren't 1 or 0.\n", lineNum);
			return 0;
		}
	}
	printDebug("MIPS Content Verified.\n");
	printDebug("MIPS Verified.\n");
	return 1;
}
