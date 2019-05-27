#include <iostream>
#include <Windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>
#include "analysis.h"
#include <string.h>

using namespace std;

HANDLE h_fore_process = NULL;

BOOL GetProcessListAll()
{
	HANDLE h_process_snap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(h_process_snap, &pe32))
	{
		printf("Process32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_process_snap);          // clean the snapshot object
		return FALSE;
	}
	// Retrieve the priority class.
	printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
	do
	{
		printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
	} while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return(TRUE);
}
BOOL GetChildProcessList(DWORD pid)
{
	HANDLE h_process_snap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(h_process_snap, &pe32))
	{
		printf("Process32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_process_snap);          // clean the snapshot object
		return FALSE;
	}
	// Retrieve the priority class.
	printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
	do
	{
		if (pe32.th32ParentProcessID == pid)
			printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
	} while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return(TRUE);
}
BOOL SuspendProcess(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	HANDLE h_thread;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		printf("Thread32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_thread_snap);          // clean the snapshot object
		return FALSE;
	}
	// Walk other threads
	do
	{
		if (th32.th32OwnerProcessID == pid)
		{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (SuspendThread(h_thread) == -1)
			{
				return FALSE;
			}
		}
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}
BOOL ResumeProcess(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	HANDLE h_thread;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		printf("Thread32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_thread_snap);          // clean the snapshot object
		return FALSE;
	}
	// Walk other threads
	do
	{
		if (th32.th32OwnerProcessID == pid)
		{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (ResumeThread(h_thread) == -1)
			{
				return FALSE;
			}
		}
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}
BOOL GetThreadList(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		printf("Thread32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_thread_snap);          // clean the snapshot object
		return FALSE;
	}
	// Walk other threads
	printf("%-50s%-20s\n", "Thread ID", "OwnerProcessID");
	printf("%-50s%-20s\n", "----------------------------------", "----------");
	do
	{
		if (th32.th32OwnerProcessID == pid)
			printf("%-50d%-20d\n", th32.th32ThreadID, th32.th32OwnerProcessID);
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}
BOOL FindProcessID(const char *process_name)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(EXIT_FAILURE);

	pe32.dwSize = sizeof(PROCESSENTRY32); // <----- IMPORTANT

	// Retrieve information about the first process,
	// and exit if unsuccessful
	printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		printf("!!! Failed to gather information on system processes! \n");
		return EXIT_FAILURE;
	}

	do
	{
		// printf("Checking process %ls\n", pe32.szExeFile);
		if (0 == strcmp(process_name, pe32.szExeFile))
		{
			printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return EXIT_SUCCESS;
}
BOOL KillProcess(DWORD pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL)
	{
		printf("Failed!");
		return EXIT_FAILURE;
	}
	if (!TerminateProcess(hProcess, 1))
	{
		printf("Failed!");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
BOOL CreateNewProcess(char **args)
{
	int wait_time;
	// Check parameter
	if (args[2] == NULL)
	{
		printf("Too few argument\n");
		return EXIT_FAILURE;
	}

	if (strcmp(args[1], "-b"))
	{
		wait_time = INFINITE;
	}
	else
	{
		wait_time = 0;
	}

	char *run_file = CombineLine(args, 2);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.lpTitle = args[1];
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,               // No module name (use command line)
		run_file,            // Command line
		NULL,               // Process handle not inheritable
		NULL,               // Thread handle not inheritable
		FALSE,              // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE, // Create new console
		NULL,               // Use parent's environment block
		NULL,               // Use parent's starting directory
		&si,                // Pointer to STARTUPINFO structure
		&pi)                // Pointer to PROCESS_INFORMATION structure
		)
	{
		int error = GetLastError();
		if (error == 2)
			printf("The batch file or execute file \"%s\" is not found.\n", run_file);
		else
			printf("Can't run this file\n");
		return EXIT_FAILURE;
	}

	// display process in new window
	HWND console_name = FindWindow(NULL, NULL);
	if (console_name)
	{
		ShowWindow(console_name, SW_SHOW);
	}
	// get handle of child fore process
	if (strcmp(args[1], "-f") == 0)
	{
		h_fore_process = pi.hProcess;
	}
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, wait_time);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return EXIT_SUCCESS;
}
BOOL AnalysisProcess(char **argv)
{
	if (argv[1] == NULL)
	{
		printf("Too few arguments.\n");
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-all") == 0)
	{
		// Print All Process
		if (GetProcessListAll())
		{
			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-child") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few arguments\n");
			return EXIT_FAILURE;
		}
		// Print All Child Process
		DWORD pid = atoi(argv[2]);
		if (GetChildProcessList(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-suspend") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few arguments\n");
			return EXIT_FAILURE;
		}
		// Suspend Process
		DWORD pid = atoi(argv[2]);
		if (SuspendProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-resume") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few argument\n");
			return EXIT_FAILURE;
		}
		// Resume Process
		DWORD pid = atoi(argv[2]);
		if (ResumeProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-thread") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few arguments\n");
			return EXIT_FAILURE;
		}
		// Get List Thread of Process
		DWORD pid = atoi(argv[2]);
		if (GetThreadList(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-find") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few arguments\n");
			return EXIT_FAILURE;
		}
		// Find a PID of Process
		if (FindProcessID(argv[2]))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "-b") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few arguments\n");
			return EXIT_FAILURE;
		}
		// Find a PID of Process
		if (CreateNewProcess(argv))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-kill") == 0)
	{
		if (argv[2] == NULL)
		{
			printf("Too few argument\n");
			return EXIT_FAILURE;
		}
		// Suspend Process
		DWORD pid = atoi(argv[2]);
		if (KillProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	printf("Command \"%s\" does not support \"%s\" options !\n", argv[0],argv[1]);
	return EXIT_FAILURE;
}