#include "util.h"

#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#include <initializer_list>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

using namespace ROUTINE ;

int test () {

	return 0 ;
}