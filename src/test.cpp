#include "util.h"

using namespace UNITTEST ;

/*
template <[^\r]+>\r\n(?!(\t+friend )?trait|\t*using)
BashRename for VS
*/

exports void test_main () {
	auto rax = BinaryTable (100) ;
	rax.joint (1 ,1) ;
	rax.joint (1 ,2) ;
	rax.joint (2 ,3) ;
	rax.joint (2 ,3) ;
	rax.joint (4 ,4) ;
	rax.joint (4 ,5) ;
	rax.joint (4 ,6) ;
	rax.remap () ;
	const auto r1x = IterArray<INDEX>::make (rax.filter (1)) ;
	const auto r2x = IterArray<INDEX>::make (rax.filter (2)) ;
	const auto r3x = IterArray<INDEX>::make (rax.filter (3)) ;
	const auto r4x = IterArray<INDEX>::make (rax.filter (4)) ;
	unittest (r1x) ;
	unittest (r2x) ;
	unittest (r3x) ;
	unittest (r4x) ;
}