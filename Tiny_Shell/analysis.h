#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#pragma warning(disable : 4996)
//#define _CRT_SECURE_NO_WARNINGS
//#define WINDOWS_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <string>


// Read input
char *ReadLine(void);

// Split input to many char *
char **SplitLine(char *buf);

// Combine path to a string
char *CombineLine(char **argv, int start);





#endif

