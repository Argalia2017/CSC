#include "util.h"

#ifdef __CSC_STREAM__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_STREAM) {
public:
	TEST_METHOD (TEST_CSC_STREAM) {
		const auto r1x = CSC::CHAR (0X11223344) ;
		auto rax = PACK<CSC::BYTE[_SIZEOF_ (CSC::CHAR)]> () ;
		_CAST_<EndianBytes<CSC::CHAR>> (rax.P1) <<= r1x ;
		_UNITTEST_ASSERT_ (rax.P1[0] == CSC::BYTE (0X11)) ;
		_UNITTEST_ASSERT_ (rax.P1[1] == CSC::BYTE (0X22)) ;
		_UNITTEST_ASSERT_ (rax.P1[2] == CSC::BYTE (0X33)) ;
		_UNITTEST_ASSERT_ (rax.P1[3] == CSC::BYTE (0X44)) ;
	}

	TEST_METHOD (TEST_CSC_STREAM_BYTEREADER) {
		auto rax = PACK<CSC::BYTE[12]> () ;
		auto ris = ByteReader (PhanBuffer<const CSC::BYTE>::make (rax.P1)) ;
		auto rbx = Buffer<int ,ARGC<4>> () ;
		ris >> rbx ;
	}

	TEST_METHOD (TEST_CSC_STREAM_BYTEWRITER) {
		auto rax = Buffer<int ,ARGC<4>> () ;
		auto wos = ByteWriter (SharedRef<FixedBuffer<CSC::BYTE>>::make (1024)) ;
		wos << rax ;
	}

	TEST_METHOD (TEST_CSC_STREAM_TEXTREADER) {
		struct WRAPPED_int :private Wrapped<int> {
			inline void friend_read (TextReader<STRU8> &reader) popping {
				reader >> WRAPPED_int::mSelf >> _GAP_ ;
			}
		} ;
		auto rax = Buffer<int ,ARGC<4>> () ;
		auto rbx = Buffer<STRU8 ,ARGC<10>> () ;
		auto ris = TextReader<STRU8> (PhanBuffer<const STRU8>::make (rbx)) ;
		ris.attr ().modify_space (STRU8 (' ') ,0) ;
		rbx[0] = STRU8 ('0') ;
		rbx[1] = STRU8 (' ') ;
		rbx[2] = STRU8 ('1') ;
		rbx[3] = STRU8 (' ') ;
		rbx[4] = STRU8 ('2') ;
		rbx[5] = STRU8 (' ') ;
		rbx[6] = STRU8 ('3') ;
		rbx[7] = STRU8 (' ') ;
		rbx[8] = STRU8 ('4') ;
		rbx[9] = ris.attr ().varify_ending_item () ;
		ris >> _CAST_<Buffer<WRAPPED_int ,ARGC<4>>> (rax) ;
		_UNITTEST_ASSERT_ (rax[0] == 0) ;
		_UNITTEST_ASSERT_ (rax[1] == 1) ;
		_UNITTEST_ASSERT_ (rax[2] == 2) ;
		_UNITTEST_ASSERT_ (rax[3] == 3) ;
	}

	TEST_METHOD (TEST_CSC_STREAM_TEXTWRITER) {
		auto rax = Buffer<int ,ARGC<4>> () ;
		auto wos = TextWriter<STRU8> (SharedRef<FixedBuffer<STRU8>>::make (1024)) ;
		wos << rax ;
	}
} ;
} ;
#endif