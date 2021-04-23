/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler
*/

#include <iomanip>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "dissem.h"

ifstream inputObject;
ifstream inputSymbol;
ofstream outputSic;
ofstream outputLis;

/* Open the obj and sym files, read in contents to vector<strings>,
    otherwise tell user file cannot be opened */
void dissem::openFile(string file)
{
    inputObject.open(file + "-.obj.txt");
    inputSymbol.open(file + ".sym.txt");
    outputSic.open(file + ".sic.txt");
    outputLis.open(file + ".lis.txt");
    if (!inputObject.is_open() || !inputSymbol.is_open())
    {
        std::cout << "File cannot be opened";
        exit(EXIT_FAILURE);
    }
    else
    {
        readIn();
        createTables();
    }
    inputObject.close();
    inputSymbol.close();
}

/* grab contents of obj and sym files line by line to store in vectors */
void dissem::readIn()
{
    string line;
    string line2;
    bool objConnection = inputObject.good();
    bool symConnection = inputSymbol.good();

    while (objConnection && symConnection)
    {
        getline(inputObject, line);
        objectVector.push_back(line);
        getline(inputSymbol, line2);
        symbolVector.push_back(line2);
    }
}

/* populate symtab, littab vectors */
void dissem::createTables()
{
    int index = 2;
    int indexL = 2;
    // fill symtab so long as element isn't null (0 ASCII val)
    for (int i = index; index < symbolVector.size() - 1; index++)
    {
        string symbol = symbolVector[i];
        if (symbolVector[index][0] != 0)
        {
            symtab.push_back(symbol);
        }
        else
        {
            index = i + 3;
            break;
        }
    }
    // populate littab from sym file vector
    for (int j = indexL; indexL < symbolVector.size() - 1; indexL++)
    {
        littab.push_back(symbolVector[j]);
    }
}

/* At the current location, determine the instruction and it's 
    appropriate format and write it's address to the lis file */
int dissem::formatNumber(int row, int index)
{
    int instruction = stoi(objectVector[row].substr(index, 2));
    optab testAssembly = *new optab;
    int format = testAssembly.getFormat(row);
    outputLis << setfill('0') << setw(4) << right << address << setfill(' ');

    if (format == 2)
    {
        format2(testAssembly, instruction, row, index);
    }
    else if (format == 3 || format == 4)
    {
        format = format3(testAssembly, instruction, row, index);
    }
    return format;
}

/* From the obj file vector, determine the first character and
    call appropriate function to grab info from records */
void dissem::findRecord()
{
    for (int i = 0; i < objectVector.size(); ++i)
    {
        char firstChar = objectVector[i][0];
        if (firstChar == 'H')
        { // Header
            getHeader(i);
        }
        else if (firstChar == 'T')
        { // Text
            getText(i);
        }
        else if (firstChar == 'M')
        { // Modification
            getMod(i);
        }
        else if (firstChar == 'E')
        { // End
            getEnd(i);
        }
        else
        {
            break;
        }
    }
}

/* Determine starting address, program length to write to sic and lis
    using the H Header record */
void dissem::getHeader(int row)
{
    /* retrieve program name from first 6 characters after H */
    string progName = objectVector[row].substr(1, 6);
    /* retrieve starting address and address from next 6 characters */
    unsigned int startAddress = strToHex(objectVector[row].substr(7, 6));
    unsigned int address = strToHex(objectVector[row].substr(7, 6));
    /* retrieve program length from next 6 characters */
    unsigned int progLen = strToHex(objectVector[row].substr(13, 6));

    string tempAddress = hexToStr(startAddress);
    /* send converted string addresses left justified to sic and lis files*/
    outputSic << setw(9) << left << progName << "START " << tempAddress << endl;
    /* for lis, pad with 0's and spaces and convert to hexadecimal format */
    outputLis << setw(4) << setfill('0') << (address) << setfill(' ');
    outputLis << setw(9) << progName << "START " << tempAddress << endl;
}

/* Determine length of text record to write to sic and lis
    using the T Text record */
void dissem::getText(int row)
{
    /* Determine length of text record from 7th to 9th characters */
    int textLen = stoi(objectVector[row].substr(7, 2).c_str(), NULL, 16);
    int index = 9;
    int bound = 2 * textLen + 9;
    /* Determine format of text record while index is less than double the length + 9 positions */
    while (index < bound)
    {
        int size = formatNumber(row, index);
        address += (size / 2);
        index += size;
    }
    /* Handle separate symtab and littab vectors */
    symtabSearch();
}

