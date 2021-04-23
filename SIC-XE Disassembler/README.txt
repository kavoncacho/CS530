/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler

Filename: ReadMe.txt
*/

-------------------------------------------------------------------

Files:

Source code: dissem.cpp, optab.cpp, main.cpp
Headers: dissem.h, optab.h
Makefile: Makefile
Sample files: sample-obj.txt,sample.sym.txt,sample.sic.txt,sample.lis.txt

----------------------------------------------------------------------

v. A README file explaining the algorithm which you have designed for the
disassembler. You should explain your thought process behind the code
and its steps. You can include description, diagrams/drawings. 

The algorithm which we chose for our Disassembler is split into 3 file main files: dissem.cpp, 
optab.cpp, and main.cpp.The main.cpp acts as the driver for our program and takes in two parameters,an executable created from make and a filename (without the extensions). If the input contains a valid .obj and .sym file name, the main class begins the disassembly process.

The optab class stores all of the 59 instructions of the SIC/XE architecture. Each instruction 
mnemonic, opcode number, and format number are stored in structs as they may be dynamically stored for this usage. Although format 1 instructions are included within the structs, our program does not account for them as they are not required. Each struct possesses a get method to retrieve the correct instruction depending on the current row and place of the disassembly index.

The dissem class opens the .obj and .sym files, disassembles the object and symbol code, and writes the translated code into two new sic and lis files. Our disassembler does not account for format 1 instructions nor does it support CONSTANT definitions such as those defined by BYTE or WORD (all symbols in symtab are assumed to be WORD as well). The sample object code is opened (or exited, if input is incorrect) and stored into individual vector<strings> for writing. Our program then determines the header of the object code (H, T, M, or E) and appropriately calls a function to write it to our output files. To determine instruction formats, our program divides the steps for each individual format into 3 separate functions: format2, format3, and format4. An additional methods to convert hex to string and string to hex and shift the nixbpe bits are included for format 3 and 4 instructions.