#include <iostream>
#include <Windows.h>
#include "analysis.h"
#include <ctime>
#include "myprocess.h"
#include <vector>

using namespace std;
const char *spe_arg[] = { "help", "cd", "exit", "enva" ,"date", "time", "dir" , "ps"};


BOOL HelpTask(char **argv)
{
	if (argv[2] != NULL)
	{
		printf("Too many arguments!\n");
		return EXIT_FAILURE;
	}
	if (argv[1] == NULL)
	{
		printf("Type \"help -[command]\" for more information about a specific command.\n");
		printf("Suppoted commands:\n cd\n date\n time\n dir\n ps\n enva\n exit\n");
		printf("Usage:\n <command> -[option]\nEXAMPLE: help -cd\n");
		printf("%-30s%s\n%-30s%s", " cd",
			"Change the current directory. You must write the new directory after this command.",
			" ", "EXAMPLES: \"cd C:/\"\n");
		printf("%-30s%s\n%-30s%s", " date",
			"Show the today's date.",
			" ", "EXAMPLES: \"date\"\n");
		printf("%-30s%s\n%-30s%s", " time",
			"Show the current time.",
			" ", "EXAMPLES: \"time\"\n");
		printf("%-30s%s\n%-30s%s", " dir",
			"Show all files and folders in the current directory.",
			" ", "EXAMPLES: \"dir\"\n");
		printf("%-30s%s\n%-30s%s\n%-30s%s", " ps",
			"Show information of process, suspend or resume any process.", " ",
			"You must enter the options in the 2nd argument, such as -all, -child, -thread, -suspend, ...\n",
			" ", "EXAMPLES: \"ps -all\"\n");
		printf("%-30s%s\n%-30s%s\n%-30s%s", " enva",
			"Get or set a specific environment variable.", " ",
			"You must enter the options in the 2nd argument, such as -get, -all, -set.\n",
			" ", "EXAMPLES: \"enva -all\"\n");
		printf("%-30s%s", " exit", "Exit this tiny shell :(\n");
	}
	else if (!strcmp(argv[1], "-cd"))
	{
		printf("Change the current directory. You must write the new directory after this command.\nEXAMPLE: \"cd C:/\"\n");
		printf("This command does not support any options.\n");
	}
	else if (!strcmp(argv[1], "-time"))
	{
		printf("Print current time to screen.\n");
		printf("This command does not support any options.\n");
	}
	else if (!strcmp(argv[1], "-date"))
	{
		printf("Print current date to screen.\n");
		printf("This command does not support any options.\n");
	}
	else if (!strcmp(argv[1], "-ps"))
	{
		printf("Supported options:\n");
		printf("%-20s%s", " all", "Show list of all running processes.\n");
		printf("%-20s%s", " b", "Run a program in background mode (by path to program).\n");
		printf("%-20s%s", " f", "Run a program in foregound mode (by path to program).\n");
		printf("%-20s%s", " find", "Get pid of specific program(s) (by name).\n");
		printf("%-20s%s", " child", "Show list of child processes of a process with specific pid.\n");
		printf("%-20s%s", " suspend", "Suspend a program (by pid).\n");
		printf("%-20s%s", " resume", "Resume a program (by pid).\n");
		printf("%-20s%s", " kill", "Terminate a program (by pid).\n");
		printf("%-20s%s", " thread", "Get all child processes id of a specific process(by pid).\n");
	}
	else if (!strcmp(argv[1], "-dir"))
	{
		printf("Show list of files or folders in current folder or specific folder (write path after this command).\n");
		printf("EXAMPLES: \"dir\", \"dir C:/\"\n");
	}
	else if (!strcmp(argv[1], "-enva"))
	{
		printf("Supported options:\n");
		printf("%-20s%s", " all", "Show list of all environment variables.\n");
		printf("%-20s%s", " get", "Get value of a specific environment variable.\n");
		printf("%-20s%s", " set", "Set value to a specific environment variable.\n");
	}
	else if (!strcmp(argv[1], "-exit"))
	{
		printf("Exit shell.\n");
		printf("This command does not support any options.\n");
	}
	else
	{
		printf("Command not found!\nType 'help' to see more information.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

BOOL CDTask(char **argv, LPSTR cur_dir) {
	if (argv[1] == NULL)
	{
		printf("Too few arguments\n");
		return EXIT_FAILURE;
	}
	char *path = CombineLine(argv, 1);
	if (SetCurrentDirectory(path) == 0)
	{
		cout << "The system cannot find the path specified.\n";
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

BOOL DateTask(char **argv)
{
	if (argv[1] != NULL)
	{
		cout << "Command \"date\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current Date: " << now->tm_mday << '/'
		<< (now->tm_mon + 1) << '/'
		<< (now->tm_year + 1900)
		<< endl;
	return true;
}

BOOL TimeTask(char **argv)
{
	if (argv[1] != NULL)
	{
		cout << "Command \"time\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	cout << "Current time: " << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;
	return true;
}

BOOL DirTask(char **argv, LPSTR cur_dir)
{
	if (argv[1] != NULL)
	{
		cout << "Command \"dir\" does not support any option\n";
		return EXIT_FAILURE;
	}
	WIN32_FIND_DATA data;
	_FILETIME time_lastwrite;
	_SYSTEMTIME convert_time;
	string date;
	string time;
	char *char_date = (char *)calloc(15, sizeof(char));
	char *char_time = (char *)calloc(15, sizeof(char));
	const char add[] = "//*";
	cur_dir = strcat(cur_dir, add);
	HANDLE han = FindFirstFile(cur_dir, &data);
	printf("%-15s%-15s%-15s%-15s\n", "Date", "Time", "Type", "Name");
	printf("=========================================================================\n");
	if (han != INVALID_HANDLE_VALUE)
	{
		do
		{
			time_lastwrite = data.ftLastWriteTime;
			FileTimeToSystemTime(&time_lastwrite, &convert_time);

			// Convert date, time to string & char *
			date = to_string(convert_time.wDay) + '/' + to_string(convert_time.wMonth) + '/' + to_string(convert_time.wYear);
			time = to_string(convert_time.wHour) + ':' + to_string(convert_time.wMinute);
			strcpy(char_date, date.c_str());
			strcpy(char_time, time.c_str());

			if (data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "FILE", data.cFileName);
			}
			if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("%-15s%-15s%-15s%-15s\n", char_date, char_time, "FOLDER", data.cFileName);
			}
		} while (FindNextFileA(han, &data) != 0);
		FindClose(han);
		delete char_time;
		delete char_date;
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}

}


// Get Full Environment Variable
BOOL GetAllEnvironmentVar()
{
	const char *str = (char *)GetEnvironmentStrings();
	if (str == NULL)
	{
		printf("Error get all environment variable\n");
		return EXIT_FAILURE;

	}
	int prev = 0;
	std::vector<std::string> env_strings;
	for (int i = 0;; i++)
	{
		if (str[i] == '\0')
		{
			env_strings.push_back(std::string(str + prev, str + i));
			prev = i;
			if (str[i + 1] == '\0')
			{
				break;
			}
		}
	}
	for (unsigned int i = 0; i < env_strings.size(); i++)
	{
		std::cout << env_strings[i] << "\n";
	}
	return EXIT_SUCCESS;
}

// Get a specific environment variable
BOOL GetSpecificEnvironmentVar(char **argv)
{
	if (argv[2] == NULL)
	{
		printf("Too few arguments\n");
		return EXIT_FAILURE;
	}
	if (argv[3] != NULL)
	{
		printf("enva -get comment doesn't suppport any arguments\n");
		return EXIT_FAILURE;
	}
	char *value = (char *)calloc(10000, sizeof(char));
	DWORD byte = 10000 * sizeof(char);
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)
	{
		printf("Don't find any '%s' variable\n", argv[2]);
	}
	else
	{
		printf("%s = %s\n", argv[2], value);
	}
	delete value;
	return EXIT_SUCCESS;
}

// Set a specific environment variable
BOOL SetEnvironmentVar(char **argv)
{
	if (argv[3] == NULL)
	{
		printf("Too few arguments\n");
		return EXIT_FAILURE;
	}
	char *new_value = CombineLine(argv, 3);
	// get evironment var
	char *value = (char *)calloc(10000, sizeof(char));
	char extension[] = ";";
	DWORD byte = 10000 * sizeof(char);
	// if variable doesn't exist, make a new variable. Else then add to existance value
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)
	{
		if (!SetEnvironmentVariableA(argv[2], new_value))
		{
			printf("Set environment variable ERROR\n");
				return EXIT_FAILURE;
		}
	}
	else
	{
		value = strcat(value, new_value);
		value = strcat(value, extension);
		if (!SetEnvironmentVariableA(argv[2], value))
		{
			printf("Set environment variable ERROR\n");
			return EXIT_FAILURE;
		}
	}
	// reset value

	delete value;
	return EXIT_SUCCESS;


}

BOOL AnalysisEnvironmentVar(char **argv)
{
	if (argv[1] == NULL)
	{
		printf("Too few arguments.\n");
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-all") == 0)
	{
		if (argv[2] != NULL)
		{
			printf("Too many arguments\n");
			return EXIT_FAILURE;
		}
		else
		{
			GetAllEnvironmentVar();
			return EXIT_SUCCESS;
		}
	}
	else if (strcmp(argv[1], "-get") == 0)
	{
		GetSpecificEnvironmentVar(argv);
		return EXIT_SUCCESS;
	}
	else if (strcmp(argv[1], "-set") == 0)
	{
		SetEnvironmentVar(argv);
		return EXIT_SUCCESS;
	}
	else
	{
		printf("enva comment doesn't support any '%s' function.\n", argv[1]);
		return EXIT_FAILURE;
	}
}

BOOL AnalysisInput(char **argv, LPSTR cur_dir)
{
	if (strcmp(argv[0], spe_arg[0]) == 0)
	{
		HelpTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[1]) == 0)
	{
		CDTask(argv, cur_dir);
	}
	else if (strcmp(argv[0], spe_arg[2]) == 0)
	{
		exit(0);
	}
	else if (strcmp(argv[0], spe_arg[3]) == 0)
	{
		AnalysisEnvironmentVar(argv);
	}
	else if (strcmp(argv[0], spe_arg[4]) == 0)
	{
		DateTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[5]) == 0)
	{
		TimeTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[6]) == 0)
	{
		DirTask(argv, cur_dir);
	}
	else if (strcmp(argv[0], spe_arg[7]) == 0)
	{
		AnalysisProcess(argv);
	}
	else
	{
		cout << "Command not found\nType 'help' to see more information\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}