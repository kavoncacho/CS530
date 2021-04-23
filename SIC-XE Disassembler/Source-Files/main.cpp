/*
Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
Course: CS530-02-Fall2020
Assignment 1: XE Disassembler
*/

#include "dissem.h"
#include "optab.h"
using namespace std;

/* Driver for Disassembler, takes in executable and filename, else 
    prompts user for correct input */
int main(int args, char *argv[])
{
    if (args == 2)
    {
        dissem DIS;
        DIS.openFile(argv[1]);
        DIS.findRecord();
        cout << "dissem successful" << endl;
    }
    else
    {
        cout << "ERROR, enter dissem followed by filename" << endl;
    }
}