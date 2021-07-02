module csc.core ;

using BOOL = bool_t ;

static constant TRUE = BOOL (true) ;
static constant FALSE = BOOL (false) ;

using VAR32 = int32_t ;
using VAR64 = int64_t ;

static constant VAR32_MAX = VAR32 (2147483647) ;
static constant VAR32_MIN = -VAR32_MAX ;
static constant VAR64_MAX = VAR64 (9223372036854775807) ;
static constant VAR64_MIN = -VAR64_MAX ;

trait VAR_HELP<> {
	require (macro ("config::VAR32")) ;

	using VAR = VAR32 ;
	using VAR_MAX = VAR32_MAX ;
	using VAR_MIN = VAR32_MIN ;
} ;

trait VAR_HELP<> {
	require (macro ("config::VAR64")) ;

	using VAR = VAR64 ;
	using VAR_MAX = VAR64_MAX ;
	using VAR_MIN = VAR64_MIN ;
} ;

using VAR = typename VAR_HELP<>::VAR ;
using VAR_MAX = VAR_HELP<>::VAR_MAX ;
using VAR_MIN = VAR_HELP<>::VAR_MIN ;

static constant ZERO = VAR (+0) ;
static constant IDEN = VAR (+1) ;
static constant NONE = VAR (-1) ;
static constant USED = VAR (-2) ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

using SINGLE = float32_t ;
using DOUBLE = float64_t ;

static constant SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constant SINGLE_MIN = -SINGLE_MAX ;
static constant SINGLE_LOW = SINGLE (1.175494351E-38) ;
static constant SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constant SINGLE_INF = SINGLE (infinity) ;
static constant DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constant DOUBLE_MIN = -DOUBLE_MAX ;
static constant DOUBLE_LOW = DOUBLE (2.2250738585072014E-308) ;
static constant DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constant DOUBLE_INF = SINGLE (infinity) ;

using STRA = char_t ;
using STRW = wchar_t ;

trait STR_HELP<> {
	require (macro ("config::STRA")) ;

	using STR = STRA ;
} ;

trait STR_HELP<> {
	require (macro ("config::STRW")) ;

	using STR = STRW ;
} ;

using STR = typename STR_HELP<>::STR ;

using BYTE = byte8_t ;
using WORD = byte16_t ;
using CHAR = byte32_t ;
using FEAT = byte64_t ;
using HUGE = byte128_t ;

static constant NULL = null ;

define SIZE_OF<ARG1> = std::sizeof (ARG1) ;
define ALIGN_OF<ARG1> = std::alignof (ARG1) ;
define COUNT_OF<ARG1> = std::countof (ARG1) ;

define IS_ENUM<ARG1> = std::is_enum (ARG1) ;
define IS_TYPE<ARG1> = std::is_type (ARG1) ;

define IS_SAME<ARG1 ,ARG2> = std::is_same (ARG1 ,ARG2) ;

using ENUM_ZERO = enum (+0) ;
using ENUM_IDEN = enum (+1) ;
using ENUM_NONE = enum (-1) ;
using ENUM_USED = enum (-2) ;

trait ENABLE_HELP<ARG1 ,ARG2> {
	require (ARG1) ;

	using RET = ARG2 ;
} ;

define ENABLE<COND> = typename ENABLE_HELP<COND ,void>::RET ;
define ENABLE<COND ,YES> = typename ENABLE_HELP<COND ,YES>::RET ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (ARG1) ;

	using RET = ARG2 ;
} ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require not (ARG1) ;

	using RET = ARG3 ;
} ;

define CONDITIONAL<COND ,YES ,NO> = typename CONDITIONAL_HELP<COND ,YES ,NO>::RET ;

