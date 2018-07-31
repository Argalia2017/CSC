#pragma once

#ifndef __CSC_EXT__
#define __CSC_EXT__
#endif

#include "csc.hpp"

#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039) //@info:warning C5039: “xxx”: 可能引发函数的指针或引用传递到 -EHc 下的外部 C 函数。如果该函数引发异常，未定义行为可能发生。
#endif
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#pragma warning (pop)
#pragma endregion

namespace CSC {
class VAR128 {
#pragma region
#pragma push_macro ("v2i0")
#pragma push_macro ("v2i1")
#pragma push_macro ("v4i0")
#pragma push_macro ("v4i1")
#pragma push_macro ("v4i2")
#pragma push_macro ("v4i3")
#undef v2i0
#undef v2i1
#undef v4i0
#undef v4i1
#undef v4i2
#undef v4i3
#define v2i0 m_v2i0 ()
#define v2i1 m_v2i1 ()
#define v4i0 m_v4i0 ()
#define v4i1 m_v4i1 ()
#define v4i2 m_v4i2 ()
#define v4i3 m_v4i3 ()

private:
	MEGA mData ;

public:
	inline VAR128 () = default ;

	inline implicit VAR128 (VAR64 right) {
		v2i1 = DATA (right) ;
		v2i0 = right >= 0 ? DATA (0) : DATA (-1) ;
	}

	inline explicit operator VAR32 () const {
		return VAR32 (v2i1) ;
	}

	inline explicit operator VAR64 () const {
		return VAR64 (v2i1) ;
	}

	inline BOOL operator== (const VAR128 &right) const {
		return v2i1 == right.v2i1 && v2i0 == right.v2i0 ;
	}

	inline BOOL operator!= (const VAR128 &right) const {
		return v2i1 != right.v2i1 || v2i0 != right.v2i0 ;
	}

	inline BOOL operator< (const VAR128 &right) const {
		auto &r1 = _CAST_<VAR64> (v2i0) ;
		auto &r2 = _CAST_<VAR64> (right.v2i0) ;
		return r1 < r2 ? TRUE : r1 > r2 ? FALSE : v2i1 < right.v2i1 ;
	}

	inline BOOL operator>= (const VAR128 &right) const {
		auto &r1 = _CAST_<VAR64> (v2i0) ;
		auto &r2 = _CAST_<VAR64> (right.v2i0) ;
		return r1 < r2 ? FALSE : r1 > r2 ? TRUE : v2i1 >= right.v2i1 ;
	}

	inline BOOL operator> (const VAR128 &right) const {
		auto &r1 = _CAST_<VAR64> (right.v2i0) ;
		auto &r2 = _CAST_<VAR64> (v2i0) ;
		return r1 < r2 ? TRUE : r1 > r2 ? FALSE : right.v2i1 < v2i1 ;
	}

	inline BOOL operator<= (const VAR128 &right) const {
		auto &r1 = _CAST_<VAR64> (right.v2i0) ;
		auto &r2 = _CAST_<VAR64> (v2i0) ;
		return r1 < r2 ? FALSE : r1 > r2 ? TRUE : right.v2i1 >= v2i1 ;
	}

