
module rust::core ;

import module rust ;

using BOOL = bool ;

static constant TRUE = BOOL (true) ;
static constant FALSE = BOOL (false) ;

using VAR32 = int32 ;
using VAR64 = int64 ;

static constant VAR32_MAX = VAR32 (+2147483647) ;
static constant VAR32_MIN = VAR32 (-2147483648) ;
static constant VAR64_MAX = VAR64 (+9223372036854775807) ;
static constant VAR64_MIN = VAR64 (-9223372036854775808) ;

trait VAR_HELP {
	require (macro ("config_int32")) ;

	using VAR = VAR32 ;

	static constant VAR_MAX = VAR32_MAX ;
	static constant VAR_MIN = VAR32_MIN ;
} ;

trait VAR_HELP {
	require (macro ("config_int64")) ;

	using VAR = VAR64 ;

	static constant VAR_MAX = VAR64_MAX ;
	static constant VAR_MIN = VAR64_MIN ;
} ;

using VAR = VAR_HELP::VAR ;

static constant VAR_MAX = VAR_HELP::VAR_MAX ;
static constant VAR_MIN = VAR_HELP::VAR_MIN ;

static constant USED = VAR (-2) ;
static constant NONE = VAR (-1) ;
static constant ZERO = VAR (+0) ;
static constant IDEN = VAR (+1) ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

using FLOAT32 = float32 ;
using FLOAT64 = float64 ;

static constant FLOAT32_MAX = FLOAT32 (3.402823466E+38) ;
static constant FLOAT32_MIN = FLOAT32 (1.175494351E-38) ;
static constant FLOAT32_EPS = FLOAT32 (1.192092896E-07) ;
static constant FLOAT32_INF = FLOAT32 (infinity) ;
static constant FLOAT64_MAX = FLOAT64 (1.7976931348623158E+308) ;
static constant FLOAT64_MIN = FLOAT64 (2.2250738585072014E-308) ;
static constant FLOAT64_EPS = FLOAT64 (2.2204460492503131E-016) ;
static constant FLOAT64_INF = FLOAT64 (infinity) ;

using BYTE = byte8 ;
using WORD = byte16 ;
using CHAR = byte32 ;
using FEAT = byte64 ;

using STRA = stra ;
using STRU8 = stru8 ;
using STRU16 = stru16 ;
using STRU32 = stru32 ;

trait STR_HELP {
	require (macro ("config_stra")) ;

	using STR = STRA ;
} ;

trait STR_HELP {
	require (macro ("config_stru8")) ;

	using STR = STRU8 ;
} ;

trait STR_HELP {
	require (macro ("config_stru16")) ;

	using STR = STRU16 ;
} ;

trait STR_HELP {
	require (macro ("config_stru32")) ;

	using STR = STRU32 ;
} ;

using STR = STR_HELP::STR ;

static constant NULL = null ;

using ENUM_NONE = enum (-1) ;
using ENUM_ZERO = enum (+0) ;
using ENUM_IDEN = enum (+1) ;

using ALIGNOF<UNIT> = enum (rust::alignof (UNIT)) ;
using SIZEOF<UNIT> = enum (rust::sizeof (UNIT)) ;
using COUNTOF<UNIT> = enum (rust::countof (UNIT)) ;

using DEF<UNIT> = UNIT ;

using CALL<UNIT> = UNIT::RET ;

trait ENUM_CHECK_HELP<ARG1> {
	require (IS_ENUM<EXPR>) ;

	using RET = ARG1 ;
} ;

trait ENUM_ALL_HELP<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using RET = ENUM_IDEN ;
} ;

trait ENUM_ALL_HELP<ARGS> {
	require (ENUM_GT<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using R1X = TYPE_FIRST<ARGS> ;
	using R2X = CALL<ENUM_ALL_HELP<TYPE_FIRST_REST<ARGS>>> ;
	using RET = ENUM_AND<R1X ,R2X> ;
} ;

trait ENUM_ANY_HELP<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using RET = ENUM_ZERO ;
} ;

