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
		ret.mName = _BUILDVAR64S_ (_TYPEUID_<_RET> ()) ;
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
	inline static void template_write_typename_arr (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_full_array_of<REMOVE_ARRAY_TYPE<_ARG1> ,_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<REMOVE_ARRAY_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ ("[") ;
		writer << _COUNTOF_ (_ARG1) ;
		writer << _PCSTR_ ("]>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_array<_ARG1>::value>> & ,const DEF<decltype (ARGVP5)> &) {
		template_write_typename_arr (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1>> &) {
		template_write_typename_x (writer ,_NULL_<ARGV<_ARG1>> ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static void template_write_typename_y (TextWriter<STR> &writer ,const ARGV<ARGVS<_ARG1 ,_ARG2 ,_ARGS...>> &) {
		template_write_typename_x (writer ,_NULL_<ARGV<_ARG1>> ()) ;
		writer << _PCSTR_ (" ,") ;
		template_write_typename_y (writer ,_NULL_<ARGV<ARGVS<_ARG2 ,_ARGS...>>> ()) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_func (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,REMOVE_MEMFUNC_TYPE<_ARG1>>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (")>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_func (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<!std::is_same<_ARG1 ,REMOVE_MEMFUNC_TYPE<_ARG1>>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DEF<") ;
		template_write_typename_x (writer ,_NULL_<ARGV<INVOKE_RESULT_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (" (") ;
		template_write_typename_y (writer ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<_ARG1>>> ()) ;
		writer << _PCSTR_ (") memptr>") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_function<_ARG1>::value>> & ,const DEF<decltype (ARGVP4)> &) {
		template_write_typename_func (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
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
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_class<_ARG1>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		template_write_typename_claz (writer ,_NULL_<ARGV<_ARG1>> () ,ARGVPX ,ARGVP9) ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU8>::value && !std::is_same<_ARG1 ,BYTE>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU8") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU16>::value && !std::is_same<_ARG1 ,WORD>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU16") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,STRU32>::value && !std::is_same<_ARG1 ,CHAR>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("STRU32") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARX>::value && !std::is_same<_ARG1 ,VAR32>::value && !std::is_same<_ARG1 ,VAR64>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARX") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VARY>::value && !std::is_same<_ARG1 ,CHAR>::value && !std::is_same<_ARG1 ,DATA>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VARY") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<std::is_same<_ARG1 ,VALX>::value && !std::is_same<_ARG1 ,VAL32>::value && !std::is_same<_ARG1 ,VAL64>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		writer << _PCSTR_ ("VALX") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<_ARG1> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("typename '") ;
		writer << _TYPEUID_<_ARG1> () ;
		writer << _PCSTR_ ("'") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BOOL> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("BOOL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAR32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAR32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<EFLAG> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("EFLAG") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL32> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAL32") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VAL64> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VAL64") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<VOID> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("VOID") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<NONE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("NONE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<decltype (NULL)> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("NULL") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<BYTE> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("BYTE") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<WORD> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("WORD") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<CHAR> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("CHAR") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<DATA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("DATA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<MEGA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("MEGA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRA> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("STRA") ;
	}

	inline static void template_write_typename_id (TextWriter<STR> &writer ,const ARGV<STRW> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		writer << _PCSTR_ ("STRW") ;
	}

	template <class _ARG1>
	inline static void template_write_typename_x (TextWriter<STR> &writer ,const ARGV<_ARG1> &) {
		template_write_typename_cv (writer ,_NULL_<ARGV<REMOVE_REFERENCE_TYPE<_ARG1>>> ()) ;
		template_write_typename_id (writer ,_NULL_<ARGV<REMOVE_CVR_TYPE<_ARG1>>> () ,ARGVPX ,ARGVP9) ;
		template_write_typename_ref (writer ,_NULL_<ARGV<REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<_ARG1>>>> ()) ;
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
class GlobalStatic<void> final :private Wrapped<void> {
private:
	struct VALUE_NODE {
		FLAG mGUID ;
		BOOL mReadOnly ;
		VAR mData ;
	} ;

	struct CLASS_NODE {
		String<STR> mGUID ;
		PTR<NONE> mData ;
	} ;

	struct Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename GlobalStatic::Detail ;

	private:
		template <class>
		friend class GlobalStatic ;
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mNodeMutex ;
		HashSet<FLAG ,VALUE_NODE> mValueSet ;
		HashSet<FLAG ,CLASS_NODE> mClassSet ;
	} ;

private:
	struct Detail ;
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Holder> ;

private:
	static Holder &static_unique () popping {
		return _CACHE_ ([] () {
			_STATIC_WARNING_ ("mark") ;
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Holder> () ;
			if SWITCH_CASE (TRUE) {
				if (rax != NULL)
					discard ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Holder>::make () ;
				const auto r1x = rbx.watch () ;
				auto &r2y = _XVALUE_<Holder> (r1x) ;
				auto &r3y = _LOAD_<NONE> (&r2y) ;
				rax = unique_atomic_address (NULL ,&r3y) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			auto &r4y = _LOAD_<Holder> (rax) ;
			return IntrusiveRef<Holder> (&r4y).watch () ;
		}) ;
	}

	static PTR<VALUE_NODE> static_new_node (Holder &self_ ,FLAG guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueSet.insert (r1x) ;
		self_.mValueSet[ix].item.mGUID = guid ;
		return &self_.mValueSet[ix].item ;
	}

	static FLAG node_guid_hash (FLAG guid) {
		return guid ;
	}

	static PTR<VALUE_NODE> static_find_node (Holder &self_ ,FLAG guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mValueSet.find (r1x) ;
		if (ix == VAR_NONE)
			return  NULL ;
		return &self_.mValueSet[ix].item ;
	}

	static PTR<CLASS_NODE> static_new_node (Holder &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassSet.insert (r1x) ;
		self_.mClassSet[ix].item.mGUID = guid ;
		return &self_.mClassSet[ix].item ;
	}

	static FLAG node_guid_hash (const String<STR> &guid) {
		const auto r1x = guid.raw () ;
		const auto r2x = PhanBuffer<const BYTE>::make (r1x) ;
		return _MEMHASH_ (r2x.self ,r2x.size ()) ;
	}

	static PTR<CLASS_NODE> static_find_node (Holder &self_ ,const String<STR> &guid) popping {
		const auto r1x = node_guid_hash (guid) ;
		INDEX ix = self_.mClassSet.find (r1x) ;
		if (ix == VAR_NONE)
			return  NULL ;
		return &self_.mClassSet[ix].item ;
	}

public:
	//@warn: this function should be implemented in a 'runtime.dll'
	static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

private:
	struct Detail {
		inline static void friend_create (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mNodeMutex) ;
			self_.mCounter.self = 0 ;
			self_.mValueSet = HashSet<FLAG ,VALUE_NODE> () ;
			self_.mClassSet = HashSet<FLAG ,CLASS_NODE> () ;
		}

		inline static void friend_destroy (Holder &self_) {
			ScopedGuard<std::mutex> ANONYMOUS (self_.mNodeMutex) ;
			self_.mValueSet = HashSet<FLAG ,VALUE_NODE> () ;
			self_.mClassSet = HashSet<FLAG ,CLASS_NODE> () ;
		}

		inline static LENGTH friend_attach (Holder &self_) popping {
			return ++self_.mCounter.self ;
		}

		inline static LENGTH friend_detach (Holder &self_) popping {
			return --self_.mCounter.self ;
		}

		inline static void friend_latch (Holder &self_) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

template <FLAG GUID>
class GlobalStatic<ARGC<GUID>> final :private Wrapped<void> {
public:
	static void init (VAR data ,BOOL read_only) {
		auto &r1y = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1y.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1y ,GUID) ;
		if (r2x != NULL)
			return ;
		const auto r3x = GlobalStatic<void>::static_new_node (r1y ,GUID) ;
		_DYNAMIC_ASSERT_ (r3x != NULL) ;
		r3x->mReadOnly = read_only ;
		r3x->mData = data ;
	}

	static VAR load () popping {
		auto &r1y = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1y.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1y ,GUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		return r2x->mData ;
	}

	static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1y = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1y.mNodeMutex) ;
		const auto r2x = GlobalStatic<void>::static_find_node (r1y ,GUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		_DYNAMIC_ASSERT_ (!r2x->mReadOnly) ;
		if (r2x->mData == expect)
			r2x->mData = data ;
		return r2x->mData ;
	}

	static void save (VAR data) {
		auto &r1y = GlobalStatic<void>::static_unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1y.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1y ,GUID) ;
		if SWITCH_CASE (TRUE) {
			if (rax != NULL)
				discard ;
			rax = GlobalStatic<void>::static_new_node (r1y ,GUID) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
		rax->mData = data ;
	}
} ;

template <class UNIT>
class GlobalStatic<Singleton<UNIT>> final :private Wrapped<void> {
private:
	struct Detail ;

	class Holder {
	public:
		using INTRUSIVE_TYPE = typename GlobalStatic::Detail ;

	public:
		friend IntrusiveRef<Holder> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Singleton<UNIT> mData ;
	} ;

private:
	struct Detail ;
	friend IntrusiveRef<Holder> ;

public:
	static Singleton<UNIT> &unique () popping {
		auto &r1y = _CACHE_ ([] () {
			auto &r2y = GlobalStatic<void>::static_unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2y.mNodeMutex) ;
			const auto r3x = U::OPERATOR_TYPENAME::invoke<Singleton<UNIT>> () ;
			auto rax = GlobalStatic<void>::static_find_node (r2y ,r3x) ;
			auto rbx = IntrusiveRef<Holder> () ;
			if SWITCH_CASE (TRUE) {
				if (rax != NULL)
					discard ;
				rax = GlobalStatic<void>::static_new_node (r2y ,r3x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Holder>::make () ;
				const auto r4x = rbx.watch () ;
				auto &r5y = _XVALUE_<Holder> (r4x) ;
				auto &r6y = _LOAD_<NONE> (&r5y) ;
				rax->mData = &r6y ;
			}
			auto &r7y = _LOAD_<Holder> (rax->mData) ;
			return IntrusiveRef<Holder> (&r7y).watch () ;
		}) ;
		return _XVALUE_<Holder> (r1y).mData ;
	}

private:
	struct Detail {
		inline static void friend_create (Holder &self_) {
			self_.mCounter.self = 0 ;
		}

		inline static void friend_destroy (Holder &self_) {
			_STATIC_WARNING_ ("noop") ;
		}

		inline static LENGTH friend_attach (Holder &self_) popping {
			return ++self_.mCounter.self ;
		}

		inline static LENGTH friend_detach (Holder &self_) popping {
			return --self_.mCounter.self ;
		}

		inline static void friend_latch (Holder &self_) {
			GlobalRuntime::thread_sleep () ;
		}
	} ;
} ;

#ifdef __CSC_DEPRECATED__
template <class CONT>
class Coroutine {
public:
	class SubRef ;

private:
	static constexpr auto STATUS_CREATED = FLAG (0) ;
	static constexpr auto STATUS_RUNNING = FLAG (1) ;
	static constexpr auto STATUS_SUSPEND = FLAG (2) ;
	static constexpr auto STATUS_STOPPED = FLAG (3) ;

private:
	class Implement ;
	Monostate<FLAG> mStatus ;
	AutoRef<CONT> mContext ;
	AnyRef<void> mBreakPoint ;
	Array<Function<DEF<void (SubRef &)> NONE::*>> mSubProc ;
	Array<AnyRef<void>> mSubBreakPoint ;
	Deque<INDEX> mSubQueue ;
	Priority<VAR ,INDEX> mSubAwaitQueue ;
	INDEX mSubCurr ;

public:
	Coroutine () {
		mStatus.self = STATUS_CREATED ;
		mSubCurr = VAR_NONE ;
	}

	BOOL ready () const {
		if (mStatus.self != STATUS_STOPPED)
			return FALSE ;
		return TRUE ;
	}

	CONT &context () & {
		_DEBUG_ASSERT_ (mContext.exist ()) ;
		return mContext ;
	}

	CONT &context () && = delete ;

	void start (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
		_DEBUG_ASSERT_ (proc.length () > 0) ;
		mContext = AutoRef<CONT>::make () ;
		mBreakPoint = AnyRef<void> () ;
		mSubProc = std::move (proc) ;
		mSubBreakPoint = Array<AnyRef<void>> (mSubProc.size ()) ;
		mSubQueue = Deque<INDEX> (mSubProc.length ()) ;
		for (INDEX i = 0 ,ie = mSubProc.length () ; i < ie ; i++)
			mSubQueue.add (i) ;
		mSubAwaitQueue = Priority<VAR ,INDEX> (mSubProc.length ()) ;
		mSubQueue.take (mSubCurr) ;
	}

	void execute () {
		_DEBUG_ASSERT_ (!mBreakPoint.exist ()) ;
		init_break_point (mBreakPoint) ;
		for (auto &&i : mSubBreakPoint) {
			_DEBUG_ASSERT_ (!i.exist ()) ;
			init_break_point (i) ;
		}
		mStatus.self = STATUS_CREATED ;
		store_break_point (mBreakPoint) ;
		for (auto &&i : mSubBreakPoint) {
			if (mStatus.self != STATUS_STOPPED)
				continue ;
			if (!i.exist ())
				continue ;
			goto_break_point (i) ;
		}
		if (mStatus.self != STATUS_CREATED)
			return ;
		for (auto &&i : mSubBreakPoint) {
			if (mStatus.self != STATUS_CREATED)
				continue ;
			store_break_point (i) ;
		}
		_DEBUG_ASSERT_ (mSubCurr != VAR_NONE) ;
		const auto r1x = mSubCurr ;
		_CALL_TRY_ ([&] () {
			if (mStatus.self == STATUS_STOPPED)
				return ;
			mStatus.self = STATUS_RUNNING ;
			mSubProc[r1x] (_CAST_<SubRef> ((*this))) ;
		} ,[&] () {
			_STATIC_WARNING_ ("noop") ;
		}) ;
		mSubBreakPoint[r1x] = AnyRef<void> () ;
		mStatus.self = STATUS_STOPPED ;
		goto_break_point (mBreakPoint) ;
	}

private:
	void init_break_point (AnyRef<void> &bp) popping ;

	void store_break_point (AnyRef<void> &bp) noexcept popping ;

	void goto_break_point (AnyRef<void> &bp) noexcept popping ;

public:
	static void csync (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) ;
} ;

template <class CONT>
class Coroutine<CONT>::SubRef :private Wrapped<Coroutine<CONT>> {
private:
	using Wrapped<Coroutine<CONT>>::mSelf ;

public:
	CONT &to () {
		_DEBUG_ASSERT_ (mSelf.mContext.exist ()) ;
		return mSelf.mContext ;
	}

	inline implicit operator CONT & () {
		return to () ;
	}

	inline PTR<CONT> operator-> () {
		return &to () ;
	}

	const CONT &to () const {
		_DEBUG_ASSERT_ (mSelf.mContext.exist ()) ;
		return mSelf.mContext ;
	}

	inline implicit operator const CONT & () const {
		return to () ;
	}

	inline PTR<const CONT> operator-> () const {
		return &to () ;
	}

	void sub_await () {
		sub_await (mSelf.mSubAwaitQueue.length ()) ;
	}

	void sub_await (VAR priority) {
		_DEBUG_ASSERT_ (priority >= 0) ;
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		_DYNAMIC_ASSERT_ (mSelf.mStatus.self == STATUS_RUNNING) ;
		mSelf.mStatus.self = STATUS_SUSPEND ;
		store_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mStatus.self != STATUS_STOPPED) ;
		if (mSelf.mStatus.self != STATUS_SUSPEND)
			return ;
		mSelf.mSubAwaitQueue.add (priority ,mSelf.mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mSubQueue.empty ()) ;
		mSelf.mSubQueue.take (mSelf.mSubCurr) ;
		mSelf.mStatus.self = STATUS_RUNNING ;
		goto_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
	}

	void sub_resume () {
		sub_resume (mSelf.mSubAwaitQueue.length ()) ;
	}

	void sub_resume (LENGTH count) {
		for (INDEX i = 0 ,ie = count ; i < ie ; i++) {
			if (mSelf.mSubAwaitQueue.empty ())
				continue ;
			const auto r1x = mSelf.mSubAwaitQueue[mSelf.mSubAwaitQueue.head ()].item ;
			mSelf.mSubAwaitQueue.take () ;
			mSelf.mSubQueue.add (r1x) ;
		}
	}

	void sub_yield () {
		_DEBUG_ASSERT_ (mSelf.mSubCurr != VAR_NONE) ;
		if (mSelf.mSubQueue.empty ())
			return ;
		_DYNAMIC_ASSERT_ (mSelf.mStatus.self == STATUS_RUNNING) ;
		mSelf.mStatus.self = STATUS_SUSPEND ;
		store_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
		_DYNAMIC_ASSERT_ (mSelf.mStatus.self != STATUS_STOPPED) ;
		if (mSelf.mStatus.self != STATUS_SUSPEND)
			return ;
		mSelf.mSubQueue.add (mSelf.mSubCurr) ;
		_DYNAMIC_ASSERT_ (!mSelf.mSubQueue.empty ()) ;
		mSelf.mSubQueue.take (mSelf.mSubCurr) ;
		mSelf.mStatus.self = STATUS_RUNNING ;
		goto_break_point (mSelf.mSubBreakPoint[mSelf.mSubCurr]) ;
	}

	void sub_return () {
		_DEBUG_ASSERT_ (mSubCurr != VAR_NONE) ;
		_DYNAMIC_ASSERT_ (mSelf.mStatus.self == STATUS_RUNNING) ;
		mSelf.mSubQueue.clear () ;
		mSelf.mSubAwaitQueue.clear () ;
		mSelf.mSubCurr = VAR_NONE ;
		mSelf.mStatus.self = STATUS_STOPPED ;
		goto_break_point (mSelf.mBreakPoint) ;
	}
} ;

template <class CONT>
inline void Coroutine<CONT>::csync (Array<Function<DEF<void (SubRef &)> NONE::*>> &&proc) {
	auto rax = Coroutine<CONT> (std::move (proc)) ;
	rax.execute () ;
}
#endif

class RandomService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual VAR entropy () const = 0 ;
		virtual void reset_seed (VAR seed_) = 0 ;
		virtual VAR random_value () popping = 0 ;
		virtual void random_skip (LENGTH len) = 0 ;
	} ;

private:
	struct Detail ;
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
		_DEBUG_ASSERT_ (min_ >= 0 && min_ <= max_) ;
		const auto r1x = mThis->random_value () ;
		return r1x % (max_ - min_ + 1) + min_ ;
	}

	Array<VAR> random_value (VAR min_ ,VAR max_ ,LENGTH len) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		Array<VAR> ret = Array<VAR> (len) ;
		const auto r1x = max_ - min_ + 1 ;
		for (INDEX i = 0 ,ie = ret.length () ; i < ie ; i++) {
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

	void compute_random_shuffle (LENGTH count ,const BitSet<> &range_ ,BitSet<> &chosen) popping {
		_DEBUG_ASSERT_ (count >= 0 && count < range_.size ()) ;
		_DEBUG_ASSERT_ (chosen.size () == range_.size ()) ;
		chosen.clear () ;
		while (TRUE) {
			if (chosen.length () >= count)
				break ;
			INDEX ix = random_value (0 ,(range_.size () - 1)) ;
			chosen[ix] = range_[ix] ;
		}
	}

	String<STR> random_uuid () popping {
		static constexpr auto M_UUID = _PCSTR_ ("00000000-0000-0000-000000000000") ;
		String<STR> ret = String<STR> (M_UUID.size ()) ;
		INDEX iw = 0 ;
		const auto r1x = random_value (0 ,36 ,28) ;
		for (INDEX i = 0 ,ie = 8 ; i < ie ; i++) {
			INDEX ix = 0 + i ;
			ret[iw++] = Detail::index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			INDEX ix = 8 + i ;
			ret[iw++] = Detail::index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			INDEX ix = 12 + i ;
			ret[iw++] = Detail::index_to_hex_str (r1x[ix]) ;
		}
		ret[iw++] = STRU8 ('-') ;
		for (INDEX i = 0 ,ie = 12 ; i < ie ; i++) {
			INDEX ix = 16 + i ;
			ret[iw++] = Detail::index_to_hex_str (r1x[ix]) ;
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
	struct Detail {
		inline static STRU8 index_to_hex_str (INDEX index) {
			if (index < 10)
				return STRU8 ('0') + STRU8 (index) ;
			return STRU8 ('A') + STRU8 (index - 10) ;
		}
	} ;
} ;
} ;