	inline VAR128 operator& (const VAR128 &right) const {
		VAR128 ret ;
		ret.v2i1 = v2i1 & right.v2i1 ;
		ret.v2i0 = v2i0 & right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator&= (const VAR128 &right) {
		v2i1 &= right.v2i1 ;
		v2i0 &= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator| (const VAR128 &right) const {
		VAR128 ret ;
		ret.v2i1 = v2i1 | right.v2i1 ;
		ret.v2i0 = v2i0 | right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator|= (const VAR128 &right) {
		v2i1 |= right.v2i1 ;
		v2i0 |= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator^ (const VAR128 &right) const {
		VAR128 ret ;
		ret.v2i1 = v2i1 ^ right.v2i1 ;
		ret.v2i0 = v2i0 ^ right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator^= (const VAR128 &right) {
		v2i1 ^= right.v2i1 ;
		v2i0 ^= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator~ () const {
		VAR128 ret ;
		ret.v2i1 = ~v2i1 ;
		ret.v2i0 = ~v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 operator>> (VAR scale) const {
		VAR128 ret ;
		ret.v2i1 = scale < 64 ? (v2i1 >> scale) | (v2i0 << (64 - scale)) : v2i0 >> (scale - 64) ;
		ret.v2i0 = scale < 64 ? v2i0 >> scale : 0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator>>= (VAR scale) {
		v2i1 = scale < 64 ? (v2i1 >> scale) | (v2i0 << (64 - scale)) : v2i0 >> (scale - 64) ;
		v2i0 = scale < 64 ? v2i0 >> scale : 0 ;
		return *this ;
	}

	inline VAR128 operator<< (VAR scale) const {
		VAR128 ret ;
		ret.v2i0 = scale < 64 ? (v2i0 << scale) | (v2i1 >> (64 - scale)) : v2i1 << (scale - 64) ;
		ret.v2i1 = scale < 64 ? v2i1 << scale : 0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator<<= (VAR scale) {
		v2i0 = scale < 64 ? (v2i0 << scale) | (v2i1 >> (64 - scale)) : v2i1 << (scale - 64) ;
		v2i1 = scale < 64 ? v2i1 << scale : 0 ;
		return *this ;
	}

	inline VAR128 operator+ (const VAR128 &right) const {
		VAR128 ret ;
		ret.v2i0 = v2i0 + right.v2i0 + EFLAG (v2i1 > ~right.v2i1) ;
		ret.v2i1 = v2i1 + right.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator+= (const VAR128 &right) {
		v2i0 += right.v2i0 + EFLAG (v2i1 > ~right.v2i1) ;
		v2i1 += right.v2i1 ;
		return *this ;
	}

	inline VAR128 operator- (const VAR128 &right) const {
		VAR128 ret ;
		ret.v2i0 = v2i0 - right.v2i0 - EFLAG (v2i1 < right.v2i1) ;
		ret.v2i1 = v2i1 - right.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-= (const VAR128 &right) {
		v2i0 -= right.v2i0 + EFLAG (v2i1 < right.v2i1) ;
		v2i1 -= right.v2i1 ;
		return *this ;
	}

	inline VAR128 operator* (const VAR128 &right) const {
		VAR128 ret ;
		auto rax = DATA () ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i3) ;
		ret.v4i3 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i2) ;
		rax += DATA (v4i2) * DATA (right.v4i3) ;
		ret.v4i2 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i1) ;
		rax += DATA (v4i2) * DATA (right.v4i2) ;
		rax += DATA (v4i1) * DATA (right.v4i3) ;
		ret.v4i1 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i0) ;
		rax += DATA (v4i2) * DATA (right.v4i1) ;
		rax += DATA (v4i1) * DATA (right.v4i2) ;
		rax += DATA (v4i0) * DATA (right.v4i3) ;
		ret.v4i0 = CHAR (rax) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator*= (const VAR128 &right) {
		return *this = *this * right ;
	}

	inline VAR128 operator/ (const VAR128 &right) const {
		_DEBUG_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret ;
		if (_CAST_<VAR64> (v2i0) >= 0 && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			auto rax = DATA () ;
			const auto r1x = DATA (right.v4i3) ;
			rax = (DATA (0) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = CHAR (rax / r1x) ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = CHAR (rax / r1x) ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = CHAR (rax / r1x) ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax / r1x) ;
		} else if ((v2i0 != DATA (VAR64_MIN) || v2i1 != 0) && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			ret = -(-*this / right) ;
		} else {
			//@warn:unimplemented
			_DEBUG_ASSERT_ (FALSE) ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator/= (const VAR128 &right) {
		return *this = *this / right ;
	}

	inline VAR128 operator% (const VAR128 &right) const {
		_DEBUG_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret ;
		if (_CAST_<VAR64> (v2i0) >= 0 && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			auto rax = DATA () ;
			const auto r1x = DATA (right.v4i3) ;
			rax = (DATA (0) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = 0 ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = 0 ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = 0 ;
			rax = (DATA (rax % r1x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax % r1x) ;
		} else if ((v2i0 != DATA (VAR64_MIN) || v2i1 != 0) && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			ret = -(-*this % right) ;
		} else {
			//@warn:unimplemented
			_DEBUG_ASSERT_ (FALSE) ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator%= (const VAR128 &right) {
		return *this = *this % right ;
	}

	inline VAR128 operator+ () const {
		return *this ;
	}

	inline VAR128 operator- () const {
		VAR128 ret ;
		ret.v2i1 = ~v2i1 + 1 ;
		ret.v2i0 = ~v2i0 + EFLAG (ret.v2i1 == DATA (0)) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator++ () {
		v2i1++ ;
		v2i0 += EFLAG (v2i1 == DATA (0)) ;
		return *this ;
	}

	inline VAR128 operator++ (int) popping {
		VAR128 ret = *this ;
		++*this ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-- () {
		v2i1-- ;
		v2i0 -= EFLAG (v2i1 == DATA (-1)) ;
		return *this ;
	}

	inline VAR128 operator-- (int) popping {
		VAR128 ret = *this ;
		--*this ;
		return std::move (ret) ;
	}

private:
	inline DATA &m_v2i0 () {
		static constexpr WORD M_VALUE = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (M_VALUE)[0]] ;
	}

	inline const DATA &m_v2i0 () const {
		static constexpr WORD M_VALUE = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (M_VALUE)[0]] ;
	}

	inline DATA &m_v2i1 () {
		static constexpr WORD M_VALUE = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (M_VALUE)[1]] ;
	}

	inline const DATA &m_v2i1 () const {
		static constexpr WORD M_VALUE = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (M_VALUE)[1]] ;
	}

	inline CHAR &m_v4i0 () {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[0]] ;
	}

	inline const CHAR &m_v4i0 () const {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[0]] ;
	}

	inline CHAR &m_v4i1 () {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[1]] ;
	}

	inline const CHAR &m_v4i1 () const {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[1]] ;
	}

	inline CHAR &m_v4i2 () {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[2]] ;
	}

	inline const CHAR &m_v4i2 () const {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[2]] ;
	}

	inline CHAR &m_v4i3 () {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[3]] ;
	}

	inline const CHAR &m_v4i3 () const {
		static constexpr CHAR M_VALUE = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (M_VALUE)[3]] ;
	}

#pragma pop_macro ("v2i1")
#pragma pop_macro ("v2i0")
#pragma pop_macro ("v4i3")
#pragma pop_macro ("v4i2")
#pragma pop_macro ("v4i1")
#pragma pop_macro ("v4i0")
#pragma endregion
} ;

template <class TYPE>
class Mutable {
private:
	mutable BOOL mCached ;
	mutable TYPE mData ;

public:
	inline Mutable () {
		signal () ;
	}

	inline BOOL available () const {
		return mCached ;
	}

	inline const TYPE &to () const {
		return mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline const TYPE &value (const TYPE &def) const {
		return available () ? to () : def ;
	}

	inline void apply (const Function<void (TYPE &)> &proc) const {
		if (!mCached)
			proc (mData) ;
		mCached = TRUE ;
	}

	inline void apply (const Function<void (const TYPE &)> &proc) const {
		if (!mCached)
			proc (mData) ;
		mCached = TRUE ;
	}

	inline void apply (const Function<DEF<void (TYPE &)> NONE::*> &proc) const {
		if (!mCached)
			proc (mData) ;
		mCached = TRUE ;
	}

	inline void apply (const Function<DEF<void (const TYPE &)> NONE::*> &proc) const {
		if (!mCached)
			proc (mData) ;
		mCached = TRUE ;
	}

	inline void signal () const {
		mCached = FALSE ;
	}
} ;

namespace U {
template <class ,class...>
struct INDEXOF_TRAITS {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEXOF_TRAITS<_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = CONDITIONAL_TYPE<std::is_same<_ARG1 ,_ARG2>::value ,ARGC<VAR_ZERO> ,ARGC<INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE::value != VAR_NONE ? INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE::value + 1 : VAR_NONE>> ;
} ;
} ;

template <class _ARG1 ,class... _ARGS>
using INDEXOF_TRAITS_TYPE = typename U::INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE ;

namespace U {
template <class ,class...>
struct VISITOF_TRATIS ;

template <class _ARG1 ,class... _ARGS>
struct VISITOF_TRATIS<ARGC<0> ,_ARG1 ,_ARGS...> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct VISITOF_TRATIS<_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = typename VISITOF_TRATIS<ARGC<_ARG1::value - 1> ,_ARGS...>::TYPE ;
} ;
} ;

template <class _ARG1 ,class... _ARGS>
using VISITOF_TRATIS_TYPE = typename U::VISITOF_TRATIS<_ARG1 ,_ARGS...>::TYPE ;

template <class... TYPES>
class Variant {
private:
	inline static constexpr LENGTH cvt_max_sizeof (const ARGVS<> &) {
		return 1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static constexpr LENGTH cvt_max_sizeof (const ARGVS<_ARG1 ,_ARGS...> &) {
		return _MAX_ (_SIZEOF_ (_ARG1) ,cvt_max_sizeof (ARGVS<_ARGS...> {})) ;
	}

	inline static constexpr LENGTH cvt_max_alignof (const ARGVS<> &) {
		return 1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static constexpr LENGTH cvt_max_alignof (const ARGVS<_ARG1 ,_ARGS...> &) {
		return _MAX_ (_ALIGNOF_ (_ARG1) ,cvt_max_alignof (ARGVS<_ARGS...> {})) ;
	}

#ifdef __CSC_COMPILER_GNUC__
	template <LENGTH _VAL1 ,LENGTH _VAL2>
	struct VARIANT_TEMP_HELPER {
		alignas (_VAL1) DEF<BYTE[_VAL2]> unused ;
	} ;

	using VARIANT_TEMP = VARIANT_TEMP_HELPER < cvt_max_alignof (ARGVS<TYPES...> {}) ,cvt_max_sizeof (ARGVS<TYPES...> {}) > ;
#else
	struct VARIANT_TEMP {
		static constexpr LENGTH TEMP_SIZE = cvt_max_sizeof (ARGVS<TYPES...> {}) ;
		static constexpr LENGTH TEMP_ALIGN = cvt_max_alignof (ARGVS<TYPES...> {}) ;

		alignas (TEMP_ALIGN) DEF<BYTE[TEMP_SIZE]> unused ;
	} ;
#endif

	struct NULLOPT ;

private:
	_STATIC_ASSERT_ (_SIZESOF_ (TYPES) > 0) ;
	VARIANT_TEMP mVariant ;
	INDEX mIndex ;

public:
	inline Variant () {
		mIndex = template_construct (&mVariant ,_NULL_<ARGVS<TYPES...>> ()) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG> ,Variant>::value>>
	inline implicit Variant (_ARG &&right) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_ARG> ,TYPES...>::value != VAR_NONE) ;
		mIndex = template_init_construct (&mVariant ,std::forward<_ARG> (right)) ;
	}

	inline ~Variant () noexcept {
		if (mIndex == VAR_NONE)
			return ;
		template_destruct (&mVariant ,mIndex ,_NULL_<ARGVS<TYPES...>> ()) ;
		mIndex = VAR_NONE ;
	}

	inline Variant (const Variant &right) {
		mIndex = right.mIndex ;
		if (mIndex != VAR_NONE)
			template_copy_construct (&mVariant ,&right.mVariant ,mIndex ,_NULL_<ARGVS<TYPES...>> ()) ;
	}

	inline Variant &operator= (const Variant &right) {
		if (this == &right)
			return *this ;
		this->~Variant () ;
		new (this) Variant (std::move (right)) ;
		return *this ;
	}

	inline Variant (Variant &&right) noexcept {
		mIndex = right.mIndex ;
		if (mIndex != VAR_NONE)
			template_move_construct (&mVariant ,&right.mVariant ,mIndex ,_NULL_<ARGVS<TYPES...>> ()) ;
	}

	inline Variant &operator= (Variant &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Variant () ;
		new (this) Variant (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mIndex != VAR_NONE ;
	}

	template <class _RET>
	inline BOOL available () const {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value != VAR_NONE) ;
		return mIndex == INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value ;
	}

	inline VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> &to () ;

	inline implicit operator VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> & () {
		return to () ;
	}

	inline const VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> &to () const ;

	inline implicit operator const VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> & () const {
		return to () ;
	}

	template <class _ARG>
	inline _ARG &value (_ARG &def) {
		return available<_ARG> () ? _CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant)) : def ;
	}

	template <class _ARG>
	inline const _ARG &value (const _ARG &def) const {
		return available<_ARG> () ? _CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant)) : def ;
	}

	template <class _ARG>
	inline void apply (const Function<void (_ARG &)> &proc) {
		if (available<_ARG> ())
			proc (_CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant))) ;
	}

