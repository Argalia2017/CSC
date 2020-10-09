#pragma once

#ifndef __CSC_DATABASE__
#error "∑(っ°Д° ;)っ : require 'csc_database.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("delegate")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef delegate
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <mysql.h>
#endif

#ifndef _mysql_h
#error "∑(っ°Д° ;)っ : require 'mysql.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_mysql
#ifdef __CSC_DEBUG__
#define use_comment_lib_mysql "libmysqld.lib"
#endif
#endif

#ifndef use_comment_lib_mysql
#ifndef __CSC_DEBUG__
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
#pragma pop_macro ("delegate")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using ::MYSQL ;

using ::mysql_init ;
using ::mysql_close ;
using ::mysql_error ;
} ;

class SQLDatabase::Private::Implement :
	delegate public SQLDatabase::Abstract {
private:
	using NATIVE_THIS = UniqueRef<api::MYSQL> ;

private:
	UniqueRef<api::MYSQL> mHolder ;

public:
	implicit Implement () = default ;

	void load_data () override {
		auto rax = UniqueRef<api::MYSQL> ([&] (api::MYSQL &me) {
			api::mysql_init (DEPTR[me]) ;
		} ,[] (api::MYSQL &me) {
			api::mysql_close (DEPTR[me]) ;
		}) ;
		mHolder = _MOVE_ (rax) ;
	}

private:
	void compute_check_error (api::MYSQL &this_) const {
		const auto r1x = api::mysql_error (DEPTR[this_]) ;
		_DYNAMIC_ASSERT_ (r1x == NULL) ;
	}
} ;

exports SQLDatabase::SQLDatabase () {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::Implement ;
	mThis = StrongRef<R1X>::make () ;
}
} ;