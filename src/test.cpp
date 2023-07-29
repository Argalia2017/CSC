#include "util.h"

using namespace SOLUTION ;

/*
[CV]Ref<(?!ARR<)[^\r]+> (?!mThis|mThat)m\w+ ;
BashRename for VS
*/

exports void test_main () {
	auto mArray = Array<SharedRef<int>> (10) ;
	for (auto &&i : mArray.iter ())
		mArray[i] = SharedRef<int>::make (int (i)) ;
	auto mIndex = Array<INDEX> (10000) ;
	for (auto &&i : mIndex.iter ())
		mIndex[i] = Random::instance ().random_value (0 ,9) ;
	auto mThread = WorkThread (TRUE) ;
	mThread.start (Function<void ,TYPEAS<INDEX>> ([&] (CREF<INDEX> item) {
		INDEX ix = mIndex[item] ;
		auto rax = mArray[ix] ;
		auto rbx = rax ;
		//unittest (rax) ;
		//unittest (rbx) ;
		rax = SharedRef<int> () ;
		rbx = SharedRef<int> () ;
	})) ;
	mThread.post_all (IterArray<INDEX>::make (iter (0 ,10000))) ;
	mThread.join () ;
	mThread.stop () ;
	mThread.stop () ;
}