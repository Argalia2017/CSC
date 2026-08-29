#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <exception>
#include <csc_begin.h>

using namespace ROUTINE ;

/*
1. VR<[^\r]+> expr_m
2. Super<Ref<\w+Layout>>
3. struct \w+ (implement [^\r]+ )?\{(\r\n(?!\} ;)[^\r]*)*(private|public):(?!\r\n\t+implicit \w+Layout)
*/

int test () {

	return 0 ;
}