#pragma once

#ifdef WIN32
	#include <Windows.h>
	#pragma warning (disable : 4996)
#elif

#endif

#include <stdlib.h>

#include "resource.h"

#include "OpenFileDialog.h"

#include "drawing.h"

#define SIZEW 400
#define SIZEH 600
#define SIZEWW SIZEW + 16
#define SIZEHH SIZEH + 39

static POINT point;

using namespace std;