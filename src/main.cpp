﻿#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	GlobalProc::shutdown () ;
	return 0 ;
}