define ENUM_CHECK<ARG1> = ENABLE<IS_ENUM<ARG1> ,ARG1> ;
define ENUM_EQUAL<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value == ENUM_CHECK<ARG2>::value) ;
define ENUM_NOT_EQUAL<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value != ENUM_CHECK<ARG2>::value) ;
define ENUM_COMPR<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value <=> ENUM_CHECK<ARG2>::value) ;
define ENUM_COMPR_LT<ARG1 ,ARG2> = enum (ENUM_COMPR<ARG1 ,ARG2>::value < 0) ;
define ENUM_COMPR_LTEQ<ARG1 ,ARG2> = enum (ENUM_COMPR<ARG1 ,ARG2>::value <= 0) ;
define ENUM_COMPR_GT<ARG1 ,ARG2> = enum (ENUM_COMPR<ARG1 ,ARG2>::value > 0) ;
define ENUM_COMPR_GTEQ<ARG1 ,ARG2> = enum (ENUM_COMPR<ARG1 ,ARG2>::value >= 0) ;
define ENUM_EQ_ZERO<ARG1> = ENUM_EQUAL<ARG1 ,ENUM_ZERO> ;
define ENUM_LT_ZERO<ARG1> = ENUM_COMPR_LT<ARG1 ,ENUM_ZERO> ;
define ENUM_GT_ZERO<ARG1> = ENUM_COMPR_GT<ARG1 ,ENUM_ZERO> ;
define ENUM_EQ_IDEN<ARG1> = ENUM_EQUAL<ARG1 ,ENUM_IDEN> ;
define ENUM_LT_IDEN<ARG1> = ENUM_COMPR_LT<ARG1 ,ENUM_IDEN> ;
define ENUM_GT_IDEN<ARG1> = ENUM_COMPR_GT<ARG1 ,ENUM_IDEN> ;

define ENUM_ADD<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value + ENUM_CHECK<ARG2>::value) ;
define ENUM_SUB<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value - ENUM_CHECK<ARG2>::value) ;
define ENUM_MUL<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value * ENUM_CHECK<ARG2>::value) ;
define ENUM_DIV<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value / ENUM_CHECK<ARG2>::value) ;
define ENUM_MOD<ARG1 ,ARG2> = enum (ENUM_CHECK<ARG1>::value % ENUM_CHECK<ARG2>::value) ;
define ENUM_MINUS<ARG1> = ENUM_SUB<ENUM_ZERO ,ARG1> ;
define ENUM_INC<ARG1> = ENUM_ADD<ARG1 ,ENUM_IDEN> ;
define ENUM_DEC<ARG1> = ENUM_SUB<ARG1 ,ENUM_IDEN> ;

define TYPE_CHECK<ARG1> = ENABLE<IS_TYPE<ARG1> ,ARG1> ;
define FIRST_ONE<ARG1 ,ARGS...> = ARG1 ;
define FIRST_REST<ARG1 ,ARGS...> = ARGS ;
define TYPE_FIRST_ONE<ARG1> = FIRST_ONE<ARG1...> ;
define TYPE_FIRST_REST<ARG1> = FIRST_REST<ARG1...> ;
define SECOND_ONE<ARG1 ,ARGS...> = ARG1 ;
define SECOND_REST<ARG1 ,ARGS...> = ARGS ;
define TYPE_SECOND_ONE<ARG1> = SECOND_ONE<ARG1...> ;
define TYPE_SECOND_REST<ARG1> = SECOND_REST<ARG1...> ;
define TYPE_UNWIND<ARG1> = ENABLE<ENUM_EQ_IDEN<COUNT_OF<ARG1>> ,TYPE_FIRST_ONE<ARG1>> ;
define TYPE_CAT<ARG1 ,ARG2> = type<ARG1... ,ARG2...> ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = type<> ;
} ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using R1X = typename TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2>>::RET ;
	using RET = TYPE_CAT<R1X ,type<ARG1>> ;
} ;

define TYPE_REPEAT<ITEM ,SIZE> = typename TYPE_REPEAT_HELP<ITEM ,SIZE>::RET ;

trait ENUM_ALL_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNT_OF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

