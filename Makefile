# A simple makefile

GCC=gcc -Wall -Wextra -Wpedantic -Wformat -Wshadow -Wredundant-decls \
    -Wstrict-prototypes
# Can also use -Wtraditional or -Wmissing-prototypes

#  Switch to the following alternative version of the "all" target
#  when you're ready to program the disassembler project.

all:	disassembler

# The assembler will probably have other source files in addition to these.
disassembler:	disassembler.h \
    		printFuncs.h \
    		process_arguments.h \
		process_arguments.c \
    		verifyMIPSInstruction.c \
		binToDec.c \
		getRegName.c \
		printDebug.c \
		printError.c \
		disassembler.c
		$(GCC) process_arguments.c verifyMIPSInstruction.c binToDec.c \
		    getRegName.c \
		    printDebug.c printError.c disassembler.c -o disassembler

clean: 
	rm -rf *.o disUtil disassembler
