#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"

namespace CSC {
namespace U {
struct OPERATOR_TYPENAME {
	struct TYPENAME {
		String<STR> mName ;
	} ;

	template <class _RET>
	inline static TYPENAME typeid_name_from_func () {
		TYPENAME ret ;
		ret.mName = _PARSESTRS_ (String<STRA> (M_FUNC)) ;
#ifdef __CSC_COMPILER_MSVC__
		static constexpr auto M_PREFIX = _PCSTR_ ("struct CSC::U::OPERATOR_TYPENAME::TYPENAME __cdecl CSC::U::OPERATOR_TYPENAME::typeid_name_from_func<") ;
		static constexpr auto M_SUFFIX = _PCSTR_ (">(void)") ;
		const auto r1x = M_PREFIX.size () ;
		const auto r2x = M_SUFFIX.size () ;
		const auto r3x = ret.mName.length () - r1x - r2x ;
		_DYNAMIC_ASSERT_ (r3x > 0) ;
		ret.mName = ret.mName.segment (r1x ,r3x) ;
#elif defined __CSC_COMPILER_GNUC__
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [with _RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		const auto r4x = M_PREFIX.size () ;
		const auto r5x = M_SUFFIX.size () ;
		const auto r6x = ret.mName.length () - r4x - r5x ;
		_DYNAMIC_ASSERT_ (r6x > 0) ;
		ret.mName = ret.mName.segment (r4x ,r6x) ;
#elif defined __CSC_COMPILER_CLANG__
		static constexpr auto M_PREFIX = _PCSTR_ ("static CSC::U::OPERATOR_TYPENAME::TYPENAME CSC::U::OPERATOR_TYPENAME::typeid_name_from_func() [_RET = ") ;
		static constexpr auto M_SUFFIX = _PCSTR_ ("]") ;
		const auto r7x = M_PREFIX.size () ;
		const auto r8x = M_SUFFIX.size () ;
		const auto r9x = ret.mName.length () - r7x - r8x ;
		_DYNAMIC_ASSERT_ (r9x > 0) ;
		ret.mName = ret.mName.segment (r7x ,r9x) ;
#else
		ret.mName = _BUILDVAR64S_ (_TYPEMID_<_RET> ()) ;
#endif
		return std::move (ret) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<const _ARG1> &) {
		writer << _PCSTR_ ("const ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<volatile _ARG1> &) {
		writer << _PCSTR_ ("volatile ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_cv (TextWriter<STR> &writer ,const ARGV<const volatile _ARG1> &) {
		writer << _PCSTR_ ("const volatile ") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1 &> &) {
		writer << _PCSTR_ (" &") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_ref (TextWriter<STR> &writer ,const ARGV<_ARG1 &&> &) {
		writer << _PCSTR_ (" &&") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP8)> &) {
		writer << _PCSTR_ ("PTR<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_POINTER_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_member_pointer<_ARG1>::value>> & ,const DEF<decltype (ARGVP7)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_MEMPTR_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" ") ;
		template_write_typename_x (writer ,_NULL_<ARGV<MEMPTR_CLASS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("::*>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_arr (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,ARR<REMOVE_ARRAY_TYPE<_ARG1>>>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("ARR<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_arr (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_bounded_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_array<_ARG1>::value>> & ,const DEF<decltype (ARGVP6)> &) {
		template_write_typename_arr (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_func (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<VOID> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_function<_ARG1>::value>> & ,const DEF<decltype (ARGVP5)> &) {
		template_write_typename_func (writer ,_NULL_<ARGV<REMOVE_FUNCATTR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_claz (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<!stl::is_template<_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("class '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_claz (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_template<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("template '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'<") ;
		template_write_typename_y (writer ,_NULL_<ARGV<TEMPLATE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (">") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_class<_ARG1>::value>> & ,const DEF<decltype (ARGVP4)> &) {
		template_write_typename_claz (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU8>::value && !std::is_same<_ARG1 ,BYTE>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU16>::value && !std::is_same<_ARG1 ,WORD>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU32>::value && !std::is_same<_ARG1 ,CHAR>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARX>::value && !std::is_same<_ARG1 ,VAR32>::value && !std::is_same<_ARG1 ,VAR64>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VARX") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARY>::value && !std::is_same<_ARG1 ,CHAR>::value && !std::is_same<_ARG1 ,DATA>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VARY") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VALX>::value && !std::is_same<_ARG1 ,VAL32>::value && !std::is_same<_ARG1 ,VAL64>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		writer << _PCSTR_ ("VALX") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BOOL> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAR32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<EFLAG> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL64> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VOID> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VOID") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<NONE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("NONE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<decltype (NULL)> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("NULL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BYTE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<WORD> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("WORD") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<CHAR> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<DATA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("DATA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<MEGA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRW> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRW") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		const auto r1x = typeid_name_from_func<_ARG1> () ;
		writer << _PCSTR_ ("typename '") ;
		writer << r1x.mName ;
		writer << _PCSTR_ ("'") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_x (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		template_write_typename_cv (writer ,_NULL_<ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>> ()) ;
		template_write_typename_id (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
		template_write_typename_ref (writer ,_NULL_<ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>> ()) ;
	}

	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1>> &) {
		template_write_typename_x (writer ,_NULL_<ARGV<_ARG1>> ()) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		template_write_typename_x (writer ,_NULL_<ARGV<ONE_HINT>> ()) ;
		writer << _PCSTR_ (" ,") ;
		template_write_typename_y (writer ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	template <class _RET>
	inline static String<STR> invoke () {
		const auto r1x = _XVALUE_<PTR<void (TextWriter<STR> &)>> ([] (TextWriter<STR> &writer) {
			template_write_typename_x (writer ,_NULL_<ARGV<_RET>> ()) ;
		}) ;
		return String<STR>::make (r1x) ;
	}
} ;
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> final
	:private Wrapped<void> {
private:
	struct VALUE_NODE {
		FLAG mGUID ;
		BOOL mReadOnly ;
		VAR mValue ;
	} ;

	struct CLASS_NODE {
		String<STR> mGUID ;
		PTR<NONE> mValue ;
	} ;

	class Pack {
	private:
		template <class>
		friend class GlobalStatic ;
		friend IntrusiveRef<Pack> ;
		using INTRUSIVE_THIS = GlobalStatic ;
		std::atomic<LENGTH> mCounter ;
		Monostate<std::mutex> mNodeMutex ;
		Deque<VALUE_NODE> mValueList ;
		HashSet<FLAG> mValueMappingSet ;
		Deque<CLASS_NODE> mClassList ;
		HashSet<FLAG> mClassMappingSet ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Pack> ;

private:
	static Pack &static_unique () popping {
		return _CACHE_ ([&] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Pack> () ;
			if switch_case (TRUE) {
				if (rax != NULL)
					discard ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack>::make () ;
				const auto r1x = rbx.watch () ;
				auto &r2x = _XVALUE_<Pack> (r1x) ;
				auto &r3x = _LOAD_<NONE> (&r2x) ;
				rax = unique_atomic_address (NULL ,&r3x) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			auto &r4x = _LOAD_<Pack> (rax) ;
			return IntrusiveRef<Pack> (&r4x).watch () ;
		}) ;
	}

	static PTR<VALUE_NODE> static_new_node (Pack &self_ ,FLAG guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mValueList.insert () ;
			self_.mValueMappingSet.add (r1x ,ix) ;
			self_.mValueList[ix].mGUID = guid ;
		}
		return &self_.mValueList[ix] ;
	}

	static FLAG node_guid_hash (FLAG guid) {
		return guid ;
	}

	static PTR<VALUE_NODE> static_find_node (Pack &self_ ,FLAG guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return &self_.mValueList[ix] ;
	}

	static PTR<CLASS_NODE> static_new_node (Pack &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = self_.mClassList.insert () ;
			self_.mClassMappingSet.add (r1x ,ix) ;
			self_.mClassList[ix].mGUID = guid ;
		}
		return &self_.mClassList[ix] ;
	}

	static FLAG node_guid_hash (const String<STR> &guid) {
		const auto r1x = guid.raw () ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		return _MEMHASH_ (r2x.self ,r2x.size ()) ;
	}

	static PTR<CLASS_NODE> static_find_node (Pack &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassMappingSet.map (r1x) ;
		if (ix == VAR_NONE)
			return NULL ;
		return &self_.mClassList[ix] ;
	}

public:
	//@warn: this function should be implemented in a 'runtime.dll'
	static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

private:
	static void friend_create (Pack &self_) {
		ScopedGuard<std::mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mCounter = 0 ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	static void friend_destroy (Pack &self_) {
		ScopedGuard<std::mutex> ANONYMOUS (self_.mNodeMutex) ;
		self_.mValueList = Deque<VALUE_NODE> () ;
		self_.mClassList = Deque<CLASS_NODE> () ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_sleep () ;
	}
} ;

template <class GUID>
class GlobalStatic final
	:private Wrapped<void> {
	_STATIC_ASSERT_ (GUID::value > 0) ;

public:
	static void init (VAR data) {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		if (r2x != NULL)
			return ;
		const auto r3x = GlobalStatic<void>::static_new_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r3x != NULL) ;
		r3x->mReadOnly = TRUE ;
		r3x->mValue = data ;
	}

	static VAR load () popping {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return r2x->mValue ;
	}

	static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		_DYNAMIC_ASSERT_ (!r2x->mReadOnly) ;
		if (r2x->mValue == expect)
			r2x->mValue = data ;
		return r2x->mValue ;
	}

	static void save (VAR data) {
		auto &r1x = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1x.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1x ,GUID::value) ;
		if switch_case (TRUE) {
			if (rax != NULL)
				discard ;
			rax = GlobalStatic<void>::static_new_node (r1x ,GUID::value) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mValue = data ;
	}
} ;

template <class UNIT>
class GlobalStatic<Singleton<UNIT>> final
	:private Wrapped<void> {
private:
	class Pack {
	private:
		friend GlobalStatic ;
		friend IntrusiveRef<Pack> ;
		using INTRUSIVE_THIS = GlobalStatic ;
		std::atomic<LENGTH> mCounter ;
		Singleton<UNIT> mValue ;
	} ;

private:
	friend IntrusiveRef<Pack> ;

public:
	static Singleton<UNIT> &unique () popping {
		auto &r1x = _CACHE_ ([&] () {
			auto &r2x = GlobalStatic<void>::static_unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2x.mNodeMutex) ;
			const auto r3x = U::OPERATOR_TYPENAME::invoke<Singleton<UNIT>> () ;
			auto rax = GlobalStatic<void>::static_find_node (r2x ,r3x) ;
			auto rbx = IntrusiveRef<Pack> () ;
			if switch_case (TRUE) {
				if (rax != NULL)
					discard ;
				rax = GlobalStatic<void>::static_new_node (r2x ,r3x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack>::make () ;
				const auto r4x = rbx.watch () ;
				auto &r5x = _XVALUE_<Pack> (r4x) ;
				auto &r6x = _LOAD_<NONE> (&r5x) ;
				rax->mValue = &r6x ;
			}
			auto &r7x = _LOAD_<Pack> (rax->mValue) ;
			return IntrusiveRef<Pack> (&r7x).watch () ;
		}) ;
		return _XVALUE_<Pack> (r1x).mValue ;
	}

private:
	static void friend_create (Pack &self_) {
		self_.mCounter = 0 ;
	}

	static void friend_destroy (Pack &self_) {
		_STATIC_WARNING_ ("noop") ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_sleep () ;
	}
} ;

#ifdef __CSC_DEPRECATED__
template <class CONT>
class Coroutine {
public:
	class SubRef ;

private:
	static constexpr auto STATE_CREATED = EFLAG (1) ;
	static constexpr auto STATE_RUNNING = EFLAG (2) ;
	static constexpr auto STATE_SUSPEND = EFLAG (3) ;
	static constexpr auto STATE_STOPPED = EFLAG (4) ;

	class Pack {
	private:
		friend Coroutine ;
		friend IntrusiveRef<Pack> ;
		using INTRUSIVE_THIS = Coroutine ;
		std::atomic<LENGTH> mCounter ;
		EFLAG mState ;
		AutoRef<CONT> mContext ;
		AnyRef<void> mBreakPoint ;
		Array<Function<DEF<void (SubRef &)> NONE::*>> mSubProc ;
		Array<AnyRef<void>> mSubBreakPoint ;
		Deque<INDEX> mSubQueue ;
		Priority<FLAG> mSubAwaitQueue ;
		INDEX mSubCurr ;
	} ;

private:
	class Implement ;
	IntrusiveRef<Pack> mThis ;

public:
	Coroutine () {
		mThis = IntrusiveRef<Pack>::make () ;
	}

	BOOL ready () const {
		if (mThis->mState != STATE_STOPPED)
			return FALSE ;
		return TRUE ;
	}

	CONT &context () & {
		_DEBUG_ASSERT_ (mThis->mContext.exist ()) ;
		return mThis->mContext ;
	}

	auto context () && ->void = delete ;

	void start (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		mThis->mContext = AutoRef<CONT>::make () ;
		mThis->mBreakPoint = AnyRef<void> () ;
		mThis->mSubProc = Array<Function<DEF<void (SubRef &)> NONE::*>> (proc.length ()) ;
		for (auto &&i : _RANGE_ (0 ,proc.length ())) {
			_DEBUG_ASSERT_ (proc[i].exist ()) ;
			mThis->mSubProc[i] = std::move (proc[i]) ;
		}
		mThis->mSubBreakPoint = Array<AnyRef<void>> (mThis->mSubProc.length ()) ;
		mThis->mSubQueue = Deque<INDEX> (mThis->mSubProc.length ()) ;
		for (auto &&i : mThis->mSubProc.range ())
			mThis->mSubQueue.add (i) ;
		mThis->mSubAwaitQueue = Priority<FLAG> (mThis->mSubProc.length ()) ;
		mThis->mSubQueue.take (mThis->mSubCurr) ;
	}

	void execute () {
		_DEBUG_ASSERT_ (!mThis->mBreakPoint.exist ()) ;
		init_break_point (mThis->mBreakPoint) ;
		for (auto &&i : mThis->mSubBreakPoint) {
			_DEBUG_ASSERT_ (!i.exist ()) ;
			init_break_point (i) ;
		}
		mThis->mState = STATE_CREATED ;
		store_break_point (mThis->mBreakPoint) ;
		for (auto &&i : mThis->mSubBreakPoint) {
			if (mThis->mState != STATE_STOPPED)
				continue ;
			if (!i.exist ())
				continue ;
			goto_break_point (i) ;
		}
		if (mThis->mState != STATE_CREATED)
			return ;
		for (auto &&i : mThis->mSubBreakPoint) {
			if (mThis->mState != STATE_CREATED)
				continue ;
			store_break_point (i) ;
		}
		const auto r1x = mThis->mSubCurr ;
		_DEBUG_ASSERT_ (r1x != VAR_NONE) ;
		_CALL_TRY_ ([&] () {
			if (mThis->mState == STATE_STOPPED)
				return ;
			mThis->mState = STATE_RUNNING ;
			mThis->mSubProc[r1x] (_CAST_<SubRef> ((*this))) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
		mThis->mSubBreakPoint[r1x] = AnyRef<void> () ;
		mThis->mState = STATE_STOPPED ;
		goto_break_point (mThis->mBreakPoint) ;
	}

private:
	void init_break_point (AnyRef<void> &bp) ;

	void store_break_point (AnyRef<void> &bp) noexcept ;

	void goto_break_point (AnyRef<void> &bp) noexcept ;

private:
	static void friend_create (Pack &self_) {
		self_.mState = STATE_CREATED ;
		self_.mSubCurr = VAR_NONE ;
	}

	static void friend_destroy (Pack &self_) {
		_STATIC_WARNING_ ("noop") ;
	}

	static LENGTH friend_attach (Pack &self_) popping {
		return ++self_.mCounter ;
	}

	static LENGTH friend_detach (Pack &self_) popping {
		return --self_.mCounter ;
	}

	static void friend_latch (Pack &self_) {
		GlobalRuntime::thread_sleep () ;
	}

public:
	static CONT csync (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
		auto rax = Coroutine<CONT> (std::move (proc)) ;
		rax.execute () ;
		return std::move (rax.context ()) ;
	}
} ;

template <class CONT>
class Coroutine<CONT>::SubRef
	:private Wrapped<Coroutine<CONT>> {
private:
	using Wrapped<Coroutine<CONT>>::mSelf ;

public:
	CONT &to () {
		_DEBUG_ASSERT_ (mSelf.mThis->mContext.exist ()) ;
		return mSelf.mThis->mContext ;
	}

	inline implicit operator CONT & () {
		return to () ;
	}

	inline PTR<CONT> operator-> () {
		return &to () ;
	}

	const CONT &to () const {
		_DEBUG_ASSERT_ (mSelf.mThis->mContext.exist ()) ;
		return mSelf.mThis->mContext ;
	}

	inline implicit operator const CONT & () const {
		return to () ;
	}

	inline PTR<const CONT> operator-> () const {
		return &to () ;
	}

	void sub_await () {
		sub_await (mSelf.mThis->mSubAwaitQueue.length ()) ;
	}

	void sub_await (FLAG priority) {
		_DEBUG_ASSERT_ (priority >= 0) ;
		_DEBUG_ASSERT_ (mSelf.mThis->mSubCurr != VAR_NONE) ;
		_DYNAMIC_ASSERT_ (mSelf.mThis->mState == STATE_RUNNING) ;
		mSelf.mThis->mState = STATE_SUSPEND ;
		store_break_point (mSelf.mThis->mSubBreakPoint[mSelf.mThis->mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mThis->mState != STATE_STOPPED) ;
		if (mSelf.mThis->mState != STATE_SUSPEND)
			return ;
		mSelf.mThis->mSubAwaitQueue.add (priority ,mSelf.mThis->mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mThis->mSubQueue.empty ()) ;
		mSelf.mThis->mSubQueue.take (mSelf.mThis->mSubCurr) ;
		mSelf.mThis->mState = STATE_RUNNING ;
		goto_break_point (mSelf.mThis->mSubBreakPoint[mSelf.mThis->mSubCurr]) ;
	}

	void sub_resume () {
		sub_resume (mSelf.mThis->mSubAwaitQueue.length ()) ;
	}

	void sub_resume (LENGTH count) {
		for (auto &&i : _RANGE_ (0 ,count)) {
			if (mSelf.mThis->mSubAwaitQueue.empty ())
				continue ;
			const auto r1x = mSelf.mThis->mSubAwaitQueue[mSelf.mThis->mSubAwaitQueue.head ()].mapx ;
			mSelf.mThis->mSubAwaitQueue.take () ;
			mSelf.mThis->mSubQueue.add (r1x) ;
		}
	}

	void sub_yield () {
		_DEBUG_ASSERT_ (mSelf.mThis->mSubCurr != VAR_NONE) ;
		if (mSelf.mThis->mSubQueue.empty ())
			return ;
		_DYNAMIC_ASSERT_ (mSelf.mThis->mState == STATE_RUNNING) ;
		mSelf.mThis->mState = STATE_SUSPEND ;
		store_break_point (mSelf.mThis->mSubBreakPoint[mSelf.mThis->mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mThis->mState != STATE_STOPPED) ;
		if (mSelf.mThis->mState != STATE_SUSPEND)
			return ;
		mSelf.mThis->mSubQueue.add (mSelf.mThis->mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mThis->mSubQueue.empty ()) ;
		mSelf.mThis->mSubQueue.take (mSelf.mThis->mSubCurr) ;
		mSelf.mThis->mState = STATE_RUNNING ;
		goto_break_point (mSelf.mThis->mSubBreakPoint[mSelf.mThis->mSubCurr]) ;
	}

	void sub_return () {
		_DEBUG_ASSERT_ (mThis->mSubCurr != VAR_NONE) ;
		_DYNAMIC_ASSERT_ (mSelf.mThis->mState == STATE_RUNNING) ;
		mSelf.mThis->mSubQueue.clear () ;
		mSelf.mThis->mSubAwaitQueue.clear () ;
		mSelf.mThis->mSubCurr = VAR_NONE ;
		mSelf.mThis->mState = STATE_STOPPED ;
		goto_break_point (mSelf.mThis->mBreakPoint) ;
	}
} ;
#endif

class RandomService final
	:private Proxy {
private:
	exports class Abstract
		:public Interface {
	public:
		virtual VAR entropy () const = 0 ;
		virtual void reset_seed (VAR seed_) = 0 ;
		virtual VAR random_value () popping = 0 ;
		virtual void random_skip (LENGTH len) = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<RandomService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	VAR entropy () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->entropy () ;
	}

	void reset_seed (VAR seed_) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->reset_seed (seed_) ;
	}

	VAR random_value (VAR min_ ,VAR max_) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (min_ <= max_) ;
		const auto r1x = max_ - min_ + 1 ;
		const auto r2x = mThis->random_value () ;
		return r2x % r1x + min_ ;
	}

	Array<VAR> random_value (VAR min_ ,VAR max_ ,LENGTH len) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		_DEBUG_ASSERT_ (min_ <= max_) ;
		Array<VAR> ret = Array<VAR> (len) ;
		const auto r1x = max_ - min_ + 1 ;
		for (auto &&i : _RANGE_ (0 ,ret.length ())) {
			const auto r2x = mThis->random_value () ;
			ret[i] = r2x % r1x + min_ ;
		}
		return std::move (ret) ;
	}

	BitSet<> random_shuffle (LENGTH count ,LENGTH range_) popping {
		return random_shuffle (count ,range_ ,BitSet<> (range_)) ;
	}

	BitSet<> random_shuffle (LENGTH count ,LENGTH range_ ,BitSet<> &&res) popping {
		_DEBUG_ASSERT_ (count >= 0 && count < range_) ;
		_DEBUG_ASSERT_ (res.size () == range_) ;
		BitSet<> ret = std::move (res) ;
		ret.clear () ;
		while (TRUE) {
			if (ret.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range_ - 1)) ;
			ret[ix] = TRUE ;
		}
		return std::move (ret) ;
	}

	BitSet<> random_shuffle (LENGTH count ,const BitSet<> &range_) popping {
		BitSet<> ret = BitSet<> (range_.size ()) ;
		compute_random_shuffle (count ,range_ ,ret) ;
		return std::move (ret) ;
	}

	void compute_random_shuffle (LENGTH count ,const BitSet<> &range_ ,BitSet<> &chosen) {
		_DEBUG_ASSERT_ (count >= 0 && count < range_.size ()) ;
		_DEBUG_ASSERT_ (chosen.size () == range_.size ()) ;
		chosen.clear () ;
		while (TRUE) {
			if (chosen.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range_.size () - 1)) ;
			chosen[ix] = BOOL (range_[ix]) ;
		}
	}

	String<STR> random_uuid () popping {
		static constexpr auto M_UUID = _PCSTR_ ("00000000-0000-0000-000000000000") ;
		String<STR> ret = String<STR> (M_UUID.size ()) ;
		INDEX iw = 0 ;
		const auto r1x = random_value (0 ,36 ,28) ;
		for (auto &&i : _RANGE_ (0 ,8)) {
			INDEX ix = 0 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 8 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 12 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (auto &&i : _RANGE_ (0 ,12)) {
			INDEX ix = 16 + i ;
			ret[iw++] = index_to_hex_str (r1x[ix]) ;
		}
		if (iw < ret.size ())
			ret[iw] = 0 ;
		return std::move (ret) ;
	}

	void random_skip (LENGTH len) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->random_skip (len) ;
	}

private:
	RandomService () ;

private:
	inline static STRU8 index_to_hex_str (INDEX index) {
		if (index < 10)
			return STRU8 (STRU8 ('0') + index) ;
		return STRU8 (STRU8 ('A' - 10) + index) ;
	}
} ;
} ;