#pragma once

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_geometry.hpp"
#include "csc_algorithm.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
namespace FUNCTIONAL {
template <class...>
trait SYNTAXTREE_HELP ;

template <>
trait SYNTAXTREE_HELP<ALWAYS> {
	struct NODE {
		FLAG mCABI ;
		String<STR> mName ;
		AnyRef<> mValue ;
		Function<void ()> mOnceActor ;
		Function<void ()> mActor ;
		BOOL mIsFunction ;
		BOOL mIsIteration ;
		BOOL mPlaying ;
		BOOL mOncePlayed ;
		BOOL mPlayed ;
		Set<INDEX> mParent ;
		Set<INDEX> mChild ;
		Set<INDEX> mMaybe ;
		LENGTH mDepth ;
	} ;

	class SyntaxTree {
	private:
		List<NODE> mTreeList ;
		Set<INDEX> mTreeCABISet ;
		Deque<INDEX> mTreeStack ;
		Priority<LENGTH> mPlayPriority ;
		Deque<PACK<INDEX ,LENGTH>> mDepthStack ;
		Deque<INDEX> mRankStack ;
		BOOL mEnableClean ;
		Deque<INDEX> mCleanQueue ;
		Deque<INDEX> mUndoQueue ;
		Deque<INDEX> mScanQueue ;
	} ;
} ;

template <class...>
trait OPERAND_HELP ;

template <class...>
trait OPERANDLIST_HELP ;

template <class...>
trait EXPRESSION_HELP ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK0>>> {
	class Expression {
	private:

	public:

	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK1>>> {
	class Expression {
	private:

	public:

	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK2>>> {
	class Expression {
	private:

	public:

	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_COMPR_GT<RANK ,RANK3>>> {
	class Expression {
	private:

	public:

	} ;
} ;
} ;
} ;

namespace CSC {
using namespace FUNCTIONAL ;
} ;