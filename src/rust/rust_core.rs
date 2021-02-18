module rust.core ;

import module rust ;

using BOOL = rust::bool ;

static constant TRUE = BOOL (rust::true) ;
static constant FALSE = BOOL (rust::false) ;

using VAR32 = rust::int32 ;
using VAR64 = rust::int64 ;

static constant VAR32_MAX = VAR32 (2147483647) ;
static constant VAR32_MIN = -VAR32_MAX - 1 ;
static constant VAR64_MAX = VAR64 (9223372036854775807) ;
static constant VAR64_MIN = -VAR64_MAX - 1 ;

trait VAR_HELP {
	require (macro ("config::int32")) ;

	using VAR = VAR32 ;

	static constant VAR_MAX = VAR32_MAX ;
	static constant VAR_MIN = VAR32_MIN ;
} ;

trait VAR_HELP {
	require (macro ("config::int64")) ;

	using VAR = VAR64 ;

	static constant VAR_MAX = VAR64_MAX ;
	static constant VAR_MIN = VAR64_MIN ;
} ;

using VAR = VAR_HELP::VAR ;

static constant VAR_MAX = VAR_HELP::VAR_MAX ;
static constant VAR_MIN = VAR_HELP::VAR_MIN ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

static constant ZERO = VAR (+0) ;
static constant NONE = VAR (-1) ;
static constant USED = VAR (-2) ;

using SINGLE = rust::float32 ;
using DOUBLE = rust::float64 ;

static constant SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constant SINGLE_MIN = SINGLE (1.175494351E-38) ;
static constant SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constant SINGLE_INF = SINGLE (rust::infinity) ;
static constant DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constant DOUBLE_MIN = DOUBLE (2.2250738585072014E-308) ;
static constant DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constant DOUBLE_INF = DOUBLE (rust::infinity) ;

using BYTE = rust::byte8 ;
using WORD = rust::byte16 ;
using CHAR = rust::byte32 ;
using FEAT = rust::byte64 ;

using STRA = rust::stra ;
using STRU8 = rust::stru8 ;
using STRU16 = rust::stru16 ;
using STRU32 = rust::stru32 ;

trait STR_HELP {
	require (macro ("config::stra")) ;

	using STR = STRA ;
} ;

trait STR_HELP {
	require (macro ("config::stru8")) ;

	using STR = STRU8 ;
} ;

trait STR_HELP {
	require (macro ("config::stru16")) ;

	using STR = STRU16 ;
} ;

trait STR_HELP {
	require (macro ("config::stru32")) ;

	using STR = STRU32 ;
} ;

using STR = STR_HELP::STR ;

static constant NULL = rust::null ;

using ENUM_ZERO = enum (+0) ;
using ENUM_IDEN = enum (+1) ;

define SIZEOF<UNIT> = enum (rust::sizeof (UNIT)) ;
define ALIGNOF<UNIT> = enum (rust::alignof (UNIT)) ;
define COUNTOF<UNIT> = enum (rust::countof (UNIT)) ;

define IS_SAME<UNIT1 ,UNIT2> = enum (rust::is_same (UNIT1 ,UNIT2)) ;

trait ENABLE_HELP<ARG1 ,ARG2> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

define ENABLE<COND> = template ENABLE_HELP<COND ,type<>>::RET ;
define ENABLE<COND ,YES> = template ENABLE_HELP<COND ,YES>::RET ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_ZERO>) ;

	using RET = ARG3 ;
} ;

define CONDITIONAL<COND ,YES ,NO> = template CONDITIONAL_HELP<COND ,YES ,NO>::RET ;

define IS_ENUM<UNIT> = enum (rust::is_enum (UNIT)) ;
define ENUM_CHECK<UNIT> = ENABLE<IS_ENUM<UNIT> ,UNIT> ;

define ENUM_EQUAL<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value) ;
define ENUM_NOT_EQUAL<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value != ENUM_CHECK<UNIT2>::value) ;
define ENUM_BOOL<UNIT> = ENUM_NOT_EQUAL<UNIT ,ENUM_ZERO> ;

