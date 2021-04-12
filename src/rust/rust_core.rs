module core ;

using BOOL = bool_t ;

static constant TRUE = BOOL (true) ;
static constant FALSE = BOOL (false) ;

using VAR32 = int32_t ;
using VAR64 = int64_t ;

static constant VAR32_MAX = VAR32 (2147483647) ;
static constant VAR32_MIN = -VAR32_MAX - 1 ;
static constant VAR64_MAX = VAR64 (9223372036854775807) ;
static constant VAR64_MIN = -VAR64_MAX - 1 ;

trait VAR_HELP<> {
	require (macro ("config_VAR32")) ;

	using VAR = VAR32 ;

	static constant VAR_MAX = VAR32_MAX ;
	static constant VAR_MIN = VAR32_MIN ;
} ;

trait VAR_HELP<> {
	require (macro ("config_VAR64")) ;

	using VAR = VAR64 ;

	static constant VAR_MAX = VAR64_MAX ;
	static constant VAR_MIN = VAR64_MIN ;
} ;

using VAR = typename VAR_HELP<>::VAR ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

static constant USED = VAR (-2) ;
static constant NONE = VAR (-1) ;
static constant ZERO = VAR (+0) ;
static constant IDEN = VAR (+1) ;

using SINGLE = float32_t ;
using DOUBLE = float64_t ;

static constant SINGLE_MAX = SINGLE (3.402823466E+38) ;
static constant SINGLE_MIN = SINGLE (1.175494351E-38) ;
static constant SINGLE_EPS = SINGLE (1.192092896E-07) ;
static constant SINGLE_INF = SINGLE (infinity) ;
static constant DOUBLE_MAX = DOUBLE (1.7976931348623158E+308) ;
static constant DOUBLE_MIN = DOUBLE (2.2250738585072014E-308) ;
static constant DOUBLE_EPS = DOUBLE (2.2204460492503131E-016) ;
static constant DOUBLE_INF = DOUBLE (infinity) ;

using BYTE = byte8_t ;
using WORD = byte16_t ;
using CHAR = byte32_t ;
using FEAT = byte64_t ;

using STRA = stra_t ;
using STRU8 = str8_t ;
using STRU16 = str16_t ;
using STRU32 = str32_t ;

trait STR_HELP<> {
	require (macro ("config_STRA")) ;

	using STR = STRA ;
} ;

trait STR_HELP<> {
	require (macro ("config_STRU8")) ;

	using STR = STRU8 ;
} ;

trait STR_HELP<> {
	require (macro ("config_STRU16")) ;

	using STR = STRU16 ;
} ;

trait STR_HELP<> {
	require (macro ("config_STRU32")) ;

	using STR = STRU32 ;
} ;

using STR = typename STR_HELP<>::STR ;

static constant NULL = null ;

using ENUM_USED = enum (-2) ;
using ENUM_NONE = enum (-1) ;
using ENUM_ZERO = enum (+0) ;
using ENUM_IDEN = enum (+1) ;

define SIZEOF<UNIT1> = enum (concept (UNIT1).size) ;

define ALIGNOF<UNIT1> = enum (concept (UNIT1).align) ;

define COUNTOF<UNIT1> = enum (concept (UNIT1).count) ;

define IS_SAME<UNIT1 ,UNIT2> = enum (concept (UNIT1) == concept (UNIT2)) ;

trait ENABLE_HELP<ARG1 ,ARG2> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

define ENABLE<COND> = typename ENABLE_HELP<COND ,type<>>::RET ;
define ENABLE<COND ,YES> = typename ENABLE_HELP<COND ,YES>::RET ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_IDEN>) ;

	using RET = ARG2 ;
} ;

trait CONDITIONAL_HELP<ARG1 ,ARG2 ,ARG3> {
	require (IS_SAME<ARG1 ,ENUM_ZERO>) ;

	using RET = ARG3 ;
} ;

