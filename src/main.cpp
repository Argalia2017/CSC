#include "util.h"

#include "csc/csc_end.h"
#include <initializer_list>
#include "csc/csc_begin.h"

using namespace SOLUTION ;

/*
Ref<[^\r]+>::reference
RefBuffer::unknown
implicit ~\w+(?<!Layout) \(\)
(?<!CREF<\w+Proc> )instance \(\) \{
Pointer::make \(address
implicit \w+ \(\) = delete
*/

void test () {
	const auto r1x = Path (slice ("D:/Documents/C++/3dgs/demo.ply")) ;
	const auto r2x = FileProc::load_file (r1x) ;
	auto mReader = PlyParser (r2x) ;
	const auto r3x = mReader.find_element (slice ("vertex")) ;
	if ifdo (TRUE) {
		const auto r4x = mReader.find_property (r3x ,slice ("x")) ;
		const auto r5x = mReader.find_property (r3x ,slice ("y")) ;
		const auto r6x = mReader.find_property (r3x ,slice ("z")) ;
		const auto r7x = mReader.find_property (r3x ,slice ("scale_0")) ;
		const auto r8x = mReader.find_property (r3x ,slice ("scale_1")) ;
		const auto r9x = mReader.find_property (r3x ,slice ("scale_2")) ;
		const auto r10x = mReader.find_property (r3x ,slice ("rot_0")) ;
		const auto r11x = mReader.find_property (r3x ,slice ("rot_1")) ;
		const auto r12x = mReader.find_property (r3x ,slice ("rot_2")) ;
		const auto r13x = mReader.find_property (r3x ,slice ("rot_3")) ;
		const auto r14x = mReader.find_property (r3x ,slice ("f_dc_0")) ;
		const auto r15x = mReader.find_property (r3x ,slice ("f_dc_1")) ;
		const auto r16x = mReader.find_property (r3x ,slice ("f_dc_2")) ;
		const auto r17x = mReader.find_property (r3x ,slice ("opacity")) ;
		mReader.guide_new (r3x) ;
		mReader.guide_put (r4x) ;
		mReader.guide_put (r5x) ;
		mReader.guide_put (r6x) ;
		mReader.guide_put (r14x) ;
		mReader.guide_put (r15x) ;
		mReader.guide_put (r16x) ;
		mReader.guide_put (r17x) ;
	}
	auto rax = Buffer<FLT32 ,RANK4> () ;
	auto mWriter = StreamFileTextWriter (slice ("D:/Documents/C++/3dgs/pointcloud.ply")) ;
	mWriter << slice ("ply") << GAP ;
	mWriter << slice ("format ascii 1.0") << GAP ;
	const auto r18x = mReader.element_size (r3x) ;
	mWriter << slice ("element vertex ") << r18x << GAP ;
	mWriter << slice ("property float x") << GAP ;
	mWriter << slice ("property float y") << GAP ;
	mWriter << slice ("property float z") << GAP ;
	mWriter << slice ("property uchar red") << GAP ;
	mWriter << slice ("property uchar green") << GAP ;
	mWriter << slice ("property uchar blue") << GAP ;
	mWriter << slice ("property uchar alpha") << GAP ;
	mWriter << slice ("end_header") << GAP ;
	for (auto &&i : iter (0 ,mReader.element_size (r3x))) {
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		mWriter << rax[0] << slice (" ") ;
		mWriter << rax[1] << slice (" ") ;
		mWriter << rax[2] << GAP ;
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		mReader.read (rax[3]) ;
		mWriter << VAL32 (rax[0]) * 255 << slice (" ") ;
		mWriter << VAL32 (rax[1]) * 255 << slice (" ") ;
		mWriter << VAL32 (rax[2]) * 255 << slice (" ") ;
		mWriter << VAL32 (rax[3]) * 255 << GAP ;
	}
	mWriter.close () ;
}

FLT32 bit_reverse (FLT32 x) {
	const auto r1x = bitwise[TYPE<CHAR>::expr] (x) ;
	const auto r2x = ByteProc::bit_reverse (r1x) ;
	const auto r3x = bitwise[TYPE<FLT32>::expr] (r2x) ;
	return r3x ;
}

