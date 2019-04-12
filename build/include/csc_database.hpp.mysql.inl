#pragma once

#ifndef __CSC_DATABASE__
#error "б╞(д├бузебу ;)д├ : require 'csc_database.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef import
#undef export
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("import")
#pragma pop_macro ("export")
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
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("import")
#pragma push_macro ("export")
#define self to ()
#define implicit
#define popping
#define import extern
#define export
#endif

namespace CSC {
template <class>
class AbstractDatabase_Engine_MYSQL :public AbstractDatabase::Abstract {
public:
	AbstractDatabase_Engine_MYSQL () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void load_data (AnyRef<void> &_this) override {
		auto rax = UniqueRef<MYSQL> ([] (MYSQL &me) {
			mysql_init (&me) ;
		} ,[] (MYSQL &me) {
			mysql_close (&me) ;
		}) ;
		_this = AnyRef<UniqueRef<MYSQL>>::make (std::move (rax)) ;
	}

private:
	static void debug_check_error (MYSQL &_self) {
		const auto r1x = _LOAD_<ARR<STRA>> (mysql_error (&_self)) ;
		_DYNAMIC_ASSERT_ (r1x == NULL) ;
		(void) r1x ;
	}
} ;
} ;