trait ENUM_ALL_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNT_OF<ARG1>>) ;

	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R3X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1>>::RET ;
	using RET = CONDITIONAL<R1X ,R3X ,ENUM_ZERO> ;
} ;

define ENUM_ALL<ARGS...> = typename ENUM_ALL_HELP<ARGS>::RET ;

trait ENUM_ANY_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNT_OF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

trait ENUM_ANY_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNT_OF<ARG1>>) ;

	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R3X = typename ENUM_ANY_HELP<TYPE_FIRST_REST<ARG1>>::RET ;
	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R3X> ;
} ;

define ENUM_ANY<ARGS...> = typename ENUM_ANY_HELP<ARGS>::RET ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_EQ_ZERO<ARG1>) ;
	
	class PlaceHolder {} ;
} ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_GT_ZERO<ARG1>) ;

	using R1X = typename PLACEHOLDER_HELP<ENUM_DEC<ARG1>>::PlaceHolder ;

	class PlaceHolder {
		extend mSuper :R1X ;
	} ;
} ;

define PlaceHolder<RANK> = typename PLACEHOLDER_HELP<RANK>::PlaceHolder ;

static constant PH0 = PlaceHolder<enum (+0)> () ;
static constant PH1 = PlaceHolder<enum (+1)> () ;
static constant PH2 = PlaceHolder<enum (+2)> () ;
static constant PH3 = PlaceHolder<enum (+3)> () ;
static constant PH4 = PlaceHolder<enum (+4)> () ;
static constant PH5 = PlaceHolder<enum (+5)> () ;
static constant PH6 = PlaceHolder<enum (+6)> () ;
static constant PH7 = PlaceHolder<enum (+7)> () ;
static constant PH8 = PlaceHolder<enum (+8)> () ;
static constant PH9 = PlaceHolder<enum (+9)> () ;
static constant PHX = PlaceHolder<enum (10)> () ;

define IS_CLASS<ARG1> = std::is_class (ARG1) ;
define IS_CONSTRUCTIBLE<RETURN ,PARAMS> = std::is_constructible (RETURN ,PARAMS) ;
define IS_CONVERTIBLE<FROM ,TO> = IS_CONSTRUCTIBLE<TO ,type<FROM>> ;
define IS_CLONEABLE<ARG1> = std::is_cloneable (ARG1) ;
define IS_NULLOPT<ARG1> = std::is_nullopt (ARG1) ;
define IS_TRIVIAL<ARG1> = std::is_trivial (ARG1) ;
define IS_INTERFACE<ARG1> = std::is_interface (ARG1) ;
define IS_EXTEND<BASE ,DERIVED> = std::is_extend (BASE ,DERIVED) ;
define IS_FUNCTION<ARG1> = std::is_function (ARG1) ;
define FUNCTION_REFLECT<ARG1> = std::function_reflect (ARG1) ;
define FUNCTION_RETURN<ARG1> = TYPE_FIRST_ONE<FUNCTION_REFLECT<ARG1>> ;
define FUNCTION_PARAMS<ARG1> = TYPE_SECOND_ONE<FUNCTION_REFLECT<ARG1>> ;
define IS_BOOL<ARG1> = IS_SAME<ARG1 ,BOOL> ;
define IS_VAR<ARG1> = ENUM_ANY<IS_SAME<ARG1 ,VAR32> ,IS_SAME<ARG1 ,VAR64>> ;
define IS_FLOAT<ARG1> = ENUM_ANY<IS_SAME<ARG1 ,SINGLE> ,IS_SAME<ARG1 ,DOUBLE>> ;
define IS_STR<ARG1> = ENUM_ANY<IS_SAME<ARG1 ,STRA> ,IS_SAME<ARG1 ,STRW>> ;
define IS_BYTE<ARG1> = ENUM_ANY<IS_SAME<ARG1 ,BYTE> ,IS_SAME<ARG1 ,WORD> ,IS_SAME<ARG1 ,CHAR> ,IS_SAME<ARG1 ,FEAT> ,IS_SAME<ARG1 ,HUGE>> ;
define IS_NULL<ARG1> = IS_SAME<ARG1 ,type (NULL)> ;
define IS_BASIC<ARG1> = ENUM_ANY<IS_BOOL<ARG1> ,IS_VAR<ARG1> ,IS_FLOAT<ARG1> ,IS_STR<ARG1> ,IS_BYTE<ARG1> ,IS_NULL<ARG1>> ;

