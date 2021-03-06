/*
 * MIPS Disassembler
 *
 * This program reads lines from a file.  For each line, the program
 *      calls verifyMIPSInstruction.  If the line contains characters
 *      representing a valid binary MIPS instruction, the program 
 *		calls binToDec for various segments of the binary string to 
 *		determine the format, R, I, or J. Each of these formats has
 *		a corresponding function that will use BinToDec again to 
 *		determine the other components of the binary string (ie Registers), 
 *		and will output the corresponding MIPS Assembly code.
 * 		The BinToDec and verifyMIPSInstruction were extensively 
 *		tested in the Disassembler Utilities PP.  For more information
 *		on the testing for those functions, see the Github page below:
 *https://github.com/comp230-2018/disassembler-utility-functions-project-tim-rutledge
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
 *		Functionality is based off this table below:
 *			http://www.cs.kzoo.edu/cs230/Projects/mipsTable.html
 * Input:
 *      The program reads its input from a file passed in as a parameter
 *      on the command line, or reads from the standard input.
 *		See README.txt for a line by line description of all test cases, valid and invalid.
 *		
 *		Briefly, the main test cases needed are:
 *      Test cases for all possible op codes.
 *		For R format:
 *			all possible function values.
 *		For I, J formats, and the srl, sll R format commands 
 *			upper and lower bound tests.
 *		All Registers tested in all 3 positions.
 *
 * Output:
 
 *		An error if the file can not be open.
 *      For each invalid line (not 32 0's and 1's), the program prints
 * 		an error message to stderr saying so.
 *		For each valid 32 binary string, an error if those 32 bits do not 
 *		correspond to a MIPS instruction given in the table, and if it does
 *		Then the MIPS Assembly instruction will be outputted.
 * 		
 *		Test cases are separated into 2 files, TestCasesInvalid and TestCasesValid. 
 *		All invalid test cases are expected to give errors.
 *		Valid test cases should all be accepted, and output the Instruction.
 *
 *
 * Implementation:
 *		I am uncertain if this is a Windows thing or a general thing,
 *		but there is not just a new line ('\n') at the end of a line,
 *		there is also a carriage return ('\r'). In order to account for
 *		this I simply used the same logic for removing the new line and
 *		repeated it for a carriage return.
 *
 * Author:  Nicolas McCabe, Tim Rutledge
 *
 * Creation Date:  April 15th
 *
 * Modifications: 
 * 		4/24/2018: Added disassembler functionality, and test cases.
 * 		5/4/2018:  Added a factor of 4 to the j and jal functions.
 */

/* include files go here */
#include "disassembler.h"

char* processRaw(char input[]);
char* processR(char input[]);
char* processI(char input[]);
char* processJ(char input[]);

const int SAME = 0;		/* useful for making strcmp readable */
						/* e.g., if (strcmp (str1, str2) == SAME) */

int main(int argc, char *argv[])
{
	FILE * fptr;               /* file pointer */
	char   input[BUFSIZ];      /* place to store line that is read in */
	int    length;             /* length of line read in */
	int    lineNum = 0;        /* keep track of input line numbers */
	char assembly[27];

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
		if (verifyMIPSInstruction(lineNum, input) == 1)
		{
			char * tempt = processRaw(input);
			strcpy(assembly, tempt);
			printf("Line %d: %s\n", lineNum, assembly);
			checkErrorCount();
		}
	}

	/* End-of-file encountered; close the file. */
	fclose(fptr);
	return 0;
}

/*Process Raw*/
char* processRaw(char input[])
{
	printDebug("Processing MIPS...\n");

	switch (binToDec(input, 0 ,5)) /* Checks the first 6 digits in the MIPS */
	{
		case 0 : /* R */
			return processR(input);

		case 2 : /* J-2 */		/* For some reason (2 || 3) was not working for the case, so they are now split up */
			return processJ(input);

		case 3 : /* J-3 */
			return processJ(input);

		default : /* I */
			return processI(input);
	}
}