trait ENUM_ANY_HELP<ARGS> {
	require (ENUM_GT<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using R1X = TYPE_FIRST<ARGS> ;
	using R2X = CALL<ENUM_ANY_HELP<TYPE_FIRST_REST<ARGS>>> ;
	using RET = ENUM_OR<R1X ,R2X> ;
} ;

using ENUM_CHECK<EXPR> = CALL<ENUM_CHECK_HELP<EXPR>> ;
using ENUM_AND<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value && ENUM_CHECK<EXPR2>::value) ;
using ENUM_OR<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value || ENUM_CHECK<EXPR2>::value) ;
using ENUM_ALL<...> = CALL<ENUM_ALL_HELP<type<...>>> ;
using ENUM_ANY<...> = CALL<ENUM_ANY_HELP<type<...>>> ;
using ENUM_NOT<EXPR> = enum (!ENUM_CHECK<EXPR>::value) ;
using ENUM_EQ<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value == ENUM_CHECK<EXPR2>::value) ;
using ENUM_NOT_EQ<EXPR1 ,EXPR2> = ENUM_NOT<ENUM_EQ<EXPR1 ,EXPR2>> ;
using ENUM_COMPR<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value <=> ENUM_CHECK<EXPR2>::value) ;
using ENUM_LT<EXPR1 ,EXPR2> = enum (ENUM_COMPR<EXPR1 ,EXPR2>::value < ZERO) ;
using ENUM_GT<EXPR1 ,EXPR2> = enum (ENUM_COMPR<EXPR1 ,EXPR2>::value > ZERO) ;
using ENUM_LT_EQ<EXPR1 ,EXPR2> = enum (ENUM_COMPR<EXPR1 ,EXPR2>::value <= ZERO) ;
using ENUM_GT_EQ<EXPR1 ,EXPR2> = enum (ENUM_COMPR<EXPR1 ,EXPR2>::value >= ZERO) ;
using ENUM_BETWEEN<EXPR ,BEGIN ,END> = ENUM_AND<ENUM_GT_EQ<EXPR ,BEGIN> ,ENUM_LT<EXPR ,END>> ;
using ENUM_ADD<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value + ENUM_CHECK<EXPR2>::value) ;
using ENUM_SUB<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value - ENUM_CHECK<EXPR2>::value) ;
using ENUM_MUL<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value * ENUM_CHECK<EXPR2>::value) ;
using ENUM_DIV<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value / ENUM_CHECK<EXPR2>::value) ;
using ENUM_MOD<EXPR1 ,EXPR2> = enum (ENUM_CHECK<EXPR1>::value % ENUM_CHECK<EXPR2>::value) ;
using ENUM_INC<EXPR> = ENUM_ADD<EXPR ,ENUM_IDEN> ;
using ENUM_DEC<EXPR> = ENUM_SUB<EXPR ,ENUM_IDEN> ;
using ENUM_MAX<EXPR1 ,EXPR2> = CONDITIONAL<ENUM_GT_EQ<EXPR1 ,EXPR2> ,EXPR1 ,EXPR2> ;
using ENUM_MIN<EXPR1 ,EXPR2> = CONDITIONAL<ENUM_LT_EQ<EXPR1 ,EXPR2> ,EXPR1 ,EXPR2> ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_EQ<ARG1 ,ENUM_ZERO>) ;

	class PLACEHOLDER {} ;
} ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_GT<ARG1 ,ENUM_ZERO>) ;

	using R1X = PLACEHOLDER_HELP<ENUM_DECREASE<ARG1>>::PLACEHOLDER ;

	class PLACEHOLDER {
		public extend :R1X ;
	} ;
} ;

using PLACEHOLDER<RANK> = PLACEHOLDER_HELP<RANK>::PLACEHOLDER ;

static constant P0 = PLACEHOLDER<enum (0)> () ;
static constant P1 = PLACEHOLDER<enum (1)> () ;
static constant P2 = PLACEHOLDER<enum (2)> () ;
static constant P3 = PLACEHOLDER<enum (3)> () ;
static constant P4 = PLACEHOLDER<enum (4)> () ;
static constant P5 = PLACEHOLDER<enum (5)> () ;
static constant P6 = PLACEHOLDER<enum (6)> () ;
static constant P7 = PLACEHOLDER<enum (7)> () ;
static constant P8 = PLACEHOLDER<enum (8)> () ;
static constant P9 = PLACEHOLDER<enum (9)> () ;
static constant PX = PLACEHOLDER<enum (10)> () ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (ARG1) ;

	using RET = ARG2 ;
} ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (ENUM_NOT<ARG1>) ;

	using RET = ARG3 ;
} ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_IDEN>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_GT<COUNTOF<ARGS> ,ENUM_IDEN>) ;

	using R1X = TYPE_FIRST<ARGS> ;
	using R2X = TYPE_SECOND<ARGS> ;
	using R3X = CALL<IS_ALL_SAME_HELP<type<R1X ,TYPE_SECOND_REST<ARGS>...>>> ;
	using RET = ENUM_ALL<IS_SAME<R1X ,R2X> ,R3X> ;
} ;

