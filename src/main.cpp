#include "util.h"

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#include <initializer_list>
#include <cstdio>
#include <csc_begin.h>

using namespace ROUTINE ;

/*
1. Proxy
2. root_ptr
3. keep
*/

int main () {
	auto rax = Set<Index> ({1 ,2 ,3 ,4 ,5 ,6}) ;
	notice (rax) ;
	notice (rax) ;
	return 0 ;
}