define IS_PLACEHOLDER<ARG1> = IS_EXTEND<ARG1 ,type (PH0)> ;

trait BYTE_BASE_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZE_OF<BYTE>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGN_OF<BYTE>>) ;

	using RET = BYTE ;
} ;

trait BYTE_BASE_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZE_OF<WORD>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGN_OF<WORD>>) ;

	using RET = WORD ;
} ;

trait BYTE_BASE_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZE_OF<CHAR>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGN_OF<CHAR>>) ;

	using RET = CHAR ;
} ;

trait BYTE_BASE_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZE_OF<FEAT>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGN_OF<FEAT>>) ;

	using RET = FEAT ;
} ;
} ;

trait BYTE_BASE_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZE_OF<HUGE>>) ;
	require (ENUM_EQUAL<ARG2 ,ALIGN_OF<HUGE>>) ;

	using RET = HUGE ;
} ;

define BYTE_BASE<SIZE ,ALIGN> = typename BYTE_BASE_HELP<SIZE ,ALIGN>::RET ;

trait STORAGE_HELP<ARG1 ,ARG2> {
	using R3X = BYTE_BASE<ARG2 ,ARG2> ;
	using R1X = SIZE_OF<R3X> ;
	using R4X = ENUM_DIV<ENUM_ADD<ARG1 ,ENUM_DEC<R1X>> ,R1X> ;
	using R2X = TYPE_REPEAT<R3X ,R4X> ;

	struct Storage {
		private variable mUnused... :R2X... ;
	} ;
} ;

define Storage<SIZE> = typename STORAGE_HELP<SIZE ,ENUM_IDEN>::Storage ;
define Storage<SIZE ,ALIGN> = typename STORAGE_HELP<SIZE ,ALIGN>::Storage ;

define ENUM_ABS<ARG1> = CONDITIONAL<ENUM_COMPR_GTEQ<ARG1 ,ENUM_ZERO> ,ARG1 ,ENUM_MINUS<ARG1>> ;
define ENUM_MIN<ARG1 ,ARG2> = CONDITIONAL<ENUM_COMPR_LTEQ<ARG1 ,ARG2> ,ARG1 ,ARG2> ;
define ENUM_MAX<ARG1 ,ARG2> = CONDITIONAL<ENUM_COMPR_GTEQ<ARG1 ,ARG2> ,ARG1 ,ARG2> ;
define ENUM_BETWEEN<CURR ,BEGIN ,END> = ENUM_ALL<ENUM_COMPR_GTEQ<CURR ,BEGIN> ,ENUM_COMPR_LT<CURR ,END>> ;

static function noop = () => {} ;

static function address = (arg1) :LENGTH => std::builtin_address (arg1) ;

static function swap = (&arg1 ,&arg2) => std::builtin_swap (arg1 ,arg2) ;

static function forward = (&&obj) => std::builtin_forward (obj) ;

static function zeroize = (&arg1) => std::builtin_zeroize (obj) ;

static funcion bad = (id) => {
	using R1X = type (id) ;
	register r1x = () :auto => ZERO ;
	assert (FALSE) ;
	return R1X (r1x ()) ;
} ;

static property memorize = [proc] => {
	static constant M_MEMORIZE = proc () ;
	return M_MEMORIZE ;
} ;

static function abs = (arg1) => {
	if (arg1 >= 0)
		return arg1 ;
	return -arg1 ;
} ;

static function min = (arg1 ,arg2) => {
	if (arg1 <= arg2)
		return arg1 ;
	return arg2 ;
} ;

