﻿#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class...>
trait FILE_HELP ;

template <class...>
trait FILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FILE_HELP<DEPEND ,ALWAYS> {
	struct Layout ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<String<STR>> file) = 0 ;
		virtual VAL64 length () const = 0 ;
		virtual VarBuffer<BYTE> load () const = 0 ;
		virtual void load (VREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual void save (CREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual CRef<RegBuffer<BYTE>> load_asset () const = 0 ;
		virtual BOOL good () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void copy_from (CREF<Layout> that) const = 0 ;
		virtual void move_from (CREF<Layout> that) const = 0 ;
		virtual void link_from (CREF<Layout> that) const = 0 ;
		virtual BOOL identical (CREF<Layout> that) const = 0 ;
	} ;

	struct Layout {
		VRef<Holder> mThis ;
	} ;

	class File implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit File () = default ;

		explicit File (CREF<String<STR>> file) {
			mThis = Holder::create () ;
			mThis->initialize (file) ;
		}

		VarBuffer<BYTE> load () const {
			return mThis->load () ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const {
			return mThis->load (item) ;
		}

		void load (VREF<VarBuffer<BYTE>> item) const {
			return load (RegBuffer<BYTE>::from (item)) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const {
			return mThis->save (item) ;
		}

		void save (CREF<VarBuffer<BYTE>> item) const {
			return save (RegBuffer<BYTE>::from (item)) ;
		}

		void save (CREF<ConBuffer<BYTE>> item) const {
			return save (RegBuffer<BYTE>::from (item)) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const {
			return mThis->load_asset () ;
		}

		BOOL good () const {
			return mThis->good () ;
		}

		VAL64 length () const {
			return mThis->length () ;
		}

		void erase () const {
			return mThis->erase () ;
		}

		void copy_from (CREF<File> that) const {
			return mThis->copy_from (that) ;
		}

		void move_from (CREF<File> that) const {
			return mThis->move_from (that) ;
		}

		void link_from (CREF<File> that) const {
			return mThis->link_from (that) ;
		}

		BOOL identical (CREF<File> that) const {
			return mThis->identical (that) ;
		}
	} ;
} ;

using File = typename FILE_HELP<DEPEND ,ALWAYS>::File ;

template <class...>
trait DIRECTORY_HELP ;

template <class...>
trait DIRECTORY_IMPLHOLDER_HELP ;

template <class DEPEND>
trait DIRECTORY_HELP<DEPEND ,ALWAYS> {
	struct CHILD {
		String<STR> mFile ;
		BOOL mIsFile ;
		BOOL mIsDire ;
		BOOL mIsLink ;
	} ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<String<STR>> dire) = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual LENGTH depth () const = 0 ;
		virtual String<STR> absolute () const = 0 ;
		virtual Array<CHILD> load () const = 0 ;
		virtual void fresh () const = 0 ;
		virtual BOOL good () const = 0 ;
		virtual BOOL lock () const = 0 ;
		virtual void build () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	class Directory {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Directory () = default ;

		explicit Directory (CREF<String<STR>> dire) {
			mThis = Holder::create () ;
			mThis->initialize (dire) ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		LENGTH depth () const {
			return mThis->depth () ;
		}

		String<STR> absolute () const {
			return mThis->absolute () ;
		}

		Array<CHILD> load () const {
			return mThis->load () ;
		}

		void fresh () const {
			return mThis->fresh () ;
		}

		BOOL good () const {
			return mThis->good () ;
		}

		BOOL lock () const {
			return mThis->lock () ;
		}

		void build () const {
			return mThis->build () ;
		}

		void erase () const {
			return mThis->erase () ;
		}

		void clear () const {
			return mThis->clear () ;
		}
	} ;
} ;

using Directory = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Directory ;

template <class...>
trait STREAMFILE_HELP ;

template <class...>
trait STREAMFILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait STREAMFILE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<String<STR>> file) = 0 ;
		virtual void open_r () = 0 ;
		virtual void open_w () = 0 ;
		virtual void open_a () = 0 ;
		virtual void close () = 0 ;
		virtual void open (CREF<BOOL> readable ,CREF<BOOL> writable) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<BYTE>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<WORD>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<CHAR>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<DATA>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<BYTE>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<WORD>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<CHAR>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<DATA>> item) = 0 ;
		virtual void flush () = 0 ;
	} ;

	class StreamFile {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit StreamFile () = default ;

		explicit StreamFile (CREF<String<STR>> file) {
			mThis = Holder::create () ;
			mThis->initialize (file) ;
		}

		void open (CREF<BOOL> readable ,CREF<BOOL> writable) {
			return mThis->open (readable ,writable) ;
		}

		LENGTH read (VREF<RegBuffer<BYTE>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<BYTE>> item) {
			return read (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<WORD>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<WORD>> item) {
			return read (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<CHAR>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<CHAR>> item) {
			return read (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<DATA>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<DATA>> item) {
			return read (RegBuffer<DATA>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<BYTE>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<BYTE>> item) {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<BYTE>> item) {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<WORD>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<WORD>> item) {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<WORD>> item) {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<CHAR>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<CHAR>> item) {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<CHAR>> item) {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<DATA>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<DATA>> item) {
			return write (RegBuffer<DATA>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<DATA>> item) {
			return write (RegBuffer<DATA>::from (item)) ;
		}

		void flush () {
			return mThis->flush () ;
		}
	} ;
} ;

using StreamFile = typename STREAMFILE_HELP<DEPEND ,ALWAYS>::StreamFile ;

template <class...>
trait BUFFERFILE_HELP ;

template <class...>
trait BUFFERFILE_HOLDER_HELP ;

template <class...>
trait BUFFERFILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<String<STR>> file ,CREF<Clazz> clazz) = 0 ;
		virtual void set_cache_size (CREF<LENGTH> size_) = 0 ;
		virtual void open_r () = 0 ;
		virtual void open_w () = 0 ;
		virtual void open_a () = 0 ;
		virtual void close () = 0 ;
		virtual void open (CREF<BOOL> readable ,CREF<BOOL> writable) = 0 ;
		virtual VAL64 length () const = 0 ;
		virtual void resize (CREF<VAL64> size_) = 0 ;
		virtual void get (CREF<VAL64> index ,VREF<RegBuffer<BYTE>> item) = 0 ;
		virtual void set (CREF<VAL64> index ,CREF<RegBuffer<BYTE>> item) = 0 ;
		virtual void flush () = 0 ;
	} ;
} ;

template <class ITEM>
trait BUFFERFILE_HELP<ITEM ,ALWAYS> {
	using Holder = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class BufferFile {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit BufferFile () = default ;

		explicit BufferFile (CREF<String<STR>> file) {
			const auto r1x = Clazz (TYPEAS<ITEM>::expr) ;
			mThis = Holder::create () ;
			mThis->initialize (file ,r1x) ;
		}

		void set_cache_size (CREF<LENGTH> size_) {
			return mThis->set_cache_size (size_) ;
		}

		void open (CREF<BOOL> readable ,CREF<BOOL> writable) {
			return mThis->open (readable ,writable) ;
		}

		VAL64 length () const {
			return mThis->length () ;
		}

		void resize (CREF<VAL64> size_) {
			return mThis->resize (size_) ;
		}

		ITEM get (CREF<VAL64> index) {
			ITEM ret ;
			mThis->get (index ,RegBuffer<BYTE>::from (unsafe_deptr (ret) ,0 ,SIZE_OF<ITEM>::expr)) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		void set (CREF<VAL64> index ,CREF<ITEM> item) {
			mThis->set (index ,RegBuffer<BYTE>::from (unsafe_deptr (item) ,0 ,SIZE_OF<ITEM>::expr)) ;
		}

		void flush () {
			return mThis->flush () ;
		}
	} ;
} ;

template <class ITEM>
using BufferFile = typename BUFFERFILE_HELP<ITEM ,ALWAYS>::BufferFile ;
} ;