/* Determine if text record should be placed in symtab 
    for WORD variables ONLY */
void dissem::symtabSearch()
{
    for (int i = 0; i < symtab.size(); i++)
    {
        /* retrieve address and name of symbol at current index*/
        unsigned int symbolAddress = strToHex(symtab[i].substr(8, 6));
        string symbolName = symtab[i].substr(0, 6);
        /* if address is valid in symtab, append line to sic and lis file
            if address is a WORD then write to lis and sic, else continue to find address */
        if (address <= symbolAddress)
        {
            outputLis << setfill('0') << setw(4) << right << address << setfill(' ');
            outputSic << setw(8) << left << symbolName << " RESW" << setfill(' ') << setw(4);
            outputLis << setw(8) << left << symbolName << " RESW" << setfill(' ') << setw(4);
            if ((i + 1) < symtab.size())
            {
                unsigned int nextAddr = ((strToHex(symtab[i + 1].substr(8, 6)) - symbolAddress) / 3);
                outputSic << setw(8) << left << nextAddr << endl;
                outputLis << setbase(10) << setw(8) << left << nextAddr << endl;
                address = address + (nextAddr * 3);
            }
            else
            {
                outputSic << setw(8) << left << ((progLen - (int)symbolAddress) / 3) << endl;
                outputLis << setbase(10) << setw(8) << left << ((progLen - (int)symbolAddress) / 3) << endl;
                address = address + ((progLen - (int)symbolAddress));
            }
        }
    }
}

/* Determine the address to be modified and write to sic and lis
    using the M Modification record */
void dissem::getMod(int row)
{
    /* if not T header */
    int i = 0;
    int index;
    while (objectVector[i][0] != 'T')
    {
        i++;
    }
    /* Retrieve address to be modified from next 6 characters after M record */
    unsigned int textAddress = strToHex(objectVector[i].substr(1, 6));
    /* Retrieve length of address to be modified from next 6 characters after address */
    unsigned int modLen = strToHex(objectVector[row].substr(7, 2));
    /* Calculate modification address */
    while (textAddress < modLen)
    {
        textAddress = textAddress + strToHex(objectVector[i].substr(7, 2));
        i++;
    }
    i--;
    /* place modified address in obj vector to be written to output files */
    textAddress = textAddress - strToHex(objectVector[i].substr(7, 2));
    index = (2 * (modLen - textAddress)) + 10;
    objectVector[i][index] = objectVector[i][index] + progLen;
}

/* Determines the end of the program */
void dissem::getEnd(int row)
{
    unsigned int endAddr = strToHex(objectVector[row].substr(1, 6));
    /* Write out end address and END mnemonic to sic file */
    for (int i = 0; i < symtab.size(); i++)
    {
        if (endAddr == strToHex(objectVector[row].substr(8, 6)))
        {
            outputSic << setw(8) << left << "END" << symtab[i].substr(0, 6) << endl;
            outputSic << setw(10) << left << "END" << symtab[i].substr(0, 6) << endl;
            break;
        }
    }
}

// shift bits to the right to set the nixbpe boolean array
bool dissem::getBit(int bit, int index)
{
    return ((bit >> index) & 1);
}