static function max = (arg1 ,arg2) => {
	if (arg1 >= arg2)
		return arg1 ;
	return arg2 ;
} ;

static function between = (curr :INDEX ,begin :INDEX ,end :INDEX) :BOOL => {
	if (curr < begin)
		return FALSE ;
	if (curr >= end)
		return FALSE ;
	return TRUE ;
} ;

trait HASHCODE_HELP<> {
	require (IS_SAME<FLAG ,VAR32>) ;

	static function hashcode = () :FLAG => FLAG (-2128831035) ;

	static function hashcode = (now :FLAG ,inc :FLAG) :FLAG => {
		using R2X = BYTE_BASE<FLAG> ;
		constant r1x = R2X (now) ^ R2X (inc) ;
		constant r2x = R2X (VAR (r1x) * VAR (16777619)) ;
		constant r3x = r2x & R2X (VAR_MAX) ;
		return FLAG (r3x) ;
	} ;
} ;

trait HASHCODE_HELP<> {
	require (IS_SAME<FLAG ,VAR64>) ;

	static function hashcode = () :FLAG => FLAG (-3750763034362895579) ;

	static function hashcode = (now :FLAG ,inc :FLAG) :FLAG => {
		using R2X = BYTE_BASE<FLAG> ;
		constant r1x = R2X (now) ^ R2X (inc) ;
		constant r2x = R2X (VAR (r1x) * VAR (1099511628211)) ;
		constant r3x = r2x & R2X (VAR_MAX) ;
		return FLAG (r3x) ;
	} ;
} ;

static function hashcode = () :FLAG => HASHCODE_HELP<>::hashcode () ;

static function hashcode = (now :FLAG ,inc :FLAG) :FLAG => HASHCODE_HELP<>::hashcode (now ,inc) ;

trait RANGE_ITERATOR_HELP<> {
	class RangeIterator {
		constant mBegin :INDEX ;
		constant mEnd :INDEX ;
		variable mCurr :INDEX ;
	} ;

	implement RangeIterator {
		function new = (begin :INDEX ,end :INDEX) => {
			mBegin = begin ;
			mEnd = max (begin ,end) ;
			mCurr = begin ;
		} ;

		function good = () :BOOL => mCurr < mEnd ;

		property at = [] :INDEX => mCurr ;

		function next = mutable () => {
			mCurr = mCurr + 1 ;
		} ;
	} ;
} ;

using RangeIterator = typename RANGE_ITERATOR_HELP<>::RangeIterator ;

static function range = (begin ,end) :RangeIterator => RangeIterator (begin ,end) ;

trait SLICE_HELP<ARG1> {
	using UNIT1 = ARG1 ;

	class Slice {
		interface Holder ;

		constant mPointer :[] :Holder ;
	} ;

	interface Slice::Holder {
		function size = () :LENGTH => virtual ;
		function addr = () :LENGTH => virtual ;
		property at = [index :INDEX] :UNIT1 => virtual ;
	} ;

	implement Slice {
		function new = () => default ;

		function new = (that) => {
			mPointer = std::builtin_slice (that) ;
		} ;

		function size = () :LENGTH => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->size () ;
		} ;

		function addr = () :LENGTH => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->addr () ;
		} ;

		property at = [index :INDEX] :UNIT1 => {
			assert (between (index ,0 ,size ())) ;
			return mPointer->at[index] ;
		} ;

		function equal = (that :Slice) :BOOL => {
			if (size () != that.size ())
				return FALSE ;
			for (i) in (range (0 ,size ())) {
				if (at[i] != that.at[i])
					return FALSE ;
			} ;
			return TRUE ;
		} ;

		function compr = (that :Slice) :FLAG => {
			constant r1x = min (size () ,that.size ()) ;
			for (i) in (range (0 ,r1x)) {
				constant r2x = at[i] <=> that.at[i] ;
				if (r2x != ZERO)
					return r2x ;
			} ;
			return ZERO ;
		} ;

		function hash = () :FLAG => {
			variable ret = hashcode () ;
			for (i) in (range (0 ,size ())) {
				constant r1x = FLAG (at[i]) ;
				ret = hashcode (ret ,r1x) ;
			} ;
			return ret ;
		} ;
	} ;
} ;

