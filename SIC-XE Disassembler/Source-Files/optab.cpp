/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler
*/

#include "optab.h"
#define SIZE 59

struct opCodeTable
{
    int opCode;
};

struct nameTable
{
    string name;
};

struct formatTable
{
    int format;
};

const struct opCodeTable ops[] = {
    0x18, 0x58, 0x90, 0x40, 0xB4, 0x28, 0x88, 0xA0, 0x24, 0x64, 0x9C, 0xC4, 0xC0, 0xF4, 0x3C, 0x30, 0x34, 0x38, 0x48, 0x00, 0x68, 0x50, 0x70, 0x08, 0x6C, 0x74, 0x04, 0xD0, 0x20, 0x60, 0x98, 0xC8, 0x44, 0xD8, 0xAC, 0x4C, 0xA4, 0xA8, 0xF0, 0xEC, 0x0C, 0x78, 0x54, 0x80, 0xD4, 0x14, 0x7C, 0xE8, 0x84, 0x10, 0x1C, 0x5C, 0x94, 0xB0, 0xE0, 0xF8, 0x2C, 0xB8, 0xDC};

const struct nameTable mnemonics[] = {
    "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX", "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
    "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
    "LDS", "LDT", "LDX", "LPS", "MUL", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW",
    "STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
    "TD", "TIO", "TIX", "TIXR", "WD"};

const struct formatTable format[] = {
    3, 3, 2, 3, 2, 3, 3, 2, 3, 3, 2, 1, 1, 1, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 2, 1, 3, 3, 2, 3, 2, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 2, 2,
    3, 1, 3, 2, 3};

// Find the mnemonics of the instruction from the nameTable struct
string optab::getName(int opCodeParam)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (ops[i].opCode == opCodeParam)
        {
            return mnemonics[i].name;
        }
    }
    return NULL;
}

// Find the format of the instruction from the formatTable struct
int optab::getFormat(int opCodeParam)
{
    int opCode = opCodeParam & 0xFC;
    for (int i = 0; i < SIZE; i++)
    {
        if (ops[i].opCode == opCodeParam)
        {
            return format[i].format;
        }
    }
    return 0;
}

// Find the op code of the instruction from the opCodeTable struct
int optab::getOp(int opCodeParam)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (ops[i].opCode == opCodeParam)
        {
            return ops[i].opCode;
        }
    }
    return 0;
}