void dissem::format2(optab mnemonics, int instruction, int row, int index)
{
    string opName = mnemonics.getName(instruction);
    /* check if instruction is at valid address, write to sic and lis */
    for (int i = 0; i < symtab.size() - 1; i++)
    {
        if (address == strToHex(symtab[i].substr(8, 6)))
        {
            outputSic << setw(8) << left << strToHex(symtab[i].substr(8, 6));
            outputLis << setw(8) << left << strToHex(symtab[i].substr(8, 6));
            break;
        }
        else if (i + 1 >= symtab.size() - 1)
        {
            outputSic << setfill(' ') << setw(9) << setw(7) << left << opName;
            outputLis << setfill(' ') << setw(9) << setw(7) << left << opName;
        }
    }
    /* check if literal is at valid address, write LTORG to sic and lis */
    for (int i = 0; i < littab.size(); i++)
    {
        if (address == strToHex(littab[i].substr(24, 6)))
        {
            outputSic << setw(10) << left << strToHex(littab[i].substr(8, 6)) << endl;
            outputSic << setw(14) << right << "LTORG" << endl;
            outputLis << setw(10) << left << strToHex(littab[i].substr(8, 6)) << endl;
            outputLis << setw(14) << right << "LTORG" << endl;
            return;
        }
    }
    /* assign obj vector into simulated registers 1 and 2 of 4 bytes */
    int reg1 = stoi(objectVector[row].substr(index + 2, 1));
    int reg2 = stoi(objectVector[row].substr(index + 3, 1));

    /* determine correct register values for end of format 2 instruction */
    if (reg1 == 0)
    {
        outputSic << "A,";
        outputLis << "A,";
    }
    else if (reg1 == 1)
    {
        outputSic << "X,";
        outputLis << "X,";
    }
    else if (reg1 == 2)
    {
        outputSic << "L,";
        outputLis << "L,";
    }
    else if (reg1 == 3)
    {
        outputSic << "B,";
        outputLis << "B,";
    }
    else if (reg1 == 4)
    {
        outputSic << "S,";
        outputLis << "S,";
    }
    else if (reg1 == 5)
    {
        outputSic << "T,";
        outputLis << "T,";
    }
    else
    {
        outputSic << "F,";
        outputLis << "F,";
    }

    if (reg2 == 0)
    {
        outputSic << "A,";
        outputLis << "A,";
    }
    else if (reg2 == 1)
    {
        outputSic << "X,";
        outputLis << "X,";
    }
    else if (reg2 == 2)
    {
        outputSic << "L,";
        outputLis << "L,";
    }
    else if (reg2 == 3)
    {
        outputSic << "B,";
        outputLis << "B,";
    }
    else if (reg2 == 4)
    {
        outputSic << "S,";
        outputLis << "S,";
    }
    else if (reg2 == 5)
    {
        outputSic << "T,";
        outputLis << "T,";
    }
    else
    {
        outputSic << "F,";
        outputLis << "F,";
    }
}

