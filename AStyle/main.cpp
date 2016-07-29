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
	ASConsole console;
	return console.DoMain(argc, argv);
}

