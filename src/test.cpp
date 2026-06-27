#include "util.h"

using namespace ROUTINE ;

/*
1. VR<[^\r]+> expr_m
2. virtual RefLayout share
3. SparseT
*/

struct SparseSlot {
	//离散化，将值A，B，C重新映射为mSlotX中的索引
	Set<Index> mY ;
	//排序，将A，B，C排序的顺序储存下来，比如mOrder.find(A)==0，mOrder.find(B)==1
	//优化：SortedMap的remap()是O(nlogn)的，每次add后调用代价较高。
	//这里改为延迟remap：只在order(y,item)查询时按需remap，避免重复的排序开销。
	//若要进一步优化到O(logn)每次插入，可考虑引入树状数组（Fenwick Tree）或红黑树顺序统计树。
	SortedMap<Index> mOrder ;
} ;

struct SparseNode implement AllocatorNode {} ;

struct SparseLayout {
	Allocator<Pointer ,SparseNode> mList ;
	//储存mList中包含第i维度中元素为y的全部的向量的集合
	List<BitSet> mSlotX ;
	//mSlotY[i]表示每一个维度的元素集合，mSlotY[i].mY.map(y)表示第i维度中的y元素的映射到mSlotX的索引
	Array<SparseSlot> mSlotY ;
} ;

struct SparseHolder implement Interface {
	imports VFat<SparseHolder> hold (VR<SparseLayout> that) ;
	imports CFat<SparseHolder> hold (CR<SparseLayout> that) ;

	virtual void initialize (CR<Length> rank) = 0 ;
	virtual Length rank () const = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual void clear () = 0 ;
	virtual Index order (CR<Index> y) const = 0 ;
	virtual Index order (CR<Index> y ,CR<Index> item) const = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual void add (CR<Wrapper<Pointer>> item) = 0 ;
	virtual Length count (CR<Wrapper<Pointer>> item) const = 0 ;
	virtual BitSet find (CR<Wrapper<Pointer>> item) const = 0 ;
	virtual void remove (CR<Index> index) = 0 ;
} ;

template <class>
class SparseT ;

template <class...A>
class SparseT<TYPE<A...>> implement SparseLayout {
private:
	using RANK = RANK_OF<TYPE<A...>> ;

public:
	implicit SparseT () = default ;

	Length rank () const {
		return SparseHolder::hold (thiz)->rank () ;
	}

	Length size () const {
		return SparseHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return SparseHolder::hold (thiz)->length () ;
	}

	void clear () {
		return SparseHolder::hold (thiz)->clear () ;
	}

	Index order (CR<Index> y) const {
		return SparseHolder::hold (thiz)->order (y) ;
	}

	Index order (CR<Index> y ,CR<Index> item) const {
		return SparseHolder::hold (thiz)->order (y ,item) ;
	}

	CR<Buffer<Index ,RANK>> at (CR<Index> index) const leftvalue {
		return SparseHolder::hold (thiz)->at (index) ;
	}

	void add (CR<A>...item) {
		return SparseHolder::hold (thiz)->add (MakeWrapper (Pointer::from (item)...)) ;
	}

	Length count (CR<A>... item) const {
		return SparseHolder::hold (thiz)->count (MakeWrapper (Pointer::from (item)...)) ;
	}

	BitSet find (CR<A>...item) const {
		return SparseHolder::hold (thiz)->find (MakeWrapper (Pointer::from (item)...)) ;
	}

	void remove (CR<Index> index) {
		return SparseHolder::hold (thiz)->remove (index) ;
	}
} ;

template <class A>
using Sparse = SparseT<TYPE_REPEAT<Index ,A>> ;

int test () {
	auto rax = Sparse<RANK3> () ;
	rax.add (1 ,1 ,1) ;
	rax.add (1 ,1 ,2) ;
	rax.add (1 ,2 ,3) ;
	//寻找元素，-1表示任意元素，则会找到（1 ,1 ,1）和（1 ,1 ,2）
	const auto r1x = rax.find (-1 ,1 ,-1) ;
	noop (r1x) ;
	return 0 ;
}

class SparseImplHolder final implement Fat<SparseHolder ,SparseLayout> {
public:
	void initialize (CR<Length> rank) override {
		self.mSlotY = Array<SparseSlot> (rank) ;
	}

	Length rank () const override {
		return self.mSlotY.length () ;
	}

	Length size () const override {
		return self.mList.size () ;
	}