trait IS_ANY_SAME<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_ZERO>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME<ARGS> {
	require (ENUM_EQ<COUNTOF<ARGS> ,ENUM_IDEN>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME<ARGS> {
	require (ENUM_GT<COUNTOF<ARGS> ,ENUM_IDEN>) ;

	using R1X = TYPE_FIRST<ARGS> ;
	using R2X = TYPE_SECOND<ARGS> ;
	using R3X = CALL<IS_ANY_SAME<type<R1X ,TYPE_SECOND_REST<ARGS>...>>> ;
	using R4X = CALL<IS_ANY_SAME<type<R2X ,TYPE_SECOND_REST<ARGS>...>>> ;
	using RET = ENUM_ANY<IS_SAME<R1X ,R2X> ,R3X ,R4X> ;
} ;

using CONDITIONAL<COND ,YES ,NO> = CALL<CONDITIONAL_HELP<COND ,YES ,NO>> ;

using IS_SAME<UNIT1 ,UNIT2> = enum (rust::is_same (UNIT1 ,UNIT2)) ;
using IS_ALL_SAME<...> = CALL<IS_ALL_SAME_HELP<type<...>>> ;
using IS_ANY_SAME<...> = CALL<IS_ANY_SAME_HELP<type<...>>> ;

using IS_BOOL<UNIT> = IS_SAME<UNIT ,BOOL> ;
using IS_VAR<UNIT> = ENUM_ALL<IS_SAME<UNIT ,VAR32> ,IS_SAME<UNIT ,VAR64>> ;
using IS_FLOAT<UNIT> = ENUM_ALL<IS_SAME<UNIT ,FLOAT32> ,IS_SAME<UNIT ,FLOAT64>> ;
using IS_BYTE<UNIT> = ENUM_ALL<IS_SAME<UNIT ,BYTE> ,IS_SAME<UNIT ,WORD> ,IS_SAME<UNIT ,CHAR> ,IS_SAME<UNIT ,FEAT>> ;
using IS_STR<UNIT> = ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<UNIT ,STRU8> ,IS_SAME<UNIT ,STRU16> ,IS_SAME<UNIT ,STRU32>> ;
using IS_BASIC<UNIT> = ENUM_ALL<IS_BOOL<UNIT> ,IS_VAR<UNIT> ,IS_FLOAT<UNIT> ,IS_BYTE<UNIT> ,IS_STR<UNIT>> ;

using IS_ENUM<UNIT> = enum (rust::is_enum (UNIT)) ;

using IS_TYPE<UNIT> = enum (rust::is_type (UNIT)) ;

using TYPE_FIRST<ALL> = ALL::FIRST_TYPE ;
using TYPE_SECOND<ALL> = ALL::SECOND_TYPE ;
using TYPE_FIRST_REST<ALL> = ALL::FIRST_REST_TYPE ;
using TYPE_SECOND_REST<ALL> = ALL::SECOND_REST_TYPE ;
using TYPE_CAT<UNIT1 ,UNIT2> = type<UNIT1... ,UNIT2...> ;

using TYPE_PICK<ALL ,POS> = TYPE_PICK_HELP<ALL ,POS> ;
using TYPE_FIND<ALL ,KEY> = TYPE_FIND_HELP<ALL ,KEY> ;
using TYPE_REPEAT<UNIT ,SIZE> = TYPE_REPEAT_HELP<UNIT ,SIZE> ;

using IS_STRUCT<UNIT> = enum (rust::is_struct (UNIT)) ;
using REFLECT_STRUCT<UNIT> = CALL<rust::REFLECT_STRUCT_HELP<UNIT>> ;
using STRUCT_BIND<PARAMS> = [...:PARAMS] ;

using IS_FUNCTION<UNIT> = enum (rust::is_function (UNIT)) ;
using REMOVE_FUNCTION<UNIT> = CALL<rust::REMOVE_FUNCTION_HELP<UNIT>> ;
using REFLECT_FUNCTION<UNIT> = CALL<rust::REFLECT_FUNCTION_HELP<UNIT>> ;
using FUNCTION_BIND<RETURN ,PARAMS> = (...:PARAMS) :RETURN ;

using IS_TRIVIAL<UNIT> = enum (rust::is_trivial (UNIT)) ;
using IS_INTERFACE<UNIT> = enum (rust::is_interface (UNIT)) ;

using IS_CLASS<UNIT> = enum (rust::is_class (UNIT)) ;
using IS_CONSTRUCTIBLE<UNIT ,PARAPMS> = enum (rust::is_constructible (UNIT ,PARAPMS)) ;
using IS_SHAREABLE<UNIT> = enum (rust::is_shareable (UNIT)) ;
using IS_CLONEABLE<UNIT> = enum (rust::is_cloneable (UNIT)) ;
using IS_EXTEND<BASE ,DERIVED> = enum (rust::is_extend (BASE ,DERIVED)) ;
using IS_IMPLEMENT<BASE ,DERIVED> = enum (rust::is_implement (BASE ,DERIVED)) ;

using IS_PLACEHOLDER<UNIT> = IS_EXTEND<PLACEHOLDER<ZERO> ,UNIT> ;

using IS_TEMP<UNIT> = concept (UNIT as class TEMP) ;
using REMOVE_TEMP<UNIT> = rust::REMOVE_TEMP_HELP<UNIT> ;

using BYTE_TRAIT<UNIT> = CALL<BYTE_TRAIT_HELP<UNIT>> ;

define _ASSERT_(...) switch { if (...) discard ; Exception ("assert failed : " ++ rust::literal (...) ++ " : at " ++ rust::function_name ++ " in " ++ rust::file_name ++ " ," ++ rust::line_name).raise () ; }

function noop = () => {
	_ASSERT_ (TRUE) ;
} ;

function unimplemented = () => {
	_ASSERT_ (FALSE) ;
} ;

function address = (obj) => {
	##???##
} ;

function alignas = (base :LENGTH ,align :LENGTH) => {
	return base + (align - base % align) % align ;
} ;

function load = (id1 ,owner ,addr :LENGTH) => {
	using R1X = type (id1) ;
	using R2X = type (owner) ;
	require (IS_FUNCTION<R2X>) ;
	##???##
} ;

function swap = (obj1 ,obj2) => {
	using R1X = type (obj1) ;
	using R2X = type (obj2) ;
	require (IS_SAME<R1X ,R2X>) ;
	##???##
} ;

function hash = (...) => HASH_HELP<type (...)>::hash (...) ;

function between = (obj :INDEX ,begin :INDEX ,end :INDEX) => {
	return obj >= begin && obj < end ;
} ;

function min = (obj1 ,obj2) => {
	if (obj1 <= obj2)
		return obj1 ;
	return obj2 ;
} ;

function max = (obj1 ,obj2) => {
	if (obj1 >= obj2)
		return obj1 ;
	return obj2 ;
} ;

trait SYNTAX_HELP<UNIT> {
	class Syntax {
		constant mPointer :PTR<UNIT> ;
	} ;

	implement Syntax {
		function new = () => delete ;

		function new = (that :UNIT) => {
			mPointer = PTR<UNIT> => that ;
		} ;
	} ;

	implement Syntax :Wrapped<UNIT> {
		function to = () => mPointer ;
	} ;
} ;

using Syntax<UNIT> = SYNTAX_HELP<UNIT>::Syntax ;

using UNION<ALIGN ,SIZE> = UNION_HELP<ALIGN ,SIZE>::UNION ;

trait TEMP_HELP<UNIT> {
	class TEMP {
		variable mStorage :UNION<ALIGNOF<UNIT> ,SIZEOF<UNIT>> ;
	} ;	

	implement TEMP {
		function new = () => default ;

		function new = (that) => {
			using R1X = type (that) ;
			using R2X = type (NULL) ;
			require (IS_SAME<R1X ,R2X>) ;
			mStorage = UNION<ALIGNOF<UNIT> ,SIZEOF<UNIT>> => zeroize ;
		} ;

		function swap = mutable (that :TEMP) => {
			variable rax = mStorage ;
			mStorage = that.mStorage ;
			that.mStorage = rax ;
		} ;
	} ;
} ;

using TEMP<UNIT> = TEMP_HELP<UNIT>::TEMP ;

trait SLICE_HELP<REAL> {
	class Slice {
		interface Holder ;
		class ImplHolder ;
		class ConcatHolder ;

		constant mPointer :PTR<Holder> ;
	} ;

	interface Slice::Holder {
		function size = () :LENGTH => virtual ;
		function get = (index :INDEX) :REAL => virtual ;
	} ;

	trait SLICE_IMPLHOLDER_HELP<WRAP> {
		class Slice::ImplHolder {
			require (IS_STRUCT<WRAP>) ;

			constant mTuple :PTR<WRAP> ;
			constant mSize :LENGTH ;
			constant mHashCode :FLAG ;
		} ;

		implement Slice::ImplHolder {
			function new = () => delete ;

			function new = (that :PTR<WRAP>) => {
				mTuple = that ;
				mSize = mTuple->count () ;
				mHashCode = mTuple->hashcode () ;
			} ;
		} ;

		implement Slice::ImplHolder :Holder {
			function size = () => mSize ;

			function hashcode = () => mHashCode ;

			function get = (index :INDEX) => mTuple->[index] ;
		} ;
	} ;

	class Slice::ConcatHolder {
		constant mHead :Slice ;
		constant mTail :Slice ;
		constant mHeadSize :INDEX ;
		constant mSize :LENGTH ;
		constant mHashCode :FLAG ;
	} ;

	implement Slice::ImplHolder {
		function new = () => delete ;

		function new = (head :Slice ,tail :Slice) => {
			mHead = head ;
			mTail = tail ;
			mHeadSize = head.size () ;
			mSize = mHeadSize + tail.size () ;
			mHashCode = hash (mHead->hashcode () ,mTail->hashcode ()) ;
		} ;
	} ;

	implement Slice::ConcatHolder :Holder {
		function size = () => mSize ;

		function hashcode = () => mHashCode ;

		function get = (index :INDEX) => {
			if (index < mHeadSize)
				return mHead.[index] ;
			return mTail.[index - mHeadSize] ;
		} ;
	} ;

	implement Slice {
		function new = () => {
			mPointer = NULL ;
		} ;
		
		function new = (id1 ,that) => {
			using R1X = type (that) ;
			using R2X = REMOVE_FUNCTION<R1X> ;
			constant r1x = PTR<R2X> => that ;
			static constant M_HOLDER = SLICE_IMPLHOLDER_HELP<R2X>::ImplHolder (r1x) ;
			mPointer = PTR<Holder> => M_HOLDER ;
		} ;
		
		function new = (id1 ,head :Slice ,tail :Slice) => {
			static constant M_HOLDER = ConcatHolder (head ,tail) ;
			mPointer = PTR<Holder> => M_HOLDER ;
		} ;

		function size = () => {
			if (mPointer == NULL)
				return ZERO ;
			return mPointer->size () ;
		} ;
	} ;

	implement Slice :Getable {
		function get = (index :INDEX) => {
			_ASSERT_ (between (index ,0 ,size ())) ;
			return mPointer->get (index) ;
		} ;
	} ;
	
	implement Slice :Concatable {	
		function concat = (that :Slice) => Slice (P0 ,this ,that) ;
	} ;

	implement Slice :Equalable<Slice> {
		function equal = (that :Slice) => {
			if (size () != that.size ())
				return FALSE ;
			if (hashcode () != that.hashcode ())
				return FALSE ;
			for (i in range (0 ,size ())) {
				if (get (i) != that.get (i))
					return FALSE ;
			}
			return TRUE ;
		} ;
	} ;

	implement Slice :Comprable<Slice> {
		function compr = (that :Slice) => {
			constant r1x = min (size () ,that.size ()) ;
			for (i in range (0 ,r1x)) {
				constant r4x = get (i) <=> that.get (i) ;
				if (r4x != ZERO)
					return r4x ;
			}
			constant r3x = size () <=> that.size () ;
			if (r3x != ZERO)
				return r3x ;
			return ZERO ;
		} ;
	} ;
} ;

using Slice<REAL> = SLICE_HELP<REAL>::Slice ;

class Class {
	interface Holder ;
	class Reflection ;
	class ImplHolder ;

	constant mPointer :PTR<Holder> ;
} ;

interface Class::Holder {
	function type_hash = () :FLAG => virtual ;
	function type_align = () :LENGTH => virtual ;
	function type_size = () :LENGTH => virtual ;
	function type_name = () :Slice<STR> => virtual ;
	function reflect = () :PTR<Reflection> => virtual ;
} ;

class Class::Reflection {
	public constant mType :Class ;
	public constant mOffset :LENGTH ;
	public constant mMember :Slice<STR> ;
	public constant mPrev :PTR<Reflection> ;
	public constant mNext :PTR<Reflection> ;
} ;

trait CLASS_IMPLHOLDER_HELP<WRAP> {
	class Class::ImplHolder {} ;

	implement Class::ImplHolder :Holder {
		function type_hash = () => address (this) ;

		function type_align = () => ALIGNOF<WRAP>::value ;

		function type_size = () => SIZEOF<WRAP>::value ;

		function type_name = () => {
			constant r1x = () => {} ;
			using R1X = type (r1x) ;
			return Slice<STR> (type<R1X>::value ,rust::func_name) ;
		} ;

		function reflect = () => {
			##???##
			return PTR<Reflection> (NULL) ;
		} ;
	} ;
} ;

implement Class {
	function new = () => delete ;

	function new = (id1) => {
		using R1X = type (id1) ;
		static constant M_HOLDER = CLASS_IMPLHOLDER_HELP<R1X>::ImplHolder () ;
		mPointer = PTR<Holder> => M_HOLDER ;
	} ;
	
	function type_hash = () => mPointer->type_hash () ;

	function type_align = () => mPointer->type_align () ;

	function type_size = () => mPointer->type_size () ;

	function type_name = () => mPointer->type_name () ;
} ;

implement Class :Equalable<Slice> {
	function equal = (that :Class) => {
		if (type_hash () != that.type_hash ())
			return FALSE ;
		if (type_name () != that.type_name ())
			return FALSE ;
		return TRUE ;
	} ;
} ;

implement Class :Comprable<Slice> {
	function compr = (that :Class) => {
		constant r1x = type_hash () <=> that.type_hash () ;
		if (r1x != ZERO)
			return r1x ;
		constant r2x = type_name () <=> that.type_name () ;
		if (r2x != ZERO)
			return r2x ;
		return ZERO ;
	} ;
} ;

class Exception {
	constant mWhat :Slice<STR> ;
} ;

implement Exception {
	function new = () => default ;

	function new = (that :Slice<STR>) => {
		mWhat = that ;
	} ;

	function what = () => mWhat ;

	function raise = () => {
		throw this ;
	} ;
} ;

interface Iteratable {
	function ibegin = () :INDEX => virtual ;
	function iend = () :INDEX => virtual ;
	function inext = (index :INDEX) :INDEX => virtual ;
} ;

class Iterator {
	constant mArray :PTR<Iteratable> ;
	constant mEnd :INDEX ;
	variable mIndex :INDEX ;
} ;

implement Iterator {
	function new = () => delete ;

	function new = (array :PTR<Iteratable>) => {
		mArray = array ;
		mIndex = mArray->ibegin () ;
		mEnd = mArray->iend () ;
	} ;
} ;

implement Iterator {
	function next = mutable () => {
		mIndex = mArray->inext (mIndex) ;
	} ;

	function good = () => mIndex != mEnd ;

	function get = () => mIndex ;
} ;

class RangeIterator {
	constant mBegin :INDEX ;
	constant mEnd :INDEX ;
} ;

implement RangeIterator {
	function new = () => delete ;

	function new = (begin :INDEX ,end :INDEX) => {
		mBegin = begin ;
		mEnd = max (begin ,end) ;
	} ;
} ;

implement RangeIterator :Iteratable {
	function ibegin = () => mBegin ;

	function iend = () => mEnd ;

	function inext = (index :INDEX) => index + 1 ;
} ;

function range = (begin :INDEX ,end :INDEX) => RangeIterator (begin ,end) ;

class Reference {
	public constant mPointer :() ;
} ;

implement Reference {
	function new = () => {
		mPointer = NULL ;
	} ;

	function new = (that) => {
		mPointer = that.mPointer ;
	} ;
} ;

trait SAFEREFERENCE_HELP<UNIT> {
	class SafeReference {
		constant mPointer :PTR<UNIT> ;
	} ;

	implement SafeReference {
		function new = () => delete ;

		trait NEW_HELP<ARG1> {
			require (IS_SAME<ARG1 ,PTR<UNIT>>) ;
			
			function new = (that :PTR<UNIT>) => {
				mPointer = that ;
			} ;
		} ;

		trait NEW_HELP<ARG1> {
			require (IS_SAME<ARG1 ,Reference>) ;

			function new = (that :Reference) => {
				_ASSERT_ (that.mPointer != NULL) ;
				mPointer = that.mPointer ;
			} ;
		} ;

		function new = (that) => NEW_HELP<type (that)>::new (that) ;
	} ;

	implement SafeReference :Wrapped<UNIT> {
		function to = () => mPointer ;
	} ;
} ;

using SafeReference<UNIT> = SAFEREFERENCE_HELP<UNIT>::SafeReference ;