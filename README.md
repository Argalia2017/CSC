<h1 align=center font-weight:100>CSC</h1>

CSC is a lightweight, modern C++ library with four key characteristics:

- Cross-platform: **Windows** and **Linux**, with **MSVC**, **GCC**, **Clang**, and **NVCC**.
- Flexible & Stable: **header-only** modules with pluggable backends; stable binary interface across **DLLs** and **threads**, with **Python** interop.
- Memory-safe: Unified smart-pointer type system with **generics**, **ownership**, and **layout** control.
- AI-friendly: Frameworks are easy to define, so **AI** can generate **shorter**, more **readable** code than STL.

# Introduction

| Module                    | Category | Description
|-------------------------- | -------- | --------------------------------
| **[csc.hpp](src/csc/csc.hpp)** | Language  | Compiler environment
| **[csc_type.hpp](src/csc/csc_type.hpp)** | Utility  | Basic types and traits
| **[csc_core.hpp](src/csc/csc_core.hpp)** | Utility  | Box, Pin, Ref, Slice, Clazz, Scope
| **[csc_basic.hpp](src/csc/csc_basic.hpp)** | Memory  | Optional, Function, AutoRef, SharedRef, UniqueRef, RefBuffer, FarBuffer, Allocator
| **[csc_math.hpp](src/csc/csc_math.hpp)** | Math  | MathProc, FloatProc, ByteProc, HashProc, Integer, Jet
| **[csc_array.hpp](src/csc/csc_array.hpp)** | Container  | Array, String, Deque, Priority, List, SortedMap, Set, HashSet, BitSet
| **[csc_image.hpp](src/csc/csc_image.hpp)** | Container  | Image, Tensor
| **[csc_matrix.hpp](src/csc/csc_matrix.hpp)** | Math  | Vector, Matrix, Quaternion, SE3
| **[csc_algorithm.hpp](src/csc/csc_algorithm.hpp)** | Algorithm | Disjoint, KMMatch, TPSFit, BCSFit, FFTransform
| **[csc_stream.hpp](src/csc/csc_stream.hpp)** | String  | ByteReader, TextReader, ByteWriter, TextWriter, Format
| **[csc_string.hpp](src/csc/csc_string.hpp)** | String  | XmlParser, JsonParser, PlyParser
| **[csc_runtime.hpp](src/csc/csc_runtime.hpp)** | System | Time, Atomic, Mutex, SharedLock, UniqueLock, Thread, Process, Library, Random
| **[csc_file.hpp](src/csc/csc_file.hpp)** | System | Path, StreamFile, BufferFile, UartFile, Console
| **[csc_thread.hpp](src/csc/csc_thread.hpp)** | Execution  | WorkThread, CalcThread, Promise

# Build

This library is provided as pure header files. To integrate it into your project, place `${root}/src/csc` in your INCLUDE_PATH and include the following two files:

* Use **[util.hpp](src/util.h)** to reference the necessary modules.
* Use **[inl.cpp](src/inl.cpp)** to include the module implementations in your compilation unit.

Pre-configured Visual Studio 2022 projects are also available in the `${root}/build` directory.

# Example

````
int main () {
	const auto r1x = Singleton<Console>::expr ;
	r1x.show () ;
	r1x.info (Format (slice ("Hello World $1 $2")) (slice ("C++") ,20)) ;
	r1x.pause () ;
	return 0 ;
}
````

# License

**CSC** is distributed under the terms of MIT License. See [LICENSE](LICENSE) for details.
