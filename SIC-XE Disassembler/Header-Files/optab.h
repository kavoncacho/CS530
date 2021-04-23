/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler
*/

#ifndef OPTAB_H
#define OPTAB_H

#include <string>

using namespace std;

class optab
{
public:
    string getName(int opCodeParam); // get name of mnemonic
    int getOp(int opCodeParam);      // get opcode of mnemonic
    int getFormat(int opCodeParam);  // get format of mnemonic
};
#endif