define ENUM_COMPR<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value <=> ENUM_CHECK<ARG2>::value) ;
define ENUM_COMPR_LT<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO) ;
define ENUM_COMPR_LT_EQ<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO) ;
define ENUM_COMPR_GT<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO) ;
define ENUM_COMPR_GT_EQ<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO) ;
define ENUM_EQ_ZERO<UNIT> = ENUM_EQUAL<UNIT ,ENUM_ZERO> ;
define ENUM_LT_ZERO<UNIT> = ENUM_COMPR_LT<UNIT ,ENUM_ZERO> ;
define ENUM_GT_ZERO<UNIT> = ENUM_COMPR_GT<UNIT ,ENUM_ZERO> ;
define ENUM_EQ_IDEN<UNIT> = ENUM_EQUAL<UNIT ,ENUM_IDEN> ;
define ENUM_LT_IDEN<UNIT> = ENUM_COMPR_LT<UNIT ,ENUM_IDEN> ;
define ENUM_GT_IDEN<UNIT> = ENUM_COMPR_GT<UNIT ,ENUM_IDEN> ;

define ENUM_ADD<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value) ;
define ENUM_SUB<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value) ;
define ENUM_MUL<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value) ;
define ENUM_DIV<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value) ;
define ENUM_MOD<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value) ;
define ENUM_INC<UNIT> = ENUM_ADD<UNIT ,ENUM_IDEN> ;
define ENUM_DEC<UNIT> = ENUM_SUB<UNIT ,ENUM_IDEN> ;

define IS_TYPE<UNIT> = enum (rust::is_type (UNIT)) ;
define TYPE_CHECK<UNIT> = ENABLE<IS_TYPE<UNIT> ,UNIT> ;

define TYPE_FIRST_ONE<UNIT> = rust::first_one (TYPE_CHECK<UNIT>) ;
define TYPE_FIRST_REST<UNIT> = rust::first_rest (TYPE_CHECK<UNIT>) ;
define TYPE_SECOND_ONE<UNIT> = rust::second_one (TYPE_CHECK<UNIT>) ;
define TYPE_SECOND_REST<UNIT> = rust::second_rest (TYPE_CHECK<UNIT>) ;
define TYPE_CAT<UNIT1 ,UNIT2> = type<TYPE_CHECK<UNIT1>... ,TYPE_CHECK<UNIT2>...> ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = type<> ;
} ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_GT_ZERO<ARG2>) ;
	
	using R1X = template TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2>>::RET ;

	using RET = TYPE_CAT<ARG1 ,R1X> ;
} ;

define TYPE_REPEAT<UNIT ,SIZE> = template TYPE_REPEAT_HELP<type<UNIT> ,SIZE>::RET ;

trait TYPE_REVERSE_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = type<> ;
} ;

trait TYPE_REVERSE_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;
	
	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = template TYPE_REVERSE_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = TYPE_CAT<R2X ,type<R1X>> ;
} ;

define TYPE_REVERSE<UNIT> = template TYPE_REVERSE_HELP<UNIT>::RET ;

trait TYPE_PICK_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using RET = TYPE_FIRST_ONE<ARG1> ;
} ;

trait TYPE_PICK_HELP<ARG1 ,ARG2> {
	require (ENUM_GT_ZERO<ARG2>) ;

	using RET = template TYPE_PICK_HELP<TYPE_FIRST_REST<ARG1> ,ENUM_DEC<ARG2>>::RET ;
} ;

define TYPE_PICK<LIST ,INDEX> = template TYPE_PICK_HELP<LIST ,INDEX>::RET ;

trait ENUM_ALL_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

trait ENUM_ALL_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_BOOL<TYPE_FIRST_ONE<ARG1>> ;
	using R2X = template ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,R2X ,ENUM_ZERO> ;
} ;

trait ENUM_ANY_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

trait ENUM_ANY_HELP<ARG1> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using R1X = ENUM_BOOL<TYPE_FIRST_ONE<ARG1>> ;
	using R2X = template ENUM_ANY_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R2X> ;
} ;