	template <class _ARG>
	inline void apply (const Function<void (const _ARG &)> &proc) const {
		if (available<_ARG> ())
			proc (_CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant))) ;
	}

	template <class _ARG>
	inline void apply (const Function<DEF<void (_ARG &)> NONE::*> &proc) {
		if (available<_ARG> ())
			proc (_CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant))) ;
	}

	template <class _ARG>
	inline void apply (const Function<DEF<void (const _ARG &)> NONE::*> &proc) const {
		if (available<_ARG> ())
			proc (_CAST_<_ARG> (_LOAD_<TEMP<_ARG>> (&mVariant))) ;
	}

	template <class _RET ,class... _ARGS>
	inline void recreate (_ARGS &&...args) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value != VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_constructible<_RET ,_ARGS &&...>::value) ;
		if (mIndex != VAR_NONE)
			template_destruct (&mVariant ,mIndex ,_NULL_<ARGVS<TYPES...>> ()) ;
		mIndex = VAR_NONE ;
		mIndex = template_create (_NULL_<ARGV<ARGC<TRUE>>> () ,&_LOAD_<TEMP<_RET>> (&mVariant) ,std::forward<_ARGS> (args)...) ;
	}

private:
	inline explicit Variant (const ARGV<NULLOPT> &) noexcept :mIndex (VAR_NONE) {}

