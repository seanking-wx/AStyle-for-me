// AStyle.cpp : 定义控制台应用程序的入口点。
//
#undef UNICODE      // use ASCII windows functions
#include "targetver.h"
#include <stdio.h>

#include "ASConsole.h"

using namespace AStyle;

int _CRT_glob = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	ASFormatter formatter;
	g_console = new ASConsole(formatter);
	// process command line and options file
	// build the vectors fileNameVector, optionsVector, and fileOptionsVector
	vector<string> argvOptions;
	argvOptions = g_console->getArgvOptions(argc, argv);
	g_console->processOptions(argvOptions);
	if(g_console->fileNameVectorIsEmpty())  // if no files have been given, use cin for input and cout for output
		g_console->formatCinToCout();
	else                                    // else process entries in the fileNameVector
		g_console->processFiles();
	delete g_console;
	return EXIT_SUCCESS;
}