void test2 () {
	// 6*4 + 4 + 4 = 8*4
	// XYZ - Position (Float32)
	// XYZ - Scale (Float32)
	// RGBA - colors (uint8)
	// IJKL - quaternion/rot (uint8)
	const auto r1x = Path (slice ("D:/Documents/C++/3dgs/bicycle.splat")) ;
	const auto r2x = FileProc::load_file (r1x) ;
	const auto rows_size = 4 * 3 + 4 * 3 + 4 + 4 ;
	assume (r2x.size () % rows_size == 0) ;
	const auto r3x = r2x.size () / rows_size ;
	auto mWriter = StreamFileByteWriter (slice ("D:/Documents/C++/3dgs/bicycle.ply")) ;
	const auto GAP = slice ("\n") ;
	mWriter << slice ("ply") << GAP ;
	mWriter << slice ("format binary_little_endian 1.0") << GAP ;
	mWriter << slice ("element vertex ") << String<STR>::make (r3x) << GAP ;
	mWriter << slice ("property float x") << GAP ;
	mWriter << slice ("property float y") << GAP ;
	mWriter << slice ("property float z") << GAP ;
	mWriter << slice ("property float f_dc_0") << GAP ;
	mWriter << slice ("property float f_dc_1") << GAP ;
	mWriter << slice ("property float f_dc_2") << GAP ;
	mWriter << slice ("property float opacity") << GAP ;
	mWriter << slice ("property float scale_0") << GAP ;
	mWriter << slice ("property float scale_1") << GAP ;
	mWriter << slice ("property float scale_2") << GAP ;
	mWriter << slice ("property float rot_0") << GAP ;
	mWriter << slice ("property float rot_1") << GAP ;
	mWriter << slice ("property float rot_2") << GAP ;
	mWriter << slice ("property float rot_3") << GAP ;
	mWriter << slice ("end_header") << GAP ;
	auto rax = Buffer<FLT32 ,RANK4> () ;
	for (auto &&i : iter (0 ,r3x)) {
		rax[0] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 0 + 0])) ;
		rax[1] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 0 + 4])) ;
		rax[2] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 0 + 8])) ;
		mWriter << bit_reverse (rax[0]) ;
		mWriter << bit_reverse (rax[1]) ;
		mWriter << bit_reverse (rax[2]) ;
		const auto r10x = MathProc::inverse (FLT32 (255)) ;
		rax[0] = FLT32 (r2x[i * rows_size + 24 + 0]) * r10x ;
		rax[1] = FLT32 (r2x[i * rows_size + 24 + 1]) * r10x ;
		rax[2] = FLT32 (r2x[i * rows_size + 24 + 2]) * r10x ;
		rax[3] = FLT32 (r2x[i * rows_size + 24 + 3]) * r10x ;
		const auto r20x = FLT32 ((rax[0] - 0.5) / 0.28209479177387814) ;
		const auto r21x = FLT32 ((rax[1] - 0.5) / 0.28209479177387814) ;
		const auto r22x = FLT32 ((rax[2] - 0.5) / 0.28209479177387814) ;
		const auto r23x = -MathProc::log ((1 / rax[3]) - 1) ;
		mWriter << bit_reverse (r20x) ;
		mWriter << bit_reverse (r21x) ;
		mWriter << bit_reverse (r22x) ;
		mWriter << bit_reverse (r23x) ;
		rax[0] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 12 + 0])) ;
		rax[1] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 12 + 4])) ;
		rax[2] = bitwise[TYPE<FLT32>::expr] (Pointer::from (r2x[i * rows_size + 12 + 8])) ;
		rax[0] = MathProc::log (rax[0]) ;
		rax[1] = MathProc::log (rax[1]) ;
		rax[2] = MathProc::log (rax[2]) ;
		mWriter << bit_reverse (rax[0]) ;
		mWriter << bit_reverse (rax[1]) ;
		mWriter << bit_reverse (rax[2]) ;
		rax[0] = FLT32 (VAL32 (r2x[i * rows_size + 28 + 0]) - 128) ;
		rax[1] = FLT32 (VAL32 (r2x[i * rows_size + 28 + 1]) - 128) ;
		rax[2] = FLT32 (VAL32 (r2x[i * rows_size + 28 + 2]) - 128) ;
		rax[3] = FLT32 (VAL32 (r2x[i * rows_size + 28 + 3]) - 128) ;
		mWriter << bit_reverse (rax[0]) ;
		mWriter << bit_reverse (rax[1]) ;
		mWriter << bit_reverse (rax[2]) ;
		mWriter << bit_reverse (rax[3]) ;
	}
	mWriter.close () ;
}

struct GsplatSphere {
	Matrix mMatT ;
	Matrix mMatR ;
	Matrix mMatS ;
	Color4B mColor ;
	Array<FLT32> mSH ;
	FLT32 mAlpha ;
} ;

