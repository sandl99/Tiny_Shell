#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <Windows.h>

#pragma warning (disable : 4996)
using namespace std;

#define MAX_BUFFER_SIZE 128
#define MAX_TOKENS 10
#define TOKEN_DELIM " \t"


#define _CRT_SECURE_NO_WARNINGS
#define WINDOWS_LEAN_AND_MEAN

char *ReadLine(void)
{
	char *p = (char *)calloc(MAX_BUFFER_SIZE, sizeof(char));
	fflush(stdin);
	cin.getline(p, MAX_BUFFER_SIZE);
	cin.clear();
	return p;
}
char **SplitLine(char *buf)
{

	int pos = 0, bufsize = MAX_TOKENS;
	char **tokens = (char **)calloc(MAX_TOKENS, sizeof(char*));
	char *token;

	if (tokens == NULL)
	{
		printf("Allocation Failed.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(buf, TOKEN_DELIM);
	while (token != NULL)
	{
		tokens[pos] = token;
		// printf("%s\n",tokens[pos])\;
		pos++;

		if (pos >= bufsize)
		{
			bufsize += MAX_TOKENS;
			char **tokens_temp = tokens;
			tokens = (char **)realloc(tokens, bufsize);
			for (int i = 0; i < bufsize - MAX_TOKENS; i++)
				tokens[i] = tokens_temp[i];
			delete[] tokens_temp;
		}

		token = strtok(NULL, TOKEN_DELIM);
	}
	tokens[pos] = NULL;
	// free(*buf);
	return tokens;
}
char *CombineLine(char **argv, int start)
{
	char *path = (char *)calloc(125, sizeof(char));
	char *pivot = (char *)" ";

	// Append argv[] and space char to path

	path = strcat(path, argv[start]);
	int i = start + 1;
	while (argv[i] != NULL)
	{
		path = strcat(path, pivot);
		path = strcat(path, argv[i]);
		++i;
	}
	return path;
	
}

