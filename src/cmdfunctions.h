#ifndef CMDFUNCTIONS_H
#define CMDFUNCTIONS_H

#include <stdio.h>
#include <string.h>

void printUsage();

void printHelp();

char* combineArgs(int startingIndex, int argc, char* argv[]);

char* loadFromFile(const char* filepath);

#endif