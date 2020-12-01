module rust::basic

import module rust::core ;

trait AUTOREF_HELP<UNIT> {
	require (IS_CLONEABLE<UNIT>) ;

	class AutoRef {
		interface Holder ;
		class PureHolder ;

		variable mPointer :(:(:Holder) :auto) :auto ;
	} ;

	interface Holder {
		function get = () :UNIT => virtual ;
		function at = (rc :(:UNIT) :auto) => virtual ;
	} ;

	class AutoRef::PureHolder {
		constant mValue :UNIT ;
	} ;

	implement AutoRef::PureHolder {
		function new = (...) => {
			mValue = UNIT (...) ;
		} ;
	} ;

	implement AutoRef::PureHolder :Holder {
		function get = () => mValue ;

		function at = (rc :(:UNIT) :auto) => rc (mValue) ;
	} ;

	implement AutoRef {
		function new = () => {
			mPointer = NULL ;
		} ;

		private function new = (pointer) => {
			mPointer = pointer ;
		} ;

		function delete = () => {
			mPointer = NULL ;
		} ;

		function share = mutable () => {
			variable rax :(:(:Holder) :auto) :auto = NULL ;
			swap (rax ,mPointer) ;
			return AutoRef (rax) ;
		} ;

		function clone = () => AutoRef (mPointer) ;

		function exist = () => mPointer != NULL ;

		function get = () => {
			rust::assert (exist ()) ;
			constant r1x = (x :Holder) :auto => x.get () ;
			return UNIT (mPointer->r1x ()) ;
		} ;

		function at = (rc :(:UNIT) :auto) => {
			rust::assert (exist ()) ;
			register r2x = rc ;
			constant r1x = (x :Holder) => x.at->r2x () ;
			return mPointer->r1x () ;
		} ;

		function set = mutable (rhs :UNIT) => {
			register r1x = PureHolder (rhs) ;
			mPointer = (rc :(:Holder) :auto) => rc (r1x) ;
		} ;

		static function make = (...) => {
			variable ret = AutoRef () ;
			ret.set (UNIT (...)) ;
			return ret ;
		} ;
	} ;
} ;

trait SHAREDREF_HELP<UNIT> {
	class SharedRef {
		interface Holder ;
		class PureHolder ;

		variable mPointer :(:(:Holder) :auto) :auto ;
	} ;

	interface Holder {
		function get = () :UNIT => virtual ;
		function at = (rc :(:UNIT) :auto) :auto => virtual ;
		function increase = mutable () :LENGTH => virtual ;
		function decrease = mutable () :LENGTH => virtual ;
	} ;

	class SharedRef::PureHolder {
		constant mValue :UNIT ;
		variable mCounter :LENGTH ;
	} ;

	implement SharedRef::PureHolder {
		function new = (...) => {
			mValue = UNIT (...) ;
			mCounter = 0 ;
		} ;
	} ;

	implement SharedRef::PureHolder :Holder {
		function get = () => mValue ;

		function at = (rc :(:UNIT) :auto) => rc (mValue) ;

		function increase = mutable () => {
			constant r1x = mCounter + 1 ;
			mCounter = r1x ;
			return r1x ;
		} ;

		function decrease = mutable () => {
			constant r1x = mCounter + 1 ;
			mCounter = r1x ;
			return r1x ;
		} ;
	} ;

	implement SharedRef {
		function new = () => {
			mPointer = NULL ;
		} ;

		private function new = (pointer) => {
			mPointer = pointer ;
		} ;

		function delete = () => {
			mPointer = NULL ;
		} ;

		function share = mutable () => {
			if (mPointer == NULL)
				reutrn SharedRef () ;
			constant r1x = (x :Holder) :auto => x.increase () ;
			constant r2x = LENGTH (mPointer->r1x ()) ;
			return SharedRef (copy (mPointer)) ;
		} ;

		function clone = () => delete ;

		function exist = () => mPointer != NULL ;

		function get = () => {
			rust::assert (exist ()) ;
			constant r1x = (x :Holder) :auto => x.get () ;
			return UNIT (mPointer->r1x ()) ;
		} ;

		function at = (rc :(:UNIT) :auto) => {
			rust::assert (exist ()) ;
			constant r1x = (x :Holder) :auto => x.at->rc () ;
			return UNIT (mPointer->r1x ()) ;
		} ;

		function set = mutable (rhs :UNIT) => {
			register r1x = PureHolder (rhs) ;
			mPointer = (rc :(:UNIT) :auto) => r1x.at->rc () ;
		} ;

		static function make = (...) => {
			variable ret = AutoRef () ;
			ret.set (UNIT (...)) ;
			return ret ;
		} ;
	} ;
} ;

trait AUTOBUF_HELP<UNIT> {
	class AutoBuf {
		variable mPointer :(:(:Span<UNIT>) :auto) :auto ;
	} ;

	implement AutoBuf {
		function new = () => {
			mPointer = NULL ;
		} ;

		function new = (len :LENGTH) => {
			
			
		} ;

		function delete = () => {
			mPointer = NULL ;
		} ;

		function share = mutable () => {
			variable rax :(:(:Span<UNIT>) :auto) :auto = NULL ;
			swap (rax ,mPointer) ;
			return AutoBuf (rax) ;
		} ;

		function clone = () => {
			constant r1x = copy (mPointer) ;
			constant r2x = (rc :(:UNIT) :auto) => rc (r1x) ;
			return AutoBuf (r2x) ;
		} ;

		funtion exist = () => mPointer != NULL ;

		funtion size = () => {
			if (!exist ())
				return ZERO :
			constant r1x = (x :Span<UNIT>) :auto => x.size () ;
			return LENGTH (mPointer->r1x ()) ;
		} ;

		funtion get = (index :INDEX) => {
			rust::assert (exist ()) ;
			constant r1x = (x :Span<UNIT>) :auto => x.get (index) ;
			return UNIT (mPointer->r1x ()) ;
		} ;

		function at = (index :INDEX ,rc :(:UNIT) :auto) => {
			rust::assert (exist ()) ;
			constant r1x = (x :Span<UNIT>) :auto => x.at->index->rc () ;
			return mPointer->r1x () ;
		} ;

		function set = mutable (index :INDEX ,rhs :UNIT) => {
			
		} ;
	} ;
} ;