/*Process R*/
char* processR(char input[])
{
	printDebug("Processing MIPS Type R...\n");

	static char assemblyInst[26] = { 0 };
	char converted[7] = { 0 };
	char registry[6] = { 0 };

	switch (binToDec(input, 26, 31))
	{
		case 0 :
			strcpy(assemblyInst, "sll ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 21, 25));
			strcat(assemblyInst, converted);						/* Shift */
		break;

		case 2 :
			strcpy(assemblyInst, "srl ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 21, 25));
			strcat(assemblyInst, converted);						/* Shift */
		break;

		case 8 :
			strcpy(assemblyInst, "jr ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, registry);							/* Registry 1*/
		break;

		case 32 :
			strcpy(assemblyInst, "add ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 33 :
			strcpy(assemblyInst, "addu ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 34 :
			strcpy(assemblyInst, "sub ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 35 :
			strcpy(assemblyInst, "subu ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 36 :
			strcpy(assemblyInst, "and ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 37 :
			strcpy(assemblyInst, "or ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 39 :
			strcpy(assemblyInst, "nor ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 42 :
			strcpy(assemblyInst, "slt ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		case 43 :
			strcpy(assemblyInst, "sltu ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 16, 20)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, registry);							/* Registry 3 */
		break;

		default :
			strcpy(assemblyInst, "Error: Unknown Function");	/* Error */
			incrementErrorCount();								/* Error Count ++ */
		break;
	}

	printDebug("MIPS Processed.\n");
	return assemblyInst;
}

/*Process I*/
char* processI(char input[])
{
	printDebug("Processing MIPS Type I...\n");

	static char assemblyInst[26] = { 0 };
	char converted[10] = { 0 };
	char registry[6] = { 0 };

	switch (binToDec(input, 0, 5))
	{
		case 4 :
			strcpy(assemblyInst, "beq ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 5 :
			strcpy(assemblyInst, "bne ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 8 :
			strcpy(assemblyInst, "addi ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 9 :
			strcpy(assemblyInst, "addiu ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 10 :
			strcpy(assemblyInst, "slti ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 11 :
			strcpy(assemblyInst, "stliu ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 12 :
			strcpy(assemblyInst, "andi ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 13 :
			strcpy(assemblyInst, "ori ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 2 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 15 :
			strcpy(assemblyInst, "lui ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, converted);						/* Integer */
		break;

		case 35 :
			strcpy(assemblyInst, "lw ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, strcat(converted, "("));			/* Integer */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ")"));			/* Registry 2 */
		break;

		case 43 :
			strcpy(assemblyInst, "sw ");							/* Function */
			strcpy(registry, getRegName(binToDec(input, 11, 15)));
			strcat(assemblyInst, strcat(registry, ", "));			/* Registry 1 */
			sprintf(converted, "%d", binToDec(input, 16, 31));
			strcat(assemblyInst, strcat(converted, "("));			/* Integer */
			strcpy(registry, getRegName(binToDec(input, 6, 10)));
			strcat(assemblyInst, strcat(registry, ")"));			/* Registry 2 */
		break;

		default :
			strcpy(assemblyInst, "Error: Unknown OP Code");		/* Error */
			incrementErrorCount();								/* Error Count ++ */
		break;
	}

	printDebug("MIPS Processed.\n");
	return assemblyInst;
}


/*Process J*/
char* processJ(char input[])
{
	printDebug("Processing MIPS Type J...\n");
	static char assemblyInst[16] = { 0 };
	char converted[10] = { 0 };

	switch (binToDec(input, 0, 5))
	{
		case 2 :
			strcpy(assemblyInst, "j ");							/* Function */
			sprintf(converted, "%d", (binToDec(input, 6, 31)*4));
			strcat(assemblyInst, converted);					/* Integer */
		break;

		default : /* This will only ever be 3 */
			strcpy(assemblyInst, "jal ");						/* Function */
			sprintf(converted, "%d", (binToDec(input, 6, 31)*4));
			strcat(assemblyInst, converted);					/* Integer */
		break;
	}

	printDebug("MIPS Processed.\n");
	return assemblyInst;
}
