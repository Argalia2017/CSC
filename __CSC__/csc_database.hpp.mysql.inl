#pragma once

#ifndef __CSC_DATABASE__
#error "��(�á㧥�� ;)�� : require 'csc_database.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef switch_case
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <mysql.h>
#endif

#ifndef _mysql_h
#error "��(�á㧥�� ;)�� : require 'mysql.h'"
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
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
class AbstractDatabase_Engine_MYSQL
	:public AbstractDatabase::Abstract {
private:
	using NATIVE_THIS = UniqueRef<MYSQL> ;

public:
	AbstractDatabase_Engine_MYSQL () {
		_STATIC_ASSERT_ (_SIZEOF_ (decltype ((*this))) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (decltype ((*this))) == _ALIGNOF_ (Interface)) ;
	}

	void compute_load_data (AnyRef<void> &this_) const override {
		auto tmp = UniqueRef<MYSQL> ([&] (MYSQL &me) {
			::mysql_init (&me) ;
		} ,[] (MYSQL &me) {
			::mysql_close (&me) ;
		}) ;
		this_ = AnyRef<NATIVE_THIS>::make (std::move (tmp)) ;
	}

private:
	inline void compute_check_error (MYSQL &self_) const {
		const auto r1x = ::mysql_error (&self_) ;
		_DYNAMIC_ASSERT_ (r1x == NULL) ;
	}
} ;
} ;