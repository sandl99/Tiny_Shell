/*******************************************************
 * @author: Dang Lam San ---- Hoang Minh Tan
 * @name of Program: Tiny Shell
 * @KSTN CNTT K62
 * @date dd/mm/yyyy
 *******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include "myprocess.h"
#include "analysis.h" 
#include "features.h"
#include <signal.h>

using namespace std;

#define MAX_BUFFER_SIZE 128
#define MAX_TOKENS 10
#define TOKEN_DELIM " \t"

extern HANDLE h_fore_process;

void Execute()
{
	LPSTR cur_dir = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
	char *line_input;
	char **line_split;
	printf("--------------------------------------Welcome to TINY SHELL ---------------------------------------\n");
	printf("* @author: Dang Lam San ---- Hoang Minh Tan \n* @name of Program : Tiny Shell\n* @KSTN CNTT K62\n* @date dd / mm / yyyy\n");
	printf("--------------------------------------Welcome to TINY SHELL ---------------------------------------\n\n");

	while (1)
	{

		DWORD len_dir = GetCurrentDirectory(80, cur_dir);
		printf("%s>", cur_dir);
		line_input = ReadLine();
		// Check ctrl C and enter
		if (strcmp(line_input, "\0") == 0)
		{
			printf("\n");
			continue;
		}
		line_split = SplitLine(line_input);
		// Check full space input
		if (line_split[0] == NULL)
		{
			continue;
		}
		AnalysisInput(line_split, cur_dir);
	}
	delete line_input;
	delete line_split;
	delete cur_dir;
	return;
}

void sigintHandler(int sig_num)
{
	if (h_fore_process != NULL)
	{
		TerminateProcess(h_fore_process, 1);
		h_fore_process = NULL;
	}
	cin.clear();
	signal(SIGINT, sigintHandler);
	return;
}


int main(int argc, char **argv)
{
	signal(SIGINT, sigintHandler);
	Execute();
	return 0;
}