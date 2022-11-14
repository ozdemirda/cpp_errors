/*
MIT License

Copyright (c) 2022 Danis Ozdemir

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
Footer
*/

#pragma once

#include <string>
#include <sstream>

namespace _code_location {
// The _code_loc function can be used to form
// the location string of the caller. As its argument
// names imply, it will bundle the function name, file
// name and the line number in a std::string object and
// return it.
// The function _code_loc is not meant to be called
// directly. The macros provided below should be used,
// so that the preprocessor can propagate the correct
// values of the arguments to this function.
inline std::string _code_loc(const char* function_name,
		const char* file_name, int line_number) {
	std::stringstream ss;

	ss << "[ " << file_name << ":" << line_number
			<< " => " << function_name << " ]";

	return ss.str();
}
} // namespace _code_location

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

// Unfortunately the following macros seem to be necessary, as the
// required arguments (function name, file name and line number)
// need to be populated using this approach. Maybe the c++
// community can offer a better solution for this. I'd be glad
// to hear about alternatives.

#define code_location() _code_location::_code_loc( \
		__PRETTY_FUNCTION__, \
		__FILE__, \
		__LINE__)

#define code_locationc() _code_location::_code_loc( \
		__PRETTY_FUNCTION__, \
		__FILE__, \
		__LINE__).c_str()
