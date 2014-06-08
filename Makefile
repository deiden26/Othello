# Makefile for Team Moop Othello program

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

Othello: TeamMoop.o
	$(CXX) $(CXXFLAGS) -o Othello TeamMoop.o

# The main.o target can be written more simply

TeamMoop.o: TeamMoop.cpp TeamMoop.h
	$(CXX) $(CXXFLAGS) -c TeamMoop.cpp