/* format 3 instructions will be determined by value of nixbpe bits */
int dissem::format3(optab mnemonics, int instruction, int row, int index)
{
    string mnemonic = mnemonics.getName(instruction);
    bool nixbpe[6];
    int currentBit = stoi(objectVector[row].substr(index + 1, 2));
    for (int i = 0; i < 6; i++)
    {
        nixbpe[i] = getBit(currentBit, 5 - i);
    }
    /* determine current instructions from current nixbpe bits */
    int endOfInst = 2 * (3 + nixbpe[5]);
    unsigned int currInstruction = strToHex(objectVector[row].substr(index, endOfInst));
    /* check if address and literal is valid in symtab and littab, write to sic and lis */
    for (int i = 0; i < symtab.size() - 1; i++)
    {
        if (address == strToHex(symtab[i].substr(8, 6)))
        {
            outputSic << setw(8) << left << strToHex(symtab[i].substr(0, 6));
            outputLis << setw(8) << left << strToHex(symtab[i].substr(0, 6));
            break;
        }
        else
        {
            outputSic << " " << setw(8);
            outputLis << " " << setw(8);
        }
    }

    /* for format 4 instructions, append + to instruction in file */
    for (int i = 0; i < littab.size(); i++)
    {
        if (address == strToHex(symtab[i].substr(24, 6)))
        {
            int literal = stoi(objectVector[row].substr(index, endOfInst));
            if (nixbpe[5])
            {
                outputSic << "+";
            }
            else
            {
                outputSic << " ";
            }
            outputSic << setw(7) << mnemonic << setw(10) << left << strToHex(littab[i].substr(8, 6)) << endl;
            outputSic << setw(14) << right << "LTORG" << endl;
            if (nixbpe[5])
            {
                outputLis << "+";
            }
            else
            {
                outputLis << " ";
            }
            outputLis << setw(7) << left << mnemonic << setw(10) << left << strToHex(littab[i].substr(8, 6));
            outputLis << setfill('0') << right << setw(endOfInst) << currInstruction << " " << endl;
            outputLis << setfill('0') << setw(4) << right << address << " " << endl;
            outputLis << setw(14) << right << "LTORG" << setfill(' ') << setw(12) << literal << endl;
            return (3 + strToHex(littab[i].substr(16, 6)) / 2);
        }
    }

    /* if the extended bit is set, calculate using format 4 instructions */
    unsigned int TA = format4(nixbpe, row, index, mnemonic);
    /* if using base relative addressing */
    if (nixbpe[3])
    {
        TA += baseAddress;
    }
    else if (nixbpe[4])
    { /* if using PC relative addressing */
        TA = TA + address + 3;
    }
    else if (nixbpe[0] == nixbpe[1])
    { /* if using simple addressing */
        outputSic << setw(8) << left << mnemonic;
        outputLis << setw(8) << left << mnemonic;
        for (int i = 0; i < symtab.size(); i++)
        {
            if (TA == strToHex(symtab[i].substr(8, 6)) && mnemonic != "RSUB" && nixbpe[2])
            {
                outputSic << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << ",X" << endl;
                break;
            }
            else if (TA == strToHex(symtab[i].substr(8, 6)) && mnemonic != "RSUB" && !nixbpe[2])
            {
                outputSic << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << endl;
            }
            else
            {
                /* if using simple addressing and instruction is RSUB*/
                outputSic << endl;
                outputLis << setfill(' ') << setw(9);
            }
        }
    }
    else if (nixbpe[0])
    { /* if using indirect addressing */
        outputSic << setw(7) << left << mnemonic << "@";
        outputLis << setw(7) << left << mnemonic << "@";
        for (int i = 0; i < symtab.size(); i++)
        {
            if (TA == strToHex(symtab[i].substr(8, 6)) && mnemonic != "RSUB" && nixbpe[2])
            {
                outputSic << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << ",X" << endl;
                outputLis << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << ",X" << endl;
                break;
            }
            else if (TA == strToHex(symtab[i].substr(8, 6)) && mnemonic != "RSUB" && !nixbpe[2])
            {
                outputSic << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << endl;
                outputLis << setw(9) << left << strToHex(symtab[i].substr(0, 6)) << endl;
                break;
            }
        }
    }
    else
    { /* if using immediate addressing */
        if (nixbpe[5])
        {
            outputSic << setw(7) << left << mnemonic << "#";
            outputLis << setw(7) << left << mnemonic << "#";
        }
        else
        {
            outputSic << TA << endl;
            outputLis << setw(9) << TA;
        }
    }

    /* if instruction is LDB and TA is a valid address in symtab */
    if (mnemonic == "LDB")
    {
        baseAddress = TA;
        for (int i = 0; i < symtab.size(); i++)
        {
            if (TA == strToHex(symtab[i].substr(8, 6)))
            {
                outputSic << setw(10) << left << strToHex(symtab[i].substr(0, 6)) << endl;
                outputSic << setw(17) << right << "BASE" << setfill(' ') << setw(4) << strToHex(symtab[i].substr(0, 6)) << endl;
                outputLis << setw(9) << left << strToHex(symtab[i].substr(0, 6));
                outputLis << setw(endOfInst) << setfill('0') << currInstruction << setfill(' ') << endl;
                outputLis << setfill('0') << setw(4) << right << address << setfill(' ') << "  ";
                outputLis << setw(17) << right << "BASE" << setfill(' ') << setw(4) << strToHex(symtab[i].substr(0, 6)) << endl;
                return (3 + nixbpe[5]); // return next 3 bits
            }
        }
    }
    /* write instruction to lis file */
    outputLis << right << setfill('0') << setw(endOfInst) << currInstruction << setfill(' ') << endl;
    return (3 + nixbpe[5]); // return next 3 bits
}

/* if extended bit of nixbpe is set, then instruction is of format 4
    hash target address with 0x000FFF## mask (depending on instruction)*/
int dissem::format4(bool nixbpe[], int row, int index, string mnemonic)
{
    unsigned int TA = 0;
    if (nixbpe[5] && (mnemonic == "JEQ" || mnemonic == "JLT" || mnemonic == "JGT" || mnemonic == "JSUB" || mnemonic == "J"))
    {
        TA = TA & 0x000FFFFF;
    }
    else if (!nixbpe[5] && (mnemonic == "JEQ" || mnemonic == "JLT" || mnemonic == "JGT" || mnemonic == "JSUB" || mnemonic == "J"))
    {
        TA = TA & 0x000FFF;
    }
    else if (nixbpe[5])
    {
        TA = strToHex(objectVector[row].substr(index, 8));
        TA = TA & 0x000FFFFF;
    }
    else
    {
        TA = strToHex(objectVector[row].substr(index, 6));
        TA = TA & 0x000FFF;
    }
    return TA;
}

/** Helper functions **/
/* converts hexadecimal address to string 
    using string stream */
string dissem::hexToStr(int hexAddress)
{
    stringstream ss;
    string convAddress;
    ss << hexAddress;
    convAddress = ss.str();
    return convAddress;
}

/* converts string to hexadecimal format
    using string stream */
unsigned int dissem::strToHex(string address)
{
    unsigned int hexAddress;
    stringstream ss;
    ss << hex << address;
    ss >> hexAddress;
    return hexAddress;
}