#pragma once

#ifndef __CSC_DATABASE__
#error "б╞(д├бузебу ;)д├ : require 'csc_database.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#endif

#ifdef __CSC_DEPRECATED__
#include <mysql.h>
#endif

#ifndef _mysql_h
#error "б╞(д├бузебу ;)д├ : require 'mysql.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_mysql
#ifdef __CSC_DEBUG__
#define use_comment_lib_mysql "libmysqld.lib"
#else
#define use_comment_lib_mysql "libmysql.lib"
#endif
#endif
#pragma comment (lib ,use_comment_lib_mysql)
#undef use_comment_lib_mysql
#endif
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#endif

namespace CSC {
template <class>
class AbstractDatabase_Engine_MYSQL :public AbstractDatabase::Abstract {
public:
	using NATIVE_TYPE = UniqueRef<MYSQL> ;

public:
	AbstractDatabase_Engine_MYSQL () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void compute_load_data (AnyRef<void> &_this) override {
		auto rax = UniqueRef<MYSQL> ([&] (MYSQL &me) {
			mysql_init (&me) ;
		} ,[] (MYSQL &me) {
			mysql_close (&me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

private:
	static void compute_check_error (MYSQL &_self) {
		const auto r1x = &_LOAD_<ARR<STRA>> (mysql_error (&_self)) ;
		_DYNAMIC_ASSERT_ (r1x == NULL) ;
	}
} ;
} ;