define CONDITIONAL<COND ,YES ,NO> = typename CONDITIONAL_HELP<COND ,YES ,NO>::RET ;

define IS_ENUM<UNIT1> = enum (concept (UNIT1).is_enum) ;
define ENUM_CHECK<UNIT1> = ENABLE<IS_ENUM<UNIT1> ,UNIT1> ;
define ENUM_EQUAL<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value == ENUM_CHECK<UNIT2>::value) ;
define ENUM_NOT<UNIT1> = ENUM_EQUAL<UNIT1 ,ENUM_ZERO> ;
define ENUM_NOT_EQUAL<UNIT1 ,UNIT2> = ENUM_NOT<ENUM_EQUAL<UNIT1 ,UNIT2>> ;
define ENUM_COMPR<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value <=> ENUM_CHECK<UNIT2>::value) ;
define ENUM_COMPR_LT<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value < ZERO) ;
define ENUM_COMPR_LTEQ<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value <= ZERO) ;
define ENUM_COMPR_GT<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value > ZERO) ;
define ENUM_COMPR_GTEQ<UNIT1 ,UNIT2> = enum (ENUM_COMPR<UNIT1 ,UNIT2>::value >= ZERO) ;
define ENUM_EQ_ZERO<UNIT1> = ENUM_EQUAL<UNIT1 ,ENUM_ZERO> ;
define ENUM_LT_ZERO<UNIT1> = ENUM_COMPR_LT<UNIT1 ,ENUM_ZERO> ;
define ENUM_GT_ZERO<UNIT1> = ENUM_COMPR_GT<UNIT1 ,ENUM_ZERO> ;
define ENUM_EQ_IDEN<UNIT1> = ENUM_EQUAL<UNIT1 ,ENUM_IDEN> ;
define ENUM_LT_IDEN<UNIT1> = ENUM_COMPR_LT<UNIT1 ,ENUM_IDEN> ;
define ENUM_GT_IDEN<UNIT1> = ENUM_COMPR_GT<UNIT1 ,ENUM_IDEN> ;
define ENUM_ADD<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value + ENUM_CHECK<UNIT2>::value) ;
define ENUM_SUB<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value - ENUM_CHECK<UNIT2>::value) ;
define ENUM_MUL<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value * ENUM_CHECK<UNIT2>::value) ;
define ENUM_DIV<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value / ENUM_CHECK<UNIT2>::value) ;
define ENUM_MOD<UNIT1 ,UNIT2> = enum (ENUM_CHECK<UNIT1>::value % ENUM_CHECK<UNIT2>::value) ;
define ENUM_INC<UNIT1> = ENUM_ADD<UNIT1 ,ENUM_IDEN> ;
define ENUM_DEC<UNIT1> = ENUM_SUB<UNIT1 ,ENUM_IDEN> ;

define IS_TYPE<UNIT1> = enum (concept (UNIT1).is_type) ;
define TYPE_CHECK<UNIT1> = ENABLE<IS_TYPE<UNIT1> ,UNIT1> ;
define TYPE_FIRST_ONE<UNIT1> = typename TYPE_CHECK<UNIT1>::first_one ;
define TYPE_FIRST_REST<UNIT1> = typename TYPE_CHECK<UNIT1>::fist_rest ;
define TYPE_SECOND_ONE<UNIT1> = typename TYPE_CHECK<UNIT1>::second_one ;
define TYPE_SECOND_REST<UNIT1> = typename TYPE_CHECK<UNIT1>::second_rest ;
define TYPE_UNWRAP<UNIT1> = TYPE_FIRST_ONE<ENABLE<ENUM_EQ_IDEN<COUNTOF<UNIT1>> ,UNIT1>> ;
define TYPE_CAT<UNIT1 ,UNIT2> = type<UNIT1... ,UNIT2...> ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;
	
	using RET = type<> ;
} ;

