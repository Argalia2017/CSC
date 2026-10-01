#pragma once

#ifndef __CSC_NUMERIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_numeric.hpp"

namespace CSC {
template class External<NumericProblemHolder ,NumericProblemLayout> ;

exports VFat<NumericProblemHolder> NumericProblemHolder::hold (VR<NumericProblemLayout> that) {
	return VFat<NumericProblemHolder> (External<NumericProblemHolder ,NumericProblemLayout>::expr ,that) ;
}

exports CFat<NumericProblemHolder> NumericProblemHolder::hold (CR<NumericProblemLayout> that) {
	return CFat<NumericProblemHolder> (External<NumericProblemHolder ,NumericProblemLayout>::expr ,that) ;
}
} ;
