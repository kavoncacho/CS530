# Names: Brandon Tang, Kavon Cacho, Jason Songvilay, Drake Nguyen
# Edoras Ids: cssc2832, cssc2779, cssc2830, cssc2813
# Course: CS530-02-Fall2020
# Assignment 1: XE Disassembler


CXX=g++
CXXFLAGS=-std=c++11 -g

dissem : dissem.o optab.o main.o
	$(CXX) $(CXXFLAGS) -o dissem $^

dissem.o : dissem.cpp dissem.h 

optab.o : optab.cpp optab.h 

main.o : main.cpp

clean :
	rm *.o

.PHONY: dissem optab main