trait TYPE_REPEAT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQ_ZERO<ARG2>) ;

	using R1X = typename TYPE_REPEAT_HELP<ARG1 ,ENUM_DEC<ARG2>>::RET ;
	
	using RET = TYPE_CAT<type<ARG1> ,R1X> ;
} ;

define TYPE_REPEAT<ITEM ,SIZE> = typename TYPE_REPEAT_HELP<ITEM ,SIZE>::RET ;

define TYPE_REVERSE<LIST> ;
define TYPE_PICK<LIST ,NTH> ;
define TYPE_FIND<LIST ,ITEM> ;

trait ENUM_ALL_HELP<ARGS> {
	require (ENUM_EQ_ZERO<COUNTOF<ARGS>>) ;
	
	using RET = ENUM_IDEN ;
} ;

trait ENUM_ALL_HELP<ARGS> {
	require (ENUM_GT_ZERO<COUNTOF<ARGS>>) ;

	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,R2X ,ENUM_ZERO> ;
} ;

define ENUM_ALL<...UNITS> = typename ENUM_ALL_HELP<UNITS>::RET ;

trait ENUM_ANY_HELP<ARGS> {
	require (ENUM_EQ_ZERO<COUNTOF<ARGS>>) ;

	using RET = ENUM_ZERO ;
} ;

trait ENUM_ANY_HELP<ARGS> {
	require (ENUM_GT_ZERO<COUNTOF<ARGS>>) ;

	using R1X = ENUM_NOT_EQUAL<TYPE_FIRST_ONE<ARG1> ,ENUM_ZERO> ;
	using R2X = typename ENUM_ALL_HELP<TYPE_FIRST_REST<ARG1>>::RET ;

	using RET = CONDITIONAL<R1X ,ENUM_IDEN ,R2X> ;
} ;

define ENUM_ANY<...UNITS> = typename ENUM_ANY_HELP<UNITS>::RET ;

trait PLACEHOLDER_HELP<ARG1> {
	struct PLACEHOLDER {} ;
} ;

define PLACEHOLDER<RANK> = typename PLACEHOLDER_HELP<RANK>::PLACEHOLDER ;

static constant PH0 = PLACEHOLDER<enum (0)> () ;
static constant PH1 = PLACEHOLDER<enum (1)> () ;
static constant PH2 = PLACEHOLDER<enum (2)> () ;
static constant PH3 = PLACEHOLDER<enum (3)> () ;
static constant PH4 = PLACEHOLDER<enum (4)> () ;
static constant PH5 = PLACEHOLDER<enum (5)> () ;
static constant PH6 = PLACEHOLDER<enum (6)> () ;
static constant PH7 = PLACEHOLDER<enum (7)> () ;
static constant PH8 = PLACEHOLDER<enum (8)> () ;
static constant PH9 = PLACEHOLDER<enum (9)> () ;
static constant PHX = PLACEHOLDER<enum (10)> () ;

