/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler
*/

#ifndef DISSEM_H
#define DISSEM_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "optab.h"

class dissem
{
public:
    void openFile(string f);               // open files
    void findRecord();                     // determine record type
    void symtabSearch();                   // WORD only
    vector<string> objectVector;           // store data from obj file to vector
    vector<string> symbolVector;           // store data from sym file to vector
    vector<string> symtab;                 // store data from symbol table to vector
    vector<string> littab;                 // store data from literal table to vector
    void readIn();                         // read in data from files
    void createTables();                   // create symtab, littab
    void getHeader(int);                   // retrieve program starting address, program length from H
    void getText(int);                     // retrieve length of text record from T
    void getMod(int);                      // retrieve address to be modified from M
    void getEnd(int);                      // retrieve end address of program from E
    bool getBit(int bit, int index);       // shift nixbpe bits
    string hexToStr(int hex);              // convert hex # to string address
    unsigned int strToHex(string address); // convert string addresses to hex
    int formatNumber(int, int);            // determine which instruction format to calculate
    void format2(optab, int, int, int);    // for formats op | r1 | r2
    int format3(optab, int, int, int);     // for formats op | nixbpe | disp
    int format4(bool[], int, int, string); // for formats op | nixbpe | address

    unsigned int progLen;     // program length
    unsigned int address;     // program starting address
    unsigned int baseAddress; // for use in base relative addressing
};

#endif
