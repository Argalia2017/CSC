#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "begin.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include "end.h"

namespace CSC {
template <class DEPEND>
trait FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SSIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SSIZE ;
	using FILE_RETRY_TIMES = RANK2 ;
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mFile ;

	public:
		void init_file (CREF<String<STR>> file_) override {
			mFile = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (file_) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		VarBuffer<BYTE> load () const override {
			const auto r1x = UniqueRef<VAL32> ([&] (VREF<VAL32> me) {
				me = open ((&mFile[0]) ,O_RDONLY) ;
				assume (me >= ZERO) ;
			} ,[] (VREF<VAL32> me) {
				close (me) ;
			}) ;
			const auto r2x = LENGTH (lseek (r1x.self ,0 ,SEEK_END)) ;
			assume (r2x >= 0) ;
			assume (r2x <= VAL32_MAX) ;
			lseek (r1x.self ,0 ,SEEK_SET) ;
			VarBuffer<BYTE> ret = VarBuffer<BYTE> (r2x) ;
			auto rax = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r3x = LENGTH (read (r1x.self ,(&ret.self[r2x - rax]) ,VAL32 (rax))) ;
				assume (r3x > 0) ;
				rax -= r3x ;
			}
			assume (rax == 0) ;
			return move (ret) ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const override {
			const auto r1x = UniqueRef<VAL32> ([&] (VREF<VAL32> me) {
				me = open ((&mFile[0]) ,O_RDONLY) ;
				assume (me >= ZERO) ;
			} ,[] (VREF<VAL32> me) {
				close (me) ;
			}) ;
			const auto r2x = LENGTH (lseek (r1x.self ,0 ,SEEK_END)) ;
			assume (r2x >= 0) ;
			assume (r2x <= VAL32_MAX) ;
			lseek (r1x.self ,0 ,SEEK_SET) ;
			const auto r3x = min_of (r2x ,item.size ()) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r4x = LENGTH (read (r1x.self ,(&item.self[r4x - rax]) ,VAL32 (rax))) ;
				assume (r4x > 0) ;
				rax -= r4x ;
			}
			assume (rax == 0) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assume (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<VAL32> ([&] (VREF<VAL32> me) {
				const auto r2x = VAL32 (O_CREAT | O_WRONLY | O_TRUNC) ;
				const auto r3x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = open ((&mFile[0]) ,r2x ,r3x) ;
				assume (me != ZERO) ;
			} ,[] (VREF<VAL32> me) {
				close (me) ;
			}) ;
			const auto r4x = item.size () ;
			auto rax = r4x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r5x = LENGTH (write (r1x.self ,(&item.self[r4x - rax]) ,VAL32 (rax))) ;
				assume (r5x > 0) ;
				rax -= r5x ;
			}
			assume (rax == 0) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const override {
			unimplemented () ;
			return bad (TYPEAS<CRef<RegBuffer<BYTE>>>::id) ;
		}

		BOOL find () const override {
			unimplemented () ;
			return FALSE ;
		}

		LENGTH file_size () const override {
			const auto r1x = UniqueRef<VAL32> ([&] (VREF<VAL32> me) {
				me = open ((&mFile[0]) ,O_RDONLY) ;
			} ,[] (VREF<VAL32> me) {
				if (me < ZERO)
					return ;
				close (me) ;
			}) ;
			if (r1x < ZERO)
				return ZERO ;
			const auto r2x = LENGTH (lseek (r1x.self ,0 ,SEEK_END)) ;
			assume (r2x >= 0) ;
			assume (r2x <= VAL32_MAX) ;
			return r2x ;
		}

		void erase () const override {
			using R1X = DEF<DIR *> ;
			const auto r1x = UniqueRef<R1X> ([&] (VREF<R1X> me) {
				me = opendir ((&mFile[0])) ;
			} ,[] (VREF<R1X> me) {
				if (me == NULL)
					return ;
				closedir (me) ;
			}) ;
			if (r1x.self != NULL)
				return ;
			unlink ((&mFile[0])) ;
		}

		void copy_from (CREF<Holder> that) const override {
			unimplemented () ;
		}

		void move_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = find () ;
			assume (ifnot (r2x)) ;
			const auto r3x = rename ((&r1x->mFile[0]) ,(&mFile[0])) ;
			noop (r3x) ;
		}

		void link_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = find () ;
			assume (ifnot (r2x)) ;
			const auto r3x = link ((&r1x->mFile[0]) ,(&mFile[0])) ;
			noop (r3x) ;
		}

		BOOL identical (CREF<Holder> that) const override {
			using R1X = DEF<struct stat> ;
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			auto rax = ARRAY2<R1X> () ;
			zeroize (rax[0]) ;
			const auto r2x = stat ((&mFile[0]) ,(&rax[0])) ;
			if (r2x != 0)
				return FALSE ;
			if (rax[0].st_nlink == 0)
				return FALSE ;
			zeroize (rax[1]) ;
			const auto r3x = stat ((&r1x->mFile[0]) ,(&rax[1])) ;
			if (r3x != 0)
				return FALSE ;
			if (rax[1].st_nlink == 0)
				return FALSE ;
			if (rax[0].st_dev != rax[1].st_dev)
				return FALSE ;
			if (rax[0].st_ino != rax[1].st_ino)
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <>
exports auto FILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;