define IS_BOOL<UNIT1> = IS_SAME<UNIT1 ,BOOL> ;
define IS_VAR<UNIT1> = ENUM_ANY<IS_SAME<UNIT1 ,VAR32> ,IS_SAME<UNIT1 ,VAR64>> ;
define IS_FLOAT<UNIT1> = ENUM_ANY<IS_SAME<UNIT1 ,SINGLE> ,IS_SAME<UNIT1 ,DOUBLE>> ;
define IS_BYTE<UNIT1> = ENUM_ANY<IS_SAME<UNIT1 ,BYTE> ,IS_SAME<UNIT1 ,WORD> ,IS_SAME<UNIT1 ,CHAR> ,IS_SAME<UNIT1 ,FEAT>> ;
define IS_STR<UNIT1> = ENUM_ANY<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT1 ,STRU32>> ;
define IS_NULL<UNIT1> = IS_SAME<UNIT1 ,type (NULL)> ;
define IS_BASIC<UNIT1> = ENUM_ANY<IS_BOOL<UNIT1> ,IS_VAR<UNIT1> ,IS_FLOAT<UNIT1> ,IS_BYTE<UNIT1> ,IS_STR<UNIT1> ,IS_NULL<UNIT1>> ;
define IS_FUNCTION<UNIT1> = enum (concept (UNIT1).is_function) ;
define IS_NOEXCEPT<UNIT1> = enum (concept (UNIT1).is_noexcept) ;
define IS_CLASS<UNIT1> = enum (concept (UNIT1).is_class) ;
define IS_STRUCT<UNIT1> = enum (concept (UNIT1).is_struct) ;
define IS_TRIVIAL<UNIT1> = enum (concept (UNIT1).is_trivial) ;
define IS_INTERFACE<UNIT1> = enum (concept (UNIT1).is_interface) ;
define IS_CONSTRUCTIBLE<RETURN ,PARAMS> = enum (concept (type<RETURN ,PARAMS>).is_constructible) ;
define IS_CONVERTIBLE<FROM ,TO> = enum (concept (type<FROM ,TO>).is_convertible) ;
define IS_CLONEABLE<UNIT1> = enum (concept (UNIT1).is_cloneable) ;
define IS_EXTEND<BASE ,DERIVED> = enum (concept (type<BASE ,DERIVED>).is_extend) ;
define IS_PLACEHOLDER<UNIT1> = enum (concept (UNIT1).name == "PLACEHOLDER") ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_EQ_ZERO<COUNTOF<ARGS>>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_EQ_IDEN<COUNTOF<ARGS>>) ;

	using RET = ENUM_IDEN ;
} ;

trait IS_ALL_SAME_HELP<ARGS> {
	require (ENUM_GT_IDEN<COUNTOF<ARGS>>) ;

	using R1X = IS_SAME<TYPE_FIRST_ONE<ARGS> ,TYPE_SECOND_ONE<ARGS>> ;
	using R2X = typename IS_ALL_SAME_HELP<TYPE_FIRST_REST<ARGS>>::RET ;

	using RET = ENUM_ALL<R1X ,R2X> ;
} ;

define IS_ALL_SAME<...UNITS> = typename IS_ALL_SAME_HELP<UNITS>::RET ;

trait IS_ANY_SAME_HELP<ARGS> {
	require (ENUM_EQ_ZERO<COUNTOF<ARGS>>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME_HELP<ARGS> {
	require (ENUM_EQ_IDEN<COUNTOF<ARGS>>) ;

	using RET = ENUM_ZERO ;
} ;

trait IS_ANY_SAME_HELP<ARGS> {
	require (ENUM_GT_IDEN<COUNTOF<ARGS>>) ;

	using R1X = IS_SAME<TYPE_FIRST_ONE<ARGS> ,TYPE_SECOND_ONE<ARGS>> ;
	using R2X = typename IS_ANY_SAME_HELP<TYPE_FIRST_REST<ARGS>>::RET ;
	using R3X = typename IS_ANY_SAME_HELP<TYPE_CAT<type<R1X> ,TYPE_SECOND_REST<ARGS>>>::RET ;

	using RET = ENUM_ANY<R1X ,R2X ,R3X> ;
} ;

define IS_ANY_SAME<...UNITS> = typename IS_ANY_SAME_HELP<UNITS>::RET ;

trait BYTE_TRAIT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<BYTE>>) ;
	require (ENUM_EQUAL<ARG1 ,ALIGNOF<BYTE>>) ;

	using RET = BYTE ;
} ;

trait BYTE_TRAIT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<WORD>>) ;
	require (ENUM_EQUAL<ARG1 ,ALIGNOF<WORD>>) ;

	using RET = WORD ;
} ;

trait BYTE_TRAIT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<CHAR>>) ;
	require (ENUM_EQUAL<ARG1 ,ALIGNOF<CHAR>>) ;

	using RET = CHAR ;
} ;