define ENUM_ALL<UNITS...> = template ENUM_ALL_HELP<UNITS>::RET ;
define ENUM_ANY<UNITS...> = template ENUM_ANY_HELP<UNITS>::RET ;

trait IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARG1> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = template IS_ALL_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>>>::RET ;

	using RET = ENUM_ALL<IS_SAME<R1X ,R2X> ,R3X> ;
} ;

trait IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_EQ_IDEN<COUNTOF<ARG1>>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME_HELP<ARG1> {
	require (ENUM_GT_IDEN<COUNTOF<ARG1>>) ;

	using R1X = TYPE_FIRST_ONE<ARG1> ;
	using R2X = TYPE_SECOND_ONE<ARG1> ;
	using R3X = template IS_ANY_SAME_HELP<TYPE_CAT<R1X ,TYPE_SECOND_REST<ARG1>>>::RET ;
	using R4X = template IS_ANY_SAME_HELP<TYPE_CAT<R2X ,TYPE_SECOND_REST<ARG1>>>::RET ;

	using RET = ENUM_ANY<IS_SAME<R1X ,R2X> ,R3X ,R4X> ;
} ;

define IS_ALL_SAME<UNITS...> = template IS_ALL_SAME_HELP<UNITS>::RET ;
define IS_ANY_SAME<UNITS...> = template IS_ANY_SAME_HELP<UNITS>::RET ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_EQ_ZERO<ARG1>) ;

	class PlaceHolder {} ;
} ;

trait PLACEHOLDER_HELP<ARG1> {
	require (ENUM_GT_ZERO<ARG1>) ;

	using BASE = template PLACEHOLDER_HELP<ENUM_DEC<ARG1>>::PlaceHolder ;

	class PlaceHolder {
		extend :BASE ;
	} ;
} ;

define PlaceHolder<RANK> = template PLACEHOLDER_HELP<RANK>::PlaceHolder ;

static constant PH0 = PlaceHolder<enum (0)> () ;
static constant PH1 = PlaceHolder<enum (1)> () ;
static constant PH2 = PlaceHolder<enum (2)> () ;
static constant PH3 = PlaceHolder<enum (3)> () ;
static constant PH4 = PlaceHolder<enum (4)> () ;
static constant PH5 = PlaceHolder<enum (5)> () ;
static constant PH6 = PlaceHolder<enum (6)> () ;
static constant PH7 = PlaceHolder<enum (7)> () ;
static constant PH8 = PlaceHolder<enum (8)> () ;
static constant PH9 = PlaceHolder<enum (9)> () ;
static constant PHX = PlaceHolder<enum (10)> () ;

define IS_BOOL<UNIT> = IS_SAME<UNIT ,BOOL> ;
define IS_VAR<UNIT> = ENUM_ANY<IS_SAME<UNIT ,VAR32> ,IS_SAME<UNIT ,VAR64>> ;
define IS_FLOAT<UNIT> = ENUM_ANY<IS_SAME<UNIT ,SINGLE> ,IS_SAME<UNIT ,DOUBLE>> ;
define IS_BYTE<UNIT> = ENUM_ANY<IS_SAME<UNIT ,BYTE> ,IS_SAME<UNIT ,WORD> ,IS_SAME<UNIT ,CHAR> ,IS_SAME<UNIT ,FEAT> ,IS_SAME<UNIT ,byte128> ,IS_SAME<UNIT ,byte256>> ;
define IS_STR<UNIT> = ENUM_ANY<IS_SAME<UNIT ,STRA> ,IS_SAME<UNIT ,STRU8> ,IS_SAME<UNIT ,STRU16> ,IS_SAME<UNIT ,STRU32>> ;
define IS_NULL<UNIT> = IS_SAME<UNIT ,type (NULL)> ;
define IS_BASIC<UNIT> = ENUM_ANY<IS_BOOL<UNIT> ,IS_VAR<UNIT> ,IS_FLOAT<UNIT> ,IS_BYTE<UNIT> ,IS_STR<UNIT> ,IS_NULL<UNIT>> ;

