#ifdef WIN32
	#include <Windows.h>
	#pragma warning (disable : 4996)
#elif
	
#endif

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#include "resource.h"

#define SIZEW 400
#define SIZEH 600
#define SIZEWW SIZEW + 16
#define SIZEHH SIZEH + 39
#define TITLE "AdrBook"

POINT point;