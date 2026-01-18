#pragma once

#include <csc.hpp>
#include <csc_type.hpp>
#include <csc_core.hpp>
#include <csc_basic.hpp>
#include <csc_math.hpp>
#include <csc_array.hpp>
#include <csc_image.hpp>
#include <csc_matrix.hpp>
#include <csc_stream.hpp>
#include <csc_string.hpp>
#include <csc_runtime.hpp>
#include <csc_file.hpp>
#include <csc_thread.hpp>

namespace ROUTINE {
using namespace CSC ;

using BOOL = Bool ;
using VAL32 = Val32 ;
using VAL64 = Val64 ;
using VAL = Val ;
using INDEX = Index ;
using LENGTH = Length ;
using FLAG = Flag ;
using FLT32 = Flt32 ;
using FLT64 = Flt64 ;
using BYTE = Byte ;
using WORD = Word ;
using CHAR = Char ;
using QUAD = Quad ;
using STRA = Stra ;
using STRW = Strw ;
using STRU8 = Stru8 ;
using STRU16 = Stru16 ;
using STRU32 = Stru32 ;
using STR = Str ;

using CSC::move ;
using CSC::swap ;

#ifndef CLOG
#define CLOG Singleton<Console>::expr
#endif
} ;