define IS_FUNCTION<UNIT> = enum (rust::is_function (UNIT)) ;
define IS_MUTABLE<UNIT> = enum (rust::is_mutable (UNIT)) ;
define IS_NOEXCPET<UNIT> = enum (rust::is_noexcept (UNIT)) ;

define IS_INTERFACE<UNIT> = enum (rust::is_interface (UNIT)) ;
define IS_EXTEND<BASE ,DERIVED> = enum (rust::is_extend (BASE ,DERIVED)) ;
define IS_PLACEHOLDER<UNIT> = IS_EXTEND<UNIT ,type (PH0)> ;

define IS_STRUCT<UNIT> = enum (rust::is_struct (UNIT)) ;
define IS_CLASS<UNIT> = enum (rust::is_class (UNIT)) ;
define IS_TRIVIAL<UNIT> = enum (rust::is_trivial (UNIT)) ;
define IS_TRIVIAL<UNIT> = enum (rust::is_dynamic (UNIT)) ;
define IS_CONSTRUCTIBLE<UNIT ,PARAMS> = enum (rust::is_constructible (UNIT ,PARAMS)) ;
define IS_CLONEABLE<UNIT> = enum (rust::is_cloneable (UNIT)) ;
define IS_SHAREABLE<UNIT> = enum (rust::is_shareable (UNIT)) ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<BYTE>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<BYTE>>) ;

	using RET = BYTE ;
} ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<WORD>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<WORD>>) ;

	using RET = WORD ;
} ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<CHAR>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<CHAR>>) ;

	using RET = CHAR ;
} ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<FEAT>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<FEAT>>) ;

	using RET = FEAT ;
} ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<byte128>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<byte128>>) ;

	using RET = byte128 ;
} ;

trait BYTE_TRAIT_HELP<ARG1> {
	require (ENUM_EQUAL<SIZEOF<ARG1> ,SIZEOF<byte256>>) ;
	require (ENUM_EQUAL<ALIGNOF<ARG1> ,ALIGNOF<byte256>>) ;

	using RET = byte256 ;
} ;

define BYTE_TRAIT<UNIT> = template BYTE_TRAIT_HELP<UNIT>::RET ;

static function noop = () => {} ;

static function bad = (id) => {
	using R1X = type (id) ;
	assert (FALSE) ;
	constant r1x = () :auto => ZERO ;
	return R1X (r1x ()) ;
} ;

trait FORWARD_HELP<ARG1> {
	require (IS_CLASS<ARG1>) ;
	require (IS_SHAREABLE<ARG1>) ;

	static function forward = (obj) => obj.share () ;
} ;

trait FORWARD_HELP<ARG1> {
	require (IS_CLASS<ARG1>) ;
	require not (IS_SHAREABLE<ARG1>) ;
	require (IS_CLONEABLE<ARG1>) ;

	static function forward = (obj) => obj.clone () ;
} ;

trait FORWARD_HELP<ARG1> {
	require not (IS_CLASS<ARG1>) ;

	static function forward = (obj) => obj ;
} ;

static function forward = (obj) => FORWARD_HELP<type (obj)>::forward (obj) ;

static function swap = (obj1 ,obj2) => {
	variable rax = forward (obj1) ;
	obj1 = forward (obj2) ;
	obj2 = forward (rax) ;
} ;

static function address = (obj) :LENGTH => rust::address (obj) ;

static function alignas = (base :LENGTH ,align :LENGTH) :LENGTH => {
	constant r1x = align - base % align ;
	return base + r1x % align ;
} ;

static function between = (index :INDEX ,begin :INDEX ,end :INDEX) :BOOL => index >= begin && index < end ;

static function abs = (obj) => {
	using R1X = type (obj) ;
	if (obj < R1X (0))
		return -obj ;
	return +obj ;
} ;

static function min = (obj1 ,obj2) => {
	if (obj1 < obj2)
		return obj1 ;
	return obj2 ;
} ;

static function max = (obj1 ,obj2) => {
	if (obj1 < obj2)
		return obj2 ;
	return obj1 ;
} ;