void test3 () {
	auto mGsplatMesh = ArrayList<GsplatSphere> () ;
	const auto r1x = Path (slice ("D:/Documents/C++/3dgs/demo.ply")) ;
	const auto r40x = CurrentTime () ;
	const auto r2x = FileProc::load_file (r1x) ;
	const auto r41x = CurrentTime () ;
	auto mReader = PlyParser (r2x) ;
	const auto r42x = CurrentTime () ;
	const auto r3x = mReader.find_element (slice ("vertex")) ;
	if ifdo (TRUE) {
		const auto r4x = mReader.find_property (r3x ,slice ("x")) ;
		const auto r5x = mReader.find_property (r3x ,slice ("y")) ;
		const auto r6x = mReader.find_property (r3x ,slice ("z")) ;
		const auto r7x = mReader.find_property (r3x ,slice ("rot_0")) ;
		const auto r8x = mReader.find_property (r3x ,slice ("rot_1")) ;
		const auto r9x = mReader.find_property (r3x ,slice ("rot_2")) ;
		const auto r10x = mReader.find_property (r3x ,slice ("rot_3")) ;
		const auto r11x = mReader.find_property (r3x ,slice ("scale_0")) ;
		const auto r12x = mReader.find_property (r3x ,slice ("scale_1")) ;
		const auto r13x = mReader.find_property (r3x ,slice ("scale_2")) ;
		const auto r14x = mReader.find_property (r3x ,slice ("f_dc_0")) ;
		const auto r15x = mReader.find_property (r3x ,slice ("f_dc_1")) ;
		const auto r16x = mReader.find_property (r3x ,slice ("f_dc_2")) ;
		const auto r17x = mReader.find_property (r3x ,slice ("opacity")) ;
		mReader.guide_new (r3x) ;
		mReader.guide_put (r4x) ;
		mReader.guide_put (r5x) ;
		mReader.guide_put (r6x) ;
		mReader.guide_put (r7x) ;
		mReader.guide_put (r8x) ;
		mReader.guide_put (r9x) ;
		mReader.guide_put (r10x) ;
		mReader.guide_put (r11x) ;
		mReader.guide_put (r12x) ;
		mReader.guide_put (r13x) ;
		mReader.guide_put (r14x) ;
		mReader.guide_put (r15x) ;
		mReader.guide_put (r16x) ;
		mReader.guide_put (r17x) ;
	}
	auto rax = Buffer<FLT32 ,RANK4> () ;
	auto rbx = Buffer<BYTE ,RANK4> () ;
	for (auto &&i : iter (0 ,mReader.element_size (r3x))) {
		INDEX ix = mGsplatMesh.insert () ;
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		mGsplatMesh[ix].mMatT = TranslationMatrix (rax[0] ,rax[1] ,rax[2]) ;
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		mReader.read (rax[3]) ;
		mGsplatMesh[ix].mMatR = Quaternion (rax[1] ,rax[2] ,rax[3] ,rax[0]).matrix () ;
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		const auto r18x = MathProc::exp (rax[0]) ;
		const auto r19x = MathProc::exp (rax[1]) ;
		const auto r20x = MathProc::exp (rax[2]) ;
		mGsplatMesh[ix].mMatS = DiagMatrix (r18x ,r19x ,r20x) ;
		mReader.read (rax[0]) ;
		mReader.read (rax[1]) ;
		mReader.read (rax[2]) ;
		mReader.read (rax[3]) ;
		const auto r21x = MathProc::inverse (1 + MathProc::exp (-rax[3])) ;
		mGsplatMesh[ix].mColor.mR = BYTE (MathProc::lerp (rax[0] * 0.28 + 0.5 ,0 ,255)) ;
		mGsplatMesh[ix].mColor.mG = BYTE (MathProc::lerp (rax[1] * 0.28 + 0.5 ,0 ,255)) ;
		mGsplatMesh[ix].mColor.mB = BYTE (MathProc::lerp (rax[2] * 0.28 + 0.5 ,0 ,255)) ;
		mGsplatMesh[ix].mColor.mA = BYTE (MathProc::lerp (rax[3] ,0 ,255)) ;
		mGsplatMesh[ix].mSH = Array<FLT32> (48) ;
		mGsplatMesh[ix].mSH.fill (0) ;
		mGsplatMesh[ix].mSH[0] = rax[0] ;
		mGsplatMesh[ix].mSH[1] = rax[1] ;
		mGsplatMesh[ix].mSH[2] = rax[2] ;
		mGsplatMesh[ix].mAlpha = r21x ;
	}
	const auto r43x = CurrentTime () ;
	Singleton<Console>::instance ().debug (slice ("load time = ") ,(r41x - r40x).milliseconds ()) ;
	Singleton<Console>::instance ().debug (slice ("parse time = ") ,(r42x - r41x).milliseconds ()) ;
	Singleton<Console>::instance ().debug (slice ("read time = ") ,(r43x - r42x).milliseconds ()) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
	barrier (mGsplatMesh) ;
}

exports int main () {
	Singleton<Console>::instance ().start () ;
	Singleton<Console>::instance ().open (slice (".")) ;
	//test2 () ;
	test3 () ;
	return 0 ;
}