trait BYTE_TRAIT_HELP<ARG1 ,ARG2> {
	require (ENUM_EQUAL<ARG1 ,SIZEOF<FEAT>>) ;
	require (ENUM_EQUAL<ARG1 ,ALIGNOF<FEAT>>) ;

	using RET = FEAT ;
} ;

define BYTE_TRAIT<SIZE ,ALIGN> = typename BYTE_TRAIT_HELP<SIZE ,ALIGN>::RET ;

trait STORAGE_HELP<ARG1 ,ARG2> {
	using ITEM = BYTE_TRAIT<ARG2 ,ARG2> ;
	using SIZE = ENUM_DIV<ENUM_DEC<ENUM_ADD<ARG1 ,SIZEOF<ITEM>>> ,SIZEOF<ITEM>> ;

	struct STORAGE {
		variable ...mUnused :TYPE_REPEAT<ITEM ,SIZE> ;
	} ;
} ;

define STORAGE<SIZE> = typename U::STORAGE_HELP<SIZE ,ENUM_IDEN>::STORAGE ;
define STORAGE<SIZE ,ALIGN> = typename U::STORAGE_HELP<SIZE ,ALIGN>::STORAGE ;

trait TEMP_HELP<ARG1> {
	struct TEMP {
		using HINT = ARG1 ;

		variable mStorage :STORAGE<SIZEOF<ARG1> ,ALIGNOF<ARG1>> ;
	} ;
} ;

define TEMP<UNIT1> = typename U::TEMP_HELP<UNIT1>::TEMP ;

define IS_TEMP<UNIT1> = enum (concept (UNIT1).name == "TEMP") ;
define REMOVE_TEMP<UNIT1> = typename ENABLE<IS_TEMP<UNIT1> ,UNIT1>::HINT ;

static function noop = () => {} ;

static function abort = () => unsafe_abort () ;

static function address = (arg1) => unsafe_address (arg1) ;

static function bitwise = (arg1) => unsafe_bitwise (arg1) ;

static function swap = (arg1 ,arg2) => unsafe_swap (arg1 ,arg2) ;

static function forward = (obj) => {
	using R1X = type (obj) ;
	variable ret = R1X () ;
	swap (ret ,obj) ;
	return ret ;
} ;

static function alignas = (base :LENGTH ,align :LENGTH) :LENGTH => {
	constant r1x = align - base % align ;
	return base + r1x % align ;
} ;

static function between = (index :INDEX ,begin :INDEX ,end :INDEX) :BOOL => {
	if not (index >= begin)
		return FALSE ;
	if not (index < end)
		return FALSE ;
	return TRUE ;
} ;

static function abs = (arg1) => {
	using R1X = type (arg1) ;
	if (arg1 < R1X (ZERO))
		return -arg1 ;
	return +arg1 ;
} ;

static function min = (arg1 ,arg2) => {
	if (arg1 < arg2)
		return arg1 ;
	return arg2 ;
} ;

static function max = (arg1 ,arg2) => {
	if (arg1 < arg2)
		return arg2 ;
	return arg1 ;
} ;

static function hashcode = () :FLAG => {
	constant r1x = VAR64 (-3750763034362895579) ;
	constant r2x = VAR64 (FEAT (r1x) & FEAT (VAR64_MAX)) ;
	return FLAG (r2x) ;
} ;

static function hashcode = (arg1 :FLAG ,arg2 :FLAG)) :FLAG => {
	constant r1x = VAR64 (FEAT (arg1) ^ FEAT (arg2)) ;
	constant r2x = r1x * VAR64 (1099511628211) ;
	constant r3x = VAR64 (FEAT (r2x) & FEAT (VAR64_MAX)) ;
	return FLAG (r3x) ;
} ;

static function bad = (id) => {
	using R1X = type (id) ;
	register r1x = () :auto => ZERO ;
	assert (FALSE) ;
	return R1X (r1x ()) ;
} ;

static function range = (begin :INDEX ,end :INDEX) => unsafe_range (begin ,end) ;

