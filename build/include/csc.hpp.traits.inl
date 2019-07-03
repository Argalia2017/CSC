#pragma once

#ifndef __CSC_USE_TRAITS__
#error "б╞(д├бузебу ;)д├ : this file is only for 'csc.hpp'"
#endif

namespace U {
template <class>
struct is_arithmetic :public stl::false_type {} ;

template <>
struct is_arithmetic<VAR32> :public stl::true_type {} ;

template <>
struct is_arithmetic<VAR64> :public stl::true_type {} ;

#ifdef __CSC_COMPILER_MSVC__
template <>
struct is_arithmetic<VARX> :public stl::true_type {} ;

template <>
struct is_arithmetic<VARY> :public stl::true_type {} ;
#endif

template <>
struct is_arithmetic<VAL32> :public stl::true_type {} ;

template <>
struct is_arithmetic<VAL64> :public stl::true_type {} ;

template <>
struct is_arithmetic<VALX> :public stl::true_type {} ;
} ;

namespace U {
template <class>
struct is_literals :public stl::false_type {} ;

template <>
struct is_literals<STRU8> :public stl::true_type {} ;

template <>
struct is_literals<STRU16> :public stl::true_type {} ;

template <>
struct is_literals<STRU32> :public stl::true_type {} ;

template <>
struct is_literals<STRA> :public stl::true_type {} ;

template <>
struct is_literals<STRW> :public stl::true_type {} ;
} ;

namespace U {
template <BOOL _VAL1>
using ENABLE_TYPE = typename stl::enable_if<_VAL1 ,VOID>::type ;
} ;