static function hashcode = () :FLAG => {
	constant r2x = VAR64 (-3750763034362895579) ;
	constant r3x = VAR64 (FEAT (r2x) & FEAT (VAR_MAX)) ;
	return FLAG (r3x) ;
} ;

static function hashcode = (obj1 :FLAG ,obj2 :FLAG) :FLAG => {
	constant r1x = VAR64 (FEAT (obj1) ^ FEAT (obj2)) ;
	constant r2x = r1x * VAR64 (1099511628211) ;
	constant r3x = VAR64 (FEAT (r2x) & FEAT (VAR_MAX)) ;
	return FLAG (r3x) ;
} ;

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

	using FIRST_ONE = TYPE_FIRST_ONE<ARG1> ;
	using FIRST_REST = template TUPLE_HELP<TYPE_FIRST_REST<ARG1>>::Tuple ;
	
	class Tuple {
		extend mSuper :FIRST_REST ;
		variable mValue :FIRST_ONE ;
	} ;

	implement Tuple {
		function new = () => default ;

		function rank = () :LENGTH => COUNTOF<ARG1>::value ;

		property one = mutable [] :FIRST_ONE => mValue ;

		property rest = mutable [] :FIRST_REST => mSuper ;

		trait PICK_HELP<ARG1> {
			require (ENUM_EQ_ZERO<ARG1>) ;

			property pick = mutable [] => one ;
		} ;

		trait PICK_HELP<ARG1> {
			require (ENUM_GT_ZERO<ARG1>) ;

			property pick = mutable [] => rest.pick[type<ENUM_DEC<ARG1>>::id] ;
		} ;

		property pick = mutable [nth] => {
			using R1X = type (nth) ;
			require (IS_ENUM<R1X>) ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNTOF<ARG1>>) ;
			return PICK_HELP<R1X>::pick ;
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
			constant r1x = mValue.hash () ;
			constant r2x = mSuper.hash () ;
			return hashcode (r1x ,r2x) ;
		} ;
	} ;
} ;

define Tuple<UNITS...> = template TUPLE_HELP<UNITS>::Tuple ;

class Pointer {
	variable mOrigin :LENGTH ;
	variable mSize :LENGTH ;
	variable mAlign :LENGTH ;
	variable mPointer :LENGTH ;
} ;

implement Pointer {
	function new = () => {
		mOrigin = ZERO ;
		mSize = ZERO ;
		mAlign = ZERO ;
		mPointer = ZERO ;
	} ;
	
	function new = (id) => {
		using R1X = type (id) ;
		require (IS_NULL<R1X>) ;
		new () ;
	} ;

	function delete = () => {
		if (mOrigin == ZERO)
			return ;
		switch {
			if (mPointer == ZERO)
				continue ;
			rust::destroy (id ,mPointer) ;
		} ;
		rust::free (mOrigin) ;
		mOrigin = ZERO ;
		mSize = ZERO ;
		mAlign = ZERO ;
		mPointer = ZERO ;
	} ;

	function exist = () => mOrigin != ZERO ;

	static function make = (size_len :LENGTH) => make (size_len ,1) ;

	static function make = (size_len :LENGTH ,align_len :LENGTH) => {
		assert (size_len > 0) ;
		assert (align_len > 0) ;
		variable ret = Pointer () ;
		constant r1x = size_len + align_len - 1 ;
		assert (r1x > 0) ;
		ret.mOrigin = rust::alloc (r1x) ;
		ret.mSize = r1x ;
		ret.mAlign = align_len ;
		return ret ;
	} ;

	function build = mutable (id) => {
		using R1X = type (id) ;
		assert (mPointer == ZERO) ;
		constant r1x = alignas (mOrigin ,mAlign) ;
		rust::create (id ,r1x) ;
		mPointer = r1x ;
	} ;

	property to = mutable [id] => {
		using R1X = type (id) ;
		assert (mPointer != ZERO) ;
		return rust::load[id ,mPointer] ;
	} ;
} ;

trait SLICE_HELP<ARG1> {
	class Slice {
		interface Holder ;
		class ImplHolder ;

