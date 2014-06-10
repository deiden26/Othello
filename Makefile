# Makefile for Team Moop Othello program

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

#Othello: TeamMoop.o
#	$(CXX) $(CXXFLAGS) -o Othello TeamMoop.o

# The main.o target can be written more simply

#TeamMoop.o: TeamMoop.cpp TeamMoop.h
#	$(CXX) $(CXXFLAGS) -c TeamMoop.cpp

Othello: MatchMaking.o TeamMoop.o 
	$(CXX) $(CXXFLAGS) -o Othello TeamMoop.o MatchMaking.o

MatchMaking.o: MatchMaking.cpp
	$(CXX) $(CXXFLAGS) -c MatchMaking.cpp 

TeamMoop.o: TeamMoop.h