define Slice<ARG1> = typename SLICE_HELP<ARG1>::Slice ;

trait CLAZZ_HELP<> {
	class Clazz {
		interface Holder ;
		
		constant mPointer :[] :Holder ;
	} ;

	interface Clazz::Holder {
		function type_size = () :LENGTH => virtual ;
		function type_align = () :LENGTH => virtual ;
		function type_cabi = () :FLAG => virtual ;
		function type_name = () :Slice<STR> => virtual ;
	} ;

	implement Clazz {
		function new = () => default ;

		function new = (that) => {
			mPointer = std::builtin_clazz (that) ;
		} ;

		function type_size = () :LENGTH => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->type_size () ;
		} ;

		function type_align = () :LENGTH => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->type_align () ;
		} ;

		function type_cabi = () :FLAG => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->type_cabi () ;
		} ;

		function type_name = () :Slice<STR> => {
			if (mPointer == NULL)
				return Slice<STR> () ;
			return mPointer->type_name () ;
		} ;

		function equal = (that :Clazz) :BOOL => {
			if (type_cabi () == that.type_cabi ())
				return TRUE ;
			if (type_size () != that.type_size ())
				return FALSE ;
			if (type_align () != that.type_align ())
				return FALSE ;
			if (type_name () != that.type_name ())
				return FALSE ;
			return TRUE ;
		} ;

		function compr = (that :Clazz) :FLAG => {		
			if (type_cabi () == that.type_cabi ())
				return ZERO ;
			constant r1x = type_name () <=> that.type_name () ;
			if (r1x != ZERO)
				return r1x ;
			constant r2x = type_size () <=> that.type_size () ;
			if (r2x != ZERO)
				return r2x ;
			constant r3x = type_align () <=> that.type_align () ;
			if (r3x != ZERO)
				return r3x ;
			return ZERO ;
		} ;

		function hash = () :FLAG => type_name ().hash () ;
	} ;
} ;

using Clazz = typename CLAZZ_HELP<>::Clazz ;

trait TUPLE_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNT_OF<ARG1>>) ;

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
	require (ENUM_GT_ZERO<COUNT_OF<ARG1>>) ;

	using UNITS = ARG1 ;
	using ONE = TYPE_FIRST_ONE<UNITS> ;
	using REST = TYPE_FIRST_REST<UNITS> ;

	class Tuple {
		extend mSuper :Tuple<REST> ;
		variable mTuple :ONE ;
	} ;

	implement Tuple {
		function new = () => default ;

		function new = (&&arg1 :ONE ,&&args... :REST...) => {
			mSuper = Tuple<REST> (forward (args)...) ;
			mTuple = forward (arg1) ;
		} ;

		function rank = () :LENGTH => COUNT_OF<UNITS>::value ;

		property one = mutable [] :ONE => mTuple ;

		property rest = mutable [] :Tuple<REST> => mSuper ;

		function equal = (that :Tuple) :BOOL => {
			if (one != that.one)
				return FALSE ;
			return rest == that.rest () ;
		} ;

		function compr = (that :Tuple) :FLAG => {
			constant r1x = one <=> that.one ;
			if (r1x != ZERO)
				return r1x ;
			return rest <=> that.rest ;
		} ;

		function hash = () :FLAG => {
			variable ret = rest.hash () ;
			ret = hashcode (ret ,operator hash (one)) ;
			return ret ;
		} ;
	} ;
} ;

define Tuple<ARGS...> = typename TUPLE_HELP<ARGS>::Tuple ;

static function tuple = (&&args...) => {
	using R1X = type (args) ;
	return Tuple<R1X> (forward (args)...) ;
} ;