static function cabi = (id) :FLAG => unsafe_cabi (id) ;

trait BOX_HELP<ARG1> {
	using UNIT1 = ARG1 ;

	class Box {
		interface Holder ;
		class ImplHolder ;

		variable mHolder :[] :Holder ;
	} ;

	interface Box::Holder {
		function destroy = (self :[] :Holder) => virtual ;
		property to = [] :UNIT1 => virtual ;
	} ;

	private trait IMPLHOLDER_HELP<ARG2> {
		using UNIT2 = ARG2 ;

		class Box::ImplHolder {
			variable mValue :UNIT2 ;
		} ;

		implement Box::ImplHolder {
			function new = (value :UNIT1) => {
				mValue = forward (value) ;
			} ;

			static function create = (that :UNIT1) => {
				constant r1x = unsafe_alloc (type<TEMP<ImplHolder>>::id) ;
				unsafe_create (r1x[] ,ImplHolder (forward (that))) ;
				return [] :Holder => unsafe_deref (r1x[]) ;
			} ;
		} ;

		implement Box::ImplHolder :Holder {
			function destroy = (self :[] :Holder) => {
				assert (address (this) == address (self[])) ;
				variable rax = TEMP<ImplHolder> () ;
				unsafe_zeroize (rax) ;
				swap (unsafe_deref (rax) ,this) ;
				unsafe_free (self) ;
				unsafe_destroy (rax) ;
			} ;

			property to = [] :UNIT1 => mValue ;
		} ;
	} ;

	implement Box {
		function new = () => {
			mHolder = NULL ;
		} ;

		function delete = noexcept () => {
			if (mHolder == NULL)
				return ;
			mHolder[].destroy (mHolder) ;
			mHolder = NULL ;
		} ;

		private trait MAKE_HELP<> {
			require (IS_INTERFACE<UNIT1>) ;

			static function make = (...that) :Box => {
				using R1X = type (that) ;
				using R3X = TYPE_UNWRAP<R1X> ;
				require (IS_EXTEND<UNIT1 ,R3X>) ;
				using R2X = typename IMPLHOLDER_HELP<R3X>::ImplHolder ;
				variable ret = Box () ;
				ret.mHolder = R2X::create (forward (that)) ;
				return ret ;
			} ;
		} ;

		private trait MAKE_HELP<> {
			require not (IS_INTERFACE<UNIT1>) ;

			static function make = (...that) :Box => {
				using R2X = typename IMPLHOLDER_HELP<UNIT1>::ImplHolder ;
				variable ret = Box () ;
				ret.mHolder = R2X::create (UNIT1 (forward (that)...)) ;
				return ret ;
			} ;
		} ;

		static function make = (...that) :Box => MAKE_HELP<>::make (forward (that)...) ;

		function exist = () :BOOL => {
			if (mHolder == NULL)
				return FALSE ;
			return TRUE ;
		} ;

		property to = mutable [] :UNIT1 => {
			assert (exist ()) ;
			return mHolder[]->this ;
		} ;
	} ;
} ;

define Box<UNIT1> = typename BOX_HELP<UNIT1>::Box ;

trait CELL_HELP<ARG1> {
	require (IS_CLONEABLE<ARG1>) ;

	using UNIT1 = ARG1 ;

	class Cell {
		volatile mValue :TEMP<UNIT1> ;
		variable mExist :BOOL ;
	} ;	