		constant mPointer :Pointer ;
	} ;

	interface Slice::Holder {
		function size = () :LENGTH => virtual ;

		function addr = () :LENGTH => virtual ;

		property at = [index :INDEX] :ARG1 => virtual ;
	} ;

	class Slice::ImplHolder {
		variable mSize :LENGTH ;
	} ;

	implement Slice::ImplHolder {
		function new = () => disable ;

		function delete = () => disable ;
	} ;

	implement Slice::ImplHolder :Holder {
		function size = () :LENGTH => mSize ;

		function addr = () :LENGTH => address (this) + SIZEOF<ImplHolder>::value ;

		property at = [index :INDEX] :ARG1 => rust::load[type<ARG1>::id ,addr () + index * SIZEOF<ARG1>::value] ;
	} ;

	implement Slice {
		function new = () => disable ;

		function new = (len :LENGTH) => {
			constant r1x = type_size (len) ;
			constant r2x = type_align () ;
			mPointer = Pointer::make (r1x ,r2x) ;
			mPointer.build (type<ImplHolder>::id) ;
		} ;

		private static function type_size = (len :LENGTH) :LENGTH => SIZEOF<ImplHolder>::value + len * SIZEOF<ARG1>::value ;

		private static function type_align = () :LENGTH => max (ALIGNOF<ImplHolder>::value ,ALIGNOF<ARG1>::value) ;

		function size = () :LENGTH => mPointer.to[type<Holder>::id].size () ;

		function addr = () :LENGTH => mPointer.to[type<Holder>::id].addr () ;

		function at = [index :INDEX] :ARG1 => mPointer.to[type<Holder>::id].at[index] ;
	} ;
} ;

define Slice<UNIT> = template SLICE_HELP<UNIT>::Slice ;

trait CLAZZ_HELP<ARG1> {
	class Clazz {
		interface Holder ;
		class ImplHolder ;

		constant mPointer :Pointer ;
	} ;

	interface Clazz::Holder {
		property type_hash = [] :FLAG => virtual ;
		property type_name = [] :Slice<STR> => virtual ;
		property type_size = [] :LENGTH => virtual ;
		property type_align = [] :LENGTH => virtual ;
	} ;

	class Clazz::ImplHolder :Holder {
		constant mTypeHash :FLAG ;
		constant mTypeName :Slice<STR> ;
		constant mTypeSize :LENGTH ;
		constant mTypeAlign :LENGTH ;
	} ;

	implement Clazz::ImplHolder {
		function new = () => disable ;

		function delete = () => disable ;
	} ;

	implement Clazz::ImplHolder :Holder {
		property type_hash = [] :FLAG => mTypeHash ;

		property type_name = [] :Slice<STR> => mTypeName ;

		property type_size = [] :LENGTH => mTypeSize ;

		property type_align = [] :LENGTH => mTypeAlign ;
	} ;

	implement Clazz {
		function new = () => {
			mPointer = Pointer::make (SIZEOF<ImplHolder>::value ,ALIGNOF<ImplHolder>::value) ;
			mPointer.build (type<ImplHolder>::id) ;
		} ;

		property type_hash = [] :FLAG => mPointer.to[type<Holder>::id].type_hash ;

		property type_name = [] :Slice<STR> => mPointer.to[type<Holder>::id].type_name ;

		property type_size = [] :LENGTH => mPointer.to[type<Holder>::id].type_size ;

		property type_align = [] :LENGTH => mPointer.to[type<Holder>::id].type_align ;

		function equal = (that :Clazz) :BOOL => {
			if (type_hash != that.type_hash)
				return FALSE ;
			if (type_name != that.type_name)
				return FALSE ;
			return TRUE ;
		} ;

		function compr = (that :Clazz) :FLAG => {
			constant r1x = type_hash <=> that.type_hash ;
			if (r1x != ZERO)
				return r1x ;
			constant r2x = type_name <=> that.type_name ;
			if (r2x != ZERO)
				return r2x ;
			return ZERO ;
		} ;
	} ;
} ;