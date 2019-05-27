#pragma once
#ifndef _FEATURES_H
#define _FEATURES_H

#include <iostream>
#include <Windows.h>
#include <ctime>
#include "analysis.h"


using namespace std;

BOOL HelpTask(char **argv);

BOOL CDTask(char **argv, LPSTR cur_dir);

BOOL DateTask(char **argv);

BOOL TimeTask(char **argv);

BOOL GetAllEnvironmentVar();

BOOL GetSpecificEnvironmentVar(char **argv);

BOOL SetEnvironmentVar(char **argv);

BOOL AnalysisEnvironmentVar(char **argv);
// Analysis input to choose the function to execute
BOOL AnalysisInput(char **argv, LPSTR cur_dir);




#endif