	implement Cell {
		function new = () => {
			mExist = FALSE ;
		} ;

		function new = (that :UNIT1) => {
			unsafe_create (mValue ,forward (that)) ;
			mExist = TRUE ;
		} ;

		function delete = noexcept () => {
			if not (mExist)
				return ;
			unsafe_destroy (mValue) ;
			mExist = FALSE ;
		} ;

		function exist = () :BOOL => mExist ;

		function clone = () :Cell => {
			if not (exist ())
				return Cell () ;
			return Cell (fetch ()) ;
		} ;
		
		function fetch = () :UNIT1 => {
			assert (exist ()) ;
			return  m_fake[] ;
		} ;
		
		function store = (obj :UNIT1) => {
			assert (exist ()) ;
			m_fake[] = forward (obj) ;
		} ;

		function exchange = (obj :UNIT1) :UNIT1 => {
			assert (exist ()) ;
			variable ret = m_fake[] ;
			m_fake[] = forward (obj) ;
			return ret ;
		} ;

		function change = (expect :UNIT1 ,obj :UNIT1) :BOOL => {
			assert (exist ()) ;
			switch {
				if (m_fake[] == expect)
					break ;
				expect = m_fake[] ;
				return FALSE ;
			} ;
			m_fake[] = forward (obj) ;
			return TRUE ;
		} ;

		private property m_fake = [] :UNIT1 => unsafe_deref (mValue) ;
	} ;
} ;

define Cell<UNIT1> = typename CELL_HELP<UNIT1>::Cell ;

trait RC_HELP<ARG1> {
	using UNIT1 = ARG1 ;
	
	class RC {
		interface Holder ;
		class ImplHolder ;

		variable mHolder :[] :Holder ;
	} ;

	interface RC::Holder {
		function destroy = (self :[] :Holder) => virtual ;
		function increase = () :LENGTH => virtual ;
		function decrease = () :LENGTH => virtual ;
		property to = [] :UNIT1 => virtual ;
	} ;

	private trait IMPLHOLDER_HELP<ARG2> {
		class RC::ImplHolder {
			variable mValue :UNIT1 ;
			variable mCounter :LENGTH ;
		} ;

		implement RC::ImplHolder {
			function new = (value :UNIT1) => {
				mValue = forward (value) ;
				mCounter = ZERO ;
			} ;

			static function create = (that :UNIT1) => {
				constant r1x = unsafe_alloc (type<TEMP<ImplHolder>>::id) ;
				unsafe_create (r1x[] ,ImplHolder (forward (that))) ;
				return [] :Holder => unsafe_deref (r1x[]) ;
			} ;
		} ;

		implement RC::ImplHolder :Holder {
			function destroy = (self :[] :Holder) => {
				assert (address (this) == address (self[])) ;
				variable rax = TEMP<ImplHolder> () ;
				unsafe_zeroize (rax) ;
				swap (unsafe_deref (rax) ,this) ;
				unsafe_free (self) ;
				unsafe_destroy (rax) ;
			} ;

			function increase = () :LENGTH => {
				mCounter = mCounter + 1 ;
				return mCounter ;
			} ;

			function decrease = () :LENGTH => {
				mCounter = mCounter - 1 ;
				return mCounter ;
			} ;

			property to = [] :UNIT1 => mValue ;
		} ;
	} ;

	implement RC {
		function new = () => {
			mHolder = NULL ;
		} ;

		function delete = noexcept () => {
			if (mHolder == NULL)
				return ;
			switch {
				constant r1x = mHolder[].decrease () ;
				if (r1x > 0)
					break ;
				mHolder[].destroy (mHolder) ;
			} ;
			mHolder = NULL ;
		} ;

		static function make = (...that) :RC => {
			using R2X = typename IMPLHOLDER_HELP<UNIT1>::ImplHolder ;
			variable ret = RC () ;
			ret.mHolder = R2X::create (UNIT1 (forward (that)...)) ;
			constant r1x = mHolder[].increase () ;
			assert (r1x == 1) ;
			return ret ;
		} ;

		function exist = () :BOOL => {
			if (mHolder == NULL)
				return FALSE ;
			return TRUE ;
		} ;

		function clone = () :RC => {
			variable ret = RC () ;
			switch {
				if not (exist ())
					break ;
				ret.mHolder = mHolder ;
				constant r1x = mHolder[].increase () ;
				assert (r1x >= 2) ;
			} ;
			return ret ;
		} ;

		property to = [] :UNIT1 => {
			assert (exist ()) ;
			return mHolder[]->this ;
		} ;
	} ;
} ;