private:
	inline static INDEX template_construct (PTR<VARIANT_TEMP> adress ,const ARGVS<> &) popping {
		return VAR_NONE ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_construct (PTR<VARIANT_TEMP> adress ,const ARGVS<_ARG1 ,_ARGS...> &) popping {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		INDEX ret = template_create (_NULL_<ARGV<ARGC<std::is_default_constructible<_ARG1>::value>>> () ,&_LOAD_<TEMP<_ARG1>> (adress)) ;
		if (ret == VAR_NONE)
			ret = template_construct (adress ,_NULL_<ARGVS<_ARGS...>> ()) ;
		return std::move (ret) ;
	}

	template <class _ARG>
	inline static INDEX template_init_construct (PTR<VARIANT_TEMP> adress ,_ARG &&arg) popping {
		INDEX ret = template_create (_NULL_<ARGV<ARGC<std::is_constructible<REMOVE_CVR_TYPE<_ARG> ,_ARG &&>::value>>> () ,&_LOAD_<TEMP<_ARG>> (adress) ,std::forward<_ARG> (arg)) ;
		_DYNAMIC_ASSERT_ (ret != VAR_NONE) ;
		return std::move (ret) ;
	}

	inline static void template_destruct (PTR<VARIANT_TEMP> adress ,INDEX index ,const ARGVS<> &) noexcept {}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_destruct (PTR<VARIANT_TEMP> adress ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
		if (index == 0) {
			_DESTROY_ (&_LOAD_<TEMP<_ARG1>> (adress)) ;
		} else {
			template_destruct (adress ,index - 1 ,_NULL_<ARGVS<_ARGS...>> ()) ;
		}
	}

	inline static void template_copy_construct (PTR<VARIANT_TEMP> dst ,PTR<const VARIANT_TEMP> src ,INDEX index ,const ARGVS<> &) {}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_copy_construct (PTR<VARIANT_TEMP> dst ,PTR<const VARIANT_TEMP> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		if (index == 0) {
			const auto r1x = template_create (_NULL_<ARGV<ARGC<std::is_copy_constructible<_ARG1>::value && std::is_nothrow_move_constructible<_ARG1>::value>>> () ,&_LOAD_<TEMP<_ARG1>> (dst) ,_CAST_<_ARG1> (_LOAD_<TEMP<_ARG1>> (src))) ;
			_DYNAMIC_ASSERT_ (r1x != VAR_NONE) ;
		} else {
			template_copy_construct (dst ,src ,index - 1 ,_NULL_<ARGVS<_ARGS...>> ()) ;
		}
	}

	inline static void template_move_construct (PTR<VARIANT_TEMP> dst ,PTR<VARIANT_TEMP> src ,INDEX index ,const ARGVS<> &) noexcept {}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_move_construct (PTR<VARIANT_TEMP> dst ,PTR<VARIANT_TEMP> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value && std::is_nothrow_move_assignable<_ARG1>::value) ;
		if (index == 0) {
			_CREATE_ (&_LOAD_<TEMP<_ARG1>> (dst) ,std::move (_CAST_<_ARG1> (_LOAD_<TEMP<_ARG1>> (src)))) ;
		} else {
			template_move_construct (dst ,src ,index - 1 ,_NULL_<ARGVS<_ARGS...>> ()) ;
		}
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_create (const ARGV<ARGC<TRUE>> & ,PTR<TEMP<_ARG1>> adress ,_ARGS &&...args) popping {
		_CREATE_ (adress ,std::forward<_ARGS> (args)...) ;
		return INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_ARG1> ,TYPES...>::value ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_create (const ARGV<ARGC<FALSE>> & ,PTR<TEMP<_ARG1>> adress ,_ARGS &&...args) popping {
		return VAR_NONE ;
	}

public:
	inline static Variant nullopt () noexcept {
		return Variant (_NULL_<const ARGV<NULLOPT>> ()) ;
	}
} ;

template <class TYPE>
using Optional = Variant<TYPE> ;

template <class... TYPES>
inline VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> &Variant<TYPES...>::to () {
	_STATIC_ASSERT_ (_SIZESOF_ (TYPES) == 1) ;
	_DYNAMIC_ASSERT_ (exist ()) ;
	return _CAST_<VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...>> (_LOAD_<TEMP<VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...>>> (&mVariant)) ;
}

template <class... TYPES>
inline const VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...> &Variant<TYPES...>::to () const {
	_STATIC_ASSERT_ (_SIZESOF_ (TYPES) == 1) ;
	_DYNAMIC_ASSERT_ (exist ()) ;
	return _CAST_<VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...>> (_LOAD_<TEMP<VISITOF_TRATIS_TYPE<ARGC<0> ,TYPES...>>> (&mVariant)) ;
}

template <class...>
class Tuple ;

template <>
class Tuple<> {
public:
	inline constexpr Tuple () = default ;

	inline constexpr LENGTH size () const {
		return 0 ;
	}

	inline constexpr BOOL equal (const Tuple &right) const {
		return TRUE ;
	}

	inline constexpr BOOL operator== (const Tuple &right) const {
		return equal (right) ;
	}

	inline constexpr BOOL operator!= (const Tuple &right) const {
		return !equal (right) ;
	}

	inline constexpr BOOL less (const Tuple &right) const {
		return FALSE ;
	}

	inline constexpr BOOL operator< (const Tuple &right) const {
		return less (right) ;
	}

	inline constexpr BOOL operator>= (const Tuple &right) const {
		return !less (right) ;
	}

	inline constexpr BOOL operator> (const Tuple &right) const {
		return right.less (*this) ;
	}

	inline constexpr BOOL operator<= (const Tuple &right) const {
		return !right.less (*this) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class Tuple<TYPE1 ,TYPES...> :private Tuple<TYPES...> {
private:
	template <class...>
	friend class Tuple ;
	TYPE1 mData ;

public:
	inline constexpr Tuple () = default ;

	inline implicit constexpr Tuple (const TYPE1 &arg1 ,const TYPES &...args) :Tuple<TYPES...> (std::move (args)...) ,mData (std::move (arg1)) {}

	inline implicit constexpr Tuple (TYPE1 &&arg1 ,TYPES &&...args) : Tuple<TYPES...> (std::move (args)...) ,mData (std::move (arg1)) {}

	inline constexpr LENGTH size () const {
		return 1 + rest ().size () ;
	}

	inline TYPE1 &one () {
		return mData ;
	}

	inline constexpr const TYPE1 &one () const {
		return mData ;
	}

	inline Tuple<TYPES...> &rest () {
		return *this ;
	}

	inline constexpr const Tuple<TYPES...> &rest () const {
		return *this ;
	}

	template <LENGTH _VAL>
	inline VISITOF_TRATIS_TYPE<ARGC<_VAL> ,TYPE1 ,TYPES...> &visit () {
		return template_visit (*this ,_NULL_<ARGV<ARGC<_VAL>>> ()) ;
	}

	template <LENGTH _VAL>
	inline constexpr const VISITOF_TRATIS_TYPE<ARGC<_VAL> ,TYPE1 ,TYPES...> &visit () const {
		return template_visit (*this ,_NULL_<ARGV<ARGC<_VAL>>> ()) ;
	}

	inline constexpr BOOL equal (const Tuple &right) const {
		return one () == right.one () && rest () == right.rest () ;
	}

	inline constexpr BOOL operator== (const Tuple &right) const {
		return equal (right) ;
	}

	inline constexpr BOOL operator!= (const Tuple &right) const {
		return !equal (right) ;
	}

	inline constexpr BOOL less (const Tuple &right) const {
		return one () != right.one () ? one () < right.one () : rest () < right.rest () ;
	}

	inline constexpr BOOL operator< (const Tuple &right) const {
		return less (right) ;
	}

	inline constexpr BOOL operator>= (const Tuple &right) const {
		return !less (right) ;
	}

	inline constexpr BOOL operator> (const Tuple &right) const {
		return right.less (*this) ;
	}

	inline constexpr BOOL operator<= (const Tuple &right) const {
		return !right.less (*this) ;
	}

private:
	inline static VISITOF_TRATIS_TYPE<ARGC<0> ,TYPE1 ,TYPES...> &template_visit (Tuple &me ,const ARGV<ARGC<0>> &) {
		return me.one () ;
	}

	template <LENGTH _VAL>
	inline static VISITOF_TRATIS_TYPE<ARGC<_VAL> ,TYPE1 ,TYPES...> &template_visit (Tuple &me ,const ARGV<ARGC<_VAL>> &) {
		_STATIC_ASSERT_ (_VAL > 0 && _VAL <= _SIZESOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (me.rest () ,_NULL_<ARGV<ARGC<_VAL - 1>>> ()) ;
	}

	inline static constexpr const VISITOF_TRATIS_TYPE<ARGC<0> ,TYPE1 ,TYPES...> &template_visit (const Tuple &me ,const ARGV<ARGC<0>> &) {
		return me.one () ;
	}

	template <LENGTH _VAL>
	inline static constexpr const VISITOF_TRATIS_TYPE<ARGC<_VAL> ,TYPE1 ,TYPES...> &template_visit (const Tuple &me ,const ARGV<ARGC<_VAL>> &) {
		_STATIC_ASSERT_ (_VAL > 0 && _VAL <= _SIZESOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (me.rest () ,_NULL_<ARGV<ARGC<_VAL - 1>>> ()) ;
	}
} ;

template <class...>
class TupleBinder ;

template <>
class TupleBinder<> {
public:
	inline TupleBinder () = default ;

	inline TupleBinder (const TupleBinder &) = delete ;
	inline TupleBinder &operator= (const TupleBinder &) = delete ;
	inline TupleBinder (TupleBinder &&) = delete ;
	inline TupleBinder &operator= (TupleBinder &&) = delete ;

	inline const TupleBinder &operator= (const Tuple<> &arg) const {
		return *this ;
	}

	inline const TupleBinder &operator= (Tuple<> &&arg) const {
		return *this ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class TupleBinder<TYPE1 ,TYPES...> :private TupleBinder<TYPES...> {
private:
	TYPE1 & mData ;

public:
	inline TupleBinder () = delete ;

	inline explicit TupleBinder (TYPE1 &arg1 ,TYPES &...args) :TupleBinder<TYPES...> (args...) ,mData (arg1) {}

	inline const TupleBinder &operator= (const Tuple<TYPE1 ,TYPES...> &arg) const {
		mData = std::move (arg.one ()) ;
		_XVALUE_<const TupleBinder<TYPES...> &> (*this) = std::move (arg.rest ()) ;
		return *this ;
	}

	inline const TupleBinder &operator= (Tuple<TYPE1 ,TYPES...> &&arg) const {
		mData = std::move (arg.one ()) ;
		_XVALUE_<const TupleBinder<TYPES...> &> (*this) = std::move (arg.rest ()) ;
		return *this ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _TYPES>
class Function<TYPE1 (TYPES...)>::ImplHolder<PTR<TYPE1 (TYPES... ,_TYPES...)>> :public Function<TYPE1 (TYPES...)>::Holder {
private:
	PTR<TYPE1 (TYPES... ,_TYPES...)> mFunctor ;
	Tuple<REMOVE_CVR_TYPE<_TYPES>...> mParameters ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<TYPE1 (TYPES... ,_TYPES...)> arg1 ,const REMOVE_CVR_TYPE<_TYPES> &...args) :mFunctor (arg1) ,mParameters (args...) {}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const override popping {
		return template_apply (mFunctor ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)... ,mParameters) ;
	}

private:
	inline static TYPE1 template_apply (PTR<TYPE1 (TYPES... ,_TYPES...)> functor ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<> &parameters ,const REMOVE_CVR_TYPE<_TYPES> &...args2) popping {
		//@info:binding parameters must be able to convert from const reference type
		return functor (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,args2...) ;
	}

	template <class... _ARGS1 ,class... _ARGS2>
	inline static TYPE1 template_apply (PTR<TYPE1 (TYPES... ,_TYPES...)> functor ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<_ARGS2...> &parameters ,const _ARGS1 &...args2) popping {
		return template_apply (functor ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,parameters.rest () ,args2... ,parameters.one ()) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _ARGS>
inline Function<TYPE1 (TYPES...)> Function<TYPE1 (TYPES...)>::make (PTR<TYPE1 (TYPES... ,_ARGS...)> arg1 ,const REMOVE_CVR_TYPE<_ARGS> &...args) {
	ScopedHolder<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>> sgd (new ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>> (arg1 ,args...)) ;
	Function ret = Function (_XVALUE_<const PTR<Holder> &> (sgd)) ;
	sgd = NULL ;
	return std::move (ret) ;
}

template <class>
class StrongRef ;

template <class>
class WeakRef ;

template <class>
class SoftRef ;

class WeakHolder {
private:
	template <class>
	friend class StrongRef ;
	SharedRef<PACK<AnyRef<void> ,LENGTH>> mHolder ;

public:
	template <class _ARG>
	inline static StrongRef<_ARG> strong_from_this (PTR<_ARG> _this) ;
} ;

template <class TYPE>
class StrongRef {
private:
	template <class>
	friend class StrongRef ;
	friend WeakRef<TYPE> ;
	friend SoftRef<TYPE> ;
	friend WeakHolder ;
	SharedRef<PACK<AnyRef<void> ,LENGTH>> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline StrongRef () noexcept {
		mPointer = NULL ;
	}

	//@error:circular reference ruins StrongRef
	template <class _ARG ,class = ENABLE_TYPE<std::is_base_of<TYPE ,_ARG>::value>>
	inline implicit StrongRef (const StrongRef<_ARG> &right) :StrongRef (right.template recast<TYPE> ()) {}

	inline implicit StrongRef (const WeakRef<TYPE> &right) ;

	inline implicit StrongRef (const SoftRef<TYPE> &right) ;

	inline ~StrongRef () noexcept {
		if (mPointer == NULL || !mHolder.exist () || !mHolder->P1.exist ())
			return ;
		const auto r1x = --mHolder->P2 == 0 ;
		if (r1x)
			mHolder->P1 = AnyRef<void> () ;
		mPointer = NULL ;
	}

	inline StrongRef (const StrongRef &right) :StrongRef (right.mHolder ,right.mPointer) {}

	inline StrongRef &operator= (const StrongRef &right) {
		if (this == &right)
			return *this ;
		this->~StrongRef () ;
		new (this) StrongRef (std::move (right)) ;
		return *this ;
	}

	inline StrongRef (StrongRef &&right) noexcept {
		mHolder = std::move (right.mHolder) ;
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline StrongRef &operator= (StrongRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~StrongRef () ;
		new (this) StrongRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL && mHolder.exist () && mHolder->P1.exist () ;
	}

	template <class _RET>
	inline StrongRef<_RET> recast () const {
		_STATIC_ASSERT_ ((std::is_base_of<Interface ,TYPE>::value && std::is_base_of<Interface ,_RET>::value) || std::is_base_of<_RET ,TYPE>::value) ;
		return StrongRef<_RET> (mHolder ,dynamic_cast<PTR<_RET>> (mPointer)) ;
	}

	inline TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return *mPointer ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	inline PTR<TYPE> operator-> () const {
		return &to () ;
	}

	inline const StrongRef &operator= (const TYPE &arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

	inline const StrongRef &operator= (TYPE &&arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

	inline BOOL equal (const StrongRef &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const StrongRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const WeakRef<TYPE> &right) const ;

	inline BOOL operator== (const WeakRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const WeakRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const SoftRef<TYPE> &right) const ;

	inline BOOL operator== (const SoftRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftRef<TYPE> &right) const {
		return !equal (right) ;
	}

private:
	inline explicit StrongRef (const SharedRef<PACK<AnyRef<void> ,LENGTH>> &holder ,PTR<TYPE> pointer) :mHolder (holder) ,mPointer (pointer) {
		if (exist ())
			mHolder->P2++ ;
	}

private:
	inline static void template_enable_shared (const SharedRef<PACK<AnyRef<void> ,LENGTH>> &holder ,PTR<WeakHolder> pointer) {
		pointer->mHolder = holder ;
	}

	inline static void template_enable_shared (const SharedRef<PACK<AnyRef<void> ,LENGTH>> &holder ,PTR<VOID> pointer) {}

public:
	template <class... _ARGS>
	inline static StrongRef make (_ARGS &&...args) {
		const auto r1x = SharedRef<PACK<AnyRef<void> ,LENGTH>>::make () ;
		r1x->P1 = AnyRef<TYPE>::make (std::forward<_ARGS> (args)...) ;
		r1x->P2 = 0 ;
		const auto r2x = &r1x->P1.rebind<TYPE> ().self ;
		template_enable_shared (r1x ,r2x) ;
		return StrongRef (r1x ,r2x) ;
	}
} ;

template <class _ARG>
inline StrongRef<_ARG> WeakHolder::strong_from_this (PTR<_ARG> _this) {
	//@error:actually 'std::is_virtual_base_of' is required
	_STATIC_ASSERT_ (std::is_base_of<WeakHolder ,_ARG>::value) ;
	const auto r1x = dynamic_cast<PTR<WeakHolder>> (_this) ;
	return r1x != NULL ? StrongRef<_ARG> (r1x->mHolder ,_this) : StrongRef<_ARG> () ;
}

template <class TYPE>
class WeakRef {
private:
	SharedRef<PACK<AnyRef<void> ,LENGTH>> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline WeakRef () noexcept {
		mPointer = NULL ;
	}

	inline BOOL exist () const {
		return mPointer != NULL && mHolder.exist () && mHolder->P1.exist () ;
	}

	inline BOOL equal (const WeakRef &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const WeakRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const WeakRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const StrongRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline StrongRef<TYPE> recast () const {
		return StrongRef<TYPE> (mHolder ,mPointer) ;
	}

	inline void assign (const StrongRef<TYPE> &right) {
		mHolder = right.mHolder ;
		mPointer = right.mPointer ;
	}

	inline WeakRef &operator= (const StrongRef<TYPE> &right) {
		assign (right) ;
		return *this ;
	}
} ;

template <class TYPE>
inline implicit StrongRef<TYPE>::StrongRef (const WeakRef<TYPE> &right) :StrongRef (right.recast ()) {}

template <class TYPE>
inline BOOL StrongRef<TYPE>::equal (const WeakRef<TYPE> &right) const {
	return right == *this ;
}

template <class TYPE>
class SoftRef {
private:
	class Pack {
	private:
		friend SoftRef ;
		StrongRef<TYPE> mData ;
		LENGTH mWeight ;
	} ;

	inline static constexpr VAR cvt_log2_var (VAR arg) {
		return arg <= 0 ? VAR_NONE : arg == 1 ? 0 : 1 + cvt_log2_var (arg >> 1) ;
	}

private:
	SharedRef<Allocator<Pack ,SFIXED>> mHeap ;
	WeakRef<TYPE> mWeakRef ;
	INDEX mIndex ;

public:
	inline SoftRef () = default ;

	inline explicit SoftRef (LENGTH len) {
		mHeap = SharedRef<Allocator<Pack ,SFIXED>>::make (len) ;
	}

	inline ~SoftRef () noexcept {
		if (!mHeap.exist () || !mWeakRef.exist () || !mHeap->used (mIndex) || mWeakRef != mHeap.self[mIndex].mData)
			return ;
		_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight >= 0) ;
		mHeap->free (mIndex) ;
	}

	inline SoftRef (const SoftRef &) = default ;

	inline SoftRef &operator= (const SoftRef &right) {
		if (this == &right)
			return *this ;
		this->~SoftRef () ;
		new (this) SoftRef (std::move (right)) ;
		return *this ;
	}

	inline SoftRef (SoftRef &&) noexcept = default ;

	inline SoftRef &operator= (SoftRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~SoftRef () ;
		new (this) SoftRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mHeap.exist () && mWeakRef.exist () && mHeap->used (mIndex) && mWeakRef == mHeap.self[mIndex].mData ;
	}

	inline BOOL equal (const SoftRef &right) const {
		return mWeakRef == right.mWeakRef ;
	}

	inline BOOL operator== (const SoftRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		return mWeakRef == right ;
	}

	inline BOOL operator== (const StrongRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline StrongRef<TYPE> recast () const {
		return mWeakRef.recast () ;
	}

	inline void assign (const StrongRef<TYPE> &right) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (!exist () || mHeap.self[mIndex].mWeight >= 0) ;
		mIndex = right.exist () ? alloc (right) : VAR_NONE ;
		mWeakRef = right.exist () ? mHeap.self[mIndex].mData : WeakRef<TYPE> () ;
	}

	inline SoftRef &operator= (const StrongRef<TYPE> &right) {
		assign (right) ;
		return *this ;
	}

	inline void lock () const {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight >= 0) ;
		if (mHeap.self[mIndex].mWeight >= 0 && mHeap.self[mIndex].mWeight < VAR_MAX)
			mHeap.self[mIndex].mWeight++ ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

	inline void unlock () const {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight < 0) ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

private:
	inline INDEX alloc (const StrongRef<TYPE> &data) const {
		if (mHeap->length () >= mHeap->size ()) {
			INDEX ix = min_weight_one () ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			const auto r1x = cvt_log2_var (mHeap.self[ix].mWeight) ;
			if (r1x > 0) {
				for (INDEX i = 0 ; i < mHeap->size () ; i++)
					mHeap.self[i].mWeight >>= r1x ;
			}
			mHeap->free (ix) ;
		}
		INDEX ret = mHeap->alloc () ;
		mHeap.self[ret].mData = data ;
		mHeap.self[ret].mWeight = 3 ;
		return std::move (ret) ;
	}

	inline INDEX min_weight_one () const {
		_DEBUG_ASSERT_ (mHeap->length () >= mHeap->size ()) ;
		INDEX ret = VAR_NONE ;
		auto rax = LENGTH () ;
		for (INDEX i = 0 ; i < mHeap->size () ; i++) {
			const auto r1x = mHeap.self[i].mWeight ;
			if (r1x < 0)
				continue ;
			if (ret != VAR_NONE && rax <= r1x)
				continue ;
			ret = i ;
			rax = r1x ;
		}
		return std::move (ret) ;
	}
} ;

template <class TYPE>
inline implicit StrongRef<TYPE>::StrongRef (const SoftRef<TYPE> &right) :StrongRef (right.recast ()) {}

template <class TYPE>
inline BOOL StrongRef<TYPE>::equal (const SoftRef<TYPE> &right) const {
	return right == *this ;
}

template <class TYPE>
class IntrusiveRef {
private:
	using INSTRUSIVE_TYPE = typename TYPE::INSTRUSIVE_TYPE ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<TEMP<TYPE>> mPointer ;

public:
	inline IntrusiveRef () noexcept {
		mPointer = NULL ;
	}

	inline ~IntrusiveRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ (_DEDUCE_<Function> (PhanRef<IntrusiveRef>::make (this) ,&IntrusiveRef::release) ,std::nothrow) ;
		mPointer = NULL ;
	}

	inline IntrusiveRef (const IntrusiveRef &right) :IntrusiveRef (right.mPointer ,FALSE) {}

	inline IntrusiveRef &operator= (const IntrusiveRef &right) {
		if (this == &right)
			return *this ;
		this->~IntrusiveRef () ;
		new (this) IntrusiveRef (std::move (right)) ;
		return *this ;
	}

	inline IntrusiveRef (IntrusiveRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline IntrusiveRef &operator= (IntrusiveRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~IntrusiveRef () ;
		new (this) IntrusiveRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return _CAST_<TYPE> (*mPointer) ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	inline PTR<TYPE> operator-> () const {
		return &to () ;
	}

	inline const IntrusiveRef &operator= (const TYPE &arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

	inline const IntrusiveRef &operator= (TYPE &&arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

private:
	inline explicit IntrusiveRef (PTR<TEMP<TYPE>> pointer ,BOOL flag) :mPointer (pointer) {
		if (mPointer == NULL)
			return ;
		if (flag)
			INSTRUSIVE_TYPE::create_counter (_CAST_<TYPE> (*mPointer)) ;
		const auto r1x = INSTRUSIVE_TYPE::increase_counter (_CAST_<TYPE> (*mPointer)) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
	}

	inline void release () {
		const auto r1x = INSTRUSIVE_TYPE::decrease_counter (_CAST_<TYPE> (*mPointer)) ;
		_DEBUG_ASSERT_ (r1x >= 0) ;
		if (r1x > 0)
			return ;
		INSTRUSIVE_TYPE::destroy_counter (_CAST_<TYPE> (*mPointer)) ;
		_DESTROY_ (mPointer) ;
		template_free_counter (mPointer) ;
	}

private:
	template <class _ARG>
	inline static PTR<TEMP<TYPE>> template_alloc_counter (const ARGV<PTR<_ARG>> & ,const ARGV<ENABLE_TYPE<std::is_same<decltype (_ARG::alloc_counter ()) ,PTR<TEMP<TYPE>>>::value && std::is_same<decltype (_ARG::free_counter (_NULL_<PTR<TEMP<TYPE>>> ())) ,void>::value>> & ,const ARGV<ARGC<2>> &) {
		return _ARG::alloc_counter () ;
	}

	template <class _ARG>
	inline static PTR<TEMP<TYPE>> template_alloc_counter (const ARGV<PTR<_ARG>> & ,const ARGV<ENABLE_TYPE<std::is_class<TYPE>::value>> & ,const ARGV<ARGC<1>> &) {
		return new TEMP<TYPE> () ;
	}

	inline static PTR<TEMP<TYPE>> template_alloc_counter () {
		return template_alloc_counter (_NULL_<ARGV<PTR<INSTRUSIVE_TYPE>>> () ,ARGV_VOID ,ARGV9) ;
	}

	template <class _ARG>
	inline static void template_free_counter (PTR<TEMP<TYPE>> adress ,const ARGV<PTR<_ARG>> & ,const ARGV<ENABLE_TYPE<std::is_same<decltype (_ARG::alloc_counter ()) ,PTR<TEMP<TYPE>>>::value && std::is_same<decltype (_ARG::free_counter (_NULL_<PTR<TEMP<TYPE>>> ())) ,void>::value>> & ,const ARGV<ARGC<2>> &) {
		_ARG::free_counter (adress) ;
	}

	template <class _ARG>
	inline static void template_free_counter (PTR<TEMP<TYPE>> adress ,const ARGV<PTR<_ARG>> & ,const ARGV<ENABLE_TYPE<std::is_class<TYPE>::value>> & ,const ARGV<ARGC<1>> &) {
		delete adress ;
	}

	inline static void template_free_counter (PTR<TEMP<TYPE>> adress) {
		template_free_counter (adress ,_NULL_<ARGV<PTR<INSTRUSIVE_TYPE>>> () ,ARGV_VOID ,ARGV9) ;
	}

public:
	template <class... _ARGS>
	inline static IntrusiveRef make (_ARGS &&...args) {
		auto rax = PTR<TEMP<TYPE>> (NULL) ;
		auto rbx = PTR<TEMP<TYPE>> (NULL) ;
		const auto r1x = [&] () noexcept {
			if (rax == NULL)
				return ;
			if (rbx != NULL)
				_DESTROY_ (rbx) ;
			template_free_counter (rax) ;
			rax = NULL ;
			rbx = NULL ;
		} ;
		class Finally :private Wrapped<decltype (r1x)> {
		public:
			inline void lock () const noexcept {}

			inline void unlock () const noexcept {
				Finally::mData () ;
			}
		} ;
		ScopedGuard<const Finally> ANONYMOUS (_CAST_<Finally> (r1x)) ;
		rax = template_alloc_counter () ;
		_DEBUG_ASSERT_ (rax != NULL) ;
		rbx = _CREATE_ (rax ,std::forward<_ARGS> (args)...) ;
		IntrusiveRef ret = IntrusiveRef (rbx ,TRUE) ;
		rbx = NULL ;
		rax = NULL ;
		return std::move (ret) ;
	}
} ;

class MemoryPool {
private:
	class Pool :public Interface {
	public:
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual PTR<VOID> alloc (LENGTH) = 0 ;
		virtual void free (PTR<VOID> adress) noexcept = 0 ;
		virtual void clean () = 0 ;
	} ;

	template <class SIZE>
	struct DATA_LAYOUT {
		alignas (8) PTR<Pool> P1 ;
		alignas (8) PTR<VOID> P2 ;
		alignas (8) DEF<BYTE[SIZE::value]> P3 ;
	} ;

	template <class SIZE>
	class ImplPool :public Pool {
	private:
		struct BLOCK {
			PTR<struct BLOCK> next ;
			DATA_LAYOUT<SIZE> data ;
		} ;

		inline static constexpr VAL64 cvt_pow_var (VAL64 base ,LENGTH power) {
			return power % 2 != 0 ? base * cvt_pow_var (base ,power - 1) : power != 0 ? _SQE_ (cvt_pow_var (base ,power / 2)) : 1 ;
		}

		struct CHUNK {
			PTR<struct CHUNK> prev ;
			PTR<struct CHUNK> next ;
			LENGTH count ;
			DEF<BLOCK[1]> list ;
		} ;

	private:
		PTR<CHUNK> mFirst ;
		PTR<BLOCK> mFree ;
		LENGTH mSize ;
		LENGTH mLength ;

	public:
		inline ImplPool () {
			mFirst = NULL ;
			mFree = NULL ;
			mSize = 0 ;
			mLength = 0 ;
		}

		inline ~ImplPool () noexcept override {
			clear () ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mLength ;
		}

		inline PTR<VOID> alloc (LENGTH len) override {
			_DEBUG_ASSERT_ (len <= SIZE::value) ;
			const auto r1x = LENGTH (cvt_pow_var (VAL64 (1.25) ,_MAX_ (VAR (1) ,16 - _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) / 8)) + VAL64 (0.5)) ;
			reserve (r1x) ;
			auto &r1 = *mFree ;
			mFree = mFree->next ;
			mLength += _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) ;
			r1.next = _CAST_<PACK<PTR<BLOCK>>> (VAR_USED).P1 ;
			return &r1.data ;
		}

		inline void free (PTR<VOID> adress) noexcept override {
			auto &r1 = _OFFSET_ (&BLOCK::data ,&_LOAD_<decltype (_NULL_<BLOCK> ().BLOCK::data)> (adress ,_ADRESS_ (adress))) ;
			_DEBUG_ASSERT_ (_ADRESS_ (r1.next) == VAR_USED) ;
			r1.next = mFree ;
			mFree = &r1 ;
			mLength -= _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) ;
		}

		inline void clean () override {
			if (mSize == mLength)
				return ;
			for (PTR<CHUNK> i = mFirst ,ir ; i != NULL ; i = ir) {
				auto &r1 = *i ;
				ir = r1.next ;
				if (!empty_node (i))
					continue ;
				(r1.prev != NULL ? r1.prev->next : mFirst) = r1.next ;
				if (r1.next != NULL)
					r1.next->prev = r1.prev ;
				mSize -= r1.count * _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) ;
				delete[] _XVALUE_<const PTR<BYTE> &> (_CAST_<BYTE[_SIZEOF_ (CHUNK)]> (*i)) ;
			}
		}

	private:
		inline void reserve (LENGTH len) {
			if (mFree != NULL)
				return ;
			const auto r1x = _SIZEOF_ (CHUNK) - _SIZEOF_ (_NULL_<CHUNK> ().CHUNK::list) + len * _SIZEOF_ (BLOCK) ;
			ScopedHolder<ARR<BYTE>> sgd (&_LOAD_<ARR<BYTE>> (new BYTE[r1x])) ;
			const auto r2x = _XVALUE_<const PTR<ARR<BYTE>> &> (sgd) ;
			auto &r1 = _LOAD_<CHUNK> (r2x ,_ADRESS_ (r2x)) ;
			r1.prev = NULL ;
			r1.next = mFirst ;
			r1.count = len ;
			if (mFirst != NULL)
				mFirst->prev = &r1 ;
			mFirst = &r1 ;
			mSize += len * _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) ;
			for (auto &i : mFirst->list) {
				i.next = mFree ;
				mFree = &i ;
			}
			sgd = NULL ;
		}

		inline void clear () noexcept {
			for (PTR<CHUNK> i = mFirst ,ir ; i != NULL ; i = ir) {
				ir = i->next ;
				delete[] _XVALUE_<const PTR<BYTE> &> (_CAST_<BYTE[_SIZEOF_ (CHUNK)]> (*i)) ;
			}
			mFirst = NULL ;
			mFree = NULL ;
			mSize = 0 ;
			mLength = 0 ;
		}

		inline BOOL empty_node (PTR<CHUNK> node) const {
			for (INDEX i = 0 ; i < _COUNTOF_ (_NULL_<CHUNK> ().CHUNK::list) ; i++)
				if (_ADRESS_ (node->list[i].next) == VAR_USED)
					return FALSE ;
			return TRUE ;
		}
	} ;

	class HugePool :public Pool {
	private:
		struct BLOCK {
			PTR<struct BLOCK> prev ;
			PTR<struct BLOCK> next ;
			LENGTH count ;
			DATA_LAYOUT<ARGC<1>> data ;
		} ;

	private:
		PTR<BLOCK> mFirst ;
		LENGTH mSize ;

	public:
		inline HugePool () {
			mFirst = NULL ;
			mSize = 0 ;
		}

		inline ~HugePool () noexcept override {
			clear () ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mSize ;
		}

		inline PTR<VOID> alloc (LENGTH len) override {
			const auto r1x = _SIZEOF_ (BLOCK) - _SIZEOF_ (_NULL_<BLOCK> ().BLOCK::data.P3) + len ;
			ScopedHolder<ARR<BYTE>> sgd (&_LOAD_<ARR<BYTE>> (new BYTE[r1x])) ;
			const auto r2x = _XVALUE_<const PTR<ARR<BYTE>> &> (sgd) ;
			auto &r1 = _LOAD_<BLOCK> (r2x ,_ADRESS_ (r2x)) ;
			r1.prev = NULL ;
			r1.next = mFirst ;
			r1.count = len ;
			if (mFirst != NULL)
				mFirst->prev = &r1 ;
			mFirst = &r1 ;
			mSize += len ;
			sgd = NULL ;
			return &r1.data ;
		}

		inline void free (PTR<VOID> adress) noexcept override {
			auto &r1 = _OFFSET_ (&BLOCK::data ,&_LOAD_<decltype (_NULL_<BLOCK> ().BLOCK::data)> (adress ,_ADRESS_ (adress))) ;
			(r1.prev != NULL ? r1.prev->next : mFirst) = r1.next ;
			if (r1.next != NULL)
				r1.next->prev = r1.prev ;
			mSize -= r1.count ;
			delete[] _XVALUE_<const PTR<BYTE> &> (_CAST_<BYTE[_SIZEOF_ (BLOCK)]> (r1)) ;
		}

		inline void clean () override {}

	private:
		inline void clear () noexcept {
			for (PTR<BLOCK> i = mFirst ,ir ; i != NULL ; i = ir) {
				ir = i->next ;
				delete[] _XVALUE_<const PTR<BYTE> &> (_CAST_<BYTE[_SIZEOF_ (BLOCK)]> (*i)) ;
			}
			mFirst = NULL ;
			mSize = 0 ;
		}
	} ;

private:
	ImplPool<ARGC<8>> mPool1 ;
	ImplPool<ARGC<16>> mPool2 ;
	ImplPool<ARGC<24>> mPool3 ;
	ImplPool<ARGC<32>> mPool4 ;
	ImplPool<ARGC<40>> mPool5 ;
	ImplPool<ARGC<48>> mPool6 ;
	ImplPool<ARGC<56>> mPool7 ;
	ImplPool<ARGC<64>> mPool8 ;
	ImplPool<ARGC<72>> mPool9 ;
	ImplPool<ARGC<80>> mPool10 ;
	ImplPool<ARGC<88>> mPool11 ;
	ImplPool<ARGC<96>> mPool12 ;
	ImplPool<ARGC<104>> mPool13 ;
	ImplPool<ARGC<112>> mPool14 ;
	ImplPool<ARGC<120>> mPool15 ;
	ImplPool<ARGC<128>> mPool16 ;
	HugePool mPool17 ;
	Buffer<PTR<Pool> ,ARGC<17>> mPool ;

public:
	inline MemoryPool () {
		mPool = DEF<PTR<Pool>[17]> {
			&mPool1 ,&mPool2 ,&mPool3 ,&mPool4 ,&mPool5 ,&mPool6 ,&mPool7 ,&mPool8 ,
			&mPool9 ,&mPool10 ,&mPool11 ,&mPool12 ,&mPool13 ,&mPool14 ,&mPool15 ,&mPool16 ,
			&mPool17} ;
	}

	inline LENGTH size () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			ret += mPool[i]->size () ;
		return std::move (ret) ;
	}

	inline LENGTH length () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			ret += mPool[i]->length () ;
		return std::move (ret) ;
	}

	//@error:held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<_RET> alloc () popping {
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_SIZEOF_ (_RET) % _ALIGNOF_ (_RET) == 0) ;
		const auto r1x = _SIZEOF_ (_RET) + _MAX_ (_ALIGNOF_ (_RET) - 8 ,VAR_ZERO) ;
		INDEX ix = _MIN_ ((r1x - 1) / 8 ,LENGTH (16)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = ((_ADRESS_ (r2x) + 16) / _ALIGNOF_ (_RET) + EFLAG ((_ADRESS_ (r2x) + 16) % _ALIGNOF_ (_RET) != 0)) * _ALIGNOF_ (_RET) - 16 ;
		auto &r1 = _LOAD_<DATA_LAYOUT<ARGC<1>>> (this ,r3x) ;
		r1.P1 = mPool[ix] ;
		r1.P2 = r2x ;
		return &_LOAD_<_RET> (this ,_ADRESS_ (&r1.P3[0])) ;
	}

	//@error:held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_SIZEOF_ (_RET) % _ALIGNOF_ (_RET) == 0) ;
		const auto r1x = len * _SIZEOF_ (_RET) + _MAX_ (_ALIGNOF_ (_RET) - 8 ,VAR_ZERO) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		INDEX ix = _MIN_ ((r1x - 1) / 8 ,LENGTH (16)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = ((_ADRESS_ (r2x) + 16) / _ALIGNOF_ (_RET) + EFLAG ((_ADRESS_ (r2x) + 16) % _ALIGNOF_ (_RET) != 0)) * _ALIGNOF_ (_RET) - 16 ;
		auto &r1 = _LOAD_<DATA_LAYOUT<ARGC<1>>> (this ,r3x) ;
		r1.P1 = mPool[ix] ;
		r1.P2 = r2x ;
		return &_LOAD_<ARR<_RET>> (this ,_ADRESS_ (&r1.P3[0])) ;
	}

	template <class _ARG>
	inline void free (PTR<_ARG> adress) noexcept {
		_STATIC_ASSERT_ (std::is_pod<REMOVE_EXTEND_TYPE<_ARG>>::value) ;
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_EXTEND_TYPE<_ARG>) % _ALIGNOF_ (REMOVE_EXTEND_TYPE<_ARG>) == 0) ;
		const auto r1x = &_LOAD_<decltype (DATA_LAYOUT<ARGC<1>>::P3)> (this ,_ADRESS_ (adress)) ;
		auto &r1 = _OFFSET_ (&DATA_LAYOUT<ARGC<1>>::P3 ,r1x) ;
		INDEX ix = _MEMCHR_ (mPool.self ,mPool.size () ,r1.P1) ;
		_DEBUG_ASSERT_ (ix != VAR_NONE) ;
		mPool[ix]->free (r1.P2) ;
	}

	inline void clean () {
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			mPool[i]->clean () ;
	}
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> :private Wrapped<void> {
private:
	class Node {
	private:
		template <class>
		friend class GlobalStatic ;
		VAR mData ;
		FLAG mUUID ;
		PTR<class Node> mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (FLAG uuid ,PTR<class Node> right) :mUUID (uuid) ,mRight (right) {}
	} ;

	class Pack {
	private:
		template <class>
		friend class GlobalStatic ;
		AutoRef<std::atomic<LENGTH>> mCounter ;
		SharedRef<std::mutex> mNodeMutex ;
		PTR<Node> mLast ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;

private:
	static LENGTH unique_atomic_value (LENGTH exp ,LENGTH val) noexcept popping ;

	inline static PTR<Pack> unique () {
		static const UniqueRef<PTR<Pack>> mUniquePack ([] (PTR<Pack> &me) {
			ScopedHolder<Pack> sgd (new Pack) ;
			const auto r1x = _XVALUE_<const PTR<Pack> &> (sgd) ;
			r1x->mCounter = AutoRef<std::atomic<LENGTH>>::make (0) ;
			r1x->mNodeMutex = SharedRef<std::mutex>::make () ;
			r1x->mLast = NULL ;
			const auto r2x = unique_atomic_value (0 ,_ADRESS_ (r1x)) ;
			me = &_LOAD_<Pack> (r1x ,r2x) ;
			if (me != NULL)
				++me->mCounter.self ;
			if (me == r1x)
				sgd = NULL ;
		} ,[] (PTR<Pack> &me) {
			if (me == NULL)
				return ;
			const auto r1x = --me->mCounter.self == 0 ;
			if (r1x) {
				const auto r2x = _ADRESS_ (me) ;
				const auto r3x = unique_atomic_value (r2x ,0) ;
				(void) r3x ;
				_DEBUG_ASSERT_ (r3x == r2x) ;
				clear_node (me ,me->mNodeMutex) ;
				delete me ;
			}
			me = NULL ;
		}) ;
		return mUniquePack.self ;
	}

	inline static PTR<Node> new_node (PTR<Pack> _this ,FLAG uuid) popping {
		ScopedHolder<Node> sgd (new Node (uuid ,_this->mLast)) ;
		_this->mLast = _XVALUE_<const PTR<Node> &> (sgd) ;
		sgd = NULL ;
		return _this->mLast ;
	}

	inline static PTR<Node> find_node (PTR<Pack> _this ,FLAG uuid) {
		for (PTR<Node> i = _this->mLast ; i != NULL ; i = i->mRight)
			if (i->mUUID == uuid)
				return i ;
		return NULL ;
	}

	inline static void clear_node (PTR<Pack> _this ,std::mutex &lock) {
		_DEBUG_ASSERT_ (&lock == &_this->mNodeMutex.self) ;
		ScopedGuard<std::mutex> ANONYMOUS (lock) ;
		for (PTR<Node> i = _this->mLast ,ir ; i != NULL ; i = ir) {
			ir = i->mRight ;
			delete i ;
		}
	}
} ;

template <FLAG UUID>
class GlobalStatic<ARGC<UUID>> :private Wrapped<void> {
public:
	inline static void init (VAR val) {
		const auto r1x = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x->mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::find_node (r1x ,UUID) ;
		if (r2x != NULL)
			return ;
		const auto r3x = GlobalStatic<void>::new_node (r1x ,UUID) ;
		_DYNAMIC_ASSERT_ (r3x != NULL) ;
		r3x->mData = val ;
	}

	inline static VAR load () {
		const auto r1x = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x->mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::find_node (r1x ,UUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return r2x->mData ;
	}

	inline static VAR compare_and_save (VAR exp ,VAR val) popping {
		const auto r1x = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x->mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::find_node (r1x ,UUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		if (r2x->mData == exp)
			r2x->mData = val ;
		return r2x->mData ;
	}

	inline static void save (VAR val) {
		const auto r1x = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x->mNodeMutex) ;
		const auto r2x = _CALL_ ([&] () {
			PTR<GlobalStatic<void>::Node> ret = GlobalStatic<void>::find_node (r1x ,UUID) ;
			if (ret == NULL)
				ret = GlobalStatic<void>::new_node (r1x ,UUID) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		r2x->mData = val ;
	}
} ;

class Object ;

class Objective :public Interface {
public:
	virtual StrongRef<Object> clone () const = 0 ;
} ;

class Object :public virtual WeakHolder ,public Objective {
private:
	class MetaData {
	private:
		friend Object ;
		LENGTH mTempSize ;
		LENGTH mTempAlign ;
		PTR<const std::type_info> mTypeInfo ;
		Function<void (PTR<NONE>)> mConstrutor ;
		Function<void (PTR<NONE> ,const AnyRef<void> &)> mInitConstructor ;
		Function<void (PTR<NONE>)> mDestructor ;

	private:
		inline MetaData () = delete ;

		template <class _ARG>
		inline explicit MetaData (const ARGV<_ARG> &) {
			mTempSize = _SIZEOF_ (_ARG) ;
			mTempAlign = _ALIGNOF_ (_ARG) ;
			mTypeInfo = &typeid (_ARG) ;
			mConstrutor = Function<void (PTR<NONE>)> ([] (PTR<NONE> adress) {
				_CREATE_ (&_LOAD_<TEMP<_ARG>> (adress)) ;
			}) ;
			mInitConstructor = template_init_initconstructor (_NULL_<ARGV<ARGC<std::is_constructible<_ARG ,const AnyRef<void> &>::value>>> () ,_NULL_<ARGV<_ARG>> ()) ;
			mDestructor = Function<void (PTR<NONE>)> ([] (PTR<NONE> adress) {
				_DESTROY_ (&_LOAD_<TEMP<_ARG>> (adress)) ;
			}) ;
		}

		template <class _ARG>
		inline static Function<void (PTR<NONE> ,const AnyRef<void> &)> template_init_initconstructor (const ARGV<ARGC<TRUE>> & ,const ARGV<_ARG> &) {
			return Function<void (PTR<NONE> ,const AnyRef<void> &)> ([] (PTR<NONE> adress ,const AnyRef<void> &param) {
				_CREATE_ (&_LOAD_<TEMP<_ARG>> (adress) ,param) ;
			}) ;
		}

		template <class _ARG>
		inline static Function<void (PTR<NONE> ,const AnyRef<void> &)> template_init_initconstructor (const ARGV<ARGC<FALSE>> & ,const ARGV<_ARG> &) {
			return Function<void (PTR<NONE> ,const AnyRef<void> &)> () ;
		}
	} ;

public:
	inline Object () = delete ;

	template <class _ARG ,class = ENABLE_TYPE<std::is_base_of<Object ,_ARG>::value>>
	inline explicit Object (PTR<_ARG> deriver) {
		_STATIC_ASSERT_ (!std::is_same<REMOVE_CVR_TYPE<_ARG> ,Object>::value) ;
		_DEBUG_ASSERT_ (_ADRESS_ (this) == _ADRESS_ (deriver)) ;
	}

	inline StrongRef<Object> clone () const override {
		return StrongRef<Object> () ;
	}
} ;
} ;