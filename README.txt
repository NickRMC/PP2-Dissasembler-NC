# disassembler-assignments: Disassembler Assignments
##  Disassembler Utility Functions, and <br> Working Disassember

### Disassembler Utility Functions
This project consists of binToDec.c, verifyMIPSInstruction.c, and disUtilDriver.c.
The Description of this project is given here.
http://www.cs.kzoo.edu/cs230/Projects/DisUtilitiesProj.html
binToDec allows for conversion of a binary string to a decimal value.
verifyMIPSInstruction checks a string to ensure it represents 32 bits.
More in-depth descriptions can be found in the specific class documentations, 
As well as descriptions on how they were implemented. 

Test cases are currently removed from this version to allow easier grading of the
DisassemblerProj, however for more information on the verifyMIPSInstruction or binToDec
check out the GitHub projects at:
https://github.com/comp230-2018/disassembler-utility-functions-project-tim-rutledge
https://github.com/comp230-2018/disassembler-utility-functions-project-nicolas-mccabe




### Working Disassembler
This project makes use of the functionality from the Disassembler Utility Functions project.
It takes in strings accepted by verifyMIPSInstruction, and if those 32 bits represents
a MIPS instruction in the table provided below, it will print the MIPS Assembly instruction.
This is done by first classifying a given binary string based on the opcode, the first 6 bits,
to R, I or J format, and then identifying the components of the given format.
Any Register are from the getRegName.c class, which is a straightforward implementation of the MIPS Registers table linked to below.


R Format:
6 bits: Opcode
5 bits: Register source
5 bits: Register target
5 bits: Register destination
5 bits: Shift amount
6 bits: Function

I Format:
6 bits: Opcode
5 bits: Register source
5 bits: Register target
16 bits: Immediate value

J Format:
6 bits: Opcode
26 bits: Address




For example, consider the string:
10001101001010000000010010110000
The first 6 bits, or the op code: 100011 = 35 (base 10)
Looking at the table, we can see this corresponds to the instruction lw, of format I.
we then determine the other components of an I command.
0000010010110000
6 bits: Opcode				100011				=   35 (base 10)
5 bits: Register source		01001				=    9 (base 10)
5 bits: Register target		01000				=    8 (base 10)
16 bits: Immediate value	0000010010110000	= 1200 (base 10)


And found this and looking at our tables we expect our output to be:
lw $t0, $t1, 1200




See the disassembler.c class for more information for specifics on how to use.

(See here for the project description for the full disassembler program.)
http://www.cs.kzoo.edu/cs230/Projects/DisassemblerProj.html)

(See here for the table of MIPS instructions)
http://www.cs.kzoo.edu/cs230/Projects/mipsTable.html

(See here for the table of MIPS Registers)
http://www.cs.kzoo.edu/cs230/Projects/mipsRegTable.html






Here is the description for all test cases. Note that these test cases are checked 
against the MIPS instruction table given above. There are two files, one for the valid and another for the invalid test cases.

TestCasesValid, 150 cases:
OP:0, R format
32 cases, testing all possible MIPS Registers in first position. 
32 cases, testing all possible MIPS Registers in second position. 
32 cases, testing all possible MIPS Registers in third position. 

11 cases, testing function values of
	33, 34, 35, 36, 37, 39, 0, 2, 42, 43, and 8
	Expected: addu, sub, subu, and, or, nor, sll, srl, slt, sltu, jr
	
4 cases testing bounds of shift values for 0 (sll) and 2 (srl)



OP:8, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:9, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:12, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:13, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:35, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:43, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:15, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:4, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:5, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:10, I format 
3 cases, min max and random middle value for 16 bit IMM.
OP:11, I format 
3 cases, min max and random middle value for 16 bit IMM.

OP:2, J format 
3 cases, min max and random middle value for 26 bit Pseudo-Address.
OP:3, J format 
3 cases, min max and random middle value for 26 bit Pseudo-Address.


TestCasesInvalid, 103 cases:
53 cases, OP:0 (R format)  testing all invalid function values. 
50 cases, testing all unused op codes.
	1,6,7,14,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,36,37,38,
	39,40,41,42,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63

	
	
	
	
	
Author:  Nicolas McCabe, Tim Rutledge
 Creation Date:  April 15th
 Modifications: 5/4/2018, Tim Rutledge, updated README