define RC<UNIT1> = typename RC_HELP<UNIT1>::RC ;

trait SLICE_HELP<ARG1> {
	using UNIT1 = ARG1 ;

	class Slice {
		interface Holder ;

		variable mHolder :RC<Box<Holder>> ;
	} ;

	interface Slice::Holder {
		function size = () :LENGTH => virtual ;
		property at = [index :INDEX] :UNIT1 => virtual ;
	} ;

	implement Slice {
		function new = () => default ;

		function new = (...that) => unsafe_slice (forward (that)...) ;

		function size = () :LENGTH => {
			if not (mHolder.exist ())
				return ZERO ;
			return mHolder->size () ;
		} ;

		function addr = () :LENGTH => {
			if not (mHolder.exist ())
				return ZERO ;
			return address (mHolder->at[0]) ;
		} ;

		property at = [index :INDEX] :UNIT1 => {
			assert (between (index ,0 ,size ())) ;
			return mHolder->at[index] ;
		} ;

		function equal = (that :Slize) :BOOL => {
			if not (size () == that.size ())
				return FALSE ;
			for (i) in range (0 ,size ()) {
				if not (mHolder->at[i] == that.mHolder->at[i])
					return FALSE ;
			} ;
			return TRUE ;
		} ;

		function compr = (that :Slize) :FLAG => {
			constant r1x = min (size () ,that.size ()) ;
			for (i) in range (0 ,r1x) {
				constant r2x = mHolder->at[i] <=> that.mHolder->at[i] ;
				if not (r2x == ZERO)
					return r2x ;
			} ;
			return size () <=> that.size () ;
		} ;

		function hash = () :FLAG => {
			variable ret = hashcode () ;
			for (i) in range (0 ,size ()) {
				constant r1x = FLAG (mHolder->at[i]) ;
				ret = hashcode (ret ,r1x) ;
			} ;
			return ret ;
		} ;
	} ;
} ;

define Slice<UNIT1> = typename SLICE_HELP<UNIT1>::Slice ;

trait CLAZZ_HELP<> {
	class Clazz {
		interface Holder ;

		variable mHolder :RC<Box<Holder>> ;
	} ;

	interface Clazz::Holder {
		function type_size = () :LENGTH => virtual ;
		function type_align = () :LENGTH => virtual ;
		function type_cabi = () :FLAG => virtual ;
		function type_name = () :Slice<STR> => virtual ;
	} ;

	implement Clazz {
		function new = () => new (type<>::id) ;

		function new = (...that) => unsafe_clazz (forward (that)...) ;

		function type_size = () :LENGTH => {
			if not (mHolder.exist ())
				return ZERO ;
			return mHolder->type_size () ;
		} ;

		function type_align = () :LENGTH => {
			if not (mHolder.exist ())
				return ZERO ;
			return mHolder->type_align () ;
		} ;

		function type_cabi = () :FLAG => {
			if not (mHolder.exist ())
				return ZERO ;
			return mHolder->type_cabi () ;
		} ;

		function type_name = () :Slice<STR> => {
			if not (mHolder.exist ())
				return "" ;
			return mHolder->type_name () ;
		} ;

		function equal = (that :Slize) :BOOL => {
			if (type_cabi () == that.type_cabi ())
				return TRUE ;
			return type_name () == that.type_name () ;
		} ;

		function compr = (that :Slize) :FLAG => {
			if (type_cabi () == that.type_cabi ())
				return ZERO ;
			return type_name () <=> that.type_name () ;
		} ;

		function hash = () :FLAG => {
			if (type_cabi () == ZERO)
				return ZERO ;
			constant r1x = type_name () ;
			return r1x.hash () ;
		} ;
	} ;
} ;

using Clazz = typename CLAZZ_HELP<>::Clazz ;