namespace U {
template <BOOL _VAL1 ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = typename stl::conditional<_VAL1 ,_ARG1 ,_ARG2>::type ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_REFERENCE_TYPE = typename stl::remove_reference<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_CONST_TYPE = typename stl::remove_const<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_VOLATILE_TYPE = typename stl::remove_volatile<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_CVR_TYPE = REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<REMOVE_REFERENCE_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_POINTER_TYPE = typename stl::remove_pointer<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_ARRAY_TYPE = typename stl::remove_extent<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_TEMP {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_TEMP<TEMP<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_TEMP_TYPE = typename REMOVE_TEMP<_ARG1>::TYPE ;
} ;

template <class _ARG1>
using REMOVE_TEMP_TYPE = U::REMOVE_TEMP_TYPE<_ARG1> ;

namespace U {
template <class>
struct REMOVE_MEMBER_POINTER ;

template <class _ARG1 ,class _ARG2>
struct REMOVE_MEMBER_POINTER<_ARG1 _ARG2::*> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_MEMBER_POINTER_TYPE = typename REMOVE_MEMBER_POINTER<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct MEMBEROF_TRAITS ;

template <class _ARG1 ,class _ARG2>
struct MEMBEROF_TRAITS<_ARG1 _ARG2::*> {
	using TYPE = _ARG2 ;
} ;

template <class _ARG1>
using MEMBEROF_TRAITS_TYPE = typename MEMBEROF_TRAITS<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct COUNTOF_TRAITS ;

template <class _ARG1>
struct COUNTOF_TRAITS<ARR<_ARG1>> {
	using TYPE = ARGC<0> ;
} ;

template <class _ARG1 ,LENGTH _VAL1>
struct COUNTOF_TRAITS<_ARG1[_VAL1]> {
	using TYPE = ARGC<_VAL1> ;
} ;

template <class _ARG1>
using COUNTOF_TRAITS_TYPE = typename COUNTOF_TRAITS<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct FORWARD_TRAITS {
	using TYPE = const _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &> {
	using TYPE = _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &&> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using FORWARD_TRAITS_TYPE = typename FORWARD_TRAITS<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
using RESULTOF_TYPE = typename stl::result_of<_ARG1>::type ;
} ;

namespace U {
template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &&> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &&> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &&> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &&> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = typename CAST_TRAITS<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct LOAD_TRAITS {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<1>> {
	using TYPE = typename LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<2>>::TYPE ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<_ARG1 ,TEMP<_ARG1> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<TEMP<_ARG1> ,_ARG1 ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<ARR<_ARG1> ,ARR<TEMP<_ARG1>> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<ARR<TEMP<_ARG1>> ,ARR<_ARG1> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<_ARG1 ,NONE ,ARGC<1>> {
	using TYPE = ARGC<stl::is_class<_ARG1>::value> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<NONE ,_ARG1 ,ARGC<1>> {
	using TYPE = ARGC<stl::is_class<_ARG1>::value> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,VOID ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU8> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU16> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU32> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRA> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRW> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_TRAITS<_ARG1 ,BYTE ,ARGC<2>> {
	//@info: not recommend
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using LOAD_TRATIS_TYPE = typename LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<1>>::TYPE ;
} ;

namespace U {
template <class ,class ,class...>
struct INDEXOF_TRAITS {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <LENGTH _VAL1 ,class _ARG1 ,class... _ARGS>
struct INDEXOF_TRAITS<ARGC<_VAL1> ,_ARG1 ,_ARG1 ,_ARGS...> {
	using TYPE = ARGC<_VAL1> ;
} ;

template <LENGTH _VAL1 ,class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEXOF_TRAITS<ARGC<_VAL1> ,_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = typename INDEXOF_TRAITS<ARGC<_VAL1 + 1> ,_ARG1 ,_ARGS...>::TYPE ;
} ;

template <class _ARG1 ,class... _ARGS>
using INDEXOF_TRAITS_TYPE = typename INDEXOF_TRAITS<ARGC<0> ,_ARG1 ,_ARGS...>::TYPE ;
} ;

namespace U {
template <class ,class...>
struct VISITOF_TRATIS ;

template <class _ARG1 ,class... _ARGS>
struct VISITOF_TRATIS<ARGC<0> ,_ARG1 ,_ARGS...> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct VISITOF_TRATIS<_ARG1 ,_ARG2 ,_ARGS...> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename VISITOF_TRATIS<ARGC<_ARG1::value - 1> ,_ARGS...>::TYPE ;
} ;

template <INDEX _VAL1 ,class... _ARGS>
using VISITOF_TRATIS_TYPE = typename VISITOF_TRATIS<ARGC<_VAL1> ,_ARGS...>::TYPE ;
} ;

namespace U {
template <class TYPE ,class = VOID>
struct is_complete_type :public stl::false_type {} ;

template <class TYPE>
struct is_complete_type<TYPE ,ENABLE_TYPE<(_SIZEOF_ (TYPE) > 0)>> :public stl::true_type {} ;
} ;

namespace U {
template <class TYPE ,class BASE ,class = VOID>
struct is_interface_type :public stl::false_type {} ;

template <class TYPE ,class BASE>
struct is_interface_type<TYPE ,BASE ,ENABLE_TYPE<_SIZEOF_ (TYPE) == _SIZEOF_ (BASE) && _ALIGNOF_ (TYPE) == _ALIGNOF_ (BASE)>> :public stl::is_base_of<BASE ,TYPE> {} ;
} ;

namespace U {
template <class _ARG1 ,class _ARG2 ,class = VOID>
struct is_always_base_of :public stl::false_type {} ;

template <class _ARG1 ,class _ARG2>
struct is_always_base_of<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0 && _SIZEOF_ (_ARG2) > 0)>> :public stl::is_base_of<_ARG1 ,_ARG2> {} ;
} ;

namespace U {
template <class...>
struct is_all_same :public stl::false_type {} ;

template <class _ARG1>
struct is_all_same<_ARG1> :public stl::true_type {} ;

template <class _ARG1>
struct is_all_same<_ARG1 ,_ARG1> :public stl::true_type {} ;

template <class _ARG1 ,class... _ARGS>
struct is_all_same<_ARG1 ,_ARG1 ,_ARGS...> :public is_all_same<_ARG1 ,_ARGS...> {} ;
} ;

namespace U {
template <class>
struct is_template_type :public stl::false_type {} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct is_template_type<_ARGT<_ARGS...>> :public stl::true_type {} ;
} ;

namespace U {
template <template <class...> class ,class>
struct is_template_of :public stl::false_type {} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct is_template_of<_ARGT ,_ARGT<_ARGS...>> :public stl::true_type {} ;
} ;