module rust::basic ;

import module rust::core::* ;

trait TUPLE_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;
	
	class Tuple {} ;

	implement Tuple {
		function new = () => default ;
		
		function rank = () :LENGTH => ZERO ;

		function equal = (that :Tuple) :BOOL => TRUE ;

		function compr = (that :Tuple) :FLAG => ZERO ;

		function hash = () :FLAG => hashcode () ;
	} ;
} ;

trait TUPLE_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using ALL = TYPE_CHECK<ARG1> ;
	using ONE = TYPE_FIRST_ONE<ALL> ;
	using REST = TYPE_FIRST_REST<ALL> ;
	using BASE = typename TUPLE_HELP<REST>::Tuple ;
	
	class Tuple {
		extend mSuper :BASE ;
		variable mValue :ONE ;
	} ;

	implement Tuple {
		function new = () => default ;

		function new = (&&that :ONE ,&&rest... :REST...) => {
			mValue = forward (that) ;
			mSuper = BASE (forward (rest)...) ;
		} ;

		function rank = () :LENGTH => COUNTOF<ALL>::value ;

		property one = [] :ONE => mValue ;

		property rest = [] :BASE => mSuper ;

		trait PICK_HELP<ARG3> {
			require (ENUM_EQ_ZERO<ARG3>) ;

			property pick = [] => one[] ;
		} ;

		trait PICK_HELP<ARG3> {
			require (ENUM_GT_ZERO<ARG3>) ;

			property pick = [] => rest.pick[type<ENUM_DEC<ARG3>>::id] ;
		} ;

		property pick = [nth] => {
			using R1X = type (nth) ;
			require (IS_ENUM<R1X>) ;
			require (ENUM_COMPR_GT_EQ<R1X ,ENUM_ZERO>) ;
			require (ENUM_COMPR_LT<R1X ,COUNTOF<ALL>>) ;
			return PICK_HELP<R1X>::pick[] ;
		} ;

		function equal = (that :Tuple) :BOOL => {
			if (mValue != that.mValue)
				return FALSE ;
			if (mSuper != that.mSuper)
				return FALSE ;
			return TRUE ;
		} ;

		function compr = (that :Tuple) :FLAG => {
			constant r1x = mValue <=> that.mValue ;
			if (r1x != ZERO)
				return r1x ;
			constant r2x = mSuper <=> that.mSuper ;
			if (r2x != ZERO)
				return r2x ;
			return ZERO ;
		} ;

		function hash = () :FLAG => {
			constant r1x = operator hash (mValue) ;
			constant r2x = operator hash (mSuper) ;
			return hashcode (r1x ,r2x) ;
		} ;
	} ;
} ;

define Tuple<UNITS...> = typename TUPLE_HELP<UNITS>::Tuple ;

trait FUNCTION_HELP<ARG1 ,ARG2> {
	using RETURN = ARG1 ;
	using PARAMS = ARG2 ;

	class Function {
		interface Holder ;
		class ImplHolder ;

		variable mPointer :PTR<Holder> ;
	} ;

	interface Function::Holder {
		function invoke = (params... :PARAMS...) :RETURN => virtual ;
	} ;

	trait IMPLHOLDER_HELP<ARG3> {
		require (IS_TRIVIAL<ARG3>) ;

		using UNIT = ARG3 ;

		class Function::ImplHolder {
			variable mValue :UNIT ;
		} ;

		implement Function::ImplHolder {
			function new = () => discard ;

			function new = (&&that :UNIT) => mValue = forward (that) ;
		} ;

		implement Function::ImplHolder :Holder {
			function invoke = (params... :PARAMS...) :RETURN => mValue (params...) ;
		} ;
	} ;

	implement Function {
		function new = () => mPointer = NULL ;

		function new = (&&that) => {
			using R1X = type (that) ;
			using R2X = typename IMPLHOLDER_HELP<R1X>::ImplHolder ;
			register r1x = R2X (forward (that)) ;
			mPointer = PTR<Holder>::make (r1x) ;
		} ;

		function exist = () :BOOL => mPointer != NULL ;

		function rank = () :LENGTH => COUNTOF<PARAMS>::value ;

		function invoke = (params... :PARAMS...) :RETURN => {
			assert (exist ()) ;
			return mPointer->invoke (params...) ;
		} ;
	} ;
} ;

define Function<RETURN ,PARAMS> = typename U::FUNCTION_HELP<RETURN ,PARAMS>::Function ;