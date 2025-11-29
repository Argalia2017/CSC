<h1 align=center font-weight:100>CSC</h1>

A lightweight, modern C++ library, providing a consistent interface for **Windows|Linux** and **MSVC|GCC|CLANG|NVCC**. Since this project is built using paradigms focused on memory layout and ownership, it can offer a stable binary interface for projects involving multiple threads and multiple dynamic link libraries.

# Introduction

| Module                    | Category | Description
|-------------------------- | -------- | --------------------------------
| **[csc.hpp](src/csc/csc.hpp)** | Language  | Detects compiler environment
| **[csc_type.hpp](src/csc/csc_type.hpp)** | Utility  | Unified basic types
| **[csc_core.hpp](src/csc/csc_core.hpp)** | Utility  | Box and Ref types
| **[csc_basic.hpp](src/csc/csc_basic.hpp)** | Memory  | Smart pointers and allocators
| **[csc_math.hpp](src/csc/csc_math.hpp)** | Math  | Wrapper of standard libraries
| **[csc_array.hpp](src/csc/csc_array.hpp)** | Container  | Various custom containers
| **[csc_image.hpp](src/csc/csc_image.hpp)** | Container  | Container for image classes
| **[csc_matrix.hpp](src/csc/csc_matrix.hpp)** | Math  | Vector and Matrix types for linear algebra
| **[csc_stream.hpp](src/csc/csc_stream.hpp)** | String  | Text and binary parsers
| **[csc_string.hpp](src/csc/csc_string.hpp)** | String  | Parsers for XML, JSON, and PLY
| **[csc_runtime.hpp](src/csc/csc_runtime.hpp)** | System | Cross-platform wrapper
| **[csc_file.hpp](src/csc/csc_file.hpp)** | System | Cross-platform wrapper
| **[csc_thread.hpp](src/csc/csc_thread.hpp)** | Execution  | Multi-threading synchronization framework

# Build

This library is provided as pure header files. To integrate it into your project, place `${root}/src/csc` in your INCLUDE_PATH and include the following two files:

* Use **[util.hpp](src/util.h)** to reference the necessary modules.
* Use **[inl.cpp](src/inl.cpp)** to include the module implementations in your compilation unit.

Pre-configured Visual Studio 2022 projects are also available in the `${root}/build` directory.

# Example

````
int main () {
	const auto r1x = Singleton<Console>::expr ;
	r1x.start () ;
	r1x.info (Format (slice ("Hello World $1 $2")) (slice ("C++") ,20)) ;
	r1x.pause () ;
	return 0 ;
}
````

# Licences

**CSC** is distributed under the terms of MIT License. See [LICENSE](LICENSE) for details.