	Length length () const override {
		return self.mList.length () ;
	}

	void clear () override {
		self.mList.clear () ;
		for (auto &&i : self.mSlotY.iter ()) {
			self.mSlotY[i].mY.clear () ;
			self.mSlotY[i].mOrder.clear () ;
		}
	}

	Index order (CR<Index> y) const override {
		return self.mSlotY[y].mOrder.length () ;
	}

	Index order (CR<Index> y ,CR<Index> item) const override {
		// 延迟remap：只在order查询时按需remap，避免add时重复的O(nlogn)排序开销
		// SortedMap::remap内部会检查mRemap标志，若已排序则直接返回
		const_cast<SortedMap<Index>&> (self.mSlotY[y].mOrder).remap () ;
		return self.mSlotY[y].mOrder.find (item) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mList.at (index) ;
	}

	void add (CR<Wrapper<Pointer>> item) override {
		for (auto &&i : range (0 ,item.rank ())) {
			const auto r1x = Index (bitwise (item[i])) ;
			assume (r1x >= 0) ;
		}
		Index ix = self.mList.alloc () ;
		const auto r2x = address (self.mList[ix]) ;
		for (auto &&i : range (0 ,item.rank ())) {
			const auto r3x = Index (bitwise (item[i])) ;
			if ifdo (TRUE) {
				const auto r4x = r2x + i * SIZE_OF<Index>::expr ;
				bitwise (Pointer::make (r4x)) = r3x ;
			}
			Index jx = self.mSlotY[i].mY.map (r3x) ;
			if ifdo (TRUE) {
				if (jx != NONE)
					discard ;
				jx = self.mSlotX.insert () ;
				self.mSlotX[jx] = BitSet (self.mList.size ()) ;
				self.mSlotY[i].mY.add (r3x ,jx) ;
				self.mSlotY[i].mOrder.add (r3x) ;
				// 移除每次add后的立即remap，改为在order查询时按需remap
			}
			self.mSlotX[jx].add (ix) ;
		}
	}

	Length count (CR<Wrapper<Pointer>> item) const override {
		BitSet result ;
		Bool first = TRUE ;
		for (auto &&i : range (0 ,item.rank ())) {
			const auto r1x = Index (bitwise (item[i])) ;
			if (r1x < 0)
				continue ;
			const auto jx = self.mSlotY[i].mY.map (r1x) ;
			if (jx == NONE)
				return 0 ;
			if (first) {
				result = self.mSlotX[jx] ;
				first = FALSE ;
			} else {
				result &= self.mSlotX[jx] ;
			}
		}
		if (first)
			return self.mList.length () ;
		return result.length () ;
	}

	BitSet find (CR<Wrapper<Pointer>> item) const override {
		BitSet result ;
		Bool first = TRUE ;
		for (auto &&i : range (0 ,item.rank ())) {
			const auto r1x = Index (bitwise (item[i])) ;
			if (r1x < 0)
				continue ;
			const auto jx = self.mSlotY[i].mY.map (r1x) ;
			if (jx == NONE)
				return BitSet () ;
			if (first) {
				result = self.mSlotX[jx] ;
				first = FALSE ;
			} else {
				result &= self.mSlotX[jx] ;
			}
		}
		if (first) {
			result = BitSet (self.mList.size ()) ;
			for (auto &&i : range (0 ,self.mList.size ())) {
				if (self.mList.used (i))
					result.add (i) ;
			}
		}
		return move (result) ;
	}

	void remove (CR<Index> index) override {
		assume (index >= 0) ;
		assume (index < self.mList.size ()) ;
		if (!self.mList.used (index))
			return ;
		const auto r2x = address (self.mList[index]) ;
		for (auto &&i : range (0 ,self.mSlotY.length ())) {
			const auto r3x = Index (bitwise (Pointer::make (r2x + i * SIZE_OF<Index>::expr))) ;
			const auto jx = self.mSlotY[i].mY.map (r3x) ;
			if (jx != NONE)
				self.mSlotX[jx].erase (index) ;
		}
		self.mList.free (index) ;
	}
} ;

exports VFat<SparseHolder> SparseHolder::hold (VR<SparseLayout> that) {
	return VFat<SparseHolder> (SparseImplHolder () ,that) ;
}

exports CFat<SparseHolder> SparseHolder::hold (CR<SparseLayout> that) {
	return CFat<SparseHolder> (SparseImplHolder () ,that) ;
}
